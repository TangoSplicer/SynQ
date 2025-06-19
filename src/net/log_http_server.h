// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// log_http_server.h
#pragma once
#include <thread>
#include <atomic>
#include <vector>
#include <string>

namespace synq::net {

  class LogHTTPServer {
  public:
    LogHTTPServer(int port = 8088);
    void start();
    void stop();
    void append(const std::string& msg);

  private:
    int port;
    std::atomic<bool> running;
    std::vector<std::string> logs;
    std::thread server_thread;

    void serve();
  };

}