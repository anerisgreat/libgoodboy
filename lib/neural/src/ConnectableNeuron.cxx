#include "libGoodBoyConfig.hxx"
#include "ConnectableNeuron.hxx"
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
            ObjectPool<NeuralConnection>& p_connectionPool,
            neuralVal_t p_degrFactor)
        :
            Neuron(p_outputFilterTaps, p_evolveFilterTaps),
            m_connectionPool(p_connectionPool),
            m_inConnectionList(std::list<std::weak_ptr<NeuralConnection>>()),
            m_degrFactor(p_degrFactor)
    {}


    ConnectableNeuron::~ConnectableNeuron(){
        for(auto connectIter = m_inConnectionList.begin();
            connectIter != m_inConnectionList.end(); ++connectIter)
        {
            m_connectionPool.Release((*connectIter).lock());
        }
    }

    void ConnectableNeuron::Connect(std::shared_ptr<Neuron>& p_toConnect){
        std::shared_ptr<NeuralConnection> connection = 
            m_connectionPool.AllocElement();
    }

    neuralVal_t ConnectableNeuron::calcOutput(){
        neuralVal_t sum = 0;

        for(auto connectIter = m_inConnectionList.begin();
            connectIter != m_inConnectionList.end(); ++connectIter)
        {
            auto connectPtr = (*connectIter).lock();
            sum += connectPtr->ConnectedNeuronPtr.lock()->GetOutput()
                * connectPtr->Weight;
        }

        return sigmoid(sum);
    }

    void ConnectableNeuron::postBackProbe(){ 
        for(auto connectIter = m_inConnectionList.begin();
            connectIter != m_inConnectionList.end(); ++connectIter)
        {
            (*connectIter).lock()->ConnectedNeuronPtr.lock()->BackProbe();
        }
         
    }

    void ConnectableNeuron::postForwardProbe(){}

    void ConnectableNeuron::evolveSelf(neuralVal_t p_amount){
        auto connectIter = m_inConnectionList.begin();

        while(connectIter != m_inConnectionList.end()){
            auto connectPtr = (*connectIter).lock();
            auto neuronPtr = connectPtr->ConnectedNeuronPtr.lock();
            neuralVal_t contribution = neuronPtr->GetContribution();
            neuralVal_t amountToChange = contribution * p_amount - m_degrFactor;
            neuralVal_t finalAlpha = connectPtr->Alpha + p_amount; 
            if(finalAlpha <= 0){
                connectIter = m_inConnectionList.erase(connectIter);
                m_connectionPool.Release(connectPtr);
            }
            else{
                connectPtr->Alpha = finalAlpha;
                ++connectIter;
            }
        }
    }

    void ConnectableNeuron::postPurgeConnections(
        const std::list<std::shared_ptr<Neuron>>& p_toPurge){}

    void ConnectableNeuron::postReset()
    {
    }

    neuralVal_t ConnectableNeuron::sigmoid(neuralVal_t p_a){
        return p_a / (p_a + abs(p_a));
    }
}
