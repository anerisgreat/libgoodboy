#include "gtest/gtest.h"
#include "NeuralConfig.hxx"
#include "NeuralConnection.hxx"
#include "ConnectableNeuron.hxx"
#include "InputNeuron.hxx"
#include "NeuralConnectionPool.hxx"
#include "TestUtils.hxx"
#include <memory>

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
        }

    }
}
