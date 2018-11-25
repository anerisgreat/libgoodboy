#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"
#include "neural/Neuron.h"

namespace PROJECT_NAMESPACE
{
    Neuron(t_neural_val* output_filter_taps
            t_neural_val* endorphinization_filter_taps,
            unsigned int filters_len)
        :
            m_checkedOutputFlag(false),
            m_forwardProbedFlag(false),
            m_backwardProbedFlag(false),
            m_endorphinizationFlag(false),

            m_output_pre_filter_buffer(filters_len),
            m_output_post_filter_buffer(filters_len),

            m_output_filter_len(filters_len),
            m_output_filter_taps(outp_filt_taps),
            m_endorphinization_filter_taps(endorphinization_filter_taps),

            m_output_connections_vec(),

            m_id(boost::uuids:random_generator()())
    {
    }

    Neuron::~Neuron()
    {
    }

    t_neural_val Neuron::GetOutput()
    {
        if(!m_checkedOutputFlag)
        {
            m_checkedOutputFlag = true;
            t_neural_val pre_filt_output = calcOutput();
            m_output_pre_filter_buffer.push(pre_filt_output)
            //TODO: Implement INNER
            t_neural_val post_filt = inner(
                    m_output_filter_taps,
                    //TODO: head?!
                    m_output_pre_filter_buffer.head(),
                    m_output_filter_len);
            m_output_post_filter_buffer.push(post_filt);
        }

        return m_output_post_filter_buffer[0];
    }

    void Neuron::ResetOutputFlag()
    {
        m_checkedOutputFlag = false;
    }

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
        return inner(
                m_output_post_filter_buffer.head(),
                m_endorphinization_filter_taps,
                m_filters_len);
    }

    void Neuron::BackProbe()
    {
        if(!m_backwardProbedFlag)
        {
            m_backwardProbedFlag = true;
            postkkkkkkkkk
        }
    }

    void Neuron::reset()
    {
        m_checkedOutputFlag = false;
        m_endorphinizationFlag = false;

        //TODO: IMPLEMENT AND RENAME
    }
}
