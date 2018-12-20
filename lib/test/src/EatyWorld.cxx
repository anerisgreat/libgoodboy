#include "NeuralConfig.hxx"
#include "NeuralUtils.hxx"
#include "GoodBoyNet.hxx"
#include <iostream>

using namespace LibGoodBoy;

void runTest(){
    std::size_t nInputs = 4;
    std::size_t nOutputs = 2;

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

    GoodBoyNet gNet(connFilt,
            evFilt,
            (neuralSize_t)nInputs, // n_inputs
            (neuralSize_t)nInputs,
            (neuralVal_t)0.05, //degr_factor
            (neuralVal_t)1, //max start weight
            (neuralVal_t)2, //default alpha,
            (neuralVal_t)0.05, //generation factor
            true); 

    std::vector<neuralVal_t> inputs(nInputs);
    std::vector<neuralVal_t> outputs(nInputs);
    while(true){
        //Define inputs

        //Run ten iterations
        gNet.SetInputs(inputs);
        gNet.Iter();
        gNet.GetOutputs(outputs);

        //Evolve
        neuralVal_t evolveAmount = 0;
        std::vector<neuralVal_t>::iterator outputIter;
        std::vector<neuralVal_t>::iterator inputIter;
        inputIter = inputs.begin();
        outputIter = outputs.begin();
        for(; outputIter != outputs.end() && inputIter != inputs.end();
                ++outputIter, ++inputIter)
        {
            evolveAmount += 
                ((*outputIter) * (*inputIter)) / (neuralVal_t)nInputs;
        }
        gNet.Evolve(evolveAmount* 100);
        std::cout << evolveAmount << std::endl;
    }
}

int main(int argc, char *argv[]){
    runTest();
}
