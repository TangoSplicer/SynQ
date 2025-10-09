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


// Author: SynQ Contributors
// CLI: Remote Plugin Publishing and Management (SynQHUB)

#include <iostream>
#include <string>
#include <pybind11/embed.h>

namespace py = pybind11;

void publish_remote(const std::string& plugin, const std::string& team) {
    py::scoped_interpreter guard{};
    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("append")("src/plugins/registry");
    py::module_ remote_hub = py::module_::import("remote_hub");
    remote_hub.attr("publish_plugin_to_synqhub")(plugin, team);
}

void pull_remote(const std::string& plugin) {
    py::scoped_interpreter guard{};
    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("append")("src/plugins/registry");
    py::module_ remote_hub = py::module_::import("remote_hub");
    remote_hub.attr("pull_plugin_from_synqhub")(plugin);
}

void sync_metadata() {
    py::scoped_interpreter guard{};
    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("append")("src/plugins/registry");
    py::module_ remote_hub = py::module_::import("remote_hub");
    remote_hub.attr("sync_synqhub_metadata")();
}

void verify_plugin(const std::string& plugin) {
    py::scoped_interpreter guard{};
    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("append")("src/plugins/registry");
    py::module_ remote_hub = py::module_::import("remote_hub");
    remote_hub.attr("validate_remote_signature")(plugin);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "SynQHUB CLI:\n"
                  << "  synq publish --remote <plugin> [team]\n"
                  << "  synq pull <plugin>\n"
                  << "  synq sync-meta\n"
                  << "  synq verify <plugin>\n";
        return 1;
    }

    std::string command = argv[1];
    if (command == "publish" && argc >= 4 && std::string(argv[2]) == "--remote") {
        publish_remote(argv[3], argc > 4 ? argv[4] : "default");
    } else if (command == "pull" && argc >= 3) {
        pull_remote(argv[2]);
    } else if (command == "sync-meta") {
        sync_metadata();
    } else if (command == "verify" && argc >= 3) {
        verify_plugin(argv[2]);
    } else {
        std::cout << "Invalid SynQHUB command or arguments.\n";
    }

    return 0;
}