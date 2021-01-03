#ifndef CONNECTION_H
#define CONNECTION_H

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

class Connection : public std::enable_shared_from_this<Connection> {
private:
    boost::asio::ip::tcp::resolver _resolver;
     boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream>> _sock;
    boost::beast::flat_buffer _buffer;
    std::string _host;
    std::string _text;

    void on_resolve(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results);
    void on_connect(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type::endpoint_type ep);
    void on_ssl_handshake(boost::beast::error_code ec);
    void on_handshake(boost::beast::error_code ec);
    void on_write(boost::beast::error_code ec, std:: size_t bytes);
    void on_read(boost::beast::error_code eec, std::size_t bytes);
    void on_close(boost::beast::error_code ec);
    void fail(boost::beast::error_code ec, const char *where);

    void do_read();
    void do_write(std::string msg);

public:
    Connection(boost::asio::io_context &ioc, boost::asio::ssl::context &ctx);
    void Start_Connection();
};


#endif