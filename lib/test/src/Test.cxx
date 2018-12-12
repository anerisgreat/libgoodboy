#include "Test.hxx"
#include "ObjectPool.hxx"

#include <stdio.h>
#include <iostream>
#include <list>
#include <nlohmann/json.hpp>
#include <string>

class MyClass(){
    private:
        float m_a;
        float m_b;
    public:
        MyClass(float p_a, float p_b) : m_a(p_a), m_b(p_b) {}
        void Print(std::cout << m_a << ' ' << m_b << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "HI" << std::endl;

    ObjectPool<MyClass, float, float> ohBoy(1, 2);
    std::vector<std::shared_ptr<MyClass>> stupidVec();
    for(int i = 0; i < 10; i++){
        stupidVec.push_back(ObjectPool.AllocElement());
    }

    for(int i = 0; i < 10; i++){
        stupidVec[i]->Print();
    }
}
