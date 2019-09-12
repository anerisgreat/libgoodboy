#include "Neuron.hxx"

#include <algorithm>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
namespace LibGoodBoy{

//Public_____________________________________________________________
//Constructor & Destructor_____________________________________
Neuron::Neuron()
    :
        Resetable(),

        m_checkedOutputFlag(false),
        m_forwardProbedFlag(false),
        m_backwardProbedFlag(false),
        m_evolveFlag(false),
        m_contributionFlag(false),

        m_lastOutput(0),
        m_lastContribution(0),
        m_outputSum(0),

        m_outputConnectionsList(),

        m_pos({}),

        m_uid(boost::uuids::random_generator()())
{}

Neuron::~Neuron(){}

//Output_______________________________________________________
neuralVal_t Neuron::GetOutput(){
    if(!m_checkedOutputFlag){
        m_checkedOutputFlag = true;
        m_lastOutput = calcOutput();
        //Calculate contribution with every step.
        m_lastContribution = 
            m_lastContribution/2 + abs(m_lastOutput)/2;
        m_outputSum += m_lastOutput;
    }

    return m_lastOutput;
}

neuralVal_t Neuron::GetLastOutput() const{
    return m_lastOutput;
}

void Neuron::ResetOutputFlag(){
    m_checkedOutputFlag = false;
}

//Connection Management________________________________________
void Neuron::PurgeConnections(const std::list<Neuron*>& p_toPurge){
    for(auto iter = p_toPurge.begin(); iter != p_toPurge.end(); ++iter){
        auto eraseIter = m_outputConnectionsList.begin();
        do{
            if((*eraseIter)==(*iter)){
                eraseIter = m_outputConnectionsList.erase(eraseIter);
            }
            else{
                ++eraseIter;
            }
        }while(eraseIter != m_outputConnectionsList.end());
    }
}

void Neuron::OnConnectedToOutput(Neuron* connected){
    m_outputConnectionsList.emplace_back(connected);
}

void Neuron::OnRemovedFromOutput(Neuron* p_removed){
    auto iter = m_outputConnectionsList.begin();
    bool found = false;
    while(!found && iter != m_outputConnectionsList.end()){
        if((*iter) == p_removed){
            found = true;
            m_outputConnectionsList.erase(iter);
        }

        ++iter;
    }
}

neuralSize_t Neuron::GetNumOutputs() const{
    return m_outputConnectionsList.size();
}

//Contribution________________________________________________
neuralVal_t Neuron::GetContribution(){
    return m_lastContribution;
}

void Neuron::ResetContributionFlag(){
    m_contributionFlag = false;
}

//OutputSum___________________________________________________
neuralVal_t Neuron::GetOutputSum(){
    return m_outputSum;
}

void Neuron::ResetOutputSum(){
    m_outputSum = 0;
}

//Probe________________________________________________________
void Neuron::BackProbe(){
    if(!m_backwardProbedFlag){
        m_backwardProbedFlag = true;
        postBackProbe();
    }
}

void Neuron::ForwardProbe(){
    if(!m_forwardProbedFlag){
        m_forwardProbedFlag = true;
        for(auto iter = m_outputConnectionsList.begin();
            iter != m_outputConnectionsList.end(); ++iter)
        {
            (*iter)->ForwardProbe();
        }

        postForwardProbe();
    }
}

bool Neuron::GetWasFullyProbed() const{
    return m_forwardProbedFlag && m_backwardProbedFlag;
}

void Neuron::ResetProbeFlag(){
    m_forwardProbedFlag = false;
    m_backwardProbedFlag = false;
}


//Reseting_____________________________________________________

void Neuron::Reset(){
    m_checkedOutputFlag = false;
    m_forwardProbedFlag = false;
    m_backwardProbedFlag = false;
    m_evolveFlag = false;

    m_lastOutput = 0;
    m_lastContribution = 0;
    m_outputSum = 0;

    m_uid = boost::uuids::random_generator()();
    m_outputConnectionsList.clear();
}

//Properties___________________________________________________
uuid_t Neuron::GetUID(){
    return m_uid;
}

void Neuron::SetUID(uuid_t p_uid){
    m_uid = p_uid;
}

json_t Neuron::GetJSON() const{
    json_t retJSON;
    retJSON[JSON_UID_KEY] = boost::uuids::to_string(m_uid);
    retJSON[JSON_OUTP_CONN_KEY] = json_t::array();
    for(auto iter = m_outputConnectionsList.begin(); 
            iter != m_outputConnectionsList.end(); ++iter)
    {
        retJSON[JSON_OUTP_CONN_KEY].push_back(
                boost::uuids::to_string((*iter)->GetUID()));
    }

    retJSON[JSON_POS_KEY] = json_t::array();
    for(posscalar_t pos : m_pos){
        retJSON[JSON_POS_KEY].push_back(pos);
    }

    return retJSON;
}

void Neuron::LoadFromJSON(json_t p_json){
    //Does not load connections, that is up to the network
    m_uid = boost::uuids::string_generator()(
            p_json[JSON_UID_KEY].get<std::string>());
    m_pos = std::valarray<posscalar_t>(p_json[JSON_POS_KEY].size());
    for(neuralSize_t i = 0; i != p_json[JSON_POS_KEY].size(); ++i){
        m_pos[i] = p_json[JSON_POS_KEY][i];
    }
}

std::string Neuron::jsonString(){
    return GetJSON().dump();
}

pos_t Neuron::GetPosition() const{
    return m_pos;
}

void Neuron::SetPosition(pos_t p_pos){
    m_pos = p_pos;
}

posscalar_t Neuron::GetNeuronDistance(const Neuron& p_a, const Neuron& p_b){
    return GetDistance(p_a.GetPosition(), p_b.GetPosition());
}

pos_t Neuron::GetNeuronAveragePosition(const Neuron& p_a, 
        const Neuron& p_b)
{
    return GetAveragePosition(p_a.GetPosition(), p_b.GetPosition());
}

}//End namespace

