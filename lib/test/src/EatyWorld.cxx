#include "NeuralConfig.hxx"
#include "NeuralUtils.hxx"
#include "GoodBoyNet.hxx"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace LibGoodBoy;

void runTest(){
    srand(time(NULL));
    const neuralVal_t WORLD_SIZE = 100;
    std::size_t nInputs = 2;
    std::size_t nOutputs = 2;

    std::vector<float> evFilt;
    for(char i = 2; i < 100; i++){
        evFilt.push_back(2/((float)i*(float)i));
    }

    std::vector<float> connFilt;
    //connFilt.push_back(0.1);
    //connFilt.push_back(0.2);
    //connFilt.push_back(0.4);
    //connFilt.push_back(0.2);
    //connFilt.push_back(0.1);
    connFilt.push_back(1);

    GoodBoyNet gNet(connFilt,
            evFilt,
            (neuralSize_t)nInputs, // n_inputs
            (neuralSize_t)nInputs,
            (neuralVal_t)0.01, //degr_factor
            (neuralVal_t)1, //max start weight
            (neuralVal_t)2, //default alpha,
            (neuralVal_t)0.01, //generation factor
            true); 

    std::vector<neuralVal_t> inputs(nInputs);
    std::vector<neuralVal_t> outputs(nOutputs);
    neuralVal_t foodPos = RandInRange<neuralVal_t>(0, WORLD_SIZE);
    neuralVal_t currentPos = RandInRange<neuralVal_t>(0, WORLD_SIZE);
    unsigned long iterNum = 0;
    while(true){
        //Define inputs
        if(foodPos < currentPos){
            inputs[0] = 1;
            inputs[1] = 0;
        }
        else{
            inputs[0] = 0;
            inputs[1] = 1;
        }
/*
        if(WORLD_SIZE - currentPos > 5){
            inputs[2] = 0;
            inputs[3] = 1;
        }
        else if(currentPos < 5){
            inputs[2] = 1;
            inputs[3] = 0;
        }
        else{
            inputs[2] = 0;
            inputs[3] = 0;
        }
*/

        //Run ten iterations
        gNet.SetInputs(inputs);
        gNet.Iter();
        gNet.GetOutputs(outputs);

        float speed = outputs[0]-outputs[1];
        currentPos += speed;

        neuralVal_t evolveAmount = 0;

        if(currentPos > WORLD_SIZE){
            currentPos = WORLD_SIZE;
            evolveAmount -= 0.5;
        }
        else if(currentPos < 0){
            currentPos = 0;
            evolveAmount -= 0.5;
        }
        std::cout << currentPos << ' ';

        if(abs(foodPos - currentPos) < 2){
            evolveAmount += 1;
            foodPos = RandInRange<neuralVal_t>(0, WORLD_SIZE);
        }
        //evolveAmount -= (abs(outputs[0]) + abs(outputs[1])) / 10000;

        //Evolve
        gNet.Evolve(evolveAmount * 1000);
        std::cout << evolveAmount << ' ' << gNet.GetMidSize() << ' ' << iterNum << std::endl;
        iterNum++;

        std::string outString = "";
        for(int i = 0; i < (int)WORLD_SIZE; i++){
            if(abs(i - foodPos) <= 0.5){
                std::cout << 'F';
            }
            if(abs(i - currentPos) <= 0.5){
                std::cout << 'X';
            }
            else{
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
        //usleep(3000);
    }
}

int main(int argc, char *argv[]){
    runTest();
}
