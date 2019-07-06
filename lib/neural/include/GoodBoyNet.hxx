#ifndef NEURAL_GOOD_BOY_NET_HXX
#define NEURAL_GOOD_BOY_NET_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralGlobals.hxx"
#include "Neuron.hxx"
#include "NeuralConnectionPool.hxx"
#include "ConnectableNeuronPool.hxx"
#include "ConnectableNeuron.hxx" 
#include "InputNeuron.hxx"

#include <vector>
#include <memory>
#include <list>

namespace LibGoodBoy{

class GoodBoyNet{
    public:
        //Constructor & Destructor____________________
        GoodBoyNet(
                const std::vector<neuralVal_t>& p_outputFilterTaps,
                const std::vector<neuralVal_t>& p_evolveFilterTaps,

                neuralVal_t p_degrFactor,
                neuralVal_t p_maxStartWeight,
                neuralVal_t p_defaultAlpha,
                neuralVal_t p_defaultGenerationFactor,

                bool p_evolvingEnabled);
        ~GoodBoyNet();

        //Runtime_____________________________________
        void Iter();

        //Input/output handling_______________________
        void CreateInputGroup(std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        void AppendToInputGroup(std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        void CreateOutputGroup(std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        void AppendToOutputGroup(std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        void GetOutputGroupValues(std::string p_groupName,
                std::vector<neuralVal_t>& p_outBuff) const;

        //Statistics__________________________________
        neuralSize_t GetMidSize() const;

        //JSON________________________________________
        json_t GetJSON() const;
        std::string jsonString();

        void Evolve(neuralVal_t p_amount);

    private:

        //Methods_____________________________________
        void adjustWeights(neuralVal_t p_amount);
        void cleanup(); //Erases all non-reachable neurons

        neuralSize_t numNeuronsToMake();
        void makeNewNeurons(neuralSize_t p_numNewNeurons);

        Neuron* getOutNeuron(neuralVal_t p_maxSelectionWeight);
        neuralVal_t calcOutNeuronWeight(Neuron* p_n);
        ConnectableNeuron* getRecvNeuron(Neuron* p_outNeuron);
        posscalar_t calcRecvNeuronWeight(
            const ConnectableNeuron* p_recvNeuron, 
            pos_t p_outPos);

        void calcOutputs();
        void resetOutputFlag();
        void resetOutputSums();

        //Members_____________________________________
        std::map<std::string, std::vector<std::shared_ptr<InputNeuron>>>
            m_inputMap;
        std::map<std::string, std::vector<ConnectableNeuron*>>
            m_outputMap;

        std::map<std::string, ConnectableNeuron*> m_outputMap;

        std::list<ConnectableNeuron*> m_midNeurons;

        neuralConnectionPool_t m_connectionPool;
        connectableNeuronPool_t m_midNeuronPool;

        const std::vector<neuralVal_t> m_outputFilterTaps;
        const std::vector<neuralVal_t> m_evolveFilterTaps;

        neuralVal_t m_degrFactor;
        neuralVal_t m_maxStartWeight;
        neuralVal_t m_defaultAlpha;
        neuralVal_t m_generationFactor;
        bool m_evolvingEnabled;
};

}//End namespace

#endif
