#include "ConnectableNeuron.hxx"
#include "NeuralUtils.hxx"

namespace LibGoodBoy{

//Public_____________________________________________________________
//Constructor & Destructor_____________________________________
ConnectableNeuron::ConnectableNeuron(
        neuralConnectionPool_t& p_connectionPool,
        neuralVal_t p_degrFactor,
        neuralVal_t p_maxStartWeight,
        neuralVal_t p_defaultAlpha)
    :
        Neuron(),
        m_connectionPool(p_connectionPool),
        m_inConnectionList(std::list<NeuralConnection*>()),
        m_degrFactor(p_degrFactor),
        m_maxStartWeight(p_maxStartWeight),
        m_defaultAlpha(p_defaultAlpha)
{}


ConnectableNeuron::~ConnectableNeuron(){
    releaseAllInputs();
}

void ConnectableNeuron::Evolve(neuralVal_t p_amount){
    auto connectIter = m_inConnectionList.begin();

    while(connectIter != m_inConnectionList.end()){
        auto connectPtr = *connectIter;
        auto neuronPtr = connectPtr->ConnectedNeuronPtr;
        neuralVal_t contribution = neuronPtr->GetContribution();
        neuralVal_t amountToChange = contribution * p_amount - m_degrFactor;
        neuralVal_t finalAlpha = connectPtr->Alpha + amountToChange; 
        if(finalAlpha <= 0){
            connectIter = m_inConnectionList.erase(connectIter);
            connectPtr->ConnectedNeuronPtr->OnRemovedFromOutput(this);
            m_connectionPool.Release(connectPtr);
        }
        else{
            connectPtr->Alpha = finalAlpha;
            ++connectIter;
        }
    }
}

void ConnectableNeuron::PurgeConnections(
        const std::list<Neuron*>& p_toPurge)
{
    Neuron::PurgeConnections(p_toPurge);
    auto iter = m_inConnectionList.begin();
    bool continueFlag;
    bool purgeSelf = false;
    while(!purgeSelf && iter != m_inConnectionList.end()){
        bool found = false;
        auto connectPtr = (*iter);
        auto neuronPtr = (*iter)->ConnectedNeuronPtr;
        auto purgeIter = p_toPurge.begin();

        while(!found && !purgeSelf && purgeIter != p_toPurge.end()){
            if(neuronPtr==(*purgeIter)){
                found = true;
            }
            else if((*purgeIter) == this){
                auto delIter = m_inConnectionList.begin();
                while(delIter != m_inConnectionList.end()){
                    NeuralConnection* neuralConn = *delIter;
                    Neuron* neuronPtr = neuralConn->ConnectedNeuronPtr;
                    delIter = m_inConnectionList.erase(delIter);
                    neuronPtr->OnRemovedFromOutput(this);
                    m_connectionPool.Release(neuralConn);
                }
                purgeSelf = true;
            }
            else{
                ++purgeIter;
            }
        }

        if(found){
            iter = m_inConnectionList.erase(iter);
            neuronPtr->OnRemovedFromOutput(this);
            m_connectionPool.Release(connectPtr);
        }
        else{
            ++iter;
        }
    }
}

void ConnectableNeuron::Reset(){
    Neuron::Reset();
    releaseAllInputs();
}

json_t ConnectableNeuron::GetJSON() const{
    json_t retJSON = Neuron::GetJSON();
    retJSON[JSON_NEURON_TYPE_KEY] = JSON_NEURON_TYPE_CONNECTABLE_VAL; 
    retJSON[JSON_INP_CONN_KEY] = json_t::array(); 
    for(auto iter = m_inConnectionList.begin(); 
            iter != m_inConnectionList.end(); ++iter)
    {
        retJSON[JSON_INP_CONN_KEY ].push_back((*iter)->GetJSON());
    }
    return retJSON;
}

void ConnectableNeuron::Connect(Neuron* p_toConnect){
    neuralVal_t weight = RandInRange<neuralVal_t>(
            -m_maxStartWeight, m_maxStartWeight);
    Connect(p_toConnect, weight, m_defaultAlpha);
}

void ConnectableNeuron::Connect(Neuron* p_toConnect,
        neuralVal_t p_weight,
        neuralVal_t p_alpha)
{
    NeuralConnection* connection = m_connectionPool.AllocElement();
    connection->ConnectedNeuronPtr = p_toConnect;
    connection->Weight = p_weight;
    connection->Alpha = p_alpha;

    m_inConnectionList.emplace_back(connection);

    connection->ConnectedNeuronPtr->OnConnectedToOutput(this);
}

neuralSize_t ConnectableNeuron::GetNumInputs() const{
    return m_inConnectionList.size();
}

neuralVal_t ConnectableNeuron::calcOutput(){
    neuralVal_t sum = 0;

    for(auto connectIter = m_inConnectionList.begin();
        connectIter != m_inConnectionList.end(); ++connectIter)
    {
        auto connectPtr = *connectIter;
        sum += connectPtr->ConnectedNeuronPtr->GetOutput()
            * connectPtr->Weight;
    }

    return ApproxSigmoid<neuralVal_t>(sum);
}

void ConnectableNeuron::postBackProbe(){ 
    for(auto connectIter = m_inConnectionList.begin();
        connectIter != m_inConnectionList.end(); ++connectIter)
    {
        (*connectIter)->ConnectedNeuronPtr->BackProbe();
    }
}

void ConnectableNeuron::postForwardProbe(){}

void ConnectableNeuron::releaseAllInputs(){
    for(auto connectIter = m_inConnectionList.begin();
        connectIter != m_inConnectionList.end(); ++connectIter)
    {
        m_connectionPool.Release(*connectIter);
    }
}

}//End namespace
