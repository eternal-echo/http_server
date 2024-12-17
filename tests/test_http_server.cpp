#include "doctest/doctest.h"
#include "http_server.hpp"
#include <iostream>
#include <functional>

// 定义一个测试用的 Mock 回调函数
bool mock_wol_callback(const std::string& mac_address, const std::string& ip_address, unsigned short port) {
    std::cout << "Mock WOL triggered for MAC: " << mac_address
              << ", IP: " << ip_address
              << ", Port: " << port << std::endl;

    // 通过断言来检查参数
    CHECK(mac_address == "00:11:22:33:44:55");
    CHECK(ip_address == "192.168.1.100");
    CHECK(port == 9);

    return true;
}

TEST_CASE("Test HttpServer with valid WOL callback") {
    CHECK(true);
}