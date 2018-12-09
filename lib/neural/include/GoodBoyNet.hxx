#ifndef NEURAL_NEURON_HXX
#define NEURAL_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
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
            GoodBoyNet();
            ~GoodBoyNet();

            void Iter();
            void SetInputs();
            
            std::vector<neuralVal_t>& GetOutputs() const;
            neuralVal_t GetOutput() const;
            void ResetOutputFlag();

            json_t GetJSON();

        protected:

        private:
            void cleanup();
            boolean shouldCleanup();
            boolean shouldMakeNeuron();

            std::shared_ptr<ConnectableNeuron> makeNewConnectableNeuron();

            std::vector<std::shared_ptr<InputNeuron>> m_inputs;
            std::vector<std::shared_ptr<ConnectableNeuron>> m_outputs;

            std::list<std::shared_ptr<ConnectableNeuron>> m_midNeurons;

            ObjectPool<ConnectableNeuron> m_midNeuronPool;
            ObjectPool<NeuralConnection> m_connectionPool;
    };
}

#endif
