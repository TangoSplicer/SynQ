// SynQ Commercial Attribution License v1.0
// Author: SynQ Contributors
// CLI tool: Verify and configure plugin signing quorum

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

std::string quorum_path = "build/quorum_config.json";
std::string signatures_path = "build/plugin_signatures.json";
std::string keyring_path = "build/keyring_registry.json";

void show_keys() {
    std::ifstream file(keyring_path);
    if (!file) {
        std::cout << "No keyring found at " << keyring_path << "\n";
        return;
    }
    json keys;
    file >> keys;
    for (auto& [k, v] : keys.items()) {
        std::cout << "Fingerprint: " << k << " | Owner: " << v["owner"] << "\n";
    }
}

void check_quorum(const std::string& plugin_id) {
    std::ifstream qf(quorum_path), sf(signatures_path);
    if (!qf || !sf) {
        std::cout << "Missing quorum or signature data.\n";
        return;
    }
    json quorum, signatures;
    qf >> quorum;
    sf >> signatures;

    if (!quorum.contains(plugin_id)) {
        std::cout << "No quorum config found for plugin: " << plugin_id << "\n";
        return;
    }

    int required = quorum[plugin_id]["required"];
    int signed_count = signatures[plugin_id].size();
    std::cout << "Quorum for '" << plugin_id << "': " << signed_count << " of " << required << " signatures\n";
    std::cout << (signed_count >= required ? "✅ Quorum met.\n" : "❌ Quorum not met.\n");
}

void set_quorum(const std::string& plugin_id, int required, int total) {
    std::ifstream infile(quorum_path);
    json conf;
    if (infile) infile >> conf;
    conf[plugin_id] = {{"required", required}, {"total", total}};
    std::ofstream outfile(quorum_path);
    outfile << conf.dump(2);
    std::cout << "Set quorum for " << plugin_id << ": " << required << " of " << total << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: synq quorum <show-keys|check|set> [args]\n";
        std::cout << "  --quorum-path <path>\n";
        std::cout << "  --signatures-path <path>\n";
        std::cout << "  --keyring-path <path>\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--quorum-path" && i + 1 < argc) {
            quorum_path = argv[++i];
        } else if (arg == "--signatures-path" && i + 1 < argc) {
            signatures_path = argv[++i];
        } else if (arg == "--keyring-path" && i + 1 < argc) {
            keyring_path = argv[++i];
        }
    }

    std::string cmd = argv[1];
    if (cmd == "show-keys") {
        show_keys();
    } else if (cmd == "check" && argc >= 3) {
        check_quorum(argv[2]);
    } else if (cmd == "set" && argc >= 5) {
        std::string plugin_id = argv[2];
        int req = std::stoi(argv[3]);
        int total = std::stoi(argv[4]);
        set_quorum(plugin_id, req, total);
    } else {
        std::cout << "Invalid command or arguments.\n";
    }

    return 0;
}