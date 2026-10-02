#include "DataStream.h"
#include <algorithm>
#include <cstring>
#include <limits>

DataStream::DataStream(const std::vector<uint8_t>& data) : buf_(data), pos_(0) {}
DataStream::DataStream(const uint8_t* data, size_t size) : buf_(data, data + size), pos_(0) {}

size_t DataStream::position() const { return pos_; }
void DataStream::seek(size_t offset) {
    pos_ = std::min(offset, buf_.size());
}
bool DataStream::isEof() const { return pos_ >= buf_.size(); }
size_t DataStream::byteLength() const { return buf_.size(); }

uint8_t DataStream::readUint8() {
    if (pos_ >= buf_.size()) return 0;
    return buf_[pos_++];
}

int8_t DataStream::readInt8() {
    return static_cast<int8_t>(readUint8());
}

uint16_t DataStream::readUint16LE() {
    uint16_t value = 0;
    if (pos_ + 1 < buf_.size()) {
        value = static_cast<uint16_t>(buf_[pos_]) |
                (static_cast<uint16_t>(buf_[pos_ + 1]) << 8);
        pos_ += 2;
    } else if (pos_ < buf_.size()) {
        value = static_cast<uint16_t>(buf_[pos_++]);
    }
    return value;
}

int16_t DataStream::readInt16LE() {
    return static_cast<int16_t>(readUint16LE());
}

uint32_t DataStream::readUint32LE() {
    uint32_t value = 0;
    if (pos_ + 3 < buf_.size()) {
        value = static_cast<uint32_t>(buf_[pos_]) |
                (static_cast<uint32_t>(buf_[pos_ + 1]) << 8) |
                (static_cast<uint32_t>(buf_[pos_ + 2]) << 16) |
                (static_cast<uint32_t>(buf_[pos_ + 3]) << 24);
        pos_ += 4;
    } else {
        size_t count = 0;
        while (pos_ < buf_.size() && count < 4) {
            value |= static_cast<uint32_t>(buf_[pos_++]) << (8 * count);
            ++count;
        }
    }
    return value;
}

int32_t DataStream::readInt32LE() {
    return static_cast<int32_t>(readUint32LE());
}

float DataStream::readFloat32LE() {
    const uint32_t bits = readUint32LE();
    float result = 0.0f;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

double DataStream::readFloat64LE() {
    uint64_t bits = 0;
    size_t available = std::min<size_t>(8u, buf_.size() - pos_);
    for (size_t i = 0; i < available; ++i) {
        bits |= static_cast<uint64_t>(buf_[pos_ + i]) << (8 * i);
    }
    pos_ += available;
    double result = 0.0;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

std::vector<uint8_t> DataStream::readUint8Array(size_t count) {
    const size_t available = std::min(count, buf_.size() - pos_);
    std::vector<uint8_t> out(buf_.begin() + static_cast<std::ptrdiff_t>(pos_),
                            buf_.begin() + static_cast<std::ptrdiff_t>(pos_ + available));
    pos_ += available;
    return out;
}

std::string DataStream::readCString(size_t maxLength) {
    const size_t remaining = buf_.size() - pos_;
    const size_t searchLength = (maxLength == static_cast<size_t>(-1)) ? remaining : std::min(maxLength, remaining);
    size_t nullIndex = 0;
    while (nullIndex < searchLength && buf_[pos_ + nullIndex] != 0) {
        ++nullIndex;
    }

    std::string out;
    out.reserve(nullIndex);
    for (size_t i = 0; i < nullIndex; ++i) {
        out.push_back(static_cast<char>(buf_[pos_ + i]));
    }

    pos_ += nullIndex;
    if (maxLength == static_cast<size_t>(-1)) {
        if (nullIndex != remaining) {
            pos_ += 1; // consume null terminator
        }
    } else {
        pos_ += (searchLength - nullIndex);
    }

    if (pos_ > buf_.size()) pos_ = buf_.size();
    return out;
}

const std::vector<uint8_t>& DataStream::getBytes() const {
    return buf_;
}
