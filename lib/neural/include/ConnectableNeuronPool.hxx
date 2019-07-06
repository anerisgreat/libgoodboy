#ifndef CONNECTABLE_NEURON_POOL_HXX
#define CONNECTABLE_NEURON_POOL_HXX
#include "ObjectPool.hxx"
#include "ConnectableNeuron.hxx"

namespace LibGoodBoy{

typedef ObjectPool<ConnectableNeuron,
    neuralConnectionPool_t&,
    neuralVal_t,
    neuralVal_t,
    neuralVal_t> connectableNeuronPool_t;

}//End namespace

#endif
