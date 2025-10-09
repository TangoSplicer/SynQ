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
#include <iostream>
#include <fstream>
#include <string>

void writePlugin(const std::string& name, const std::string& desc) {
    std::string filename = name + ".synq";
    std::ofstream file(filename);
    file << "// Plugin: " << filename << "\n";
    file << "// Description: " << desc << "\n\n";
    file << "plugin " << name << " {\n";
    file << "  version: \"1.0.0\"\n";
    file << "  description: \"" << desc << "\"\n\n";
    file << "  macro run(input) {\n";
    file << "    // your logic here\n";
    file << "    return input;\n";
    file << "  }\n";
    file << "}\n";
    file.close();
    std::cout << "[✓] Plugin created: " << filename << std::endl;
}

int main() {
    std::string name, desc;
    std::cout << "Plugin Name: ";
    std::getline(std::cin, name);
    std::cout << "Plugin Description: ";
    std::getline(std::cin, desc);
    writePlugin(name, desc);
    return 0;
}