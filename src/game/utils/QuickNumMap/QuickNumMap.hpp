#pragma once

#include <utility>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <exception>
#include <algorithm>

template <typename T, typename K>
class QuickNumMap {
private:
    std::vector<std::pair<T, K>> pairs;
    bool listSorted = false;
public:
    QuickNumMap();
    ~QuickNumMap();

    void addItem(T key, K& val);
    void addItem(T key, K&& val);
    void sortList();

    K& quickSearch(T key);

    std::vector<std::pair<T, K>>::iterator begin();
    std::vector<std::pair<T, K>>::iterator end();
};

#ifdef QuickNumMapImpl

template<typename T, typename K>
QuickNumMap<T, K>::QuickNumMap() {}

template<typename T, typename K>
QuickNumMap<T, K>::~QuickNumMap() {}

template<typename T, typename K>
void QuickNumMap<T, K>::addItem(T key, K& val) {
    pairs.push_back(std::pair<T, K>(key, val));
}

template<typename T, typename K>
void QuickNumMap<T, K>::addItem(T key, K&& val) {
    pairs.push_back(std::pair<T, K>(key, val));
    listSorted = false;
}

template<typename T, typename K>
void QuickNumMap<T, K>::sortList() {
    std::sort(pairs.begin(), pairs.end(), [](const auto& first, const auto& second) {
        return first.first < second.first;
    });
    listSorted = true;
}

template<typename T, typename K>
K& QuickNumMap<T, K>::quickSearch(T key) {
    std::size_t l = 0;
    std::size_t r = pairs.size() - 1;

    // Implement binary search lookup
    while (l <= r) {
        std::size_t m = l + (r - l) / 2;
        auto v = pairs.at(m).first;
 
        // Check if x is present at mid
        if (v == key)
            return pairs.at(m).second;
 
        // If x greater, ignore left half
        if (v < key)
            l = m + 1;
 
        // If x is smaller, ignore right half
        else
            r = m - 1;
    }
 
    // If we reach here, then element was not present
    throw std::runtime_error("No block found");
}

template<typename T, typename K>
std::vector<std::pair<T, K>>::iterator QuickNumMap<T, K>::begin() {
    return pairs.begin();
}

template<typename T, typename K>
std::vector<std::pair<T, K>>::iterator QuickNumMap<T, K>::end() {
    return pairs.end();
}

#endif