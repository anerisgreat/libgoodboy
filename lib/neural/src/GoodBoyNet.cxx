#include "GoodBoyNet.hxx"
#include "NeuralUtils.hxx"
#include <memory>

namespace LibGoodBoy{

//Public_____________________________________________________________
//Constructor & Destructor_____________________________________
GoodBoyNet::GoodBoyNet(
        const std::vector<neuralVal_t>& p_outputFilterTaps,
        const std::vector<neuralVal_t>& p_evolveFilterTaps,

        neuralVal_t p_degrFactor,
        neuralVal_t p_maxStartWeight,
        neuralVal_t p_defaultAlpha,
        neuralVal_t p_defaultGenerationFactor,

        bool p_evolvingEnabled)
    :
        m_midNeuronPool(
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
{}

GoodBoyNet::~GoodBoyNet(){}

void GoodBoyNet::Iter(){
    calcOutputs();
}

void GoodBoyNet::CreateInputGroup(const std::string p_groupName,
    const std::vector<pos_t>& p_positions)
{
    //If not found proceed
    if(m_inputMap.find(p_groupName) == m_inputMap.end()){
        m_inputMap[p_groupName] = std::vector<InputNeuron*>();
        appendInputNeurons(p_groupName, p_positions);
    }
    else{
        //TODO: Throw exception
    }
}

void GoodBoyNet::AppendToInputGroup(const std::string p_groupName,
    const std::vector<pos_t>& p_positions)
{
    //If found proceed
    if(m_inputMap.find(p_groupName) != m_inputMap.end()){
        appendInputNeurons(p_groupName, p_positions);
    }
    else{
        //TODO: Throw exception
    }
}

void GoodBoyNet::SetInputGroupValues(const std::string p_groupName,
        const std::vector<neuralVal_t>& p_vals)
{
    auto inGroup = m_inputMap.find(p_groupName);
    if(inGroup == m_inputMap.end()){
        //TODO:Proper exception
    }
    if(p_vals.size() != inGroup.value()->size()){
        //TODO: Proper exception
    }

    auto iIter = inGroup->begin();
    auto vIter = p_vals.begin();
    while(iIter!= p_vals.end() && vIter != inGroup->end()){

        *oIter++ = (*vIter++)->GetLastOutput();
    }
}

void GoodBoyNet::CreateOutputGroup(const std::string p_groupName,
    const std::vector<pos_t>& p_positions)
{
    //If not found proceed
    if(m_outputMap.find(p_groupName) == m_outputMap.end()){
        m_outputMap[p_groupName] = std::vector<ConnectableNeuron*>;
        appendOutputNeurons(p_groupName, p_positions);
    }
    else{
        //TODO: Throw exception
    }
}

void GoodBoyNet::AppendToOutputGroup(const std::string p_groupName,
    const std::vector<pos_t>& p_positions)
{
    //If found proceed
    if(m_outputMap.find(p_groupName) != m_outputMap.end()){
        appendOutputNeurons(p_groupName, p_positions);
    }
    else{
        //TODO: Throw exception
    }
}

void GoodBoyNet::GetOutputGroupValues(std::string p_groupName,
    std::vector<neuralVal_t>& p_outBuff) const
{
    auto outGroup = m_outputMap.find(p_groupName);
    if(outGroup == m_outputMap.end()){
        //TODO:Proper exception
    }
    if(p_outBuff.size() != outGroup->size()){
        //TODO: Proper exception
    }
    auto vIter = outGroup->begin();
    auto oIter = p_outBuff.begin();
    while(oIter!= p_outBuff.end() && vIter != outGroup->end()){
        *oIter++ = (*vIter++)->GetLastOutput();
    }
}

neuralVal_t GoodBoyNet::GetOutput(neuralVal_t p_nOutput) const{
    return m_lastOutputs[p_nOutput];
}

neuralSize_t GoodBoyNet::GetMidSize() const{
    return m_midNeurons.size();
}

json_t GoodBoyNet::GetJSON() const{
    /*
    json_t retJson;

    retJson[NET_INP_KEY] = json_t::array();
    for(auto iter = m_inputMap.begin(); iter != m_inputMap.end(); ++iter){
        json_t inGroup;
        inGroup[NET_INP_GROUP_NAME_KEY] = iter->getKey();
        inGroup[NET_INP_GROUP_NEURONS_KEY] = iter->json_t::array();
        for(auto neurIter = iter->getValue()->begin();
                neurIter != iter->getValue()->end();
                ++neurIter)
        {
            inGroup[NET_INP_GROUP_NEURONS_KEY].push_back(neurIter->GetJSON());
        }
        retJson[NET_INP_KEY].push_back(inGroup);
    }

    retJson[NET_OUT_KEY] = json_t::array();
    for(auto iter = m_outputs.begin(); iter != m_outputs.end(); ++iter){
        json_t outGroup;
        outGroup[NET_OUT_GROUP_NAME_KEY] = iter->getKey();
        outGroup[NET_OUT_GROUP_NEURONS_KEY] = iter->json_t::array();
        for(auto neurIter = iter->getValue()->begin();
                neurIter != neurIter->getValue()->end();
                ++neurIter)
        {
            outGroup[NET_OUT_GROUP_NEURONS_KEY].push_back(neurIter->GetJSON());
        }
        retJson[NET_OUT_KEY].push_back(outGroup);
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
    */
}

std::string GoodBoyNet::jsonString(){
    /*
    return GetJSON().dump();
    */
}

void GoodBoyNet::Evolve(neuralVal_t p_amount){
    /*
    adjustWeights(p_amount);
    neuralSize_t numNewNeurons = numNeuronsToMake();
    if(numNewNeurons > 0){
        cleanup();
        makeNewNeurons(numNewNeurons);
    }

    resetOutputSums();
    */
}

Neuron* GoodBoyNet::getOutNeuron(neuralVal_t p_maxSelectionWeight){
    /*
    neuralVal_t outWeight = 
        RandInRange<neuralVal_t>(0, p_maxSelectionWeight);
    neuralVal_t outWeightAcc = 0;

    for(auto iter=m_midNeurons.begin(); iter!=m_midNeurons.end(); ++iter){
        outWeightAcc+=calcOutNeuronWeight(*iter);
        if(outWeightAcc > outWeight){
            return *iter;
        }
    }

    for(auto iter=m_inputs.begin(); iter!=m_inputs.end(); ++iter){
        outWeightAcc+=calcOutNeuronWeight(iter->get());
        if(outWeightAcc > outWeight){
            return (*iter).get();
        }
    }

    return m_inputs[m_inputs.size() - 1].get();
    */
}


neuralVal_t GoodBoyNet::calcOutNeuronWeight(Neuron* p_n){
    /*
    neuralSize_t noutputs = p_n->GetNumOutputs();
    neuralVal_t ret = (log(abs(p_n->GetOutputSum()) + 1))/(noutputs + 1);
    return ret;
    */
}

ConnectableNeuron* GoodBoyNet::getRecvNeuron(Neuron* p_outNeuron){
    /*
    pos_t neuronPos = p_outNeuron->GetPosition();

    posscalar_t maxSelectionWeight = 0;
    for(auto iter = m_midNeurons.begin();
            iter != m_midNeurons.end();
            ++iter)
    {
        maxSelectionWeight += calcRecvNeuronWeight(*iter, neuronPos);
    }

    for(auto iter = m_outputs.begin();
            iter != m_outputs.end();
            ++iter)
    {
        maxSelectionWeight += calcRecvNeuronWeight(*iter, neuronPos);
    }

    posscalar_t outWeight = RandInRange<posscalar_t>(0, maxSelectionWeight);
    posscalar_t outWeightAcc = 0;

    for(auto iter=m_midNeurons.begin(); iter!=m_midNeurons.end(); ++iter){
        outWeightAcc += calcRecvNeuronWeight(*iter, neuronPos);
        if(outWeightAcc > outWeight){
            return *iter;
        }
    }

    for(auto iter=m_outputs.begin(); iter!=m_outputs.end(); ++iter){
        outWeightAcc += calcRecvNeuronWeight(*iter, neuronPos);
        if(outWeightAcc > outWeight){
            return *iter;
        }
    }

    return m_outputs[m_outputs.size() - 1];
    */
}

posscalar_t GoodBoyNet::calcRecvNeuronWeight(
        const ConnectableNeuron* p_recvNeuron, 
        pos_t p_outPos)
{
    neuralSize_t ninputs = p_recvNeuron->GetNumInputs();
    posscalar_t d = GetDistance(p_recvNeuron->GetPosition(), p_outPos);
    if(d == 0)
        return 0;
    return 1/(d*(ninputs + 1));
}

void GoodBoyNet::adjustWeights(neuralVal_t p_amount){
    /*
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
    */
}

void GoodBoyNet::cleanup(){
    /*
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
    */
}

neuralSize_t GoodBoyNet::numNeuronsToMake(){
    /*
    neuralVal_t val = RandInRange<neuralVal_t>(0,2);
    neuralSize_t ret = 0;
    while(val < m_generationFactor && val != 0){
        ++ret;
        val /= m_generationFactor;
    }

    return ret;
    */
}

void GoodBoyNet::makeNewNeurons(neuralSize_t p_numNewNeurons){
    /*
    //Getting max selection weight for output selection
    neuralVal_t maxSelectionWeight = 0;
    for(auto iter=m_midNeurons.begin(); iter!=m_midNeurons.end(); ++iter){
        maxSelectionWeight += calcOutNeuronWeight(*iter);
    }
    for(auto iter=m_inputs.begin(); iter!=m_inputs.end(); ++iter){
        maxSelectionWeight += calcOutNeuronWeight(iter->get());
    }

    for(neuralSize_t i = 0; i < p_numNewNeurons; ++i){
        neuralSize_t numOfRecv = m_midNeurons.size() + m_outputs.size();
        neuralSize_t numOfOut = m_midNeurons.size() + m_inputs.size();
        ConnectableNeuron* recvNeuronPtr;
        Neuron* outNeuronPtr;

        neuralSize_t recIndex = RandInRange<neuralSize_t>(0, numOfRecv);
        if(recIndex < m_midNeurons.size()){
            auto iter = m_midNeurons.begin();
            std::advance(iter, recIndex);
            recvNeuronPtr = *iter;
        }
        else{
            recvNeuronPtr = m_outputs[recIndex - m_midNeurons.size()];
        }

        outNeuronPtr = getOutNeuron(maxSelectionWeight);
        recvNeuronPtr = getRecvNeuron(outNeuronPtr);

        ConnectableNeuron* newNeuron = m_midNeuronPool.AllocElement();

        newNeuron->Connect(outNeuronPtr);
        recvNeuronPtr->Connect(newNeuron);
        newNeuron->SetPosition(Neuron::GetNeuronAveragePosition(
                    *outNeuronPtr, 
                    *recvNeuronPtr));
        m_midNeurons.push_back(newNeuron);
    }
    */
}

void GoodBoyNet::appendInputNeurons(const std::string p_groupName,
        const std::vector<pos_t>& p_positions)
{
    /*
    for(auto iter = p_positions.begin();
            iter != p_positions.end();
            ++iter)
    {
        InputNeuron* tmp = m_inputNeuronPool.AllocElement();
        tmp->SetPosition(*iter);
        m_inputMap[p_groupName].push_back(tmp);
    }
    */
}

void GoodBoyNet::appendOutputNeurons(const std::string p_groupName,
        const std::vector<pos_t>& p_positions)
{
    /*
    for(auto iter = p_positions.begin();
            iter != p_positions.end();
            ++iter)
    {
        ConnectableNeuron* tmp = m_midNeuronPool.AllocElement();
        tmp->SetPosition(*iter);
        m_outputMap[p_groupName].push_back(tmp);
    }
    */
}

void GoodBoyNet::calcOutputs(){
    /*
    //Do this to make sure all inputs always update
    for(auto iter = m_inputs.begin(); iter != m_inputs.end(); ++iter){
        (*iter)->GetOutput();
    }

    auto oIter = m_outputs.begin();
    auto lIter = m_lastOutputs.begin();

    for(;oIter != m_outputs.end() && lIter != m_lastOutputs.end();
            ++oIter, ++lIter)
    {
        *lIter = (*oIter)->GetOutput();
    }
    resetOutputFlag();
    */
}

void GoodBoyNet::resetOutputFlag(){
    /*
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
    */
}

void GoodBoyNet::resetOutputSums(){
    /*
    for(auto neurIter = m_outputs.begin(); 
            neurIter != m_outputs.end(); 
            ++neurIter)
    {
        (*neurIter)->ResetOutputSum();
    }

    for(auto neurIter = m_midNeurons.begin(); 
            neurIter != m_midNeurons.end(); 
            ++neurIter)
    {
        (*neurIter)->ResetOutputSum();
    }

    for(auto neurIter = m_inputs.begin(); 
            neurIter != m_inputs.end(); 
            ++neurIter)
    {
        (*neurIter)->ResetOutputSum();
    }
    */
}

}//End namespace
