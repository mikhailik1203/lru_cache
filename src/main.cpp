#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include "CustLRUCache.h"
#include "CustLRUCacheSplit.h"
#include "CustLRUCacheCHV.h"
#include "HashVector.h"

using namespace std;

namespace {
    size_t ORDER_RESERVE_COUNT = 1000;
}

int main() {
    cache::CustLRUCacheCHV<int, int, 5> cont;
    cont.add(1, 1001);
    cont.add(2, 1002);
    cont.add(3, 1003);
    cont.add(4, 1004);
    cont.add(5, 1005);
    cont.add(6, 1006);
    cont.add(7, 1007);
    cont.add(5, 1005);

    auto val = cont.get(1);
    val = cont.get(5);
    val = cont.get(5);
    val = cont.get(3);
    return 0;
}
