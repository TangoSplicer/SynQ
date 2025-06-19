// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// log_http_server.cpp
#include "log_http_server.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace synq::net {

  LogHTTPServer::LogHTTPServer(int port)
    : port(port), running(false) {}

  void LogHTTPServer::append(const std::string& msg) {
    logs.push_back(msg);
  }

  void LogHTTPServer::start() {
    running = true;
    server_thread = std::thread(&LogHTTPServer::serve, this);
  }

  void LogHTTPServer::stop() {
    running = false;
    if (server_thread.joinable()) server_thread.join();
  }

  void LogHTTPServer::serve() {
    std::cout << "🧠 Log HTTP server running on port " << port << "\n";
    while (running) {
      std::this_thread::sleep_for(std::chrono::seconds(3));
      std::cout << "📖 Logs available: " << logs.size() << "\n";
    }
  }

}