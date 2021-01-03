#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include "connection.hpp"


int main(){
    boost::asio::io_context ioc;
    boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);
    std::make_shared<Connection>(ioc, ctx)->Start_Connection();

    ioc.run();
}
