#pragma once
#include <cstdint>
#include <vector>
#include <string>

class VirtualFile {
public:
    VirtualFile(const std::vector<uint8_t>& data, const std::string& name);
    const std::vector<uint8_t>& getBytes() const;
    const std::string& getName() const;
private:
    std::vector<uint8_t> data_;
    std::string name_;
};
