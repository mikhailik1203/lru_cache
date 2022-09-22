#include <gtest/gtest.h>
#include "../src/HashVector.h"

TEST(HashVectorTest, vanilla){
    cache::HashVector<int, int> cont(5);
    EXPECT_TRUE(cont.add(1, 1001));
    EXPECT_EQ(1001, cont.find(1));
    cont.erase(1);
    EXPECT_EQ(std::nullopt, cont.find(1));
}

TEST(HashVectorTest, add){
    cache::HashVector<int, int> cont(5);
    EXPECT_TRUE(cont.add(1, 1001));
    EXPECT_TRUE(cont.add(2, 1002));
    EXPECT_TRUE(cont.add(3, 1003));
    EXPECT_TRUE(cont.add(4, 1004));
    EXPECT_TRUE(cont.add(5, 1005));
    EXPECT_FALSE(cont.add(6, 1006)); /// hash table for 5 elements
    EXPECT_EQ(1001, cont.find(1));
    EXPECT_EQ(1002, cont.find(2));
    EXPECT_EQ(1003, cont.find(3));
    EXPECT_EQ(1004, cont.find(4));
    EXPECT_EQ(1005, cont.find(5));
    cont.erase(3);
    EXPECT_TRUE(cont.add(6, 1006));
    EXPECT_EQ(std::nullopt, cont.find(3));
    EXPECT_EQ(1006, cont.find(6));
}

TEST(HashVectorTest, erase){
    cache::HashVector<int, int> cont(5);
    EXPECT_TRUE(cont.add(1, 1001));
    EXPECT_TRUE(cont.add(2, 1002));
    EXPECT_TRUE(cont.add(3, 1003));
    EXPECT_TRUE(cont.add(4, 1004));
    EXPECT_TRUE(cont.add(5, 1005));
    cont.erase(1);

    // try to erase again
    cont.erase(1);

    EXPECT_EQ(std::nullopt, cont.find(1));
    EXPECT_EQ(1002, cont.find(2));
    EXPECT_EQ(1003, cont.find(3));
    EXPECT_EQ(1004, cont.find(4));
    EXPECT_EQ(1005, cont.find(5));

    // erase other elements
    cont.erase(2);
    cont.erase(3);
    cont.erase(4);
    cont.erase(5);
    EXPECT_EQ(std::nullopt, cont.find(1));
    EXPECT_EQ(std::nullopt, cont.find(2));
    EXPECT_EQ(std::nullopt, cont.find(3));
    EXPECT_EQ(std::nullopt, cont.find(4));
    EXPECT_EQ(std::nullopt, cont.find(5));
}

TEST(HashVectorTest, erase_add_keys){
    cache::HashVector<int, int> cont(5);
    EXPECT_TRUE(cont.add(1, 1001));
    EXPECT_TRUE(cont.add(2, 1002));
    EXPECT_TRUE(cont.add(3, 1003));
    EXPECT_TRUE(cont.add(4, 1004));
    EXPECT_TRUE(cont.add(5, 1005));

    cont.erase(1);
    cont.erase(2);
    cont.erase(3);
    cont.erase(4);
    cont.erase(5);
    EXPECT_EQ(std::nullopt, cont.find(1));
    EXPECT_EQ(std::nullopt, cont.find(2));
    EXPECT_EQ(std::nullopt, cont.find(3));
    EXPECT_EQ(std::nullopt, cont.find(4));
    EXPECT_EQ(std::nullopt, cont.find(5));

    EXPECT_TRUE(cont.add(1, 2001));
    EXPECT_TRUE(cont.add(2, 2002));
    EXPECT_TRUE(cont.add(3, 2003));
    EXPECT_TRUE(cont.add(4, 2004));
    EXPECT_TRUE(cont.add(5, 2005));

    EXPECT_EQ(2001, cont.find(1));
    EXPECT_EQ(2002, cont.find(2));
    EXPECT_EQ(2003, cont.find(3));
    EXPECT_EQ(2004, cont.find(4));
    EXPECT_EQ(2005, cont.find(5));

    cont.erase(1);
    cont.erase(2);
    cont.erase(3);
    cont.erase(4);
    cont.erase(5);
    EXPECT_EQ(std::nullopt, cont.find(1));
    EXPECT_EQ(std::nullopt, cont.find(2));
    EXPECT_EQ(std::nullopt, cont.find(3));
    EXPECT_EQ(std::nullopt, cont.find(4));
    EXPECT_EQ(std::nullopt, cont.find(5));


    EXPECT_TRUE(cont.add(1, 3001));
    EXPECT_TRUE(cont.add(2, 3002));
    EXPECT_TRUE(cont.add(3, 3003));
    EXPECT_TRUE(cont.add(4, 3004));
    EXPECT_TRUE(cont.add(5, 3005));

    EXPECT_EQ(3001, cont.find(1));
    EXPECT_EQ(3002, cont.find(2));
    EXPECT_EQ(3003, cont.find(3));
    EXPECT_EQ(3004, cont.find(4));
    EXPECT_EQ(3005, cont.find(5));

    cont.erase(1);
    cont.erase(2);
    cont.erase(3);
    cont.erase(4);
    cont.erase(5);
    EXPECT_EQ(std::nullopt, cont.find(1));
    EXPECT_EQ(std::nullopt, cont.find(2));
    EXPECT_EQ(std::nullopt, cont.find(3));
    EXPECT_EQ(std::nullopt, cont.find(4));
    EXPECT_EQ(std::nullopt, cont.find(5));
}

TEST(HashVectorTest, erase_add_same_chunk){
    int key1 = 14001, key2 = 15001, key3 = 16001, key4 = 17001, key5 = 18001, key6 = 19001;
    cache::HashVector<int, int> cont(5);
    EXPECT_TRUE(cont.add(key1, 1014));
    EXPECT_TRUE(cont.add(key2, 1015));
    EXPECT_TRUE(cont.add(key3, 1016));
    EXPECT_TRUE(cont.add(key4, 1017));
    EXPECT_TRUE(cont.add(key5, 1018));
    EXPECT_FALSE(cont.add(key6, 1019));

    // erase in fifo order 
    cont.erase(key1);
    cont.erase(key2);
    cont.erase(key3);
    cont.erase(key4);
    cont.erase(key5);
    EXPECT_EQ(std::nullopt, cont.find(key1));
    EXPECT_EQ(std::nullopt, cont.find(key2));
    EXPECT_EQ(std::nullopt, cont.find(key3));
    EXPECT_EQ(std::nullopt, cont.find(key4));
    EXPECT_EQ(std::nullopt, cont.find(key5));

    EXPECT_TRUE(cont.add(key1, 2014));
    EXPECT_TRUE(cont.add(key2, 2015));
    EXPECT_TRUE(cont.add(key3, 2016));
    EXPECT_TRUE(cont.add(key4, 2017));
    EXPECT_TRUE(cont.add(key5, 2018));
    EXPECT_FALSE(cont.add(key6, 2019));

    EXPECT_EQ(2014, cont.find(key1));
    EXPECT_EQ(2015, cont.find(key2));
    EXPECT_EQ(2016, cont.find(key3));
    EXPECT_EQ(2017, cont.find(key4));
    EXPECT_EQ(2018, cont.find(key5));

    // delete in the reverse order
    cont.erase(key5);
    cont.erase(key4);    
    cont.erase(key3);
    cont.erase(key2);
    cont.erase(key1);

    EXPECT_EQ(std::nullopt, cont.find(key1));
    EXPECT_EQ(std::nullopt, cont.find(key2));
    EXPECT_EQ(std::nullopt, cont.find(key3));
    EXPECT_EQ(std::nullopt, cont.find(key4));
    EXPECT_EQ(std::nullopt, cont.find(key5));


    EXPECT_TRUE(cont.add(key1, 3001));
    EXPECT_TRUE(cont.add(key2, 3002));
    EXPECT_TRUE(cont.add(key3, 3003));
    EXPECT_TRUE(cont.add(key4, 3004));
    EXPECT_TRUE(cont.add(key5, 3005));
    EXPECT_FALSE(cont.add(key6, 3006));

    EXPECT_EQ(3001, cont.find(key1));
    EXPECT_EQ(3002, cont.find(key2));
    EXPECT_EQ(3003, cont.find(key3));
    EXPECT_EQ(3004, cont.find(key4));
    EXPECT_EQ(3005, cont.find(key5));

    // delete in random order

    cont.erase(key3);
    cont.erase(key2);
    cont.erase(key4);
    cont.erase(key5);
    cont.erase(key1);
    EXPECT_EQ(std::nullopt, cont.find(key1));
    EXPECT_EQ(std::nullopt, cont.find(key2));
    EXPECT_EQ(std::nullopt, cont.find(key3));
    EXPECT_EQ(std::nullopt, cont.find(key4));
    EXPECT_EQ(std::nullopt, cont.find(key5));
}

