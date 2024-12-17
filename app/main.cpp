// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <cstdlib>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "exampleConfig.h"
#include "example.h"

#include <thread>
#include "asio_example.h"

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

static void run_server() {
    try {
        BOOST_LOG_NAMED_SCOPE("run_server");
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Server exception: " << e.what();
    }
}

static void run_client(const std::string& host) {
    try {
        BOOST_LOG_NAMED_SCOPE("run_client");
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(host, "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;) {
            std::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);

            BOOST_LOG_TRIVIAL(info) << std::string(buf.data(), len);
        }
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Client exception: " << e.what();
    }
}

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
    logging::core::get()->add_global_attribute("Scope", logging::attributes::named_scope());

    logging::add_console_log(
        std::cout,
        keywords::format = "[%TimeStamp%] <%Severity%> [%Scope%]: %Message%"
    );

    logging::add_common_attributes();

    BOOST_LOG_NAMED_SCOPE("Main");

    BOOST_LOG_TRIVIAL(info) << "C++ Boiler Plate v"
                            << PROJECT_VERSION_MAJOR
                            << "."
                            << PROJECT_VERSION_MINOR
                            << "."
                            << PROJECT_VERSION_PATCH
                            << "."
                            << PROJECT_VERSION_TWEAK;

    std::system("cat ../LICENSE");


  /* Dummy example */
  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  Dummy d = Dummy();

  // /* Asio example */
  std::thread server_thread(run_server);

  // Allow server to start
  std::this_thread::sleep_for(std::chrono::seconds(1));

  run_client("127.0.0.1");

  server_thread.join();

  return d.doSomething() ? 0 : -1;
}
