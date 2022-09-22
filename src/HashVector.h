#pragma once

#include<optional>
#include <vector>
#include <unordered_map>

namespace cache {
    const int DUMMY_IDX = 0;

    /// @brief Hash table for simple key type and with fixed capacity; based on vector container
    /// @tparam KeyT 
    /// @tparam ValT 
    /// @tparam CapacityT 
    template<typename KeyT, typename ValT>
    class HashVector {
    public:
        explicit HashVector(int capacity): capacity_(capacity) {
            // add dummy item to avoid checking negative index values
            chunks_.assign(capacity_ + 1, ChunkData{});

            data_.assign(capacity_ + 1, DataNode{});
            for(size_t i = 1; i < capacity_; ++i){
                data_[i].next_idx_ = i + 1;
            }
            free_node_idx_ = 1;
        }

        ~HashVector() = default;

        /// @brief  just to simplify implementation - disable copy and move
        HashVector(const HashVector &) = delete;
        HashVector(HashVector &&) = delete;
        HashVector &operator=(const HashVector &) = delete;
        HashVector &operator=(HashVector &&) = delete;

        /// @brief Adds key:val pair to the container
        /// @param key 
        /// @param val 
        /// @return true, if key+val were added otherwise return false
        bool add(const KeyT &key, const ValT &val) {
            int chunk_idx = 1 + std::hash<std::uint32_t>{}(key) % (capacity_);
            auto idx = chunks_[chunk_idx].first_idx_;
            bool chunk_not_empty = (DUMMY_IDX != idx);
            auto prev_idx = DUMMY_IDX;
            if (chunk_not_empty){
                /// iterate from first_idx_ till next_idx_=DUMMY_IDX to find the key
                do{
                    if(key == data_[idx].key_){
                        /// in case key exist - update value
                        data_[idx].val_ = val;
                        return true;
                    }
                    prev_idx = idx;
                    idx = data_[idx].next_idx_;
                }while (DUMMY_IDX != idx);
            }
            if(DUMMY_IDX == free_node_idx_)
                return false; // Ups! no free space

            /// in case key doesn't exist - use next free cell in data_ to store key+value and update next_idx
            auto data_idx = free_node_idx_;
            free_node_idx_ = data_[free_node_idx_].next_idx_; // update next free node
            data_[data_idx] = DataNode{key, val, DUMMY_IDX};
            if(!chunk_not_empty){
                chunks_[chunk_idx].first_idx_ = data_idx;
            }
            data_[prev_idx].next_idx_ = data_idx;
            return true;
        }

        void erase(const KeyT &key) {
            int chunk_idx = 1 + std::hash<std::uint32_t>{}(key) % (capacity_);
            auto idx = chunks_[chunk_idx].first_idx_;
            /// iterate from first_idx_ till next_idx_=DUMMY_IDX to find the key
            auto prev_idx = DUMMY_IDX;
            while (DUMMY_IDX != idx){
                if(key == data_[idx].key_){
                    /// in case key exists - change next to remove idx node
                    if(DUMMY_IDX == prev_idx){
                        chunks_[chunk_idx].first_idx_ = data_[idx].next_idx_;
                    }else{
                        data_[prev_idx].next_idx_ = data_[idx].next_idx_;
                    }
                    /// add node to the free nodes
                    data_[idx].next_idx_ = free_node_idx_;
                    free_node_idx_ = idx;
                    return;
                }
                prev_idx = idx;
                idx = data_[idx].next_idx_;
            }
        }

        std::optional<ValT> find(const KeyT &key) {
            int chunk_idx = 1 + std::hash<std::uint32_t>{}(key) % (capacity_);
            auto idx = chunks_[chunk_idx].first_idx_;
            if (DUMMY_IDX != idx){
                /// iterate from first_idx_ till next_idx_=DUMMY_IDX to find the key
                do{
                    if(key == data_[idx].key_){
                        /// in case key exist - return value
                        return data_[idx].val_;
                    }
                    idx = data_[idx].next_idx_;
                }while (DUMMY_IDX != idx);
            }
            return std::optional<ValT>();
        }

    protected:
        struct ChunkData {
            int first_idx_ = DUMMY_IDX;
            int last_idx_ = DUMMY_IDX;
        };
        using HashChunksT = std::vector<ChunkData>;
        HashChunksT chunks_;

        struct DataNode {
            KeyT key_;
            ValT val_;
            int next_idx_ = DUMMY_IDX;
        };
        using ChunkDataT = std::vector<DataNode>;
        ChunkDataT data_;

        int free_node_idx_ = 1;
        int capacity_ = 0;
    };
}
