add_executable(example main.cpp)
target_link_libraries(example Threads::Threads websockets OpenSSL::SSL OpenSSL::Crypto curl z glog::glog binance)
