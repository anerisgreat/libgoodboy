#ifndef NEURAL_CONNECTION_HXX
#define NEURAL_CONNECTION_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"

#include <memory>

namespace LibGoodBoy
{
    class NeuralConnection : public Resetable
    {
        public:
            std::weak_ptr<Neuron> ConnectedNeuronPtr;

            neuralVal_t Weight;
            neuralVal_t Alpha;

            NeuralConnection(){}
            
            void Reset(){}
    };
}

#endif
