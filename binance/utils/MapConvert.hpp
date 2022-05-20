#pragma once
#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <unordered_map>
using std::unordered_map;

#include <list>
using std::list;

namespace binance
{
    namespace utils
    {
        class MapConvert
        {
        public:
            static string ToUrlParams(const unordered_map<string, string> &parameters)
            {
                stringstream query;
                for (auto it = parameters.cbegin(); it != parameters.cend(); ++it)
                {
                    if (it != parameters.cbegin())
                    {
                        query << "&";
                    }
                    query << it->first << "=" << it->second;
                }
                return query.str();
            }

            static list<string> ToHeaderParams(const unordered_map<string, string> &parameters)
            {
                list<string> header;
                for (auto it = parameters.cbegin(); it != parameters.cend(); ++it)
                {
                    header.push_back(it->first + ":" + it->second);
                }
                return header;
            }
        };
    }
}