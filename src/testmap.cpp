#include <gtest/gtest.h> 	 	    		
#include "../include/KeyValueMap.h"
#include <string.h>

TEST(KeyValueMap, EmptyTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = KeyValueMapCreate();
    EXPECT_EQ(KeyValueMapSize(kvmap), 0);
    KeyValueMapDestroy(kvmap);
}

TEST(KeyValueMap, EmptyGetTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = KeyValueMapCreate();
    EXPECT_TRUE(KeyValueMapFind(kvmap, "key") == nullptr);
    KeyValueMapDestroy(kvmap);
}

TEST(KeyValueMap, SetTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = KeyValueMapCreate();

    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");
    
    EXPECT_EQ(KeyValueMapSize(kvmap), 3);
    EXPECT_TRUE(KeyValueMapFind(kvmap, "four") == nullptr);

    KeyValueMapDestroy(kvmap);

}

TEST(KeyValueMap, GetTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;
    EXPECT_EQ(KeyValueMapGet(kvmap, "one"), nullptr);

    kvmap = KeyValueMapCreate();
    EXPECT_EQ(KeyValueMapGet(kvmap, NULL), nullptr);

    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");

    ASSERT_STREQ(KeyValueMapGet(kvmap, "one"), "1");
    ASSERT_STREQ(KeyValueMapGet(kvmap, "two"), "2");
    ASSERT_STREQ(KeyValueMapGet(kvmap, "three"), "3");
    EXPECT_EQ(KeyValueMapGet(kvmap, "four"), nullptr);

    KeyValueMapDestroy(kvmap);
}


TEST(KeyValueMap, DeleteTest){
    SKeyValueMapRef kvmap = nullptr;
    EXPECT_EQ(KeyValueMapDelete(kvmap, "one"), false);
   
    kvmap = KeyValueMapCreate();
    EXPECT_EQ(KeyValueMapDelete(kvmap, NULL), false);

    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");
    KeyValueMapSet(kvmap, "four", "4");
    KeyValueMapSet(kvmap, "five", "5");
 //   KeyValueMapSet(kvmap, "six", "6");

    printf("%s @ line: %d\n",__FILE__,__LINE__);
    ASSERT_EQ(KeyValueMapDelete(kvmap, "four"), true);
    printf("%s @ line: %d\n",__FILE__,__LINE__);
    ASSERT_EQ(KeyValueMapDelete(kvmap, "three"), true);
    printf("%s @ line: %d\n",__FILE__,__LINE__);
    ASSERT_EQ(KeyValueMapDelete(kvmap, "two"), true);
    printf("%s @ line: %d\n",__FILE__,__LINE__);
 //   ASSERT_EQ(KeyValueMapDelete(kvmap, "five"), true);
 //   printf("%s @ line: %d\n",__FILE__,__LINE__);

 //   EXPECT_EQ(KeyValueMapSize(kvmap), 1);
    
 //   KeyValueMapDestroy(kvmap);
}


TEST(KeyValueMap, ForwardIteratorTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;
    SKeyValueMapIteratorRef begin = KeyValueMapBegin(kvmap);
    EXPECT_EQ(begin, nullptr);

    kvmap = KeyValueMapCreate();
    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");

    begin = KeyValueMapBegin(kvmap);
    EXPECT_TRUE(begin != nullptr);
    int size = KeyValueMapSize(kvmap);

    for(int i = 0; i < size + 1; ++i){
         EXPECT_TRUE(begin != nullptr);
         begin = KeyValueMapIteratorNext(begin);
    }
    EXPECT_TRUE(begin == nullptr);

}

TEST(KeyValueMap, ReverseIteratorTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;
    SKeyValueMapIteratorRef end = KeyValueMapEnd(kvmap);
    EXPECT_EQ(end, nullptr);

    kvmap = KeyValueMapCreate();
    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");

    end = KeyValueMapEnd(kvmap);
    EXPECT_TRUE(end != nullptr);
    int size = KeyValueMapSize(kvmap);

    for(int i = 0; i < size + 1; ++i){
         EXPECT_TRUE(end != nullptr);
         end = KeyValueMapIteratorPrevious(end);
    }
    EXPECT_TRUE(end == nullptr);
}

TEST(KeyValueMap, IteratorFindTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;
    SKeyValueMapIteratorRef end = KeyValueMapEnd(kvmap);

    kvmap = KeyValueMapCreate();
    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");
    int size = KeyValueMapSize(kvmap);
    SKeyValueMapIteratorRef curr = KeyValueMapBegin(kvmap);
    

    EXPECT_TRUE(curr != nullptr);
    ASSERT_STREQ(KeyValueMapIteratorValue(curr), "2");
    curr = KeyValueMapIteratorNext(curr);
    ASSERT_STREQ(KeyValueMapIteratorValue(curr), "3");
    curr = KeyValueMapIteratorNext(curr);
    ASSERT_STREQ(KeyValueMapIteratorValue(curr), "1");
    curr = KeyValueMapIteratorNext(curr);
    ASSERT_STREQ(KeyValueMapIteratorValue(curr), NULL);
    curr = KeyValueMapIteratorNext(curr);
    EXPECT_TRUE(curr == nullptr);


}

TEST(KeyValueMap, IteratorFindSetTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;
    SKeyValueMapIteratorRef end = KeyValueMapEnd(kvmap);

    kvmap = KeyValueMapCreate();
    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");

    ASSERT_STREQ(KeyValueMapIteratorKey(KeyValueMapBegin(kvmap)), "two");
    ASSERT_STREQ(KeyValueMapIteratorValue(KeyValueMapBegin(kvmap)), "2");


}

TEST(KeyValueMap, IteratorFindDeleteTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;
    SKeyValueMapIteratorRef end = KeyValueMapEnd(kvmap);

    kvmap = KeyValueMapCreate();
    KeyValueMapSet(kvmap, "one", "1");
    KeyValueMapSet(kvmap, "two", "2");
    KeyValueMapSet(kvmap, "three", "3");

    SKeyValueMapIteratorRef begin = KeyValueMapBegin(kvmap);
    SKeyValueMapIteratorRef next = KeyValueMapIteratorNext(begin);
    EXPECT_EQ(KeyValueMapIteratorDelete(KeyValueMapBegin(kvmap)), next);
}

TEST(KeyValueMap, InvalidParameterTest){
    /* INSERT TESTS HERE */
    SKeyValueMapRef kvmap = nullptr;

    // All invalid NULL input
    EXPECT_EQ(KeyValueMapDestroy(kvmap), false);
    EXPECT_EQ(KeyValueMapSize(kvmap), 0);
    EXPECT_EQ(KeyValueMapGet(kvmap, "key"), nullptr);
    EXPECT_EQ(KeyValueMapSet(kvmap, nullptr, "1"), false);
    EXPECT_EQ(KeyValueMapSet(kvmap, "1", nullptr), false);
    EXPECT_EQ(KeyValueMapDelete(kvmap, nullptr), false);
    EXPECT_EQ(KeyValueMapBegin(kvmap), nullptr);
    EXPECT_EQ(KeyValueMapEnd(kvmap), nullptr);
    EXPECT_EQ(KeyValueMapFind(kvmap, nullptr), nullptr);
    EXPECT_EQ(KeyValueMapIteratorPrevious(nullptr), nullptr); 
    EXPECT_EQ(KeyValueMapIteratorKey(nullptr), nullptr); 
    EXPECT_EQ(KeyValueMapIteratorValue(nullptr), nullptr); 
    EXPECT_EQ(KeyValueMapIteratorSet(nullptr, "0"), false); 
    EXPECT_EQ(KeyValueMapIteratorDelete(nullptr), nullptr); 
  
}