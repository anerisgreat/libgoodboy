#ifndef NEURAL_CONNECTABLE_NEURON_HXX
#define NEURAL_CONNECTABLE_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"

#include <memory>

namespace LibGoodBoy
{
    class NeuralConnection
    {
        public:
            std::weak_ptr<Neuron> ConnectedNeuronPtr;

            neuralVal_t Weight;
            neuralVal_t Alpha;

            NeuralConnection(){}
    };
}

#endif
