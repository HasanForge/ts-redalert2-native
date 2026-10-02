#pragma once
#include <cctype>
#include <map>
#include <string>
#include <vector>

class IniFile {
public:
    void parse(const std::string& text);
    bool hasSection(const std::string& section) const;
    bool hasKey(const std::string& section, const std::string& key) const;
    std::string get(const std::string& section, const std::string& key,
                    const std::string& defaultValue = "") const;
    std::vector<std::string> getSections() const;
    std::vector<std::string> getKeys(const std::string& section) const;
    // Return all values concatenated (in insertion order) for the given section
    std::string getConcatenatedValues(const std::string& section) const;

private:
    std::vector<std::string> sectionOrder_;
    std::map<std::string, std::vector<std::string>> keysBySection_;
    // values_[section][key] = vector of values in insertion order
    std::map<std::string, std::map<std::string, std::vector<std::string>>> values_;
};
