#pragma once

#include <string>
using std::string;

namespace binance
{
	namespace utils
	{
        const string WSS_HOST = "fstream.binance.com";
        const string REST_HOST = "fapi.binance.com";

        const size_t WS_RECV_BUF_SIZE = 4096 * 100;
        const size_t WS_SEND_BUF_SIZE = 1024;
    } // namespace utils
} // namespace binance