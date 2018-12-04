#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"
#include "Resetable.hxx"

#include <vector>
#include <memory>
#include <list>
#include <algorithm>

#include <boost/circular_buffer.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace LibGoodBoy
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    Neuron::Neuron( const std::vector<neuralVal_t>& p_outputFilterTaps,
                    const std::vector<neuralVal_t>& p_evolveFilterTaps)
        :
            Resetable(),
            m_checkedOutputFlag(false),
            m_forwardProbedFlag(false),
            m_backwardProbedFlag(false),
            m_evolveFlag(false),
            m_contributionFlag(false),

            m_lastContribution(0),

            m_outputConnectionsList(),

            m_outputPreFilterBuffer(p_outputFilterTaps.size()),
            m_outputPostFilterBuffer(p_evolveFilterTaps.size()),


            m_outputFilterTaps(p_outputFilterTaps),
            m_evolveFilterTaps(p_evolveFilterTaps),

            m_id(boost::uuids::random_generator()())
    {
    }

    Neuron::~Neuron()
    {
    }

    //Output_______________________________________________________
    neuralVal_t Neuron::GetOutput()
    {
        if(!m_checkedOutputFlag)
        {
            m_checkedOutputFlag = true;
            neuralVal_t pre_filt_output = calcOutput();
            m_outputPreFilterBuffer.push_back(pre_filt_output);

            neuralVal_t post_filter = tapsCircBuffInner(
                    m_outputFilterTaps,
                    m_outputPreFilterBuffer);

            m_outputPostFilterBuffer.push_back(post_filter);
        }

        return m_outputPostFilterBuffer[0];
    }

    void Neuron::ResetOutputFlag()
    {
        m_checkedOutputFlag = false;
    }

    //Connection Management________________________________________
    void Neuron::PurgeConnections(
            const std::list<std::weak_ptr<Neuron>>& p_toPurge)
    {
        for(std::list<std::weak_ptr<Neuron>>::const_iterator iter =
                p_toPurge.begin();
            iter != p_toPurge.end(); ++iter)
        {
            std::list<std::weak_ptr<Neuron>>::const_iterator eraseIter
                = m_outputConnectionsList.begin();
            do{
                if((*eraseIter).lock()==(*iter).lock()){
                    eraseIter = m_outputConnectionsList.erase(eraseIter);
                }
                else{
                    ++eraseIter;
                }
            }while(eraseIter != m_outputConnectionsList.end());
        }

        postPurgeConnections(p_toPurge);
    }

    void Neuron::OnConnectedToOutput(const std::weak_ptr<Neuron> connected)
    {
        m_outputConnectionsList.push_back(connected);
    }

    void Neuron::OnRemovedFromOutput(const std::weak_ptr<Neuron> p_removed)
    {

        std::list<std::weak_ptr<Neuron>>::const_iterator iter
            = m_outputConnectionsList.begin();
        bool found = false;
        while(!found && iter != m_outputConnectionsList.end()){
            if((*iter).lock() == p_removed.lock()){
                found = true;
                m_outputConnectionsList.remove(p_removed);
            }
        }
    }

    //Evolving____________________________________________________
    void Neuron::Evolve(neuralVal_t p_amount){
        if(!m_evolveFlag){
            m_evolveFlag = true;
            evolveSelf(p_amount);
        }
    }

    void Neuron::ResetEvolveFlag(){
        m_evolveFlag = false;
    }

    neuralVal_t Neuron::GetContribution(){
        if(!m_contributionFlag){
            m_lastContribution =  tapsCircBuffInner(
                                    m_evolveFilterTaps,
                                    m_outputPostFilterBuffer);
        }

        return m_lastContribution;
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
            for(auto iter =
                    m_outputConnectionsList.begin();
                iter != m_outputConnectionsList.end(); ++iter)
            {
                (*iter).lock()->ForwardProbe();
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

        m_outputPreFilterBuffer.clear();
        m_outputPostFilterBuffer.clear();

        m_id = boost::uuids::random_generator()();
        m_outputConnectionsList.clear();

        postReset();
    }

    //Protected__________________________________________________________
    neuralVal_t Neuron::tapsCircBuffInner(
            const std::vector<neuralVal_t>& p_taps,
            const boost::circular_buffer<neuralVal_t>& p_samps) const
    {
        neuralVal_t retVal = 0;
        std::vector<neuralVal_t>::const_iterator tapIter;
        boost::circular_buffer<neuralVal_t>::const_iterator sampIter;
        for(tapIter = p_taps.begin(), sampIter = p_samps.begin();
            tapIter != p_taps.end() && sampIter != p_samps.end();
            ++tapIter, ++sampIter)
        {
            retVal += *tapIter * *sampIter;
        }

        return retVal;
    }

    //Private____________________________________________________________
}
