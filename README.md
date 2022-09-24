# lru_cache
Low latency LRU cache 

LRUCache - vanilla implementation of the LRU cache, based on std::unordered_map and std::list containers - used to compare performance.

CustLRUCache - custom implementation, based on std::unordered_map and std::vector containers to reduce mem allocations.

CustLRUCacheSplit - custom implementation, based on std::unordered_map and separate std::vector containers for keys and values.

HashVector - custom hash table implementation, based on std::vector container - it preallocates space, because maximum number elements is known.

CustLRUCacheCHV - custom implementation, based on HashVector and separate std::vector containers for keys and values.

--------------------------------------------------------------------------------------------------
Benchmarks inserts 10mln int key-values pairs to the cache. Cache size is 1000 and 10000 elements:

Throughput (nsec) of adding 10,000,000 elements to the cache:
Container | Cache size 1000elem | Cache size 10000elem 
--- | --- | --- 
LRUCache | 1,187,504,067 | 1,972,771,068
CustLRUCache | 1,011,177,636 | 1,273,019,696
CustLRUCacheSplit | 1,024,139,675 | 1,303,043,118
CustLRUCacheCHV | 395,036,726 | 434,141,667

Latency (nsec, including avg timer latency 18nsec) of adding 10,000,000 elements to the cache for 1000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 87 | 138 | 189 | 221 | 12127
CustLRUCache | 57 | 116 | 164 | 193 | 19748
CustLRUCacheSplit | 56 | 117 | 170 | 198 | 10773
CustLRUCacheCHV | 40 | 59 | 79 | 91 | 11645

Latency (nsec, including avg timer latency 18nsec) of adding 10,000,000 elements to the cache for 10000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 74 | 206 | 305 | 357 | 289374
CustLRUCache | 55 | 138 | 208 | 252 | 884063
CustLRUCacheSplit | 58 | 139 | 210 | 253 | 752277
CustLRUCacheCHV | 39 | 61 | 87 | 101 | 14309

--------------------------------------------------------------------------------------------------
Benchmarks retrieves values of 10mln int existing keys from cache. Cache size is 1000 and 10000 elements:

Throughput (nsec) of getting of 10,000,000 elements from the cache:
Container | Cache size 1000elem | Cache size 10000elem 
--- | --- | --- 
LRUCache | 568,476,315 | 1,314,937,051
CustLRUCache | 206,421,002 | 303,010,403
CustLRUCacheSplit | 200,785,297 | 372,977,555
CustLRUCacheCHV | 164,640,238 | 203,266,744

Latency (nsec, including avg timer latency 18nsec) of getting of 10,000,000 elements from the cache for 1000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 31 | 75 | 116 | 141 | 14889
CustLRUCache | 28 | 43 | 64 | 76 | 11286
CustLRUCacheSplit | 28 | 42 | 64 | 76 | 13383
CustLRUCacheCHV | 28 | 40 | 56 | 67 | 11759

Latency (nsec, including avg timer latency 18nsec) of getting of 10,000,000 elements from the cache for 10000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 34 | 141 | 214 | 251 | 193153
CustLRUCache | 28 | 72 | 109 | 129 | 12457
CustLRUCacheSplit | 28 | 70 | 106 | 127 | 11597
CustLRUCacheCHV | 28 | 49 | 71 | 82 | 11146

--------------------------------------------------------------------------------------------------
Benchmarks retrieves values of 10mln int not-existing keys from cache (cache-miss scenario). Cache size is 1000 and 10000 elements:

Throughput (nsec) of getting of 10,000,000 missed elements from the cache:
Container | Cache size 1000elem | Cache size 10000elem 
--- | --- | --- 
LRUCache | 225,348,313 | 336,500,983
CustLRUCache | 245,731,806 | 296,438,541
CustLRUCacheSplit | 244,798,651 | 297,022,729
CustLRUCacheCHV | 167,001,618 | 185,629,090

Latency (nsec, including avg timer latency 18nsec) of getting of 10,000,000 missed elements from the cache for 1000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 21 | 38 | 81 | 105 | 17712
CustLRUCache | 22 | 42 | 75 | 94 | 14469
CustLRUCacheSplit | 21 | 42 | 74 | 92 | 11342
CustLRUCacheCHV | 22 | 37 | 48 | 57 | 11216

Latency (nsec, including avg timer latency 18nsec) of getting of 10,000,000 missed elements from the cache for 10000elements :
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 22 | 60 | 132 | 162 | 15412
CustLRUCache | 22 | 55 | 97 | 121 | 20525
CustLRUCacheSplit | 22 | 55 | 98 | 123 | 11409
CustLRUCacheCHV | 22 | 38 | 54 | 64 | 10506

