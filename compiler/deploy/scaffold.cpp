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
#include "scaffold.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

namespace synq {
namespace deploy {

Scaffold::Scaffold(const std::string& name)
    : appName(name), deployRoot("deploy/" + name) {}

void Scaffold::generate() {
    initDirectories();
    writeManifest();
    injectDefaults();
    exportDocker();
    printSummary();
}

void Scaffold::initDirectories() {
    fs::create_directories(deployRoot + "/manifests");
    fs::create_directories(deployRoot + "/scripts");
    fs::create_directories(deployRoot + "/configs");
    fs::create_directories(deployRoot + "/binaries");
    cout << "[✓] Created deployment folder structure." << endl;
}

void Scaffold::writeManifest() {
    ofstream file(deployRoot + "/manifests/deploy.yaml");
    file << "app: " << appName << endl;
    file << "entry: main.synq" << endl;
    file << "runtime: hybrid" << endl;
    file << "quantum_backend: auto" << endl;
    file << "ai_model: synq-native" << endl;
    file.close();
    cout << "[✓] Wrote deploy.yaml manifest." << endl;
}

void Scaffold::injectDefaults() {
    ofstream model(deployRoot + "/configs/model.json");
    model << "{\\n  \\\"default_model\\\": \\\"synq-native\\\",\\n  \\\"routing\\\": []\\n}" << endl;
    model.close();

    ofstream qcfg(deployRoot + "/configs/.qcfg");
    qcfg << "backend: SynQSim\\nthreads: 4" << endl;
    qcfg.close();

    ofstream script(deployRoot + "/scripts/run.sh");
    script << "#!/bin/bash\\n"
           << "echo 'Launching SynQ app: " << appName << "'\\n"
           << "synq run main.synq --cfg ./configs/.qcfg --model ./configs/model.json" << endl;
    script.close();

    cout << "[✓] Injected default configs and scripts." << endl;
}

void Scaffold::exportDocker() {
    ofstream docker(deployRoot + "/Dockerfile");
    docker << "FROM synq/base:latest\\n"
           << "COPY . /app\\n"
           << "WORKDIR /app\\n"
           << "CMD [\\\"synq\\\", \\\"run\\\", \\\"main.synq\\\"]" << endl;
    docker.close();
    cout << "[✓] Exported Dockerfile." << endl;
}

void Scaffold::printSummary() {
    cout << "\\nDeployment scaffold generated for app: " << appName << endl;
    cout << "Location: " << deployRoot << endl;
    cout << "Includes: manifests/, configs/, scripts/, binaries/, Dockerfile" << endl;
}

} // namespace deploy
} // namespace synq