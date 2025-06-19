// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_META_IMPORTER_H
#define SYNQ_META_IMPORTER_H

#include <string>

namespace synq {

class MetaImporter {
public:
    static void importAlias(const std::string& name, const std::string& realModule);
};

}

#endif // SYNQ_META_IMPORTER_H