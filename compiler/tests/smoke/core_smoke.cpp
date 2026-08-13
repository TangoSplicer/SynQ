#include "compiler/ir/binary_format.h"

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

}  // namespace

int main() {
    using synq::compiler::ir::BinaryReader;
    using synq::compiler::ir::BinaryWriter;

    BinaryWriter writer;
    writer.write_u8(42);
    writer.write_u32(0xA1B2C3D4U);
    writer.write_string("synq-core-smoke");

    BinaryReader reader(writer.get_buffer());
    if (!require(reader.read_u8() == 42, "u8 round trip")) return 1;
    if (!require(reader.read_u32() == 0xA1B2C3D4U, "u32 round trip")) return 1;
    if (!require(reader.read_string() == "synq-core-smoke", "string round trip")) return 1;
    if (!require(reader.at_end(), "reader consumes complete buffer")) return 1;

    bool read_past_end_rejected = false;
    try {
        static_cast<void>(reader.read_u8());
    } catch (const std::runtime_error&) {
        read_past_end_rejected = true;
    }
    if (!require(read_past_end_rejected, "reader rejects reads past the end")) return 1;

    std::cout << "SynQ core smoke test passed\n";
    return 0;
}
