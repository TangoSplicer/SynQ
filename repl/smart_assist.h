// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// smart_assist.h
#pragma once
#include <string>
#include <vector>
#include <map>

namespace synq::repl {

  class SmartAssist {
  public:
    std::string get_doc_for(const std::string& token);
    std::vector<std::string> suggest_completions(const std::string& prefix);
  private:
    std::map<std::string, std::string> docs;
    void load_builtin_docs();
  };

}