#include "GoodBoyNet.hxx"
#include "NeuralGlobals.hxx"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>

int main(int argc, char *argv[]){
    std::cout << "Loading file: " << argv[1] << std::endl;
    std::ifstream t(argv[1]);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());

    LibGoodBoy::json_t parsed = LibGoodBoy::json_t::parse(str);
    LibGoodBoy::GoodBoyNet net = LibGoodBoy::GoodBoyNet(parsed);
    LibGoodBoy::json_t netJSON = net.GetJSON();

    std::cout << net.GetJSONString() << std::endl;
}
