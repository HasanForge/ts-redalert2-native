#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>

class MixFile {
public:
    explicit MixFile(const std::vector<uint8_t>& data);
    bool containsFile(const std::string& filename) const;
    std::vector<uint8_t> openFile(const std::string& filename) const; // returns file bytes
private:
    struct Entry { uint32_t hash; uint32_t offset; uint32_t length; };
    std::map<uint32_t, Entry> index_;
    std::vector<uint8_t> data_;
    size_t dataStart_;
    void parseTdHeader(const std::vector<uint8_t>& src, size_t& pos);
};
