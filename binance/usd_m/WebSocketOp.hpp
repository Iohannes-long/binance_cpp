#pragma once
#include <string>
using std::string;

#include <list>
using std::list;

#include <sstream>
using std::stringstream;

#include <ctime>
#include <map>
using std::map;
using std::pair;

#include <algorithm>
#include <cctype>
#include <thread>
#include <chrono>

#include "binance/utils/WebSocket.hpp"
using binance::utils::WebSocket;

typedef std::function<void(const string &)> _call_back_fun;

namespace binance
{
    namespace usd_m
    {
        class WebSocketOp
        {
        public:
            WebSocketOp(const string &param, const _call_back_fun &callback,
                        const string &url = utils::WSS_HOST)
            {
                this->param = param;
                this->callback = callback;
                this->url = url + "/ws/" + param;
                this->p_ws = nullptr;
                this->Connect();
            }

            ~WebSocketOp()
            {
                Disconnect();
            }

        private:
            bool Connect()
            {
                p_ws = new WebSocket();
                p_ws->OnOpen = std::bind(&WebSocketOp::WsOnOpen, this);
                p_ws->OnClose = std::bind(&WebSocketOp::WsOnClose, this);
                p_ws->OnMsg = std::bind(&WebSocketOp::WsOnMsg, this, std::placeholders::_1);
                p_ws->OnError = std::bind(&WebSocketOp::WsOnError, this, std::placeholders::_1);

                stringstream str_buf;
                str_buf << "wss://" << url;
                string url = str_buf.str();
                LOG(INFO) << "ws url: " << url;
                bool con_ok = p_ws->Connect(url);
                if (!con_ok)
                {
                    return false;
                }
                std::thread t1([&]
                               { p_ws->RunForever(); });
                t1.detach();
                return true;
            }

            void Disconnect()
            {
                if (p_ws == NULL)
                {
                    return;
                }

                delete p_ws;
                p_ws = NULL;
            }

        private:
            void WsOnOpen()
            {
                LOG(INFO) << "ws opened.";

                stringstream ss;
                ss << "{\"method\":\"SUBSCRIBE\",\"params\":["
                   << "\"" << param << "\""
                   << "],\"id\":1}";

                p_ws->SendMsg(ss.str());
                LOG(INFO) << "ws send: " << ss.str();
            }

            void WsOnClose()
            {
                LOG(INFO) << "ws close.";
            }

            void WsOnMsg(const string &data)
            {
                if (callback != nullptr)
                {
                    callback(data);
                }
            }

            void WsOnError(const string &data)
            {
                LOG(ERROR) << data;
            }

            string param;
            _call_back_fun callback;
            string url;
            WebSocket *p_ws;
        };
    } // namespace linear_swap
} // namespace binance
