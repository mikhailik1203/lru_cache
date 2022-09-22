#pragma once

#include "HashVector.h"

#include <optional>
#include <array>
#include <iostream>

namespace cache {
    
    /// @brief LRU cache based on vector container (to reduce allocations). Key, value and auxiliary parameters 
    ///     stored in separate containers 
    /// @tparam KeyT 
    /// @tparam ValT 
    /// @tparam CapacityT 
    template<typename KeyT, typename ValT, int CapacityT>
    class CustLRUCacheCHV {
        const int DUMMY_IDX = 0;
    public:
        CustLRUCacheCHV():key_to_idx_(CapacityT) {
            // add dummy item to avoid checking negative index values
            key_data_.reserve(CapacityT + 1);
            key_data_.push_back(KeyT());
            val_data_.reserve(CapacityT + 1);
            val_data_.push_back(ValT());
            links_data_.reserve(CapacityT + 1);
            links_data_.push_back({DUMMY_IDX, DUMMY_IDX});
            cache_has_space_ = true;
        }

        ~CustLRUCacheCHV() = default;

        /// @brief  just to simplify implementation - disable copy and move
        CustLRUCacheCHV(const CustLRUCacheCHV &) = delete;
        CustLRUCacheCHV(CustLRUCacheCHV &&) = delete;
        CustLRUCacheCHV &operator=(const CustLRUCacheCHV &) = delete;
        CustLRUCacheCHV &operator=(CustLRUCacheCHV &&) = delete;

        std::optional<ValT> get(const KeyT &key) {
            auto idx_val = key_to_idx_.find(key);
            if (idx_val.has_value()) {
                auto key_idx = *idx_val;
                if (first_idx != key_idx) {
                    move_front(key_idx);
                }
                return val_data_[key_idx];
            }
            return std::optional<ValT>();
        }

        void add(const KeyT &key, const ValT &val) {
            auto idx_val = key_to_idx_.find(key);
            if (!idx_val.has_value()) {
                if (cache_has_space_) { /// add new element as first of the cache
                    auto prev_first = first_idx;
                    first_idx = val_data_.size();
                    key_data_.push_back(key);
                    val_data_.push_back(val);
                    links_data_.push_back({DUMMY_IDX, prev_first});
                    links_data_[prev_first].next_ = first_idx;
                    cache_has_space_ = (val_data_.size() <= CapacityT);
                } else {
                    // replace last item in cache with new key&value and make it first
                    auto prev_idx = first_idx;
                    first_idx = last_idx;
                    last_idx = links_data_[first_idx].next_;

                    links_data_[last_idx].prev_ = DUMMY_IDX;
                    key_to_idx_.erase(key_data_[first_idx]);
                    key_data_[first_idx] = key;
                    val_data_[first_idx] = val;
                    links_data_[first_idx] = {DUMMY_IDX, prev_idx};
                    links_data_[prev_idx].next_ = first_idx;
                }
                if(!key_to_idx_.add(key, first_idx)){
                    std::cerr<< "Unable to add element with key "<< key<< std::endl;
                }
            } else { /// key exist in cache - just make it first and update value
                auto key_idx = *idx_val;
                if (first_idx != key_idx) {
                    move_front(key_idx);
                }
                val_data_[key_idx] = val; ///// update value in case it is changed
            }
        }

        using KeyToIdxT = HashVector<KeyT, int>;

        const KeyToIdxT &keys() const { return key_to_idx_; }

    private:
        void move_front(int curr_idx) {
            auto curr_lnk = links_data_[curr_idx];
            links_data_[curr_lnk.prev_].next_ = curr_lnk.next_;
            links_data_[curr_lnk.next_].prev_ = curr_lnk.prev_;

            links_data_[curr_idx] = {DUMMY_IDX, first_idx};
            links_data_[first_idx].next_ = curr_idx;
            first_idx = curr_idx;
        }

    private:
        using CacheKeyDataT = std::vector<KeyT>;
        CacheKeyDataT key_data_;
        using CacheValDataT = std::vector<ValT>;
        CacheValDataT val_data_;
        struct DblLinks {
            int next_ = 0;
            int prev_ = 0;
        };
        using CacheNextPrevDataT = std::vector<DblLinks>;
        CacheNextPrevDataT links_data_;


        KeyToIdxT key_to_idx_;
        int first_idx = DUMMY_IDX;
        int last_idx = DUMMY_IDX + 1;
        bool cache_has_space_ = true;
    };
}
