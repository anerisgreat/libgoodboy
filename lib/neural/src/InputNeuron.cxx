#include "InputNeuron.hxx"
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

    InputNeuron::~InputNeuron(){
    }

    void InputNeuron::Evolve(neuralVal_t p_amount){}

    void InputNeuron::FeedInput(neuralVal_t p_input){
        m_nextInput = p_input; 
    }

    void InputNeuron::Reset(){
        Neuron::Reset();
        m_nextInput = 0;
    }

    json_t InputNeuron::GetJSON() const{
        json_t retJSON = Neuron::GetJSON();
        retJSON[JSON_NEURON_TYPE_KEY] = JSON_NEURON_TYPE_INPUT_VAL; 
        return retJSON;
    }

    neuralVal_t InputNeuron::calcOutput(){
        return m_nextInput;
    }

    void InputNeuron::postBackProbe(){}
    void InputNeuron::postForwardProbe(){}
}
