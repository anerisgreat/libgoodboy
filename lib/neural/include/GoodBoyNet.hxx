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

namespace LibGoodBoy
{
    class GoodBoyNet
    {
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

            void Iter();
            void SetInputs(const std::vector<neuralVal_t>& p_inVec);
            void SetInput(neuralSize_t p_nInput, neuralVal_t p_inputVal);

            void CreateInputs(const std::vector<pos_t>& p_positions);
            void CreateOutputs(const std::vector<pos_t>& p_positions);

            void GetOutputs(std::vector<neuralVal_t>& p_outBuff) const;
            neuralVal_t GetOutput(neuralVal_t p_nOutput) const;

            neuralSize_t GetMidSize() const;

            json_t GetJSON() const;
            std::string jsonString();

            void Evolve(neuralVal_t p_amount);

        private:

            void adjustWeights(neuralVal_t p_amount);
            void cleanup();

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

            std::vector<std::shared_ptr<InputNeuron>> m_inputs;
            std::vector<ConnectableNeuron*> m_outputs;

            std::vector<neuralVal_t> m_lastOutputs;

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
}

#endif
