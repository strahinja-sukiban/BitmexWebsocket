#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "connection.hpp"


void Connection::on_resolve(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type results){
    if(ec){
        return fail(ec, "resolve");
    }
    std::cout << "Host resolved!" << std::endl;
    boost::beast::get_lowest_layer(_sock).async_connect(results, 
        boost::beast::bind_front_handler(&Connection::on_connect, shared_from_this()));
}


void Connection::on_connect(boost::beast::error_code ec, boost::asio::ip::tcp::resolver::results_type::endpoint_type ep){
    if(ec){
        return fail(ec, "connect");
    }

    std::cout << "Connected! trying handshake" << std::endl;


    //update host string (necessary for handshake)
    _host += ':' + std::to_string(ep.port());

    boost::beast::get_lowest_layer(_sock).expires_after(std::chrono::seconds(30));


    //perform ssl handshake
    _sock.next_layer().async_handshake(
        boost::asio::ssl::stream_base::client,
        boost::beast::bind_front_handler(&Connection::on_ssl_handshake, shared_from_this()));

    // _sock.async_handshake(_host, "/realtime?subscribe=instrument,orderBookL2_25:XBTUSD", boost::beast::bind_front_handler(
    //     &Connection::on_handshake, shared_from_this()));

}


void Connection::on_ssl_handshake(boost::beast::error_code ec){
    if(ec){
        return fail(ec, "ssl_handshake");
    }

    //reccommended timeout settings
    boost::beast::get_lowest_layer(_sock).expires_never();
    _sock.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::client));


    //set decorator to change user-agent of handhsake
    _sock.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::request_type &req)
        {
            req.set(boost::beast::http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) +
                " websocket-client-async-ssl");
        }));

    //perform the websocket handshake
    _sock.async_handshake(_host, "/realtime?subscribe=instrument,orderBookL2_25:XBTUSD",
        boost::beast::bind_front_handler(&Connection::on_handshake, shared_from_this()));

}



void Connection::on_handshake(boost::beast::error_code ec){
    if(ec){
        return fail(ec, "ssl_handshake");
    }


    do_read();

    // //send the message
    // _sock.async_write(boost::asio::buffer(_text),
    //     boost::beast::bind_front_handler(&Connection::on_write, shared_from_this()));
}

void Connection::on_write(boost::beast::error_code ec, std:: size_t bytes){
    boost::ignore_unused(bytes);
    if(ec){
        return fail(ec, "write");
    }

    do_read();
    // //read a message
    // _sock.async_read(_buffer, 
    //     boost::beast::bind_front_handler(&Connection::on_read, shared_from_this()));
}

void Connection::on_read(boost::beast::error_code ec, std::size_t bytes){
    boost::ignore_unused(bytes);

    if(ec){
        return fail(ec, "read");
    }

    //print data
    std::cout << "received: " << boost::beast::make_printable(_buffer.data()) << std::endl;


    do_read();


    // //close
    // _sock.async_close(boost::beast::websocket::close_code::normal,
    //     boost::beast::bind_front_handler(&Connection::on_close, shared_from_this()));
}

void Connection::on_close(boost::beast::error_code ec){
    if(ec){
        return fail(ec, "close");
    }

    std::cout << "disconnected" << std::endl;
}

void Connection::fail(boost::beast::error_code ec, const char *where){
    std::cerr << where << ": " << ec.message() << "\n";
}


Connection::Connection(boost::asio::io_context &ioc, boost::asio::ssl::context &ctx): _resolver(boost::asio::make_strand(ioc)), 
    _sock(boost::asio::make_strand(ioc), ctx) {
        std::cout << "Connecting to host..." << std::endl;
}

void Connection::Start_Connection(){
    _host = "www.bitmex.com";
    _text = "test";
    std::cout << "trying to resolve host!" << std::endl;
    _resolver.async_resolve(_host, "443", 
        boost::beast::bind_front_handler(&Connection::on_resolve, shared_from_this()));
}


void Connection::do_read(){

    _buffer = {};

    _sock.async_read(_buffer, 
        boost::beast::bind_front_handler(&Connection::on_read, shared_from_this()));
}


void Connection::do_write(std::string msg){
    _sock.async_write(boost::asio::buffer(msg),
        boost::beast::bind_front_handler(&Connection::on_write, shared_from_this()));
}

