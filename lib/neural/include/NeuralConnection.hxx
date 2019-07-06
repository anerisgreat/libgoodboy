#ifndef NEURAL_CONNECTION_HXX
#define NEURAL_CONNECTION_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralGlobals.hxx"
#include "Neuron.hxx"

#include <memory>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

namespace LibGoodBoy{

class NeuralConnection : public Resetable{
    public:
        Neuron* ConnectedNeuronPtr;

        neuralVal_t Weight;
        neuralVal_t Alpha;

        NeuralConnection(){}

        void Reset(){}

        json_t GetJSON(){
            json_t retJSON;
            retJSON[JSON_CONNECTION_ALPHA_KEY] = Alpha;
            retJSON[JSON_CONNECTION_WEIGHT_KEY] = Weight;
            retJSON[JSON_UID_KEY] = 
                boost::uuids::to_string(ConnectedNeuronPtr->GetUID());
            return retJSON;
        }
};

}//End namespace

#endif
