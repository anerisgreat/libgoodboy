#include "Neuron.hxx"

#include <algorithm>

#include <boost/circular_buffer.hpp>
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

            m_outputConnectionsList(),

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
        }

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

    //Contribution________________________________________________
    neuralVal_t Neuron::GetContribution(){

        return m_lastContribution;
    }

    void Neuron::ResetContributionFlag(){
        m_contributionFlag = false;
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

        m_uid = boost::uuids::random_generator()();
        m_outputConnectionsList.clear();
    }

    //Properties___________________________________________________
    uuid_t Neuron::GetUID(){
        return m_uid;
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

        return retJSON;
    }

    std::string Neuron::jsonString(){
        return GetJSON().dump();
    }

    //Private____________________________________________________________
}
