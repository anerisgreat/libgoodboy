#include "gtest/gtest.h"
#include "KeyedGroupCollection.hxx"

#include <list>

namespace LibGoodBoy{
    namespace Test{

        TEST(KeyedGroupCollection, BasicGroupCreation){
            KeyedGroupCollection<int> groupCollection;
            groupCollection["even"] = {2, 4, 6};
            auto iter = groupCollection["even"].begin();
            ASSERT_EQ(*iter, 2);
            iter++;
            ASSERT_EQ(*iter, 4);
            iter++;
            ASSERT_EQ(*iter, 6);
            iter++;
            ASSERT_EQ(iter, groupCollection["even"].end());
            groupCollection["odd"] = {1, 3, 5};
            iter = groupCollection["odd"].begin();
            ASSERT_EQ(*iter, 1);
            iter++;
            ASSERT_EQ(*iter, 3);
            iter++;
            ASSERT_EQ(*iter, 5);
            groupCollection["even"].insert(groupCollection["even"].begin(), 0);
            ASSERT_EQ(*(groupCollection["even"].begin()), 0);
            ASSERT_EQ(*(++groupCollection["even"].begin()), 2);
        }

        TEST(KeyedGroupCollection, IteratorEqauality){
            KeyedGroupCollection<int> groupCollection;
            groupCollection["even"] = {2, 4, 6};
            groupCollection["odd"] = {1, 3, 5};
            auto iter = groupCollection.begin();
            auto iter2 = groupCollection.begin();
            ASSERT_TRUE(iter == iter2);
            ASSERT_FALSE(iter != iter2);
            ++iter;
            ASSERT_TRUE(iter != iter2);
            ASSERT_FALSE(iter == iter2);
            ++iter2;
            ASSERT_TRUE(iter == iter2);
            ASSERT_FALSE(iter != iter2);
            iter = groupCollection.end();
            ASSERT_TRUE(iter != iter2);
            ASSERT_FALSE(iter == iter2);
            iter2 = groupCollection.end();
            ASSERT_TRUE(iter == iter2);
            ASSERT_FALSE(iter != iter2);
            auto iter3 = groupCollection.begin();
            for(int i = 0;
                    i < groupCollection["even"].size() + \
                        groupCollection["odd"].size();
                    ++i)
            {
                ++iter3;
            }
            ASSERT_TRUE(iter3 == groupCollection.end());
            ASSERT_FALSE(iter3 != groupCollection.end());

        }

        TEST(KeyedGroupCollection, GroupMemberIteration){
            KeyedGroupCollection<int> groupCollection;
            groupCollection["even"] = {2, 4, 6};
            groupCollection["odd"] = {1, 3, 5};
            for(auto iter = groupCollection.begin();
                    iter != groupCollection.end();
                    ++iter)
            {
                //TODO: Add to string, assert it all equals
            }
        }
    }
}

