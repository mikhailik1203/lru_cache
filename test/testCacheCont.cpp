#include <gtest/gtest.h>
#include "../src/CustLRUCache.h"

#include <list>

namespace{
    class TestCache: public cache::CustLRUCache<int, int, 5>{
    public:
        TestCache() = default;
        ~TestCache() = default;

        size_t size()const { return data_.size() - 1;}

        bool check_data(const std::list<std::pair<int, int>> &data)const { 
            EXPECT_EQ(data.size() + 1, data_.size());
            auto idx = first_idx_;
            for(auto [k, v]: data){
                if(0 == idx)
                    return false;
                EXPECT_EQ(k, data_[idx].key_);
                EXPECT_EQ(v, data_[idx].val_);
                idx = data_[idx].prev_idx_;
            }
            return true;
        }
    };
}

TEST(CacheContTest, vanilla){

    TestCache cont;
    EXPECT_EQ(0, cont.size());
    EXPECT_TRUE(cont.check_data({}));
    cont.add(1, 1001);
    EXPECT_EQ(1, cont.size());
    EXPECT_TRUE(cont.check_data({{1, 1001}}));
    cont.add(2, 1002);
    EXPECT_EQ(2, cont.size());
    EXPECT_TRUE(cont.check_data({{2, 1002}, {1, 1001}}));
    cont.add(3, 1003);
    EXPECT_EQ(3, cont.size());
    EXPECT_TRUE(cont.check_data({{3, 1003}, {2, 1002}, {1, 1001}}));
    cont.add(4, 1004);
    EXPECT_EQ(4, cont.size());
    EXPECT_TRUE(cont.check_data({{4, 1004}, {3, 1003}, {2, 1002}, {1, 1001}}));
    cont.add(5, 1005);
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{5, 1005}, {4, 1004}, {3, 1003}, {2, 1002}, {1, 1001}}));
    cont.add(6, 1006);
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{6, 1006}, {5, 1005}, {4, 1004}, {3, 1003}, {2, 1002}}));
    cont.add(7, 1007);
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{7, 1007}, {6, 1006}, {5, 1005}, {4, 1004}, {3, 1003}}));
    cont.add(5, 1005);
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{5, 1005}, {7, 1007}, {6, 1006}, {4, 1004}, {3, 1003}}));

    // get element that not exist - nothing to change in cont
    EXPECT_EQ(std::nullopt, cont.get(1));
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{5, 1005}, {7, 1007}, {6, 1006}, {4, 1004}, {3, 1003}}));

    // get first element - nothing to change in cont
    EXPECT_EQ(1005, cont.get(5));
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{5, 1005}, {7, 1007}, {6, 1006}, {4, 1004}, {3, 1003}}));

    // get first element again - nothing to change in cont
    EXPECT_EQ(1005, cont.get(5));
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{5, 1005}, {7, 1007}, {6, 1006}, {4, 1004}, {3, 1003}}));

    // get last element - it is moved to front
    EXPECT_EQ(1003, cont.get(3));
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{3, 1003}, {5, 1005}, {7, 1007}, {6, 1006}, {4, 1004}}));

    // get second element - it is moved to front
    EXPECT_EQ(1005, cont.get(5));
    EXPECT_EQ(5, cont.size());
    EXPECT_TRUE(cont.check_data({{5, 1005}, {3, 1003}, {7, 1007}, {6, 1006}, {4, 1004}}));

}

TEST(CacheContTest, add_same){

    TestCache cont;
    EXPECT_EQ(0, cont.size());
    EXPECT_TRUE(cont.check_data({}));
    cont.add(1, 1001);
    EXPECT_EQ(1, cont.size());
    EXPECT_TRUE(cont.check_data({{1, 1001}}));

    /// add same key with different value
    cont.add(1, 2001);
    EXPECT_EQ(1, cont.size());
    EXPECT_TRUE(cont.check_data({{1, 2001}}));

    /// add same key one more time
    cont.add(1, 2001);
    EXPECT_EQ(1, cont.size());
    EXPECT_TRUE(cont.check_data({{1, 2001}}));


    cont.add(2, 1002);
    EXPECT_EQ(2, cont.size());
    EXPECT_TRUE(cont.check_data({{2, 1002}, {1, 2001}}));


    /// add same key one more time
    cont.add(1, 3001);
    EXPECT_EQ(2, cont.size());
    EXPECT_TRUE(cont.check_data({{1, 3001}, {2, 1002}}));
}