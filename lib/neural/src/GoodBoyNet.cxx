#include "NeuralUtils.hxx"
#include "GoodBoyNet.hxx"

namespace LibGoodBoy
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    GoodBoyNet::GoodBoyNet(
            std::vector<neuralVal_t>& p_outputFilterTaps,
            std::vector<neuralVal_t>& p_evolveFilterTaps,

            neuralSize_t p_nInputs,
            neuralSize_t p_nOutputs,

            neuralVal_t p_degrFactor,
            neuralVal_t p_maxStartWeight,
            neuralVal_t p_defaultAlpha,
            neuralVal_t p_defaultGenerationFactor,

            bool p_evolvingEnabled = true)
        :
            m_inputs(std::vector<std::shared_ptr<InputNeuron>>()),
            m_outputs(std::vector<std::shared_ptr<ConnectableNeuron>>()),

            m_lastOutputs(std::vector<neuralVal_t>()),

            m_midNeurons(std::list<std::shared_ptr<ConnectableNeuron>>()),

            m_midNeuronPool(
                ObjectPool<ConnectableNeuron>(makeNewConnectableNeuronPtr)),
            m_connectionPool(
                ObjectPool<NeuralConnection>(makeNewNeuralConnectionPtr)),

            m_outputFilterTaps(p_outputFilterTaps),
            m_evolveFilterTaps(p_evolveFilterTaps),

            m_degrFactor(p_degrFactor),
            m_maxStartWeight(p_maxStartWeight),
            m_defaultAlpha(p_defaultAlpha),
            m_generationFactor(p_defaultGenerationFactor),
            m_evolvingEnabled(p_evolvingEnabled)

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

    neuralVal_t GoodBoyNet::GetOutput(neuralVal_t p_nOutput) const{
        return m_lastOutputs[p_nOutput];
    }

    json_t GoodBoyNet::GetJSON() const{
        json_t retJson;

        retJson[NET_INP_KEY] = json_t::array();
        for(auto iter = m_inputs.begin(); iter != m_inputs.end(); ++iter){
            retJson[NET_INP_KEY].push_back((*iter)->GetJSON());
        }

        retJson[NET_OUT_KEY] = json_t::array();
        for(auto iter = m_outputs.begin(); iter != m_outputs.end(); ++iter){
            retJson[NET_OUT_KEY].push_back((*iter)->GetJSON());
        }

        retJson[NET_MID_KEY] = json_t::array();
        for(auto iter = m_midNeurons.begin(); 
                iter != m_midNeurons.end(); 
                ++iter)
        {
            retJson[NET_MID_KEY].push_back((*iter)->GetJSON());
        }

        retJson[NET_DEGR_FACTOR_KEY] = m_degrFactor;
        retJson[NET_MAX_START_WEIGHT_KEY] = m_maxStartWeight;
        retJson[NET_DEFAULT_ALPHA] = m_defaultAlpha;
        retJson[NET_GEN_FACTOR] = m_generationFactor;
        retJson[NET_EVOLVE_ENABLED] = m_evolvingEnabled;

        retJson[NET_OUT_FILT_TAPS_KEY] = json_t::array();
        for(auto iter = m_outputFilterTaps.begin(); 
                iter != m_outputFilterTaps.end(); 
                ++iter)
        {
            retJson[NET_OUT_FILT_TAPS_KEY].push_back(*iter);
        }

        retJson[NET_EVOLVE_FILT_TAPS_KEY] = json_t::array();
        for(auto iter = m_evolveFilterTaps.begin(); 
                iter != m_evolveFilterTapsend(); 
                ++iter)
        {
            retJson[NET_OUT_FILT_TAPS_KEY].push_back(*iter);
        }

        return retJson;
    }

    void GoodBoyNet::evolve(){
        adjustWeights();

        neuralSize_t numNewNeurons = numNeuronsToMake();
        if(numNewNeurons > 0){
            cleanup();
            makeNewNeurons(numNewNeurons);
        }

    }

    void GoodBoyNet::adjustWeights(){
        for(auto neurIter = m_midNeurons.begin();
                neurIter != m_midNeurons.end();
                ++neurIter)
        {
            (*neurIter)->Evolve();
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->Evolve();
        }
    }

    void GoodBoynet::cleanup(){
        for(auto neurIter = m_inputs.begin();
                neurIter != m_inputs.end();
                ++neurIter)
        {
            (*neurIter)->ForwardProbe();
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->BackProbe();
        }

        std::list<std::shared_ptr<Neuron>> toPurge();
        for(auto neurIter = m_midNeurons.begin();
                neurIter != m_midNeurons.end();
                ++neurIter)
        {
            if(!(*neurIter)->GetWasFullyProbed()){
                toPurge.push_back(*neurIter);
            }
            (*neurIter)->ResetProbeFlag();
        }

        for(auto neurIter = m_midNeurons.begin();
                neurIter != m_midNeurons.end();
                ++neurIter)
        {
            (*neurIter)->PurgeConnections(toPurge);
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->PurgeConnections(toPurge);
        }

        for(auto neurIter = toPurge.begin();
                neurIter != toPurge.end();
                ++neurIter)
        {
            m_midNeurons.remove(*neurIter);
            m_midNeuronPool.Release(*neurIter);
        }
    }

    neuralSize_t numNeuronsToMake(){
        neuralVal_t val = RandInRange<neuralVal_t>(0,1);
        neuralSize_t ret = 0;
        while(val < m_generationFactor){
            ++ret;
            val /= m_generationFactor;
        }

        return ret;
    }

    void GoodBoyNet::makeNewNeurons(neuralSize_t p_numNewNeurons){
        //We are going to do "weightless" adding. Maybe in the future, we
            //will do weighted addition.

        neuralSize_t numOfRecv = m_midNeurons.size() + m_outputs.size();
        neuralSize_t numOfOut = m_midNeurons.size() + m_inputs.size();

        for(neuralSize_t i = 0; i < p_numNewNeurons; ++i){
            std::shared_ptr<Neuron> recvNeuronPtr;
            std::shared_ptr<Neuron> outNeuronPtr;

            neuralSize_t recIndex = RandInRange<neuralSize_t>(0, numOfRecv);
            neuralSize_t outIndex = RandInRange<neuralSize_t>(0, numOfOut);

            if(recIndex < m_midNeurons.size()){
                recvNeuronPtr = m_midNeurons[recIndex];
            }
            else{
                recvNeuronPtr = m_outputs[recIndex - m_midNeurons.size()];
            }

            if(outIndex < m_midNeurons.size()){
                outNeuronPtr = m_midNeurons[outIndex];
            }
            else{
                outNeuronPtr = m_inputs[outIndex - m_midNeurons.size()];
            }

            std::shared_ptr<ConnectableNeuron> newNeuron 
                = m_midNeuronPool.AllocElement();

            newNeuron->Connect(outNeuronPtr);
            recvNeuronPtr->Connect(newNeuron);
            m_midNeurons.push_back(newNeuron);
        }
    }

    void GoodBoyNet::calcOutputs(){
        for(auto oIter = m_outputs.begin(), auto lIter = m_lastOutputs.begin();
                oIter != m_outputs.end() && lIter != m_lastOutputs.end();
                ++oIter, ++lIter)
        {
            *lIter = (*oIter)->GetOutput();
        }
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
}
