#include "http_server.hpp"
#include <iostream>

// HTTP 服务器构造函数
HttpServer::HttpServer(asio::io_context& ioc, WOLCallback wol_callback)
    : ioc_(ioc), acceptor_(ioc)
{
    wol_callback_ = wol_callback;
}

// 启动 HTTP 服务器，开始监听指定端口
void HttpServer::start_server(unsigned short port) {
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);

    // todo: 可以通过传入不同的 endpoint 来支持 IPv6

    // acceptor_.open(endpoint.protocol());
    acceptor_.open(asio::ip::tcp::v4()); // 使用 IPv4 协议
    acceptor_.bind(endpoint);
    acceptor_.listen(asio::socket_base::max_listen_connections);

    do_accept();
}

// 处理 HTTP 请求
void HttpServer::handle_request(const std::string& mac_address, const std::string& ip_address, unsigned short port) {
    if (wol_callback_) {
        // 调用回调函数来处理 WOL 操作
        if (!wol_callback_(mac_address, ip_address, port)) {
            std::cerr << "WOL signal failed!" << std::endl;
        }
    } else {
        std::cerr << "No callback function provided!" << std::endl;
    }
}

void HttpServer::parse_param(const std::string& param, std::unordered_map<std::string, std::string>& params) {
    size_t equal_pos = param.find('=');
    if (equal_pos != std::string::npos) {
        std::string key = param.substr(0, equal_pos);
        std::string value = param.substr(equal_pos + 1);
        params[key] = value;
    }
}

std::unordered_map<std::string, std::string> HttpServer::analysis_request(const http::request<http::string_body>& req) {
    std::string URL = std::string(req.target());
    std::unordered_map<std::string, std::string> params;

    // 提取查询字符串
    size_t start_index = URL.find('?');
    if (start_index == std::string::npos) return params;  // 如果没有查询参数，直接返回空的params

    start_index += 1;  // 跳过问号

    size_t end_index;
    while ((end_index = URL.find('&', start_index)) != std::string::npos) {
        parse_param(URL.substr(start_index, end_index - start_index), params);
        start_index = end_index + 1;
    }

    // 处理最后一个参数
    parse_param(URL.substr(start_index), params);

    // 默认值处理（如果参数缺失，使用默认值）
    if (params.find("mac") == params.end()) {
        std::cerr << "MAC address is missing!" << std::endl;
    }
    if (params.find("ip") == params.end()) {
        params["ip"] = "255.255.255.255";  // 默认 IP
    }
    if (params.find("port") == params.end()) {
        params["port"] = "9";  // 默认端口
    }
    
    return params;
}

// 监听并接受客户端连接
void HttpServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                // 在这里处理 HTTP 请求
                beast::flat_buffer buffer;
                http::request<http::string_body> req;

                // 解析 HTTP 请求
                http::read(socket, buffer, req);

                // 打印请求信息（调试成功后注释）
                std::cout << "Received HTTP request: " << req.target() << " " << req.target() << " " << req.version() << std::endl;

                // 获取查询参数并转换为 std::string
                std::unordered_map<std::string, std::string> params = analysis_request(req);
                if (!params.empty()) {
                    std::string mac_address = params["mac"];
                    std::string ip_address = params["ip"];
                    std::cout << "mac_address: " << mac_address << std::endl;
                    std::cout << "ip_address: " << ip_address << std::endl;
                    std::cout << "port: " << params["port"] << std::endl;
                    unsigned short port = std::stoi(params["port"]);

                    // 处理请求并调用回调
                    handle_request(mac_address, ip_address, port);

                    // 创建响应
                    http::response<http::string_body> res{http::status::ok, req.version()};
                    // res.set(http::field::server, "Boost.Beast/HTTP Server");
                    res.set(http::field::content_type, "text/plain");
                    res.body() = "WOL signal sent successfully!";
                    res.prepare_payload();

                    // 发送响应
                    http::write(socket, res);
                } else {
                    std::cerr << "Invalid request!" << std::endl;
                }
            }

            // 接受下一个连接
            do_accept();
        });
}

// 实现停止服务器的函数
void HttpServer::stop_server() {
    // 关闭 acceptor，停止监听新的连接
    acceptor_.close();
    // 停止 io_context 的运行，这样后续 ioc.run() 就会退出循环
    ioc_.stop();
}