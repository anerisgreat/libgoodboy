#include "gtest/gtest.h"
#include "NeuralConfig.hxx"
#include "NeuralConnection.hxx"
#include "ConnectableNeuron.hxx"
#include "InputNeuron.hxx"
#include "NeuralConnectionPool.hxx"
#include "TestUtils.hxx"

#include <memory>
#include <string>
#include <iostream>

namespace LibGoodBoy{
    namespace Test{

        TEST(TestNeuron, TestConnect){
            neuralConnectionPool_t neuralConnPool;

            InputNeuron* neurA 
                = new InputNeuron(GetConnectionFilter(), GetEvFilter());

            ConnectableNeuron* neurB 
                = new ConnectableNeuron(
                        GetConnectionFilter(), 
                        GetEvFilter(),
                        neuralConnPool,
                        DEFAULT_DEGR_FACTOR,
                        DEFAULT_MAX_START_WEIGHT,
                        DEFAULT_DEFAULT_ALPHA);

            neurB->Connect(neurA);
            std::string neurAIDSTR = neurA->GetJSON()[JSON_UID_KEY];
            std::string connectIDSTR
                = neurB->GetJSON()[JSON_INP_CONN_KEY][0][JSON_UID_KEY];
            ASSERT_STREQ(connectIDSTR.c_str(), neurAIDSTR.c_str());

            std::string neurBIDSTR = neurB->GetJSON()[JSON_UID_KEY];
            std::string outConnIDSTR 
                = neurA->GetJSON()[JSON_OUTP_CONN_KEY][0];

            ASSERT_STREQ(neurBIDSTR.c_str(), outConnIDSTR.c_str());
        }

        TEST(TestNeuron, TestWeights){
            neuralConnectionPool_t neuralConnPool;

            auto a = GetConnectionFilter();
            for(int i = 0; i < a.size(); i++){
                std::cout << "A" << a[i] << std::endl;
            }

            InputNeuron* neurA 
                = new InputNeuron(GetConnectionFilter(), GetEvFilter());

            ConnectableNeuron* neurB 
                = new ConnectableNeuron(
                        GetConnectionFilter(), 
                        GetEvFilter(),
                        neuralConnPool,
                        DEFAULT_DEGR_FACTOR,
                        DEFAULT_MAX_START_WEIGHT,
                        DEFAULT_DEFAULT_ALPHA);
            neuralVal_t currOutput = 0;
            neurB->Connect(neurA, 1, 2);
            for(int i = 0; i < 10; i++){
                currOutput = neurB->GetOutput();
                neurA->ResetOutputFlag();
                neurB->ResetOutputFlag();
            }
            std::cerr << "HERE?" << std::endl;
            ASSERT_EQ(currOutput, 0);
            std::cerr << "BA?" << std::endl;
            neurA->FeedInput(1);
            std::cerr << "FED" << std::endl;
            for(int i = 0; i < 10; i++){
                currOutput = neurB->GetOutput();
                std::cerr << "GOT AN OUTPUT AT LEAST" << std::endl;
                neurA->ResetOutputFlag();
                neurB->ResetOutputFlag();
                std::cerr << "RESET AT LEAST" << std::endl;
            }
            std::cerr << "CALC HERE?" << std::endl;
            ASSERT_GT(currOutput, 0);

            std::cerr << "OR HERE?" << std::endl;
            neuralVal_t prevAlpha = 
                neurB->
                    GetJSON()[JSON_INP_CONN_KEY][0][JSON_CONNECTION_ALPHA_KEY];
            neurB->Evolve(10);
            neurA->Evolve(10);

            neuralVal_t currentAlpha = 
                neurB->
                    GetJSON()[JSON_INP_CONN_KEY][0][JSON_CONNECTION_ALPHA_KEY];

            std::cerr << "MAYB HERE?" << std::endl;
            ASSERT_GT(currentAlpha, prevAlpha);
        }

    }
}
