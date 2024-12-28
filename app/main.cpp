#include <iostream>
#include <cstdlib>
#include "http_server.hpp"

// 用户自定义的回调函数（如果需要）
bool custom_wol_callback(const std::string& mac_address, const std::string& ip_address, unsigned short port) {
    std::cout << "WOL triggered for MAC: " << mac_address
              << ", IP: " << ip_address
              << ", Port: " << port << std::endl;
    std::string command = "wakeonlan " + mac_address;
    int ret = std::system(command.c_str());
    if (ret != 0) {
        std::cerr << "Failed to trigger WOL signal!" << std::endl;
        return false;
    }
    return true;
}

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
    try {
        asio::io_context ioc;

        // 创建并启动 HTTP 服务器，传入回调函数
        HttpServer server(ioc, custom_wol_callback);
        server.start_server(8080);

        // 运行 IO 服务
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "main Error: " << e.what() << std::endl;
    }

    return 0;
}
