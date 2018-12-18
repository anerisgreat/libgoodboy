#ifndef CONNECTABLE_NEURON_POOL_HXX
#define CONNECTABLE_NEURON_POOL_HXX
#include "ObjectPool.hxx"
#include "ConnectableNeuron.hxx"

namespace LibGoodBoy{
    typedef ObjectPool<ConnectableNeuron,
        const std::vector<neuralVal_t>&,
        const std::vector<neuralVal_t>&,
        neuralConnectionPool_t&,
        neuralVal_t,
        neuralVal_t,
        neuralVal_t> connectableNeuronPool_t;
}

#endif
