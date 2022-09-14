#pragma once

#include<unordered_map>
#include<list>

namespace cache {
    template<typename K, typename V, int capacity = 100>
    class LRUCache {

    private:
        std::list<K> items;
    public:
        std::unordered_map<K, std::pair<V, typename std::list<K>::iterator>> keyValuesMap;

    public:
        void add(const K key, const V value) {
            auto pos = keyValuesMap.find(key);
            if (pos == keyValuesMap.end()) {
                items.push_front(key);
                keyValuesMap[key] = {value, items.begin()};
                if (keyValuesMap.size() > capacity) {
                    keyValuesMap.erase(items.back());
                    items.pop_back();
                }
            } else {
                items.erase(pos->second.second);
                items.push_front(key);
                keyValuesMap[key] = {value, items.begin()};
            }
        }

        std::optional<V> get(const K key) {
            auto pos = keyValuesMap.find(key);
            if (pos == keyValuesMap.end())
                return std::nullopt;
            if (pos->second.second != items.begin()) {
                items.erase(pos->second.second);
                items.push_front(key);
                keyValuesMap[key] = {pos->second.first, items.begin()};
            }
            return pos->second.first;
        }
    };
}
