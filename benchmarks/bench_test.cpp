
#include "../src/CustLRUCache.h"
#include "../src/CustLRUCacheSplit.h"
#include "../src/CustLRUCacheCHV.h"
#include "../src/LRUCache.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <set>
#include <sched.h>
#include <cassert>

using namespace cache;

namespace {
#ifdef DEBUG
    const uint64_t BENCHMARK_DATA_COUNT = 100000;
    const int CACHE_SIZE = 1000;
#else
    const uint64_t BENCHMARK_DATA_COUNT = 10000000; //10000000
    const int CACHE_SIZE = 1000;
#endif

    using CacheValuesT = std::vector<std::pair<int, int>>;
    using KeyValuesT = std::vector<int>;

    const int CPU_CORE = 2; ///Please check that you have this CPU core!!

    cpu_set_t g_mask;

    inline void assign_to_cpu(int core_id) {
        CPU_ZERO(&g_mask);
        CPU_SET(core_id, &g_mask);
        sched_setaffinity(0, sizeof(g_mask), &g_mask);
    }

    void benchmark_vanilla_add_value(const CacheValuesT &values) {
        { /// measure throughput
            LRUCache<int, int, CACHE_SIZE> cont;

            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: values) {
                    cont.add(val.first, val.second);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "Vanilla LRUCache (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into LRUCache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            LRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                cont.add(val.first, val.second);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of adding into LRUCache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }


    void benchmark_add_value(const CacheValuesT &values) {
        { /// measure throughput
            CustLRUCache<int, int, CACHE_SIZE> cont;

            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: values) {
                    cont.add(val.first, val.second);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CustLRUCache (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into CustLRUCache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CustLRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                cont.add(val.first, val.second);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of adding into CustLRUCache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }

    void benchmark_arr_add_value(const CacheValuesT &values) {
        { /// measure throughput
            CustLRUCacheSplit<int, int, CACHE_SIZE> cont;

            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: values) {
                    cont.add(val.first, val.second);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CustLRUCacheSplit (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into CustLRUCacheSplit " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CustLRUCacheSplit<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                cont.add(val.first, val.second);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of adding into CustLRUCacheSplit: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }

    void benchmark_split_vect_add_value(const CacheValuesT &values) {
        { /// measure throughput
            CustLRUCacheCHV<int, int, CACHE_SIZE> cont;

            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: values) {
                    cont.add(val.first, val.second);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CustLRUCacheCHV (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into CustLRUCacheCHV " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CustLRUCacheCHV<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                cont.add(val.first, val.second);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of adding into CustLRUCacheCHV: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }

    void benchmark_vanilla_get_value(const CacheValuesT &values, const KeyValuesT &keys) {
        { /// measure throughput
            LRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: keys) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "Vanilla LRUCache (size " << CACHE_SIZE << ", res=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from LRUCache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            LRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            for (const auto &val: keys) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from LRUCache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << " summ= " << summ << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;

        }
    }

    void benchmark_get_value(const CacheValuesT &values, const KeyValuesT &keys) {
        { /// measure throughput
            CustLRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: keys) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CustLRUCache (size " << CACHE_SIZE << ", summ=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from CustLRUCache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            uint64_t summ = 0;
            CustLRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            for (const auto &val: keys) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from CustLRUCache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "Summ=" << summ << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }

    void benchmark_arr_get_value(const CacheValuesT &values, const KeyValuesT &keys) {
        { /// measure throughput
            CustLRUCacheSplit<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: keys) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CustLRUCacheSplit (size " << CACHE_SIZE << ", summ=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from CustLRUCacheSplit " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CustLRUCacheSplit<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            for (const auto &val: keys) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from CustLRUCacheSplit: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "Summ=" << summ << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }

    void benchmark_arr_vector_get_value(const CacheValuesT &values, const KeyValuesT &keys) {
        { /// measure throughput
            CustLRUCacheCHV<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: keys) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CustLRUCacheCHV (size " << CACHE_SIZE << ", summ=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from CustLRUCacheCHV " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CustLRUCacheCHV<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            for (const auto &val: keys) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from CustLRUCacheCHV: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "Summ=" << summ << std::endl
                      << "\t" << *lat_data.begin()<< " | "<< lat_data[lat_data.size() / 2]<< " | "<< lat_data[index_95perc] << " | "
                      << lat_data[index_99perc] << " | "<< *lat_data.rbegin()<< std::endl;
        }
    }

}

int main(int argc, char **argv) {
    assign_to_cpu(CPU_CORE);

    int random_seed = 123456789; /// use predefined generator to test same sequence on each run
    std::srand(random_seed);


    CacheValuesT values;
    values.reserve(BENCHMARK_DATA_COUNT);
    std::vector<int> hit_keys;
    hit_keys.reserve(BENCHMARK_DATA_COUNT);    
    std::vector<int> miss_keys;
    miss_keys.reserve(BENCHMARK_DATA_COUNT);  
    std::set<int> left_keys;  

    { ///generate data to add
        int key_id = 1;
        for (size_t i = 0; i < BENCHMARK_DATA_COUNT; ++i) {
            auto key = ++key_id;
            values.push_back({key, 10000 + key_id});
            left_keys.insert(key);
        }
        std::shuffle(values.begin(), values.end(), std::default_random_engine(7));
    }
    std::cout<< "Key-values were generated..."<< std::endl;

    { /// prepare keys for benchmark
        std::set<int> keys;
        for(auto it = values.rbegin(); it != values.rend(); ++it){
            if(keys.size() < CACHE_SIZE){
                keys.insert(it->first);
                left_keys.erase(it->first);                
            }else
                break;
        }
        std::cout<< "Key were prepared..."<< std::endl;
        auto hit_it = keys.begin();
        auto miss_it = left_keys.begin();
        for (int i = 0; i < BENCHMARK_DATA_COUNT; ++i) {
            hit_keys.push_back(*hit_it);
            miss_keys.push_back(*miss_it);
            if(keys.end() == hit_it){
                hit_it = keys.begin();
            }else
                ++hit_it;
            if(left_keys.end() == miss_it){
                miss_it = left_keys.begin();
            }else
                ++miss_it;
        }
        std::shuffle(hit_keys.begin(), hit_keys.end(), std::default_random_engine(7));
        std::shuffle(miss_keys.begin(), miss_keys.end(), std::default_random_engine(7));
        std::cout<< "Hit/Miss keys were generated..."<< std::endl;
    }
 
    benchmark_vanilla_add_value(values);
    benchmark_add_value(values);
    benchmark_arr_add_value(values);
    benchmark_split_vect_add_value(values);
    std::cout<< "-------------------------- Test hit cache ---------------------------------------"<< std::endl;
    benchmark_vanilla_get_value(values, hit_keys);
    benchmark_get_value(values, hit_keys);
    benchmark_arr_get_value(values, hit_keys);
    benchmark_arr_vector_get_value(values, hit_keys);
    std::cout<< "--------------------------- Test miss cache --------------------------------------"<< std::endl;
    benchmark_vanilla_get_value(values, miss_keys);
    benchmark_get_value(values, miss_keys);
    benchmark_arr_get_value(values, miss_keys);
    benchmark_arr_vector_get_value(values, miss_keys);
    return 1;
}