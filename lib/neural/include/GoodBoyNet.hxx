#ifndef NEURAL_NEURON_HXX
#define NEURAL_NEURON_HXX

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

                    bool p_evolvingEnabled
                    );
            ~GoodBoyNet();

            void Iter();
            void SetInputs();
            void SetInput(p_nInput);

            std::vector<neuralVal_t>& GetOutputs() const;
            neuralVal_t GetOutput(neuralVal_t p_nOutput) const;

            json_t GetJSON();

        protected:

        private:
            void cleanup();
            boolean shouldCleanup();
            neuralSize_t numNeuronsToMake();

            void calcOutputs();
            void evolve();

            std::shared_ptr<ConnectableNeuron> makeNewConnectableNeuronPtr();
            std::shared_ptr<ConnectableNeuron> makeNewNeuralConnectionPtr();

            std::vector<std::shared_ptr<InputNeuron>> m_inputs;
            std::vector<std::shared_ptr<ConnectableNeuron>> m_outputs;

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
