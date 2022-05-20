#include <iostream>
#include <glog/logging.h>

#include "../binance/usd_m/WebSocketOp.hpp"
using binance::usd_m::WebSocketOp;

#include "../binance/usd_m/RestOp.hpp"
using binance::usd_m::RestOp;

#include "../binance/utils/json.hpp"
using nlohmann::json;

const string api_key = "x";
const string sec_key = "x";
RestOp rest(api_key, sec_key);

bool be_first = true;
bool finished = false;
void call_back(const string &data)
{
    LOG(INFO) << data;
    if (!be_first)
    {
        return;
    }

    json jdata = json::parse(data);
    if(!jdata.contains("k"))
    {
        return;
    }
    string temp = jdata["k"]["c"];
    double price = std::stod(temp) + 5000;

    // place order
    unordered_map<string, string> params;
    params.insert({{"timestamp", Timestamp::Now()},
                   {"timeInForce", "GTC"},
                   {"symbol", "btcusdt"},
                   {"side", "SELL"},
                   {"positionSide", "short"},
                   {"type", "LIMIT"},
                   {"quantity", "0.001"},
                   {"price", std::to_string(price)}});
    string result = rest.PostPrivate("/fapi/v1/order", params);
    LOG(INFO) << result;

    // cancel order
    jdata = json::parse(result);
    if(!jdata.contains("orderId"))
    {
        return;
    }
    long order_id = jdata["orderId"];
    params.clear();
    params.insert({{"timestamp", Timestamp::Now()},
                   {"symbol", "btcusdt"},
                   {"orderId", std::to_string(order_id)}});
    result = rest.DeletePrivate("/fapi/v1/order", params);
    LOG(INFO) << result;

    be_first = false;
    finished = true;
}

int main(int argc, char *argv[])
{
    string param = "btcusdt@kline_1m";
    WebSocketOp ws(param, call_back);
    while (!finished)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}