// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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