#include "NeuralGlobals.hxx"
#include "NeuralUtils.hxx"
#include "GoodBoyNet.hxx"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "libGoodBoyConfig.hxx"

using namespace LibGoodBoy;

void runTest(){
    srand(time(NULL));
    const neuralVal_t WORLD_SIZE = 100;

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
            (neuralVal_t)0.01, //degr_factor
            (neuralVal_t)2, //max start weight
            (neuralVal_t)2, //default alpha,
            (neuralVal_t)0.01, //generation factor
            true); 

    std::vector<pos_t> inpPos = {{-1, -1, 0}, {1, -1, 0}};
    std::vector<pos_t> outPos = {{0, 1, 1}, {0, 1, -1}};
    gNet.CreateInputGroup("IN", inpPos);
    gNet.CreateOutputGroup("OUT", outPos);
    std::vector<neuralVal_t> inputs(2);
    std::vector<neuralVal_t> outputs(2);
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

        //Run ten iterations
        gNet.SetInputGroupValues("IN", inputs);
        gNet.Iter();
        gNet.GetOutputGroupValues("OUT", outputs);

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

        //Evolve
        gNet.Evolve(evolveAmount * 10);
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
        if(iterNum % 1000 == 0){
            std::ofstream jsonstream("out.json");
            jsonstream << gNet.GetJSONString();
            jsonstream.close();
        }
        usleep(1000);
    }
}

int main(int argc, char *argv[]){
    std::cout << "Version: " << libGoodBoy_VERSION_MAJOR << "." \
        << libGoodBoy_VERSION_MINOR << std::endl;
    runTest();
}
