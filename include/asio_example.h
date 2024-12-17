#ifndef ASIO_EXAMPLE_H
#define ASIO_EXAMPLE_H

#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/attributes/named_scope.hpp>



using boost::asio::ip::tcp;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        BOOST_LOG_NAMED_SCOPE("tcp_connection::start");
        message_ = make_daytime_string();
        BOOST_LOG_TRIVIAL(info) << "Starting connection with message: " << message_;
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            std::bind(&tcp_connection::handle_write, shared_from_this()));
    }

private:
    tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

    void handle_write() {
        BOOST_LOG_NAMED_SCOPE("tcp_connection::handle_write");
        BOOST_LOG_TRIVIAL(info) << "Message sent successfully.";
    }

    std::string make_daytime_string() {
        auto now = std::time(nullptr);
        return std::ctime(&now);
    }

    tcp::socket socket_;
    std::string message_;
};

class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context, unsigned short port = 12345)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    void init_logging() {
        boost::log::add_common_attributes();
        boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
        boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%] [%Scope%]: %Message%");
        BOOST_LOG_TRIVIAL(info) << "Logging initialized.";
    }

    void start_accept() {
        tcp_connection::pointer new_connection = tcp_connection::create(io_context_);
        acceptor_.async_accept(new_connection->socket(),
            std::bind(&tcp_server::handle_accept, this, new_connection,
                      std::placeholders::_1));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error) {
        BOOST_LOG_NAMED_SCOPE("tcp_server::handle_accept");
        if (!error) {
            BOOST_LOG_TRIVIAL(info) << "Accepted new connection.";
            new_connection->start();
        } else {
            BOOST_LOG_TRIVIAL(error) << "Failed to accept connection: " << error.message();
        }
        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

#endif // ASIO_EXAMPLE_H
