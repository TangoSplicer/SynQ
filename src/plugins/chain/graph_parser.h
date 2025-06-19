// synq/src/plugins/chain/graph_parser.h
// Parser for SynQ Chain Task Graph JSON/YAML
// License: SynQ Commercial Attribution License v1.0

#pragma once
#include <map>
#include <string>
#include "chain_engine.h"

namespace synq::chain {

std::map<std::string, ChainTask> parse_chain_graph(const std::string& filepath);

}