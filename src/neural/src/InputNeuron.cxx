#include "libGoodBoyConfig.hxx"
#include "neural/NeuralConfig.hxx"
#include "neural/InputNeuron.hxx"

#include <vector>

namespace lib_good_boy::neural
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    InputNeuron::InputNeuron( 
            const std::vector<neuralVal_t>& outputFilterTaps,
            const std::vector<neuralVal_t>& evolveFilterTaps)
        :
            Neuron(outputFilterTaps, evolveFilterTaps),
            m_nextInput(0)
    {
    }

    Neuron::~Neuron()
    {
    }

    void InputNeuron::FeedInput(neuralVal_t input)
    {
        m_nextInput = input; 
    }

    neuralVal_t InputNeuron::calcOutput()
    {
        return m_nextInput;
    }

    void InputNeuron::postBackProbe(){}
    void InputNeuron::postForwardProbe(){}
    void InputNeuron::evolveSelf(neuralVal_t amount){}
    void InputNeuron::postPurgeConnections(
            const std::list<std::shared_ptr<Neuron>>& toPurge){}

    void InputNeuron::postFlush()
    {
        m_nextInput = 0;   
    }


}
