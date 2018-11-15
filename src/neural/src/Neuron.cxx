#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"
#include "neural/Neuron.h"

namespace PROJECT_NAMESPACE
{
    Neuron::Neuron()
    {
        m_checkedOutputFlag = false;
        m_endorphinizationFlag = false;
    }

    Neuron::~Neuron()
    {

    }

    t_neural_val Neuron::GetOutput()
    {
        if(!m_checkedOutputFlag)
        {
            m_checkedOutputFlag = true;
            m_lastOutput = calcOutput();
        }
        //NEED TO FILTER HERE
        return m_lastOutput;
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
}
