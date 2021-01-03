
// #include "connection.hpp"
// #include <boost/asio.hpp>
// #include <boost/beast/websocket.hpp>
// #include <boost/beast/core.hpp>
// #include <iostream>
// #include <string>
// #include <cstdlib>

// namespace beast = boost::beast;         // from <boost/beast.hpp>
// namespace http = beast::http;           // from <boost/beast/http.hpp>
// namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
// namespace net = boost::asio;            // from <boost/asio.hpp>
// using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// // Sends a WebSocket message and prints the response
// int main(int argc, char** argv)
// {
//     const char *host = argv[1];
//     const char *port = "80";
//     const char *text = "Hello World";
//     const char *text2 = "Testing";

//     boost::asio::io_context ioc;

//     std::shared_ptr<Connection> c(new Connection(ioc));
//     //td::shared_ptr<Connection> b(new Connection(ioc));
//     c->Start_Connection(host, port, text);
//     //b->Start_Connection(host, port, text2);
//     //std::make_shared<Connection>(ioc)->Start_Connection(host, port, text);

//     ioc.run();

// }

// #include <boost/beast/core.hpp>
// #include <boost/beast/ssl.hpp>
// #include <boost/beast/websocket.hpp>
// #include <boost/beast/websocket/ssl.hpp>
// #include <boost/asio/connect.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/ssl/stream.hpp>
// #include <cstdlib>
// #include <iostream>
// #include <string>

// namespace beast = boost::beast;         // from <boost/beast.hpp>
// namespace http = beast::http;           // from <boost/beast/http.hpp>
// namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
// namespace net = boost::asio;            // from <boost/asio.hpp>
// namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
// using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// // Sends a WebSocket message and prints the response
// int main(int argc, char** argv)
// {
//     try
//     {
//         // Check command line arguments.
//         auto const host = "www.bitmex.com";
//         auto const port = "443";
//         auto const rpcJson = R"({"method":"getCurrency", "params":{"currency":"ETH"},"id":0})";

//         // The io_context is required for all I/O
//         net::io_context ioc;

//         // The SSL context is required, and holds certificates
//         ssl::context ctx{ssl::context::tlsv12_client};

//         // This holds the root certificate used for verification
//         //load_root_certificates(ctx);

//         // These objects perform our I/O
//         tcp::resolver resolver{ioc};
//         websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

//         // Look up the domain name
//         auto const results = resolver.resolve(host, port);

//         // Make the connection on the IP address we get from a lookup
//         net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

//         // Perform the SSL handshake
//         ws.next_layer().handshake(ssl::stream_base::client);

//         // Set a decorator to change the User-Agent of the handshake
//         ws.set_option(websocket::stream_base::decorator(
//             [](websocket::request_type& req)
//             {
//                 req.set(http::field::user_agent,
//                     std::string(BOOST_BEAST_VERSION_STRING) +
//                         " websocket-client-coro");
//             }));

//         // Perform the websocket handshake
//         ws.handshake(host, "/realtime?subscribe=instrument,orderBookL2_25:XBTUSD");

//         // Our message in this case should be stringified JSON-RPC request
//         //ws.write(net::buffer(std::string(rpcJson)));

//         // This buffer will hold the incoming message
//         beast::flat_buffer buffer;

//         // Read a message into our buffer
//         ws.read(buffer);

//         // Close the WebSocket connection
//         ws.close(websocket::close_code::normal);

//         // If we get here then the connection is closed gracefully

//         // The make_printable() function helps print a ConstBufferSequence
//         std::cout << beast::make_printable(buffer.data()) << std::endl;
//     }
//     catch(std::exception const& e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return EXIT_FAILURE;
//     }
//     return EXIT_SUCCESS;
// }


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