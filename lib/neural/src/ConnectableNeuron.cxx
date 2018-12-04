#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"
#include "NeuralConnection.hxx"
#include "ObjectPool.hxx"

namespace LibGoodBoy
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    ConnectableNeuron::ConnectableNeuron(
            const std::vector<neuralVal_t>& p_outputFilterTaps,
            const std::vector<neuralVal_t>& p_evolveFilterTaps,
            const std::weak_ptr<ObjectPool<NeuralConnection>>& p_connectionPool)
        :
            Neuron(p_outputFilterTaps, p_evolveFilterTaps),
            m_connectionPool(p_connectionPool),
            m_inConnectionList(std::list<std::weak_ptr<NeuralConnection>>()),
            m_degrFactor(p_degrFactor)
    {}

    neuralVal_t ConnectableNeuron::calcOutput(){
        neuralVal_t sum = 0;

        for(std::list<std::weak_ptr<NeuralConnection>>::iterator connectIter
            = m_inConnectionList.begin(),
            connectIter != m_inConnectionList.end(), ++connectIter)
        {
            sum += (*connectIter)->ConnectedNeuronPtr->GetOutput()
                * (*connectIter)->Weight;
        }

        return sigmoid(sum);
    }

    Neuron::~Neuron()
    {
        for(std::list<std::weak_ptr<NeuralConnection>>::iterator connectIter
            = m_inConnectionList.begin(),
            connectIter != m_inConnectionList.end(), ++connectIter)
        {
            m_connectionPool.Release(*connectIter);
        }
    }

    void InputNeuron::postBackProbe()
    { 
        for(std::list<std::weak_ptr<NeuralConnection>>::iterator connectIter
            = m_inConnectionList.begin(),
            connectIter != m_inConnectionList.end(), ++connectIter)
        {
            (*connectIter)->ConnectedNeuronPtr->BackProbe();
        }
         
    }
    void InputNeuron::postForwardProbe(){}

    void InputNeuron::evolveSelf(neuralVal_t p_amount){
        std::list<std::weak_ptr<NeuralConnection>>::iterator connectIter
            = m_inConnectionList.begin();

        while(connectIter != m_inConnectionList.end(){
            neuralVal_t contribution 
                = (*connectIter)->ConnectedNeuronPtr->GetContribution();
            neuralVal_T amountToChange 
                = contribution * p_amount - m_degrFactor;
            neuralVal_T finalAlpha = (*connectIter)->Alpha + p_amount; 
            if(finalAlpha <= 0){
                std::weak_ptr toRemove = (*connectIter);
                connectIter = m_inConnectionList.erase(connectIter);
                m_connectionPool.Release(toRemove);
            }
            else{
                (*connectIter)->Alpha = finalAlpha;
                ++connectIter;
            }
        }
        
    }

    void InputNeuron::postPurgeConnections(
        std::list<std::weak_ptr<Neuron>>& p_toPurge){}

    void InputNeuron::postReset()
    {
        m_nextInput = 0;   
    }

    neuralVal_t ConnectableNeuron::sigmoid(neuralVal_t p_a){
        return p_a / (p_a + abs(p_a));
    }
}
