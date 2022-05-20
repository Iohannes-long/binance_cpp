#pragma once

#include <curl/curl.h>

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <glog/logging.h>
#include <thread>
#include <chrono>

#include <list>
using std::list;

namespace binance
{
    namespace utils
    {
        class HttpRequest
        {

        public:
            enum method_t
            {
                GET,
                POST,
                DEL
            };

            virtual ~HttpRequest()
            {
                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }

            static HttpRequest &Instance()
            {
                return HttpRequest::instance;
            }

            string Send(const string &url,
                        const string &param,
                        const list<string> &headers = {},
                        method_t method = method_t::GET)
            {
                while (doing)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                doing = true;
                stringstream full_url;
                full_url << url << "?" << param;

                string flag;
                switch (method)
                {
                case method_t::POST:
                    flag = "POST";
                    break;
                case method_t::DEL:
                    flag = "DELETE";
                    break;
                case method_t::GET:
                default:
                    flag = "GET";
                    break;
                }
                // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, flag.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0L);
                curl_easy_setopt(curl, CURLOPT_URL, full_url.str().c_str());

                stringstream msg;
                msg << flag << ": " << full_url.str() << std::endl
                    << "params: " << param;

                if (!headers.empty())
                {
                    msg << std::endl
                        << "header: ";
                    curl_slist *plist = nullptr;
                    for (string item : headers)
                    {
                        plist = curl_slist_append(plist, item.c_str());
                        msg << item << ";";
                    }
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
                }
                LOG(INFO) << msg.str();

                string sBuffer;
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sBuffer);

                CURLcode res = curl_easy_perform(curl);
                if (res != CURLE_OK)
                {
                    LOG(ERROR) << "curl_easy_perform error:" << curl_easy_strerror(res);
                }

                doing = false;
                return sBuffer;
            }

        private:
            static size_t callback_func(void *input, size_t uSize, size_t uCount, void *avg)
            {
                size_t uLen = uSize * uCount;
                std::string &sBuffer = *reinterpret_cast<std::string *>(avg);
                sBuffer.append(reinterpret_cast<const char *>(input), uLen);
                return uLen;
            }

            HttpRequest()
            {
                CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
                if (res != CURLE_OK)
                {
                    LOG(ERROR) << "curl_global_init error:" << curl_easy_strerror(res);
                }

                curl = curl_easy_init();
                if (curl == NULL)
                {
                    LOG(ERROR) << "curl_easy_init error.";
                }
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_func);
            }

            static HttpRequest instance;
            CURL *curl = NULL;
            volatile bool doing = false;
        };

        HttpRequest HttpRequest::instance;

    } // namespace utils
} // namespace binance