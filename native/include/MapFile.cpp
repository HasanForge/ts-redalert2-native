#include "MapFile.h"
#include "IniFile.h"
#include "Format5.h"
#include "DataStream.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

MapFile::MapFile(const std::string& text) {
    parseIni(text);
    // find IsoMapPack5 section
    std::string pack = getSectionConcatenatedValue("IsoMapPack5");
    if (!pack.empty()) {
        auto compressed = Format5::base64ToBytes(pack);
        // compute expected decoded size: TS uses i = (2*width-1)*height; decodedData = new Uint8Array(11*i+4)
        int a = 2 * width_ - 1;
        int i = a * height_;
        size_t decodedSize = 11u * i + 4u;
        std::vector<uint8_t> decoded(decodedSize);
        try {
            Format5::decodeInto(compressed, decoded);
        } catch (const std::exception& ex) {
            // If compressed blocks not supported, rethrow with context
            throw;
        }
        DataStream ds(decoded.data(), decoded.size());
        int aWidth = 2 * width_ - 1;
        tiles_.resize(aWidth * height_);
        for (int T = 0; T < i; ++T) {
            uint16_t rx = ds.readUint16LE();
            uint16_t ry = ds.readUint16LE();
            int16_t tileNum = ds.readInt16LE();
            ds.readInt16LE();
            uint8_t sub = ds.readUint8();
            uint8_t z = ds.readUint8();
            ds.readUint8();
            int dx = (int)rx - (int)ry + width_ - 1;
            int dy = (int)rx + (int)ry - width_ - 1;
            if (0 <= dx && dx < 2 * width_ && 0 <= dy && dy < 2 * height_) {
                MapTile t; t.dx = dx; t.dy = dy; t.rx = rx; t.ry = ry; t.z = z; t.tileNum = tileNum; t.subTile = sub;
                int h = dx * height_ + (dy/2);
                if (h >= 0 && h < (int)tiles_.size()) tiles_[h] = t;
            }
        }
    }
}

void MapFile::parseIni(const std::string& text) {
    IniFile ini;
    ini.parse(text);
    // Look for [Map] or [Basic] to get width/height
    std::vector<std::string> sections = ini.getSections();
    for (const auto& sec : sections) {
        if (sec == "Map" || sec == "Basic") {
            std::string w = ini.get(sec, "SizeX", "0");
            std::string h = ini.get(sec, "SizeY", "0");
            if (w != "0" && h != "0") {
                width_ = std::stoi(w);
                height_ = std::stoi(h);
                break;
            }
        }
    }
}

std::string MapFile::getSectionConcatenatedValue(const std::string& section) const {
    // The IniFile in native stores keys; we will look for a key named "" (empty) that stores concatenated values
    // For simplicity, combine all keys in section
    // TODO: implement proper getConcatenatedValues behavior; use key ordering
    return "";
}

std::string MapFile::getName() const { return name_; }
int MapFile::getWidth() const { return width_; }
int MapFile::getHeight() const { return height_; }
int MapFile::getTileCount() const { return (int)tiles_.size(); }
int MapFile::getDecodedIsoTilesCount() const { return (int)tiles_.size(); }
