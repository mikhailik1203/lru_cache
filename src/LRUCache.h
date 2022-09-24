#pragma once

#include<unordered_map>
#include<list>

namespace cache {
    /// @brief  Vanilla implementation of the LRU cache
    /// @tparam K - key type 
    /// @tparam V - value type
    /// @tparam capacity of the cache
    template<typename K, typename V, int capacity = 100>
    class LRUCache {
    private:
        std::list<K> items;
    public:
        std::unordered_map<K, std::pair<V, typename std::list<K>::iterator>> keyValuesMap;

        LRUCache(){
            keyValuesMap.reserve(capacity);
        }
        ~LRUCache() = default;

    public:
        /// @brief Adds new key-value to the cache
        /// @param key
        /// @param value
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

        /// @brief Locates key in the cache and returns value
        /// @param key 
        /// @return value or none, if key is not found
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
