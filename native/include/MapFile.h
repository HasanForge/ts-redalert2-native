#pragma once
#include <string>
#include <vector>
#include <optional>

struct MapTile {
    int dx, dy;
    int rx, ry;
    int z;
    int tileNum;
    int subTile;
};

class MapFile {
public:
    explicit MapFile(const std::string& text);
    std::string getName() const;
    int getWidth() const;
    int getHeight() const;
    int getTileCount() const;
    int getDecodedIsoTilesCount() const;
    // parse IsoMapPack5 and provide tiles
private:
    std::string name_;
    int width_ = 0;
    int height_ = 0;
    std::vector<MapTile> tiles_;
    void parseIni(const std::string& text);
    std::string getSectionConcatenatedValue(const std::string& section) const;
};
