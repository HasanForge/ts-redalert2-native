#pragma once
#include <map>
#include <string>
#include <vector>

template <typename K, typename V>
class OrderedMap {
public:
    void set(const K& key, const V& value) {
        if (values_.find(key) == values_.end()) {
            order_.push_back(key);
        }
        values_[key] = value;
    }

    bool contains(const K& key) const {
        return values_.find(key) != values_.end();
    }

    V& get(const K& key) {
        return values_[key];
    }

    const V& get(const K& key) const {
        return values_.at(key);
    }

    const std::vector<K>& keys() const {
        return order_;
    }

private:
    std::vector<K> order_;
    std::map<K, V> values_;
};
