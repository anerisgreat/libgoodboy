#include "Test.hxx"
//#include "GoodBoyNet.hxx"
#include "ObjectPool.hxx"
#include "InputNeuron.hxx"
#include "NeuralConnection.hxx"
#include "ConnectableNeuron.hxx"
#include "Neuron.hxx"
#include "GoodBoyNet.hxx"

#include <stdio.h>
#include <iostream>
#include <list>
#include <nlohmann/json.hpp>
#include <string>

using namespace LibGoodBoy;
void testNet(){
    std::vector<float> evFilt;
    for(char i = 2; i < 100; i++){
        evFilt.push_back(2/((float)i*(float)i));
    }

    std::vector<float> connFilt;
    connFilt.push_back(0.1);
    connFilt.push_back(0.2);
    connFilt.push_back(0.4);
    connFilt.push_back(0.2);
    connFilt.push_back(0.1);
    
    GoodBoyNet gNet(connFilt, evFilt, (float)2, (float)2, 0.05, (float)1, (float)2, 0.1, true);

    std::vector<float> outVec(2);
    gNet.SetInput(0, 1);
    gNet.SetInput(1, 1);
    std::cout << "BEGIN BEGIN BEGIN" << std::endl;
    for(int i = 0; i < 10000; i++){
        gNet.Iter();
        gNet.GetOutputs(outVec);
        gNet.Evolve(100);
    }

    std::cout << gNet.GetJSON() << std::endl;
}

void testNeuron(){

    std::vector<float> evFilt;
    for(char i = 2; i < 100; i++){
        evFilt.push_back(2/((float)i*(float)i));
    }

    std::vector<float> connFilt;
    connFilt.push_back(0.1);
    connFilt.push_back(0.2);
    connFilt.push_back(0.4);
    connFilt.push_back(0.2);
    connFilt.push_back(0.1);
    ObjectPool<InputNeuron, std::vector<float>&, std::vector<float>&> 
        inpNeuronPool(connFilt, evFilt);
    auto inpNeuron = inpNeuronPool.AllocElement();

    ObjectPool<NeuralConnection> neuralConnPool;

    ObjectPool<ConnectableNeuron,
        std::vector<float>&,
        std::vector<float>&,
        ObjectPool<NeuralConnection>&,
        float,
        float,
        float>
            connNeuronPool(connFilt, evFilt, neuralConnPool, 0.01, 2, 2);

    auto connNeuron = connNeuronPool.AllocElement();
    connNeuron->Connect(inpNeuron, (float)1, (float)10);

    inpNeuron->FeedInput(0);

    for(int i = 0; i < 10; i++){
        float t = connNeuron->GetOutput();
        std::cout << t << " IS OUTPUT" << std::endl;
        connNeuron->GetOutput();
        connNeuron->ResetOutputFlag();
        inpNeuron->ResetOutputFlag();
    }

    inpNeuron->FeedInput(1);

    for(int i = 0; i < 10; i++){
        float t = connNeuron->GetOutput();
        std::cout << t << " IS OUTPUT" << std::endl;
        std::cout << inpNeuron->GetOutput() << " IS OUTPUT FOR INP" << std::endl;
        connNeuron->ResetOutputFlag();
        inpNeuron->ResetOutputFlag();
    }
}

int main(int argc, char *argv[]){
    testNet();
    //testNeuron();
    
}
