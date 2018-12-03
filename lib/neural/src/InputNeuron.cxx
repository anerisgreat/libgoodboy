#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "InputNeuron.hxx"

#include <vector>

namespace LibGoodBoy
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    InputNeuron::InputNeuron( 
            const std::vector<neuralVal_t>& p_outputFilterTaps,
            const std::vector<neuralVal_t>& p_evolveFilterTaps)
        :
            Neuron(p_outputFilterTaps, p_evolveFilterTaps),
            m_nextInput(0)
    {
    }

    Neuron::~Neuron()
    {
    }

    void InputNeuron::FeedInput(neuralVal_t p_input)
    {
        m_nextInput = p_input; 
    }

    neuralVal_t InputNeuron::calcOutput()
    {
        return m_nextInput;
    }

    void InputNeuron::postBackProbe(){}
    void InputNeuron::postForwardProbe(){}
    void InputNeuron::evolveSelf(neuralVal_t p_amount){}
    void InputNeuron::postPurgeConnections(
            const std::list<std::shared_ptr<Neuron>>& p_toPurge){}

    void InputNeuron::postFlush()
    {
        m_nextInput = 0;   
    }


}
