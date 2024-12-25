#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <string>
#include <functional>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <unordered_map>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

// 定义一个WOL操作的回调函数类型
using WOLCallback = std::function<bool(const std::string&, const std::string&, unsigned short)>;

// HTTP服务器的类，接受回调函数作为参数
class HttpServer {
public:
    // 构造函数，初始化HTTP服务器，接受一个WOL操作的回调函数
    HttpServer(asio::io_context& ioc, WOLCallback wol_callback=nullptr);

    // 设置回调函数（用于动态设置）
    void set_wol_callback(WOLCallback wol_callback);

    // 启动HTTP服务器，开始监听端口
    void start_server(unsigned short port);

    // 新增的停止服务器接口
    void stop_server();


private:
    // 处理HTTP请求，解析请求参数，并调用回调函数
    void handle_request(const std::string& mac_address, const std::string& ip_address, unsigned short port);

    // 异步操作相关
    void do_accept();
    std::unordered_map<std::string, std::string> analysis_request(http::request<http::string_body> req);
    asio::io_context& ioc_; // I/O 上下文
    asio::ip::tcp::acceptor acceptor_; // 监听器
    WOLCallback wol_callback_; // 回调函数
};

#endif // HTTP_SERVER_HPP
