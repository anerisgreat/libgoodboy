#include "GoodBoyNet.hxx"
#include "NeuralUtils.hxx"

#include <stdexcept>

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

//Runtime______________________________________________________
void GoodBoyNet::Iter(){
    //Do this to make sure all inputs always update
    for(auto iter = getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->GetOutput();
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->GetOutput();
    }

    //Reset output flags
    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetOutputFlag();
    }

    for(auto iter = m_midNeurons.begin(); iter != m_midNeurons.end(); ++iter){
        (*iter)->ResetOutputFlag();
    }

    for(auto iter = getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetOutputFlag();
    }
}

void GoodBoyNet::Evolve(neuralVal_t p_amount){
    adjustWeights(p_amount);
    neuralSize_t numNewNeurons = numNeuronsToMake();
    if(numNewNeurons > 0){
        cleanup();
        makeNewNeurons(numNewNeurons);
    }

    //Reset output sums
    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetOutputSum();
    }

    for(auto iter = m_midNeurons.begin(); iter != m_midNeurons.end(); ++iter){
        (*iter)->ResetOutputSum();
    }

    for(auto iter = getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetOutputSum();
    }
}

//Input/output handling________________________________________
void GoodBoyNet::CreateInputGroup(const std::string p_groupName,
    const std::vector<pos_t>& p_positions)
{
    //If not found proceed
    if(m_inputMap.find(p_groupName) == m_inputMap.end()){
        m_inputMap[p_groupName] = std::vector<InputNeuron*>();
        appendInputNeurons(p_groupName, p_positions);
    }
    else{
        throw std::invalid_argument("An input group with that name already " \
                "exists!");
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
        throw std::invalid_argument("No input group with that name exists!");
    }
}

void GoodBoyNet::SetInputGroupValues(const std::string p_groupName,
        const std::vector<neuralVal_t>& p_vals)
{
    auto inGroup = m_inputMap.find(p_groupName);
    if(inGroup == m_inputMap.end()){
        throw std::invalid_argument("No input group with that name exists!");
    }
    if(p_vals.size() != inGroup->second.size()){
        throw std::invalid_argument("Number of values in input vector does "\
                "not match number of neurons in input group!");
    }

    auto iIter = inGroup->second.begin();
    auto vIter = p_vals.begin();
    while(iIter!= inGroup->second.end() && vIter != p_vals.end()){
        (*iIter++)->FeedInput((*vIter++));
    }
}

void GoodBoyNet::CreateOutputGroup(const std::string p_groupName,
    const std::vector<pos_t>& p_positions)
{
    //If not found proceed
    if(m_outputMap.find(p_groupName) == m_outputMap.end()){
        m_outputMap[p_groupName] = std::vector<ConnectableNeuron*>();
        appendOutputNeurons(p_groupName, p_positions);
    }
    else{
        throw std::invalid_argument("An output group with that name already " \
                "exists!");
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
        throw std::invalid_argument("No input group with that name exists!");
    }
}

void GoodBoyNet::GetOutputGroupValues(std::string p_groupName,
    std::vector<neuralVal_t>& p_outBuff) const
{
    auto outGroup = m_outputMap.find(p_groupName);
    if(outGroup == m_outputMap.end()){
        throw std::invalid_argument("No output group with that name exists!");
    }
    if(p_outBuff.size() != outGroup->second.size()){
        throw std::invalid_argument("Number of values in output vector does "\
                "not match number of neurons in input group!");
    }
    auto vIter = outGroup->second.begin();
    auto oIter = p_outBuff.begin();
    while(oIter!= p_outBuff.end() && vIter != outGroup->second.end()){
        *oIter++ = (*vIter++)->GetLastOutput();
    }
}

//Statistics___________________________________________________
neuralSize_t GoodBoyNet::GetMidSize() const{
    return m_midNeurons.size();
}

//JSON_________________________________________________________
json_t GoodBoyNet::GetJSON() const{
    json_t retJson;

    retJson[NET_INP_KEY] = json_t::array();
    for(auto iter = m_inputMap.begin(); iter != m_inputMap.end(); ++iter){
        json_t inGroup;
        inGroup[NET_INP_GROUP_NAME_KEY] = iter->first;
        inGroup[NET_INP_GROUP_NEURONS_KEY] = json_t::array();
        for(auto neurIter = iter->second.begin();
                neurIter != iter->second.end();
                ++neurIter)
        {
            inGroup[NET_INP_GROUP_NEURONS_KEY].push_back(
                    (*neurIter)->GetJSON());
        }
        retJson[NET_INP_KEY].push_back(inGroup);
    }

    retJson[NET_OUT_KEY] = json_t::array();
    for(auto iter = m_outputMap.begin(); iter != m_outputMap.end(); ++iter){
        json_t outGroup;
        outGroup[NET_OUT_GROUP_NAME_KEY] = iter->first;
        outGroup[NET_OUT_GROUP_NEURONS_KEY] = json_t::array();
        for(auto neurIter = iter->second.begin();
                neurIter != iter->second.end();
                ++neurIter)
        {
            outGroup[NET_OUT_GROUP_NEURONS_KEY].push_back(
                    (*neurIter)->GetJSON());
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
}

std::string GoodBoyNet::GetJSONString(){
    return GetJSON().dump();
}

//Private____________________________________________________________
//Runtime______________________________________________________
void GoodBoyNet::adjustWeights(neuralVal_t p_amount){
    for(auto iter = m_midNeurons.begin(); iter != m_midNeurons.end(); ++iter){
        (*iter)->Evolve(p_amount);
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->Evolve(p_amount);
    }

    for(auto iter = m_midNeurons.begin(); iter != m_midNeurons.end(); ++iter){
        (*iter)->ResetContributionFlag();
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetContributionFlag();
    }
}

void GoodBoyNet::cleanup(){
    for(auto iter = getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ForwardProbe();
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->BackProbe();
    }

    std::list<Neuron*> toPurge;
    std::list<ConnectableNeuron*> toPurgeAsConnectable;
    for(auto iter = m_midNeurons.begin(); iter != m_midNeurons.end(); ++iter){
        if(!(*iter)->GetWasFullyProbed()){
            toPurge.push_back(*iter);
            toPurgeAsConnectable.push_back(*iter);
        }
        (*iter)->ResetProbeFlag();
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetProbeFlag();
    }

    for(auto iter = getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->ResetProbeFlag();
    }

    for(auto iter = m_midNeurons.begin(); iter != m_midNeurons.end(); ++iter){
        (*iter)->PurgeConnections(toPurge);
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        (*iter)->PurgeConnections(toPurge);
    }

    for(auto iter = toPurgeAsConnectable.begin();
            iter != toPurgeAsConnectable.end();
            ++iter)
    {
        m_midNeurons.remove(*iter);
    }

    for(auto iter = toPurgeAsConnectable.begin();
            iter != toPurgeAsConnectable.end();
            ++iter)
    {
        m_midNeuronPool.Release(*iter);
    }
}

neuralSize_t GoodBoyNet::numNeuronsToMake(){
    neuralVal_t val = RandInRange<neuralVal_t>(0,2);
    neuralSize_t ret = 0;
    while(val < m_generationFactor && val != 0){
        ++ret;
        val /= m_generationFactor;
    }

    return ret;
}

void GoodBoyNet::makeNewNeurons(neuralSize_t p_numNewNeurons){
    //Getting max selection weight for output selection
    neuralVal_t maxSelectionWeight = 0;
    for(auto iter=m_midNeurons.begin(); iter!=m_midNeurons.end(); ++iter){
        maxSelectionWeight += calcOutNeuronWeight(*iter);
    }
    for(auto iter=getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        maxSelectionWeight += calcOutNeuronWeight(*iter);
    }

    for(neuralSize_t i = 0; i < p_numNewNeurons; ++i){
        neuralSize_t numOfRecv = m_midNeurons.size() + getNumOutputs();
        neuralSize_t numOfOut = m_midNeurons.size() + getNumInputs();
        ConnectableNeuron* recvNeuronPtr;
        Neuron* outNeuronPtr;

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
}

Neuron* GoodBoyNet::getOutNeuron(neuralVal_t p_maxSelectionWeight){
    neuralVal_t outWeight = 
        RandInRange<neuralVal_t>(0, p_maxSelectionWeight);
    neuralVal_t outWeightAcc = 0;

    for(auto iter=m_midNeurons.begin(); iter!=m_midNeurons.end(); ++iter){
        outWeightAcc+=calcOutNeuronWeight(*iter);
        if(outWeightAcc > outWeight){
            return *iter;
        }
    }

    for(auto iter=getInputNeuronIterator(); !iter.IsEnd(); ++iter){
        outWeightAcc+=calcOutNeuronWeight(*iter);
        if(outWeightAcc > outWeight){
            return (*iter);
        }
    }

    return NULL;
}

neuralVal_t GoodBoyNet::calcOutNeuronWeight(Neuron* p_n){
    neuralSize_t noutputs = p_n->GetNumOutputs();
    neuralVal_t ret = (log(abs(p_n->GetOutputSum()) + 1))/(noutputs + 1);
    return ret;
}

ConnectableNeuron* GoodBoyNet::getRecvNeuron(Neuron* p_outNeuron){
    pos_t neuronPos = p_outNeuron->GetPosition();

    posscalar_t maxSelectionWeight = 0;
    for(auto iter = m_midNeurons.begin();
            iter != m_midNeurons.end();
            ++iter)
    {
        maxSelectionWeight += calcRecvNeuronWeight(*iter, neuronPos);
    }

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
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

    for(auto iter = getOutputNeuronIterator(); !iter.IsEnd(); ++iter){
        outWeightAcc += calcRecvNeuronWeight(*iter, neuronPos);
        if(outWeightAcc > outWeight){
            return *iter;
        }
    }

    throw std::logic_error("An error occured while selecting a receiving "\
            "neuron. This should never have happened.");
    return NULL;
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

//Iterators____________________________________________________
GoodBoyNet::groupIterator<InputNeuron*> GoodBoyNet::getInputNeuronIterator(){
    return groupIterator<InputNeuron*>(m_inputMap);
}

GoodBoyNet::groupIterator<ConnectableNeuron*>
    GoodBoyNet::getOutputNeuronIterator()
{
    return groupIterator<ConnectableNeuron*>(m_outputMap);
}

//Input/output_________________________________________________
void GoodBoyNet::appendInputNeurons(const std::string p_groupName,
        const std::vector<pos_t>& p_positions)
{
    for(auto iter = p_positions.begin();
            iter != p_positions.end();
            ++iter)
    {
        InputNeuron* tmp = m_inputNeuronPool.AllocElement();
        tmp->SetPosition(*iter);
        m_inputMap[p_groupName].push_back(tmp);
    }
}

void GoodBoyNet::appendOutputNeurons(const std::string p_groupName,
        const std::vector<pos_t>& p_positions)
{
    for(auto iter = p_positions.begin();
            iter != p_positions.end();
            ++iter)
    {
        ConnectableNeuron* tmp = m_midNeuronPool.AllocElement();
        tmp->SetPosition(*iter);
        m_outputMap[p_groupName].push_back(tmp);
    }
}

ConnectableNeuron* GoodBoyNet::getOutputAtIndex(neuralSize_t p_index){
    neuralSize_t currentIndex = 0;
    for(auto iter = m_outputMap.begin();
            iter != m_outputMap.end();
            ++iter)
    {
        if(currentIndex + iter->second.size() > p_index)
            return iter->second[p_index - currentIndex];
        else
            currentIndex += iter->second.size();
    }
    throw std::invalid_argument("Error while trying to find output at index. "\
            "Invalid index.");
    return NULL;
}

neuralSize_t GoodBoyNet::getNumOutputs(){
    neuralSize_t ret = 0;
    for(auto iter = m_outputMap.begin();
            iter != m_outputMap.end();
            ++iter)
    {
        ret += iter->second.size();
    }
    return ret;
}

InputNeuron* GoodBoyNet::getInputAtIndex(neuralSize_t p_index){
    neuralSize_t currentIndex = 0;
    for(auto iter = m_inputMap.begin();
            iter != m_inputMap.end();
            ++iter)
    {
        if(currentIndex + iter->second.size() > p_index)
            return iter->second[p_index - currentIndex];
        else
            currentIndex += iter->second.size();
    }
    throw std::invalid_argument("Error while trying to find input at index. "\
            "Invalid index.");
    return NULL;
}

neuralSize_t GoodBoyNet::getNumInputs(){
    neuralSize_t ret = 0;
    for(auto iter = m_inputMap.begin();
            iter != m_inputMap.end();
            ++iter)
    {
        ret += iter->second.size();
    }
    return ret;
}

}//End namespace
