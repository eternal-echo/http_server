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

    acceptor_.open(endpoint.protocol());
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
                std::cout << "Received HTTP request: " << req.method_string() << " " << req.target() << " " << req.version() << std::endl;

                // 获取查询参数并转换为 std::string
                std::string mac_address = std::string(req["mac"].begin(), req["mac"].end());
                std::string ip_address = std::string(req["ip"].begin(), req["ip"].end());
                unsigned short port = std::stoi(req["port"].to_string());

                // 处理请求并调用回调
                handle_request(mac_address, ip_address, port);

                // 创建响应
                http::response<http::string_body> res{http::status::ok, req.version()};
                res.set(http::field::server, "Boost.Beast/HTTP Server");
                res.set(http::field::content_type, "text/plain");
                res.body() = "WOL signal sent successfully!";
                res.prepare_payload();

                // 发送响应
                http::write(socket, res);
            }

            // 接受下一个连接
            do_accept();
        });
}