#pragma once

#include<optional>
#include <vector>
#include <unordered_map>

namespace cache {
    template<typename KeyT, typename ValT, int CapacityT>
    class CacheCont {
        const int DUMMY_IDX = 0;
    public:
        CacheCont() {
            key_to_idx_.reserve(CapacityT);
            data_.reserve(CapacityT + 1);
            // add dummy item to avoid checking negative index values
            data_.push_back(Node{KeyT(), ValT(), DUMMY_IDX, DUMMY_IDX});
        }

        ~CacheCont() = default;

        void add(const KeyT &key, const ValT &val) {
            auto idx_it = key_to_idx_.find(key);
            if (key_to_idx_.end() == idx_it) {
                if (data_.size() <= CapacityT) {
                    auto prev_first = first_idx;
                    first_idx = data_.size();
                    data_.push_back(Node{key, val, DUMMY_IDX, prev_first});
                    data_[prev_first].next_idx_ = first_idx;
                } else {
                    // update first_idx and last_idx
                    auto prev_idx = first_idx;
                    first_idx = last_idx;
                    last_idx = data_[first_idx].next_idx_;

                    data_[last_idx].prev_idx_ = DUMMY_IDX;
                    key_to_idx_.erase(data_[first_idx].key_);
                    data_[first_idx] = Node{key, val, DUMMY_IDX, prev_idx};
                    data_[prev_idx].next_idx_ = first_idx;
                }
                key_to_idx_[key] = first_idx;
            } else {
                auto key_idx = idx_it->second;
                auto prev_first = first_idx;
                move_front(key_idx);
                data_[key_idx] = Node{key, val, DUMMY_IDX, prev_first};
                idx_it->second = first_idx;
            }
        }

        std::optional<ValT> get(const KeyT &key) {
            auto idx_it = key_to_idx_.find(key);
            if (key_to_idx_.end() != idx_it) {
                auto key_idx = idx_it->second;
                move_front(key_idx);
                return data_[key_idx].val_;
            }
            return std::optional<ValT>();
        }

        using KeyToIdxT = std::unordered_map<KeyT, int>;

        const KeyToIdxT &keys() const { return key_to_idx_; }

    private:
        void move_front(int curr_idx) {
            if (first_idx != curr_idx) {
                auto &curr_data = data_[curr_idx];
                auto prev_idx = curr_data.prev_idx_;
                auto next_idx = curr_data.next_idx_;
                data_[prev_idx].next_idx_ = next_idx;
                data_[next_idx].prev_idx_ = prev_idx;

                curr_data.prev_idx_ = first_idx;
                curr_data.next_idx_ = DUMMY_IDX;
                data_[first_idx].next_idx_ = curr_idx;
                first_idx = curr_idx;
            }
        }

    private:
        struct Node {
            KeyT key_;
            ValT val_;
            int next_idx_ = DUMMY_IDX;
            int prev_idx_ = DUMMY_IDX;
        };
        using CacheDataT = std::vector<Node>;
        CacheDataT data_;
        KeyToIdxT key_to_idx_;
        int first_idx = DUMMY_IDX;
        int last_idx = DUMMY_IDX + 1;
    };
}
