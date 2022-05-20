#include <gtest/gtest.h>

#include "../binance/usd_m/RestOp.hpp"
using binance::usd_m::RestOp;

const string api_key = "x";
const string sec_key = "x";
RestOp rest(api_key, sec_key);

TEST(Rest, GetKline)
{
    RestOp rest;
    unordered_map<string, string> params;
    params.insert({{"symbol", "btcusdt"}, {"interval", "1m"}});
    LOG(INFO) << rest.GetPubulic("/fapi/v1/klines", params);
}

TEST(Rest, GetSignature)
{
    const string sec_key = "2b5eb11e18796d12d88f13dc27dbbd02c2cc51ff7059765ed9821957d82bb4d9";
    const string msg = "symbol=BTCUSDT&side=BUY&type=LIMIT&quantity=1&price=9000&timeInForce=GTC&recvWindow=5000&timestamp=1591702613943";
    string sign = RestOp::GetSignature(sec_key, msg);
    ASSERT_FALSE(sign != "3c661234138461fcc7a7d8746c6558c9842d4e10870d2ecbedf7777cad694af9");
    LOG(INFO) << sign;
}

TEST(Rest, GetBalance)
{
    unordered_map<string, string> params;
    params.insert({{"timestamp", Timestamp::Now()}});
    LOG(INFO) << rest.GetPrivate("/fapi/v2/balance", params);
}

TEST(Rest, PlaceOrder)
{
    unordered_map<string, string> params;
    params.insert({{"timestamp", Timestamp::Now()},
                   {"timeInForce", "GTC"},
                   {"symbol", "btcusdt"},
                   {"side", "SELL"},
                   {"positionSide", "short"},
                   {"type", "LIMIT"},
                   {"quantity", "0.001"},
                   {"price", "40000"}});
    LOG(INFO) << rest.PostPrivate("/fapi/v1/order", params);
}

TEST(Rest, GetAllOpenOrder)
{
    unordered_map<string, string> params;
    params.insert({{"timestamp", Timestamp::Now()},
                   {"symbol", "btcusdt"}});
    LOG(INFO) << rest.GetPrivate("/fapi/v1/openOrders", params);
}

TEST(Rest, CancelOrder)
{
    unordered_map<string, string> params;
    params.insert({{"timestamp", Timestamp::Now()},
                   {"symbol", "btcusdt"},
                   {"orderId", "54715375947"}});
    LOG(INFO) << rest.DeletePrivate("/fapi/v1/order", params);
}