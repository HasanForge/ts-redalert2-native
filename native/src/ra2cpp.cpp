#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Crc32.h"
#include "Fnv32a.h"
#include "IniFile.h"
#include "MixEntry.h"
#include "Prng.h"

int main(int argc, char** argv) {
    std::cout << "ra2cpp native headless deterministic core\n";

    if (argc < 2 || std::string(argv[1]) != "--headless") {
        std::cout << "Usage: ra2cpp.exe --headless <game-data>\n";
        return 1;
    }

    const std::string test = "123456789";
    const uint32_t crc = Crc32::calculateCrc(
        reinterpret_cast<const uint8_t*>(test.data()), test.size());
    std::cout << "CRC32(\"123456789\") = 0x" << std::hex << crc << std::dec << "\n";
    if (crc != 0xCBF43926u) {
        std::cerr << "CRC32 mismatch\n";
        return 2;
    }

    const uint32_t fnv = fnv1a_32(reinterpret_cast<const uint8_t*>(test.data()), test.size());
    std::cout << "FNV32A(\"123456789\") = 0x" << std::hex << fnv << std::dec << "\n";

    Prng prng = Prng::factory_from_string("12345", 0);
    std::cout << "PRNG sample: " << prng.generateRandom() << " | " << prng.generateRandomInt(0, 10) << "\n";

    IniFile ini;
    ini.parse("[section]\nkey=value\n# comment\n[section]\nother=1\n");
    std::cout << "INI section count = " << ini.getSections().size() << "\n";
    std::cout << "INI key value = " << ini.get("section", "key") << "\n";

    const uint32_t mixHash = MixEntry::hashFilename("ART.INI", false);
    std::cout << "MixEntry.hashFilename(ART.INI) = 0x" << std::hex << mixHash << std::dec << "\n";

    return 0;
}
