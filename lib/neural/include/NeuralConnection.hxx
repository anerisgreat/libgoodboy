#ifndef NEURAL_CONNECTION_HXX
#define NEURAL_CONNECTION_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"

#include <memory>
#include <string>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

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

            json_t GetJSON(){
                json_t retJSON;
                retJSON[JSON_CONNECTION_ALPHA_KEY] = std::to_string(Alpha);
                retJSON[JSON_CONNECTION_WEIGHT_KEY] = std::to_string(Weight);
                retJSON[JSON_UID_KEY] = 
                    boost::uuids::to_string(ConnectedNeuronPtr.lock()->GetUID());
                return retJSON;
            }
    };
}

#endif
