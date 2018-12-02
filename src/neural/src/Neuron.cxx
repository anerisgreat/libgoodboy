#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"
#include "neural/Neuron.h"

#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/circular_buffer.hpp>

namespace PROJECT_NAMESPACE
{
    //Public:
    //Constructor & Destructor__________________________________________________
    Neuron( std::vector<t_neural_val>* output_filter_taps_ptr
            std::vector<t_neural_val>* endorphinization_filter_taps_ptr)
        :
            m_checkedOutputFlag(false),
            m_forwardProbedFlag(false),
            m_backwardProbedFlag(false),
            m_endorphinizationFlag(false),

            m_output_pre_filter_buffer(filters_len),
            m_output_post_filter_buffer(filters_len),

            m_output_filter_taps_ptr(output_filter_taps_ptr),
            m_endorphinization_filter_taps_ptr(
                    endorphinization_filter_taps_ptr),

            m_output_connections_vec(),

            m_id(boost::uuids:random_generator()())
    {
    }

    Neuron::~Neuron()
    {
    }

    //Output____________________________________________________________________
    t_neural_val Neuron::GetOutput()
    {
        if(!m_checkedOutputFlag)
        {
            m_checkedOutputFlag = true;
            t_neural_val pre_filt_output = calcOutput();
            m_output_pre_filter_buffer.push(pre_filt_output);

            t_neural_val post_filter = taps_circ_buff_inner(
                    m_output_filter_taps_ptr,
                    &m_output_pre_filter_buffer);

            m_output_post_filter_buffer.push(post_filter);
        }

        return m_output_post_filter_buffer[0];
    }

    void Neuron::ResetOutputFlag()
    {
        m_checkedOutputFlag = false;
    }

    //Connection Management_____________________________________________________
    void Neuron::RemoveAllConnections()
    {
        //TODO:Implement
    }

    void Neuron::OnConnectedToOutput()
    {
        //TODO:Implement
    }

    void Neuron::OnRemovedFromOutput()
    {
        //TODO:Implement
    }

    //Endorphinization__________________________________________________________
    void Neuron::Endorphinize(t_neural_val amount)
    {
        if(!m_endorphinizationFlag)
        {
            m_endorphinizationFlag = true;
            endorphinizeSelf(amount);
        }
    }

    void Neuron::ResetEndorphinizationFlag()
    {
        m_endorphinizationFlag = false;
    }

    t_neural_val Neuron::CalcContribution()
    {
        return taps_circ_buff_inner(
                m_endorphinization_filter_taps_ptr,
                &m_output_post_filter_buffer);
    }

    //Probe_____________________________________________________________________
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
            for(std::vector<Neuron*>::iterator iter =
                    m_output_connections_vec.begin();
                iter != m_output_connections_vec.end(); ++iter)
            {
                *iter->ForwardProbe();
            }
        }
    }

    void Neuron::GetWasFullyProbed()
    {
        return m_forwardProbedFlag && m_backwardProbedFlag;
    }

    void Neuron::ResetProbeFlag()
    {
        m_forwardProbedFlag = false;
        m_backwardProbedFlag = false;
    }


    //Flushing__________________________________________________________________

    void Neuron::Flush()
    {
        m_checkedOutputFlag = false;
        m_forwardProbedFlag = false;
        m_backwardProbedFlag = false;
        m_endorphinizationFlag = false;

        m_output_pre_filter_buffer.erase();
        m_output_post_filter_buffer.erase();

        m_id = boost::uuids:random_generator()();
        m_output_connections_vec.clear();

        postFlush();
    }

    //Protected:
    t_neural_val taps_circ_buff_inner(
            std::vector<t_neural_val>* taps,
            boost:circular_buffer<t_neural_val>* samps)
    {
        t_neural_val ret_val = 0;
        std::vector<t_neural_val>::iterator tap_iter;
        std::circular_buffer<t_neural_val>::iterator samp_iter;
        for(tap_iter = taps->begin(), samp_iter = samps->begin();
            tap_iter != taps->end() && samp_iter != samps->end();
            ++tap_iter, ++samp_iter)
        {
            ret_val += *tap_iter * *samp_iter;
        }

        return ret_val;
    }

    //Private:

}
