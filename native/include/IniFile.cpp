#include "IniFile.h"
#include <cctype>
#include <sstream>

static inline std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

void IniFile::parse(const std::string& text) {
    std::istringstream ss(text);
    std::string line;
    std::string currentSection;
    while (std::getline(ss, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        if (trimmed[0] == ';' || trimmed[0] == '#') continue;

        if (trimmed.size() >= 2 && trimmed.front() == '[' && trimmed.back() == ']') {
            currentSection = trim(trimmed.substr(1, trimmed.size() - 2));
            sectionOrder_.push_back(currentSection);
            keysBySection_[currentSection] = std::vector<std::string>();
            continue;
        }

        const auto pos = trimmed.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(trimmed.substr(0, pos));
            std::string value = trim(trimmed.substr(pos + 1));
            auto& sectionValues = values_[currentSection];
            auto& sectionKeys = keysBySection_[currentSection];
            if (sectionValues.find(key) == sectionValues.end()) {
                sectionKeys.push_back(key);
            }
            sectionValues[key] = value;
        }
    }
}

bool IniFile::hasSection(const std::string& section) const {
    return values_.find(section) != values_.end();
}

bool IniFile::hasKey(const std::string& section, const std::string& key) const {
    auto it = values_.find(section);
    if (it == values_.end()) return false;
    return it->second.find(key) != it->second.end();
}

std::string IniFile::get(const std::string& section, const std::string& key,
                        const std::string& defaultValue) const {
    auto it = values_.find(section);
    if (it == values_.end()) return defaultValue;
    auto keyIt = it->second.find(key);
    if (keyIt == it->second.end()) return defaultValue;
    return keyIt->second;
}

std::vector<std::string> IniFile::getSections() const {
    return sectionOrder_;
}

std::vector<std::string> IniFile::getKeys(const std::string& section) const {
    auto it = keysBySection_.find(section);
    if (it == keysBySection_.end()) return {};
    return it->second;
}
