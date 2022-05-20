#pragma once
#include <iostream>
#include <curl/curl.h>
#include <string>
using std::string;

#include <openssl/hmac.h>
#include <iomanip>
#include <sstream>
using std::stringstream;

#include <unordered_map>
using std::unordered_map;

#include "../utils/HttpRequest.hpp"
using binance::utils::HttpRequest;

#include "../utils/const_val.hpp"
#include "../utils/Timestamp.hpp"
using binance::utils::Timestamp;

#include "../utils/MapConvert.hpp"
using binance::utils::MapConvert;

namespace binance
{
    namespace usd_m
    {
        class RestOp
        {
        public:
            RestOp(const string &api_key = "", const string &secret_key = "")
                : api_key(api_key), secret_key(secret_key) {}

            string DeletePrivate(const string &path,
                                 const unordered_map<string, string> &parameters,
                                 const string &host = utils::REST_HOST)
            {
                stringstream url;
                url << "https://" << host << path;

                stringstream params;
                if (!parameters.empty())
                {
                    params << MapConvert::ToUrlParams(parameters);
                }
                const string to_sign = params.str();
                LOG(INFO) << "to sign: " << to_sign;
                params << "&signature=" << GetSignature(this->secret_key, to_sign);

                unordered_map<string, string> headers;
                headers.insert({{"Content-Type", "application/x-www-form-urlencoded"},
                                {"X-MBX-APIKEY", this->api_key}});
                return HttpRequest::Instance().Send(url.str(),
                                                    params.str(),
                                                    MapConvert::ToHeaderParams(headers),
                                                    HttpRequest::method_t::DEL);
            }

            string PostPrivate(const string &path,
                               const unordered_map<string, string> &parameters,
                               const string &host = utils::REST_HOST)
            {
                stringstream url;
                url << "https://" << host << path;

                stringstream params;
                if (!parameters.empty())
                {
                    params << MapConvert::ToUrlParams(parameters);
                }
                const string to_sign = params.str();
                LOG(INFO) << "to sign: " << to_sign;
                params << "&signature=" << GetSignature(this->secret_key, to_sign);

                unordered_map<string, string> headers;
                headers.insert({{"Content-Type", "application/json"},
                                {"X-MBX-APIKEY", this->api_key}});
                return HttpRequest::Instance().Send(url.str(),
                                                    params.str(),
                                                    MapConvert::ToHeaderParams(headers),
                                                    HttpRequest::method_t::POST);
            }

            string GetPrivate(const string &path,
                              const unordered_map<string, string> &parameters,
                              const string &host = utils::REST_HOST)
            {
                stringstream url;
                url << "https://" << host << path;

                stringstream params;
                if (!parameters.empty())
                {
                    params << MapConvert::ToUrlParams(parameters);
                }
                const string to_sign = params.str();
                LOG(INFO) << "to sign: " << to_sign;
                params << "&signature=" << GetSignature(this->secret_key, to_sign);

                unordered_map<string, string> headers;
                headers.insert({{"Content-Type", "application/x-www-form-urlencoded"},
                                {"X-MBX-APIKEY", this->api_key}});

                return HttpRequest::Instance().Send(url.str(),
                                                    params.str(),
                                                    MapConvert::ToHeaderParams(headers));
            }

            string GetPubulic(const string &path,
                              const unordered_map<string, string> &parameters,
                              const string &host = utils::REST_HOST)
            {
                stringstream url;
                url << "https://" << host << path;

                unordered_map<string, string> headers;
                headers.insert({{"Content-Type", "application/x-www-form-urlencoded"}});
                return HttpRequest::Instance().Send(url.str(),
                                                    MapConvert::ToUrlParams(parameters),
                                                    MapConvert::ToHeaderParams(headers));
            }

        private:
            string api_key;
            string secret_key;

        public:
            static string GetSignature(const string &key, const string &data)
            {
                unsigned char *result;
                static char res_hexstring[64];
                int result_len = 32;
                string signature;

                result = HMAC(EVP_sha256(), key.c_str(), key.length(), (unsigned char *)data.c_str(), data.length(), NULL, NULL);
                for (int i = 0; i < result_len; i++)
                {
                    sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
                }

                for (int i = 0; i < 64; i++)
                {
                    signature += res_hexstring[i];
                }

                return signature;
            }
        };
    }
}