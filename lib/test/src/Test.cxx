#include "Test.hxx"
#include "Neuron.hxx"
#include "ObjectPool.hxx"

#include <stdio.h>
#include <iostream>
#include <list>

int main(int argc, char *argv[])
{
    std::cout << "HI" << std::endl;

    int myints[]= {17,89,7,14};
    std::list<int> stupid (myints,myints+4);
    stupid.push_back(2);
    stupid.push_back(2);
    stupid.push_back(2);
    stupid.push_back(3);

    for (std::list<int>::iterator it=stupid.begin(); it!=stupid.end(); ++it)
    	std::cout << ' ' << *it;
  	std::cout << '\n';
}
