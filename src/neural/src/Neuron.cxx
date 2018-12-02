#include "libGoodBoyConfig.hxx"
#include "neural/NeuralConfig.hxx"
#include "neural/Neuron.hxx"

#include <vector>
#include <memory>
#include <list>
#include <algorithm>

#include <boost/circular_buffer.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace lib_good_boy::neural
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    Neuron::Neuron( const std::vector<neuralVal_t>& outputFilterTaps,
                    const std::vector<neuralVal_t>& evolveFilterTaps)
        :
            m_checkedOutputFlag(false),
            m_forwardProbedFlag(false),
            m_backwardProbedFlag(false),
            m_evolveFlag(false),

            m_outputConnectionsList(),

            m_outputPreFilterBuffer(outputFilterTaps.size()),
            m_outputPostFilterBuffer(evolveFilterTaps.size()),

            m_outputFilterTaps(outputFilterTaps),
            m_evolveFilterTaps(evolveFilterTaps),

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
        const std::list<const std::shared_ptr<Neuron>>& toPurge)
    {
        for(std::list<const std::shared_ptr<Neuron>>::const_iterator iter =
                toPurge.begin();
            iter != toPurge.end(); ++iter)
        {
            std::list<std::shared_ptr<Neuron>>::const_iterator find_iter
                = find(m_outputConnectionsList.begin(),
                        m_outputConnectionsList.end(), *iter);
            if(find_iter != m_outputConnectionsList.end())
            {
                m_outputConnectionsList.erase(find_iter); 
            }
        }

        postPurgeConnections(toPurge);
    }

    void Neuron::OnConnectedToOutput(const std::shared_ptr<Neuron> connected)
    {
        m_outputConnectionsList.push_back(connected);
    }

    void Neuron::OnRemovedFromOutput(const std::shared_ptr<Neuron> removed)
    {
        //TODO:Implement
    }

    //Endorphinization____________________________________________
    void Neuron::Evolve(neuralVal_t endorph)
    {
        if(!m_evolveFlag)
        {
            m_evolveFlag = true;
            evolveSelf(endorph);
        }
    }

    void Neuron::ResetEvolveFlag()
    {
        m_evolveFlag = false;
    }

    neuralVal_t Neuron::GetContribution() const
    {
        return tapsCircBuffInner(
                m_evolveFilterTaps,
                m_outputPostFilterBuffer);
    }

    //Probe________________________________________________________
    void Neuron::BackProbe()
    {
        if(!m_backwardProbedFlag)
        {
            m_backwardProbedFlag = true;
            postBackProbe();
        }
    }

    void Neuron::ForwardProbe()
    {
        if(!m_forwardProbedFlag)
        {
            m_forwardProbedFlag = true;
            for(std::list<std::shared_ptr<Neuron>>::iterator iter =
                    m_outputConnectionsList.begin();
                iter != m_outputConnectionsList.end(); ++iter)
            {
                (*iter)->ForwardProbe();
            }

            postForwardProbe();
        }
    }

    bool Neuron::GetWasFullyProbed() const
    {
        return m_forwardProbedFlag && m_backwardProbedFlag;
    }

    void Neuron::ResetProbeFlag()
    {
        m_forwardProbedFlag = false;
        m_backwardProbedFlag = false;
    }


    //Flushing_____________________________________________________

    void Neuron::Flush()
    {
        m_checkedOutputFlag = false;
        m_forwardProbedFlag = false;
        m_backwardProbedFlag = false;
        m_evolveFlag = false;

        m_outputPreFilterBuffer.clear();
        m_outputPostFilterBuffer.clear();

        m_id = boost::uuids::random_generator()();
        m_outputConnectionsList.clear();

        postFlush();
    }

    //Protected__________________________________________________________
    neuralVal_t Neuron::tapsCircBuffInner(
            const std::vector<neuralVal_t>& taps,
            const boost::circular_buffer<neuralVal_t>& samps) const
    {
        neuralVal_t retVal = 0;
        std::vector<neuralVal_t>::const_iterator tapIter;
        boost::circular_buffer<neuralVal_t>::const_iterator sampIter;
        for(tapIter = taps.begin(), sampIter = samps.begin();
            tapIter != taps.end() && sampIter != samps.end();
            ++tapIter, ++sampIter)
        {
            retVal += *tapIter * *sampIter;
        }

        return retVal;
    }

    //Private____________________________________________________________
}
