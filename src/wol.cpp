// wol/wol.cpp
#include "wol.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <vector>

namespace asio = boost::asio;

WOLStatus wake_on_lan(const std::string& mac_address, const std::string& ip_address, unsigned short port) {
    // MAC 地址验证
    if (mac_address.size() != 17 || mac_address[2] != ':' || mac_address[5] != ':' || mac_address[8] != ':' || mac_address[11] != ':' || mac_address[14] != ':') {
        return WOLStatus::INVALID_MAC;
    }

    // 转换 MAC 地址
    std::array<unsigned char, 6> mac_bytes;
    std::istringstream ss(mac_address);
    for (size_t i = 0; i < 6; ++i) {
        unsigned int byte;
        ss >> std::hex >> byte;
        mac_bytes[i] = static_cast<unsigned char>(byte);
        if (ss.peek() == ':') ss.ignore();
    }

    // 构造魔术包
    std::vector<unsigned char> magic_packet(6, 0xFF); // 开头是 6 个 0xFF
    for (int i = 0; i < 16; ++i) {
        magic_packet.insert(magic_packet.end(), mac_bytes.begin(), mac_bytes.end());
    }

    // 设置 I/O 上下文和套接字
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context);
    asio::ip::udp::endpoint endpoint(asio::ip::address::from_string(ip_address), port);

    try {
        // 打开套接字
        socket.open(asio::ip::udp::v4());

        // 发送魔术包
        socket.send_to(asio::buffer(magic_packet), endpoint);
    } catch (const std::exception& e) {
        return WOLStatus::SEND_FAILED;
    }

    return WOLStatus::SUCCESS;
}
