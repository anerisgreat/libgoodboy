#ifndef NEURAL_CONNECTION_POOL_HXX
#define NEURAL_CONNECTION_POOL_HXX

#include "ObjectPool.hxx"
#include "NeuralConnection.hxx"

namespace LibGoodBoy{

typedef ObjectPool<NeuralConnection> neuralConnectionPool_t;

}//End namespace

#endif
