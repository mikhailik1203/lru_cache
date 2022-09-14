#pragma once

#include<optional>
#include <array>
#include <unordered_map>

namespace cache {
    template<typename KeyT, typename ValT, int CapacityT>
    class CacheArrCont {
        const int DUMMY_IDX = 0;
    public:
        CacheArrCont() {
            key_to_idx_.reserve(CapacityT);
            // add dummy item to avoid checking negative index values
            key_data_.reserve(CapacityT + 1);
            key_data_.push_back(KeyT());
            val_data_.reserve(CapacityT + 1);
            val_data_.push_back(ValT());
            links_data_.reserve(CapacityT + 1);
            links_data_.push_back({DUMMY_IDX, DUMMY_IDX});
        }

        ~CacheArrCont() = default;

        void add(const KeyT &key, const ValT &val) {
            auto idx_it = key_to_idx_.find(key);
            if (key_to_idx_.end() == idx_it) {
                if (val_data_.size() <= CapacityT) {
                    auto prev_first = first_idx;
                    first_idx = val_data_.size();
                    //data_.push_back(Node{key, val, DUMMY_IDX, prev_first});
                    key_data_.push_back(key);
                    val_data_.push_back(val);
                    links_data_.push_back({DUMMY_IDX, prev_first});
                    //data_[prev_first].next_idx_ = first_idx;
                    links_data_[prev_first].next_ = first_idx;
                } else {
                    // update first_idx and last_idx
                    auto prev_idx = first_idx;
                    first_idx = last_idx;
                    //last_idx = data_[first_idx].next_idx_;
                    last_idx = links_data_[first_idx].next_;

                    //data_[last_idx].prev_idx_ = DUMMY_IDX;
                    links_data_[last_idx].prev_ = DUMMY_IDX;
                    //key_to_idx_.erase(data_[first_idx].key_);
                    key_to_idx_.erase(key_data_[first_idx]);
                    //data_[first_idx] = Node{key, val, DUMMY_IDX, prev_idx};
                    key_data_[first_idx] = key;
                    val_data_[first_idx] = val;
                    links_data_[first_idx] = {DUMMY_IDX, prev_idx};
                    //data_[prev_idx].next_idx_ = first_idx;
                    links_data_[prev_idx].next_ = first_idx;
                }
                key_to_idx_[key] = first_idx;
            } else {
                auto key_idx = idx_it->second;
                auto prev_first = first_idx;
                move_front(key_idx);
                //data_[key_idx] = Node{key, val, DUMMY_IDX, prev_first};
                key_data_[key_idx] = key;
                val_data_[key_idx] = val;
                links_data_[key_idx] = {DUMMY_IDX, prev_first};
                idx_it->second = first_idx;
            }
        }

        std::optional<ValT> get(const KeyT &key) {
            auto idx_it = key_to_idx_.find(key);
            if (key_to_idx_.end() != idx_it) {
                auto key_idx = idx_it->second;
                move_front(key_idx);
                //return data_[key_idx].val_;
                return val_data_[key_idx];
            }
            return std::optional<ValT>();
        }

        using KeyToIdxT = std::unordered_map<KeyT, int>;

        const KeyToIdxT &keys() const { return key_to_idx_; }

    private:
        void move_front(int curr_idx) {
            if (first_idx != curr_idx) {
                /*auto &curr_data = data_[curr_idx];
                auto prev_idx = curr_data.prev_idx_;
                auto next_idx = curr_data.next_idx_;
                data_[prev_idx].next_idx_ = next_idx;
                data_[next_idx].prev_idx_ = prev_idx;*/
                auto curr_lnk = links_data_[curr_idx];
                links_data_[curr_lnk.prev_].next_ = curr_lnk.next_;
                links_data_[curr_lnk.next_].prev_ = curr_lnk.prev_;

                //curr_data.prev_idx_ = first_idx;
                //curr_data.next_idx_ = DUMMY_IDX;
                links_data_[curr_idx] = {DUMMY_IDX, first_idx};
                //data_[first_idx].next_idx_ = curr_idx;
                links_data_[first_idx].next_ = curr_idx;
                first_idx = curr_idx;
            }
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
    };
}
