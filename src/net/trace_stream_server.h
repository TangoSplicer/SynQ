// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// trace_stream_server.h
#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include "../compiler/execution_tracer.h"

namespace synq::net {

  class TraceStreamServer {
  public:
    TraceStreamServer(int port = 8765);
    void start();
    void stop();
    void push_trace(const synq::compiler::TraceEntry& entry);

  private:
    int port;
    std::thread server_thread;
    std::atomic<bool> running;
    std::vector<synq::compiler::TraceEntry> buffer;

    void serve();
  };

}