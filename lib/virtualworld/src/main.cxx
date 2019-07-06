#include "NeuralGlobals.hxx"
#include "NeuralUtils.hxx"
#include "GoodBoyNet.hxx"
#include <irrlicht/irrlicht.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace LibGoodBoy;
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void runTest(){
    IrrlichtDevice *device;
    device = irr::createDevice( video::EDT_SOFTWARE,
            dimension2d<u32>(640, 480),
            16,
            false,
            false,
            false,
            0);

    if (device == NULL){
        std::cout << "Device failed to create" << std::endl;
        return;
    }

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World" \
        "This is the Irrlicht Software renderer!",
        rect<s32>(10,10,260,22), true);
    srand(time(NULL));

    std::vector<float> evFilt;
    for(char i = 2; i < 100; i++){
        evFilt.push_back(2/((float)i*(float)i));
    }

    std::vector<float> connFilt;
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
    gNet.CreateInputs(inpPos);
    gNet.CreateOutputs(outPos);
    std::vector<neuralVal_t> inputs(2);
    std::vector<neuralVal_t> outputs(2);
    unsigned long iterNum = 0;
    while(device->run()){
        gNet.SetInputs(inputs);
        gNet.Iter();
        gNet.GetOutputs(outputs);

        neuralVal_t evolveAmount = 0;
        gNet.Evolve(evolveAmount);
        iterNum++;

        driver->beginScene(true, true, SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }

    device->drop();
}

int main(int argc, char *argv[]){
    runTest();
}
