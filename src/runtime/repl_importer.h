// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_REPL_IMPORTER_H
#define SYNQ_REPL_IMPORTER_H

#include <string>

namespace synq {

class REPLImporter {
public:
    static void import(const std::string& module);
    static void unload(const std::string& module);
};

}

#endif // SYNQ_REPL_IMPORTER_H