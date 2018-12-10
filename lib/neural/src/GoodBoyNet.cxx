#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "ConnectableNeuron.hxx"
#include "Neuron.hxx"
#include "NeuralConnection.hxx"
#include "NeuralUtils.hxx"
#include "ObjectPool.hxx"

namespace LibGoodBoy
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    GoodBoyNet::GoodBoynet(
            std::vector<neuralVal_t>& p_outputFilterTaps,
            std::vector<neuralVal_t>& p_evolveFilterTaps,

            neuralSize_t p_nInputs,
            neuralSize_t p_nOutputs,

            neuralVal_t p_degrFactor,
            neuralVal_t p_maxStartWeight,
            neuralVal_t p_defaultAlpha,
        :
            m_inputs(std::vector<std::shared_ptr<InputNeuron>>()),
            m_outputs(std::vector<std::shared_ptr<ConnectableNeuron>>()),

            m_lastOutputs(std::vector<neuralVal_t>(),

            m_midNeurons(std::vector<std::shared_ptr<ConnectableNeurons>>()),

            m_midNeuronPool(
                ObjectPool<ConnectableNeuron>(makeNewConnectableNeuronPtr)),
            m_connectionPool(
                ObjectPool<NeuralConnection>(makeNewNeuralConnectionPtr)),

            m_outputFilterTaps(p_outputFilterTaps),
            m_evolveFilterTaps(p_evolveFilterTaps),

            m_degrFactor(p_degrFactor),
            m_maxStartWeight(p_maxStartWeight),
            m_defaultAlpha(p_defaultAlpha),
            m_evolvingEnabled(p_evolvingEnabled)
            m_lastOutputs
    {
        CreateInputs(neuralSize_t p_nInputs);
        CreateOutputs(neuralSize_t p_nOutputs);
    }

    GoodBoyNet::~GoodBoyNet(){}

    void GoodBoyNet::Iter(){
        calcOutputs();
        if(m_evolvingEnabled){
            evolve();
        }
    }

    void GoodBoyNet::SetInputs(std::vector<neuralVal_t>& p_inVec){
        for(auto inputIter = p_inVec.begin(), auto neurIter = m_inputs.begin();
                inputIter != p_inVec.end() && neurIter != m_inputs.end(),
                ++inputIter, ++neurIter)
        {
            (*neurIter)->FeedInput(*inputIter);
        }

    }

    void GoodBoyNet::SetInput(neuralSize_t p_nInput, neuralVal_t p_inputVal){
        m_inputs[neuralSize_t]->FeedInput(p_inputVal);
    }

    void GoodBoyNet::CreateInputs(neuralSize_t p_nInputs){
        for(neuralSize_t i = 0; i < p_nInputs; ++i){
            m_inputs.emplace_back(m_outputFilterTaps, m_evolveFilterTaps);
        }
    }

    void GoodBoyNet::CreateOutputs(neuralSize_t p_nOutputs){
        for(neuralSize_t i = 0; i < p_nOutputs; ++i){
            m_outputs.push_back(makeNewNeuralConnectionPtr());
        }
    }

    void GoodBoyNet::GetOutputs(std::vector<neuralVal_T>& p_outBuff) const{
        for(auto oIter = p_outBuff.begin(), auto vIter = m_lastOutputs.begin();
                oIter!= p_outBuff.end() && vIter != m_lastOutputs.end();
                ++oIter, ++vIter)
        {
            *oIter = *vIter;
        }
    }

    neuralVal_t GetOutput(neuralVal_t p_nOutput) const{
        return m_lastOutputs[p_nOutput];
    }

    void GoodBoyNet::evolve(){
        adjustWeights();

        if(shouldCleanup()){
            cleanup();
        }

        makeNewNeurons();
    }

    std::shared_ptr<ConnectableNeuron> 
    GoodBoyNet::makeNewConnectableNeuronPtr(){
        //We use the default constructor as opposed to make_shared because
            //of the use of weak pointers. Using the constructor as is
            //seperates the intance to poinpt towards from the control block,
            //allows the control block and the instance to be deallocated
            //seperately. Once all the weak pointers are gone, so will be these
            //members.
        return std::shared_ptr<ConnectableNeuron>(new ConnectableNeuron(
                    m_outputFilterTaps,
                    m_evolveFilterTaps,
                    m_connectionPool,
                    m_maxDegrFactor,
                    m_maxStartWeight,
                    m_defaultAlpha
                    ));
    }

    std::shared_ptr<ConnectableNeuron> GoodBoyNet::makeNewNeuralConnectionPtr(){
        //See notes for GoodBoyNet::makeNewConnectableNeuron.
        return std::shared_ptr<NeuralConnection>(new NeuralConnection());
    }

    void GoodBoyNet::calcOutputs(){
        forzz
    }

}
