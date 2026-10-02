#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "Crc32.h"
#include "DataStream.h"
#include "Fnv32a.h"
#include "IniFile.h"
#include "MersenneTwister.h"
#include "MixEntry.h"
#include "Prng.h"

static void require(bool condition, const std::string& msg) {
    if (!condition) {
        std::cerr << "TEST FAILED: " << msg << "\n";
        std::exit(1);
    }
}

int main() {
    using std::string;
    using std::vector;

    const string s = "123456789";
    const uint32_t crc = Crc32::calculateCrc(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    std::cout << "CRC32(\"123456789\") = 0x" << std::hex << crc << std::dec << "\n";
    require(crc == 0xCBF43926u, "CRC32 mismatch for 123456789");

    const uint32_t fnv = fnv1a_32(reinterpret_cast<const uint8_t*>(s.data()), s.size());
    std::cout << "FNV32A(\"123456789\") = 0x" << std::hex << fnv << std::dec << "\n";
    require(fnv != 0u, "FNV32A returned zero");

    MersenneTwister mt(5489u);
    const double first = mt.random();
    const uint32_t approx = static_cast<uint32_t>(first * 4294967296.0);
    std::cout << "MT(5489) first random approx = " << approx << "\n";
    require(approx != 0u, "MersenneTwister yielded zero");

    Prng prng = Prng::factory_from_string("123", 7);
    const int v = prng.generateRandomInt(0, 9);
    const double r = prng.generateRandom();
    std::cout << "PRNG int = " << v << ", PRNG random = " << r << "\n";
    require(v >= 0 && v <= 9, "Prng.generateRandomInt range failure");

    vector<uint8_t> raw = {1, 2, 3, 4, 5, 0, 'a', 'b', 0};
    DataStream ds(raw);
    require(ds.readUint8() == 1, "DataStream::readUint8 failed");
    require(ds.readUint16LE() == 0x0302u, "DataStream::readUint16LE failed");
    require(ds.readUint32LE() == 0x05040302u, "DataStream::readUint32LE failed");
    ds.seek(5);
    std::string cs = ds.readCString(8);
    require(cs.empty(), "DataStream::readCString with leading null mismatch");

    IniFile ini;
    ini.parse("[main]\nkey=hello\n[main]\nother=world\n");
    require(ini.get("main", "key") == "hello", "IniFile get failed");
    require(ini.get("main", "other") == "world", "IniFile duplicate-section handling failed");

    const uint32_t mixHash = MixEntry::hashFilename("ART.INI", false);
    std::cout << "MixEntry.hashFilename(ART.INI) = 0x" << std::hex << mixHash << std::dec << "\n";
    require(mixHash != 0u, "MixEntry hash returned zero");

    std::cout << "All native deterministic core tests passed.\n";
    return 0;
}
