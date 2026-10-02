#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class DataStream {
public:
    explicit DataStream(const std::vector<uint8_t>& data);
    DataStream(const uint8_t* data, size_t size);

    size_t position() const;
    void seek(size_t offset);
    bool isEof() const;
    size_t byteLength() const;

    uint8_t readUint8();
    int8_t readInt8();
    uint16_t readUint16LE();
    int16_t readInt16LE();
    uint32_t readUint32LE();
    int32_t readInt32LE();
    float readFloat32LE();
    double readFloat64LE();

    std::vector<uint8_t> readUint8Array(size_t count);
    std::string readCString(size_t maxLength = static_cast<size_t>(-1));

    const std::vector<uint8_t>& getBytes() const;

private:
    std::vector<uint8_t> buf_;
    size_t pos_;
};
