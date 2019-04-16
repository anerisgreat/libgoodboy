#include "Test.hxx"
//#include "GoodBoyNet.hxx"
#include "ObjectPool.hxx"
#include "InputNeuron.hxx"
#include "NeuralConnection.hxx"
#include "ConnectableNeuron.hxx"
#include "Neuron.hxx"
#include "GoodBoyNet.hxx"
#include "NeuralGlobals.hxx"

#include <stdio.h>
#include <iostream>
#include <list>
#include <nlohmann/json.hpp>
#include <string>

using namespace LibGoodBoy;
int main(int argc, char *argv[]){
    coord_t x = {2.0, 2.0, 2.0};
    coord_t y = {2.0, 5.0, 6.0};
    pos_t fuck = GetDistance(x, y);
    std::cout << fuck << std::endl;
}
