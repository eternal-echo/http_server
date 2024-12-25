#include "doctest/doctest.h"
#include "http_server.hpp"
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

bool mock_wol_callback(const std::string& mac_address, const std::string& ip_address, unsigned short port);

asio::io_context ioc;
HttpServer server(ioc, mock_wol_callback);

// 定义一个测试用的 Mock 回调函数
bool mock_wol_callback(const std::string& mac_address, const std::string& ip_address, unsigned short port) {
    std::cout << "Mock WOL triggered for MAC: " << mac_address
              << ", IP: " << ip_address
              << ", Port: " << port << std::endl;

    // 通过断言来检查参数
    CHECK(mac_address == "00:11:22:33:44:55");
    CHECK(ip_address == "192.168.1.100");
    CHECK(port == 9);

    server.stop_server();

    return true;
}

// 测试处理请求的逻辑，验证是否能正确解析请求并调用回调函数
TEST_CASE("Test HttpServer handle_request method") {
    std::string mac = "00:11:22:33:44:55";
    std::string ip = "192.168.1.100";
    unsigned short port = 9;

    // 启动服务器
    server.start_server(8080);

    //创建一个测试用的请求流
    asio::io_context test_ioc;
    boost::beast::tcp_stream test_request_stream(test_ioc);
    asio::ip::tcp::resolver resolver(test_ioc);
    asio::ip::basic_resolver_results const results = resolver.resolve("localhost", "8080");
    test_request_stream.connect(results);

    // 构造一个简单的 WOL 请求
    http::request<http::string_body> test_req{http::verb::get, "/wol?mac=" + mac + "&ip=" + ip + "&port=" + std::to_string(port), 11};
    test_req.set(http::field::host, "localhost");
    test_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // 发送请求
    http::write(test_request_stream, test_req);

    // 启动请求流的IO上下文
    test_ioc.run();

    // 启动服务器端IO上下文
    ioc.run();

} // TEST_CASE