// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEPLOY_SCAFFOLD_H
#define SYNQ_DEPLOY_SCAFFOLD_H

#include <string>

namespace synq {
namespace deploy {

class Scaffold {
public:
    explicit Scaffold(const std::string& appName);
    void generate();
    void writeManifest();
    void initDirectories();
    void injectDefaults();
    void exportDocker();
    void printSummary();

private:
    std::string appName;
    std::string deployRoot;
};

} // namespace deploy
} // namespace synq

#endif // SYNQ_DEPLOY_SCAFFOLD_H