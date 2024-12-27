// wol/wol.hpp
#ifndef WOL_HPP
#define WOL_HPP

#include <string>

#include "config.h"

enum class WOLStatus {
    SUCCESS,        // WOL 发送成功
    INVALID_MAC,    // 无效的 MAC 地址
    SEND_FAILED     // 发送失败
};


WOLStatus wake_on_lan(const std::string& mac_address = MAC_ADDRESS, const std::string& ip_address = IP_ADDRESS, unsigned short port = PORT);

#endif // WOL_HPP
