#include <gtest/gtest.h>

#include "../binance/usd_m/WebSocketOp.hpp"
using binance::usd_m::WebSocketOp;

TEST(Ws, SubKline)
{
    string param = "trxusdt@kline_1m";
    WebSocketOp ws(param, [](const string &data)
                   { LOG(INFO) << data; });
    std::this_thread::sleep_for(std::chrono::seconds(60));
}