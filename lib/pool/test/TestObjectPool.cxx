#include "gtest/gtest.h"
#include "ObjectPool.hxx"

#include <list>

namespace LibGoodBoy{
    namespace Test{
        class TestResetableClass : public Resetable{
            private:
                bool m_flag;
            public:
                TestResetableClass(): m_flag(false) {}
                TestResetableClass(bool p_flag): m_flag(p_flag) {}
                ~TestResetableClass(){}

                void Reset(){m_flag = false;}
                bool getFlag(){return m_flag;}
                void setFlag(){m_flag = true;}
        };

        TEST(Pool, ObjectPool){
            ObjectPool<TestResetableClass> resetObjectPool;
            std::list<TestResetableClass*> alloced;

            std::size_t poolSize = resetObjectPool.PoolSize();

            ASSERT_EQ(poolSize, 0);

            for(std::size_t i = 0; i < 16 - poolSize; i++){
                alloced.push_back(resetObjectPool.AllocElement());
            }

            ASSERT_EQ(resetObjectPool.PoolSize(), 16);

            bool flag = false;
            poolSize = resetObjectPool.PoolSize();

            auto iter = alloced.begin();
            while(iter != alloced.end()){
                TestResetableClass * e = *iter;
                iter = alloced.erase(iter);
                e->setFlag();
                resetObjectPool.Release(e);
            }

            for(std::size_t i = 0; i < poolSize; i++){
                TestResetableClass* e = resetObjectPool.AllocElement();
                flag = flag | e->getFlag();
                alloced.push_back(e);
            }

            ASSERT_EQ(poolSize, resetObjectPool.PoolSize());
            ASSERT_FALSE(flag);
            ASSERT_NE(*alloced.begin(), *(++alloced.begin()));
        }

        TEST(Pool, ObjectPoolWithParams){
            ObjectPool<TestResetableClass, bool> resetObjectPool(true);

            TestResetableClass* ptr = resetObjectPool.AllocElement();
            ASSERT_TRUE(ptr->getFlag());
        }
    }
}
