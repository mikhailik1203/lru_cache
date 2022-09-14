
#include "../src/CacheCont.h"
#include "../src/CacheArrCont.h"
#include "../src/LRUCache.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <sched.h>
#include <cassert>

using namespace cache;

namespace {
#ifdef DEBUG
    const uint64_t BENCHMARK_DATA_COUNT = 100000;
    const int CACHE_SIZE = 1000;
#else
    const uint64_t BENCHMARK_DATA_COUNT = 100000000;
    const int CACHE_SIZE = 1000;
#endif

    using CacheValuesT = std::vector<std::pair<int, int>>;

    const int CPU_CORE = 2; ///Please check that you have this CPU core!!

    cpu_set_t g_mask;

    std::vector<int> g_keys_bench;

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
            std::cout << "Vanilla Cache (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into cache " << thoughput_nsec
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
            std::cout << " Latency of adding into cache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl;
        }
    }


    void benchmark_add_value(const CacheValuesT &values) {
        { /// measure throughput
            CacheCont<int, int, CACHE_SIZE> cont;

            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: values) {
                    cont.add(val.first, val.second);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "Cache (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into cache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CacheCont<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                cont.add(val.first, val.second);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of adding into cache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl;
        }
    }

    void benchmark_arr_add_value(const CacheValuesT &values) {
        { /// measure throughput
            CacheArrCont<int, int, CACHE_SIZE> cont;

            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: values) {
                    cont.add(val.first, val.second);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CacheArr (size " << CACHE_SIZE << "):" << std::endl;
            std::cout << " Throughput of adding " << BENCHMARK_DATA_COUNT << " values into cache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CacheArrCont<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                cont.add(val.first, val.second);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of adding into cache arr: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl;
        }
    }

    void benchmark_vanilla_get_value(const CacheValuesT &values) {
        { /// measure throughput
            LRUCache<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            assert(0 < g_keys_bench.size());

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: g_keys_bench) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "Vanilla Cache (size " << CACHE_SIZE << ", res=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from cache " << thoughput_nsec
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
            for (const auto &val: g_keys_bench) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from cache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << " summ= " << summ << std::endl;
        }
    }

    void benchmark_get_value(const CacheValuesT &values) {
        { /// measure throughput
            CacheCont<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            {
                g_keys_bench.reserve(BENCHMARK_DATA_COUNT);
                const auto &keys_vals = cont.keys();
                std::vector<int> keys;
                keys.reserve(keys_vals.size());
                for (const auto &v: keys_vals) {
                    keys.push_back(v.first);
                }
                for (int i = 0; i < BENCHMARK_DATA_COUNT; ++i) {
                    g_keys_bench.push_back(keys[i % keys.size()]);
                }
            }

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: g_keys_bench) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "Cache (size " << CACHE_SIZE << ", summ=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from cache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            uint64_t summ = 0;
            CacheCont<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            for (const auto &val: g_keys_bench) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from cache: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "Summ=" << summ << std::endl;
        }
    }

    void benchmark_arr_get_value(const CacheValuesT &values) {
        { /// measure throughput
            CacheArrCont<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            assert(0 < g_keys_bench.size());

            uint64_t summ = 0;
            uint64_t thoughput_nsec = 0;
            {
                auto start_tm = std::chrono::high_resolution_clock::now();
                for (const auto &val: g_keys_bench) {
                    summ += cont.get(val).value_or(0);
                }
                auto finish_tm = std::chrono::high_resolution_clock::now();
                thoughput_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count();
            }
            std::cout << "CacheArr (size " << CACHE_SIZE << ", summ=" << summ << "):" << std::endl;
            std::cout << " Throughput of getting " << BENCHMARK_DATA_COUNT << " values from cache " << thoughput_nsec
                      << " nsec"
                      << std::endl;
        }

        { /// measure latency
            using LatencyT = std::vector<uint64_t>;
            LatencyT lat_data;
            lat_data.reserve(BENCHMARK_DATA_COUNT);

            CacheArrCont<int, int, CACHE_SIZE> cont;
            for (const auto &val: values) {
                cont.add(val.first, val.second);
            }

            uint64_t summ = 0;
            for (const auto &val: g_keys_bench) {
                auto start_tm = std::chrono::high_resolution_clock::now();
                summ += cont.get(val).value_or(0);
                auto finish_tm = std::chrono::high_resolution_clock::now();
                lat_data.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish_tm - start_tm).count());
            }
            std::sort(lat_data.begin(), lat_data.end());

            size_t index_95perc = static_cast<double>(lat_data.size()) * 0.95;
            size_t index_99perc = static_cast<double>(lat_data.size()) * 0.99;
            std::cout << " Latency of getting value from cache arr: min=" << *lat_data.begin() << " nsec; 50%="
                      << lat_data[lat_data.size() / 2] << " nsec; 95%=" << lat_data[index_95perc]
                      << " nsec; 99%=" << lat_data[index_99perc] << " nsec; max=" << *lat_data.rbegin() << " nsec"
                      << std::endl
                      << "Summ=" << summ << std::endl;
        }
    }

}

int main(int argc, char **argv) {
    assign_to_cpu(CPU_CORE);

    int random_seed = 123456789; /// use predefined generator to test same sequence on each run
    std::srand(random_seed);


    CacheValuesT values;
    values.reserve(BENCHMARK_DATA_COUNT);

    { ///generate data to add
        int key_id = 1;
        for (size_t i = 0; i < BENCHMARK_DATA_COUNT; ++i) {
            auto key = ++key_id;
            values.push_back({key, 10000 + key_id});
        }
        std::shuffle(values.begin(), values.end(), std::default_random_engine(7));
    }

    benchmark_add_value(values);
    benchmark_arr_add_value(values);
    benchmark_vanilla_add_value(values);

    benchmark_get_value(values);
    benchmark_arr_get_value(values);
    benchmark_vanilla_get_value(values);

    return 1;
}