#include "GoodBoyNet.hxx"
#include "NeuralUtils.hxx"
#include <memory>

namespace LibGoodBoy
{
    //Public_____________________________________________________________
    //Constructor & Destructor_____________________________________
    GoodBoyNet::GoodBoyNet(
            const std::vector<neuralVal_t>& p_outputFilterTaps,
            const std::vector<neuralVal_t>& p_evolveFilterTaps,

            neuralSize_t p_nInputs,
            neuralSize_t p_nOutputs,

            neuralVal_t p_degrFactor,
            neuralVal_t p_maxStartWeight,
            neuralVal_t p_defaultAlpha,
            neuralVal_t p_defaultGenerationFactor,

            bool p_evolvingEnabled)
        :
            m_inputs(std::vector<std::shared_ptr<InputNeuron>>()),
            m_outputs(std::vector<ConnectableNeuron*>()),

            m_lastOutputs(std::vector<neuralVal_t>()),

            m_midNeurons(std::list<ConnectableNeuron*>()),

            m_connectionPool(),
            m_midNeuronPool(
                        p_outputFilterTaps,
                        p_evolveFilterTaps,
                        m_connectionPool,
                        p_degrFactor,
                        p_maxStartWeight,
                        p_defaultAlpha),

            m_outputFilterTaps(p_outputFilterTaps),
            m_evolveFilterTaps(p_evolveFilterTaps),

            m_degrFactor(p_degrFactor),
            m_maxStartWeight(p_maxStartWeight),
            m_defaultAlpha(p_defaultAlpha),
            m_generationFactor(p_defaultGenerationFactor),
            m_evolvingEnabled(p_evolvingEnabled)
    {
        m_inputs.push_back(std::make_shared<InputNeuron>(
            m_outputFilterTaps, m_evolveFilterTaps));
        m_inputs[0]->FeedInput(1);
        CreateInputs(p_nInputs);
        CreateOutputs(p_nOutputs);
    }

    GoodBoyNet::~GoodBoyNet(){}

    void GoodBoyNet::Iter(){
        calcOutputs();
    }

    void GoodBoyNet::SetInputs(const std::vector<neuralVal_t>& p_inVec){
        auto inputIter = p_inVec.begin();
        auto neurIter = m_inputs.begin()++;
        for(;inputIter != p_inVec.end() && neurIter != m_inputs.end();
                ++inputIter, ++neurIter)
        {
            (*neurIter)->FeedInput(*inputIter);
        }
    }

    void GoodBoyNet::SetInput(neuralSize_t p_nInput, neuralVal_t p_inputVal){
        m_inputs[p_nInput + 1]->FeedInput(p_inputVal);
    }

    void GoodBoyNet::CreateInputs(neuralSize_t p_nInputs){
        for(neuralSize_t i = 0; i < p_nInputs; ++i){
            m_inputs.push_back(std::make_shared<InputNeuron>(
                        m_outputFilterTaps, m_evolveFilterTaps));
        }
    }

    void GoodBoyNet::CreateOutputs(neuralSize_t p_nOutputs){
        for(neuralSize_t i = 0; i < p_nOutputs; ++i){
            m_outputs.push_back(m_midNeuronPool.AllocElement());
            m_lastOutputs.push_back(0);
        }
    }

    void GoodBoyNet::GetOutputs(std::vector<neuralVal_t>& p_outBuff) const{
        auto oIter = p_outBuff.begin();
        auto vIter = m_lastOutputs.begin();

        while(oIter!= p_outBuff.end() && vIter != m_lastOutputs.end()){
            *oIter++ = *vIter++;
        }
    }

    neuralVal_t GoodBoyNet::GetOutput(neuralVal_t p_nOutput) const{
        return m_lastOutputs[p_nOutput];
    }

    neuralSize_t GoodBoyNet::GetMidSize() const{
        return m_midNeurons.size();
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
                iter != m_evolveFilterTaps.end(); 
                ++iter)
        {
            retJson[NET_OUT_FILT_TAPS_KEY].push_back(*iter);
        }

        return retJson;
    }

    void GoodBoyNet::Evolve(neuralVal_t p_amount){
        adjustWeights(p_amount);
        neuralSize_t numNewNeurons = numNeuronsToMake();
        if(numNewNeurons > 0){
            cleanup();
            makeNewNeurons(numNewNeurons);
        }

    }

    void GoodBoyNet::adjustWeights(neuralVal_t p_amount){
        for(auto neurIter = m_midNeurons.begin();
                neurIter != m_midNeurons.end();
                ++neurIter)
        {
            (*neurIter)->Evolve(p_amount);
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->Evolve(p_amount);
        }

        for(auto neurIter = m_midNeurons.begin();
                neurIter != m_midNeurons.end();
                ++neurIter)
        {
            (*neurIter)->ResetContributionFlag();
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->ResetContributionFlag();
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->ResetContributionFlag();
        }
    }

    void GoodBoyNet::cleanup(){
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

        std::list<Neuron*> toPurge;
        std::list<ConnectableNeuron*> toPurgeAsConnectable;
        for(auto neurIter = m_midNeurons.begin();
                neurIter != m_midNeurons.end();
                ++neurIter)
        {
            if(!(*neurIter)->GetWasFullyProbed()){
                toPurge.push_back(*neurIter);
                toPurgeAsConnectable.push_back(*neurIter);
            }
            (*neurIter)->ResetProbeFlag();
        }

        for(auto neurIter = m_outputs.begin();
                neurIter != m_outputs.end();
                ++neurIter)
        {
            (*neurIter)->ResetProbeFlag();
        }
        for(auto neurIter = m_inputs.begin();
                neurIter != m_inputs.end();
                ++neurIter)
        {
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

        for(auto neurIter = toPurgeAsConnectable.begin();
                neurIter != toPurgeAsConnectable.end();
                ++neurIter)
        {
            m_midNeurons.remove(*neurIter);
        }

        for(auto neurIter = toPurgeAsConnectable.begin();
                neurIter != toPurgeAsConnectable.end();
                ++neurIter)
        {
            m_midNeuronPool.Release(*neurIter);
        }
    }

    neuralSize_t GoodBoyNet::numNeuronsToMake(){
        neuralVal_t val = RandInRange<neuralVal_t>(0,1);
        neuralSize_t ret = 0;
        while(val < m_generationFactor && val != 0){
            ++ret;
            val /= m_generationFactor;
        }

        return ret;
    }

    void GoodBoyNet::makeNewNeurons(neuralSize_t p_numNewNeurons){
        //We are going to do "weightless" adding. Maybe in the future, we
            //will do weighted addition.
        for(auto iter=m_midNeurons.begin(); iter!=m_midNeurons.end(); ++iter){
        }

        for(neuralSize_t i = 0; i < p_numNewNeurons; ++i){
            neuralSize_t numOfRecv = m_midNeurons.size() + m_outputs.size();
            neuralSize_t numOfOut = m_midNeurons.size() + m_inputs.size();
            ConnectableNeuron* recvNeuronPtr;
            Neuron* outNeuronPtr;

            neuralSize_t recIndex = RandInRange<neuralSize_t>(0, numOfRecv);
            neuralSize_t outIndex = RandInRange<neuralSize_t>(0, numOfOut);

            if(recIndex < m_midNeurons.size()){
                auto iter = m_midNeurons.begin();
                std::advance(iter, recIndex);
                recvNeuronPtr = *iter;
            }
            else{
                recvNeuronPtr = m_outputs[recIndex - m_midNeurons.size()];
            }

            if(outIndex < m_midNeurons.size()){
                auto iter = m_midNeurons.begin();
                std::advance(iter, outIndex);
                outNeuronPtr = *iter;
            }
            else{
                outNeuronPtr = m_inputs[outIndex - m_midNeurons.size()].get();
            }

            ConnectableNeuron* newNeuron = m_midNeuronPool.AllocElement();

            newNeuron->Connect(outNeuronPtr);
            recvNeuronPtr->Connect(newNeuron);
            m_midNeurons.push_back(newNeuron);
        }
    }

    void GoodBoyNet::calcOutputs(){
        auto oIter = m_outputs.begin();
        auto lIter = m_lastOutputs.begin();

        for(;oIter != m_outputs.end() && lIter != m_lastOutputs.end();
                ++oIter, ++lIter)
        {
            *lIter = (*oIter)->GetOutput();
        }
        resetOutputFlag();
    }

    void GoodBoyNet::resetOutputFlag(){
        for(auto neurIter = m_outputs.begin(); 
                neurIter != m_outputs.end(); 
                ++neurIter)
        {
            (*neurIter)->ResetOutputFlag();
        }

        for(auto neurIter = m_midNeurons.begin(); 
                neurIter != m_midNeurons.end(); 
                ++neurIter)
        {
            (*neurIter)->ResetOutputFlag();
        }

        for(auto neurIter = m_inputs.begin(); 
                neurIter != m_inputs.end(); 
                ++neurIter)
        {
            (*neurIter)->ResetOutputFlag();
        }
    }
}
