#include "VirtualFile.h"

VirtualFile::VirtualFile(const std::vector<uint8_t>& data, const std::string& name)
: data_(data), name_(name) {}

const std::vector<uint8_t>& VirtualFile::getBytes() const { return data_; }
const std::string& VirtualFile::getName() const { return name_; }
