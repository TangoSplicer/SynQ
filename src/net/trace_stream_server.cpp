// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// trace_stream_server.cpp
#include "trace_stream_server.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace synq::net {

  TraceStreamServer::TraceStreamServer(int port)
    : port(port), running(false) {}

  void TraceStreamServer::start() {
    running = true;
    server_thread = std::thread(&TraceStreamServer::serve, this);
  }

  void TraceStreamServer::stop() {
    running = false;
    if (server_thread.joinable()) server_thread.join();
  }

  void TraceStreamServer::push_trace(const synq::compiler::TraceEntry& entry) {
    buffer.push_back(entry);
    // In real system, this would push via websocket
    std::cout << "📡 Trace pushed: " << entry.stage << " (" << entry.type << ")\n";
  }

  void TraceStreamServer::serve() {
    std::cout << "🌐 TraceStreamServer listening on port " << port << "\n";
    while (running) {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      std::cout << "🛰️ Streaming buffer size: " << buffer.size() << "\n";
    }
  }

}