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
            typedef unsigned int alpha_t;

            std::shared_ptr<Neuron> ConnectedNeuronPtr;

            neuralVal_t Weight;
            alpha_t Alpha;

            NeuralConnection(){}
    };
}

#endif
