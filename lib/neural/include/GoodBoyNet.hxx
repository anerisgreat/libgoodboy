#ifndef NEURAL_GOOD_BOY_NET_HXX
#define NEURAL_GOOD_BOY_NET_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"
#include "ConnectableNeuron.hxx"
#include "ObjectPool.hxx"
#include "Resetable.hxx"

#include <vector>
#include <memory>
#include <list>

namespace LibGoodBoy
{
    class GoodBoyNet : public std::enable_shared_from_this<GoodBoyNet>
    {
        public:
            //Constructor & Destructor____________________
            GoodBoyNet(
                    std::vector<neuralVal_t>& p_outputFilterTaps,
                    std::vector<neuralVal_t>& p_evolveFilterTaps,

                    neuralVal_t p_degrFactor,
                    neuralVal_t p_maxStartWeight,
                    neuralVal_t p_defaultAlpha,

                    bool p_evolvingEnabled = true
                    );
            ~GoodBoyNet();

            void Iter();
            void SetInputs(std::vector<neuralVal_t>& p_inVec);
            void SetInput(neuralSize_t p_nInput, neuralVal_t p_inputVal);

            void CreateInputs(neuralSize_t p_nInputs);
            void CreateOutputs(neuralSize_t p_nOutputs);

            void GetOutputs(std::vector<neuralVal_T>& p_outBuff) const;
            neuralVal_t GetOutput(neuralVal_t p_nOutput) const;

            json_t GetJSON();

        protected:

        private:
            void evolve();
            void adjustWeights();
            boolean shouldCleanup();
            void cleanup();
            neuralSize_t numNeuronsToMake();
            void makeNewNeurons();

            void calcOutputs();

            std::shared_ptr<ConnectableNeuron> makeNewConnectableNeuronPtr();
            std::shared_ptr<NeuralConnection> makeNewNeuralConnectionPtr();

            std::vector<std::shared_ptr<InputNeuron>> m_inputs;
            std::vector<std::shared_ptr<ConnectableNeuron>> m_outputs;

            std::vector<neuralVal_t> m_lastOutputs;

            std::list<std::shared_ptr<ConnectableNeuron>> m_midNeurons;

            ObjectPool<ConnectableNeuron> m_midNeuronPool;
            ObjectPool<NeuralConnection> m_connectionPool;

            std::vector<neuralVal_t>& m_outputFilterTaps,
            std::vector<neuralVal_t>& m_evolveFilterTaps,

            neuralVal_t m_degrFactor,
            neuralVal_t m_maxStartWeight,
            neuralVal_t m_defaultAlpha,
            bool m_evolvingEnabled;
    };
}

#endif
