#ifndef NEURAL_GOOD_BOY_NET_HXX
#define NEURAL_GOOD_BOY_NET_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"
#include "ConnectableNeuron.hxx"
#include "InputNeuron.hxx"
#include "ObjectPool.hxx"
#include "InstanceFactory.hxx"

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

                    neuralSize_t p_nInputs,
                    neuralSize_t p_nOutputs,

                    neuralVal_t p_degrFactor,
                    neuralVal_t p_maxStartWeight,
                    neuralVal_t p_defaultAlpha,
                    neuralVal_t p_defaultGenerationFactor,

                    bool p_evolvingEnabled);
            ~GoodBoyNet();

            void Iter();
            void SetInputs(const std::vector<neuralVal_t>& p_inVec);
            void SetInput(neuralSize_t p_nInput, neuralVal_t p_inputVal);

            void CreateInputs(neuralSize_t p_nInputs);
            void CreateOutputs(neuralSize_t p_nOutputs);

            void GetOutputs(std::vector<neuralVal_t>& p_outBuff) const;
            neuralVal_t GetOutput(neuralVal_t p_nOutput) const;

            json_t GetJSON() const;

        private:

            void evolve();
            void adjustWeights();
            void cleanup();
            neuralSize_t numNeuronsToMake();
            void makeNewNeurons(neuralSize_t p_numNewNeurons);

            void calcOutputs();

            std::shared_ptr<ConnectableNeuron> makeNewConnectableNeuronPtr();
            std::shared_ptr<NeuralConnection> makeNewNeuralConnectionPtr();

            std::vector<std::shared_ptr<InputNeuron>> m_inputs;
            std::vector<std::shared_ptr<ConnectableNeuron>> m_outputs;

            std::vector<neuralVal_t> m_lastOutputs;

            std::list<std::shared_ptr<ConnectableNeuron>> m_midNeurons;

            ObjectPool<NeuralConnection> m_connectionPool;
            ObjectPool<ConnectableNeuron,
                //Constructor arguments passed to template
                    const std::vector<neuralVal_t>&,
                    const std::vector<neuralVal_t>&,
                    ObjectPool<NeuralConnection>&,
                    neuralVal_t,
                    neuralVal_t,
                    neuralVal_t> 
                m_midNeuronPool;

            const std::vector<neuralVal_t>& m_outputFilterTaps;
            const std::vector<neuralVal_t>& m_evolveFilterTaps;

            neuralVal_t m_degrFactor;
            neuralVal_t m_maxStartWeight;
            neuralVal_t m_defaultAlpha;
            neuralVal_t m_generationFactor;
            bool m_evolvingEnabled;
    };
}

#endif
