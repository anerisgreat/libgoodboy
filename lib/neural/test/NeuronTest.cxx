#include "gtest/gtest.h"
#include "NeuralConfig.hxx"
#include "ConnectableNeuron.hxx"
#include "InputNeuron.hxx"
#include "NeuralConnectionPool.hxx"
#include "TestUtils.hxx"
#include "NeuralUtils.hxx"

#include <string>

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

            ConnectableNeuron* neurC
                = new ConnectableNeuron(
                        GetConnectionFilter(), 
                        GetEvFilter(),
                        neuralConnPool,
                        DEFAULT_DEGR_FACTOR,
                        DEFAULT_MAX_START_WEIGHT,
                        DEFAULT_DEFAULT_ALPHA);

            neurC->Connect(neurB);
            std::string neurCIDSTR = neurC->GetJSON()[JSON_UID_KEY];
            std::string cConnectIDSTR = neurC->
                GetJSON()[JSON_INP_CONN_KEY][0][JSON_UID_KEY];
            std::string bOutConnectIDSTR = neurB->
                GetJSON()[JSON_OUTP_CONN_KEY][0];

            ASSERT_STREQ(neurCIDSTR.c_str(), bOutConnectIDSTR.c_str());
            ASSERT_STREQ(cConnectIDSTR.c_str(), neurBIDSTR.c_str());

            std::list<Neuron*> purgeList;
            purgeList.push_back(neurB);

            neurA->PurgeConnections(purgeList);
            neurB->PurgeConnections(purgeList);
            neurC->PurgeConnections(purgeList);

            ASSERT_EQ(neurA->GetJSON()[JSON_OUTP_CONN_KEY].size(), 0);
            ASSERT_EQ(neurC->GetJSON()[JSON_INP_CONN_KEY].size(), 0);
            ASSERT_EQ(neurB->GetJSON()[JSON_OUTP_CONN_KEY].size(), 0);
            ASSERT_EQ(neurB->GetJSON()[JSON_INP_CONN_KEY].size(), 0);
        }

        TEST(TestNeuron, TestWeights){
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
            neuralVal_t currOutput = 0;
            neurB->Connect(neurA, 1, 2);
            for(int i = 0; i < 1; i++){
                currOutput = neurB->GetOutput();
                neurA->ResetOutputFlag();
                neurB->ResetOutputFlag();
            }
            ASSERT_EQ(currOutput, 0);
            neurA->FeedInput(1);
            for(int i = 0; i < 10; i++){
                currOutput = neurB->GetOutput();
                neurA->ResetOutputFlag();
                neurB->ResetOutputFlag();
            }
            ASSERT_GT(currOutput, 0);

            neuralVal_t prevAlpha = 
                neurB->
                    GetJSON()[JSON_INP_CONN_KEY][0][JSON_CONNECTION_ALPHA_KEY];
            neurB->Evolve(1);
            neurA->Evolve(1);
            neurB->ResetContributionFlag();
            neurA->ResetContributionFlag();

            neuralVal_t currentAlpha = neurB->
                    GetJSON()[JSON_INP_CONN_KEY][0][JSON_CONNECTION_ALPHA_KEY];

            ASSERT_GT(currentAlpha, prevAlpha);
            prevAlpha = currentAlpha;
            neurA->FeedInput(-1);
            neuralVal_t inputOutput = 0;
            for(int i = 0; i < 10; i++){
                inputOutput = neurA->GetOutput();
                currOutput = neurB->GetOutput();
                neurA->ResetOutputFlag();
                neurB->ResetOutputFlag();
            }
            ASSERT_LT(currOutput, 0);

            neurB->Evolve(0);
            neurA->Evolve(0);
            neurB->ResetContributionFlag();
            neurA->ResetContributionFlag();
            currentAlpha = neurB->
                    GetJSON()[JSON_INP_CONN_KEY][0][JSON_CONNECTION_ALPHA_KEY];

            ASSERT_LT(currentAlpha, prevAlpha);

            ASSERT_LT(currOutput, 0);
            for(int i = 0; i < 10; i++){
                currOutput = neurB->GetOutput();
                neurA->ResetOutputFlag();
                neurB->ResetOutputFlag();
                neurB->Evolve(-1);
                neurA->Evolve(-1);
                neurB->ResetContributionFlag();
                neurA->ResetContributionFlag();
            }

            ASSERT_TRUE(neurB->GetJSON()[JSON_INP_CONN_KEY].size() == 0);
            ASSERT_TRUE(neurB->GetJSON()[JSON_OUTP_CONN_KEY].size() == 0);
        }

        TEST(TestNeuron, TestContribution){
            neuralConnectionPool_t neuralConnPool;

            InputNeuron* neurA 
                = new InputNeuron(GetConnectionFilter(), GetEvFilter());

            neurA->FeedInput(1);
            ASSERT_EQ(neurA->GetOutput(), GetConnectionFilter()[0]);
            neurA->ResetOutputFlag();

            neuralVal_t contribution = neurA->GetContribution();
            neurA->ResetContributionFlag();
            neuralVal_t predicted = GetConnectionFilter()[0]*GetEvFilter()[0];
            ASSERT_EQ(contribution, predicted);

            InputNeuron* neurB 
                = new InputNeuron(GetConnectionFilter(), GetEvFilter());

            neurB->FeedInput(-1);
            ASSERT_EQ(neurB->GetOutput(), -GetConnectionFilter()[0]);
            neurB->ResetOutputFlag();

            contribution = neurB->GetContribution();
            neurB->ResetContributionFlag();
            predicted = GetConnectionFilter()[0]*GetEvFilter()[0];

            ASSERT_EQ(contribution, predicted);
        }

    }
}
