# lru_cache
Low latency LRU cache 

LRUCache - vanilla implementation of the LRU cache, based on unordered_map and list containers. It is used to compare performance.

CustLRUCache - custom implementation, based on unordered_map and vector containers to reduce mem allocations.

CustLRUCacheSplit - custom implementation, based on unordered_map and separate vector containers for keys and values.

HashVector - custom hash table implementation, based on vector container.

CustLRUCacheCHV - custom implementation, based on HashVector and separate vector containers for keys and values.

Throughput (nsec) of adding 100000000 elements to the cache:
Container | Cache size 1000elem | Cache size 10000elem 
--- | --- | --- 
LRUCache | 12,045,856,826 | 14,669,643,322 
CustLRUCache | 10,775,969,966 | 12,080,115,448 
CustLRUCacheSplit | 10,804,838,493 | 12,365,412,200 
CustLRUCacheCHV | 4,301,971,616 | 4,666,714,070 

Latency (nsec) of adding 100000000 elements to the cache for 1000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 80 | 145 | 199 | 270 | 22548 
CustLRUCache | 78 | 120 | 180 | 235 | 22617 
CustLRUCacheSplit | 57 | 122 | 184 | 241 | 33494 
CustLRUCacheCHV | 41 | 63 | 86 | 103 | 41164 

Latency (nsec) of adding 100000000 elements to the cache for 10000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 84 | 161 | 236 | 282 | 93914 
CustLRUCache | 56 | 135 | 197 | 233 | 47986 
CustLRUCacheSplit | 64 | 142 | 199 | 237 | 55150 
CustLRUCacheCHV | 41 | 66 | 95 | 113 | 12808 

Throughput (nsec) of getting of 100000000 elements from the cache:
Container | Cache size 1000elem | Cache size 10000elem 
--- | --- | --- 
LRUCache | 5,708,996,898 | 8,069,745,179 
CustLRUCache | 1,596,721,727 | 2,241,789,900 
CustLRUCacheSplit | 1,569,660,720 | 2,126,552,518 
CustLRUCacheCHV | 1,295,164,334 | 2,035,872,910 

Latency (nsec) of getting of 100000000 elements from the cache for 1000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 80 | 145 | 199 | 270 | 22548 
CustLRUCache | 78 | 120 | 180 | 235 | 22617 
CustLRUCacheSplit | 57 | 122 | 184 | 241 | 33494 
CustLRUCacheCHV | 41 | 63 | 86 | 103 | 41164 

Latency (nsec) of getting of 100000000 elements from the cache for 10000elements:
Container | min | 50% | 95% | 99% | max
--- | --- | --- | --- |--- |--- 
LRUCache | 56 | 135 | 197 | 233 | 47986 
CustLRUCache | 64 | 142 | 199 | 237 | 55150 
CustLRUCacheSplit | 41 | 66 | 95 | 113 | 12808 
CustLRUCacheCHV | 84 | 161 | 236 | 282 | 93914 
