#ifndef NEURAL_INPUT_NEURON_HXX
#define NEURAL_INPUT_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "neural/NeuralConfig.hxx"
#include "neural/Neuron.hxx"

namespace lib_good_boy::neural
{
    class InputNeuron : public Neuron
    {
        public:
            InputNeuron(const std::vector<neuralVal_t>& outputFilterTaps,
                        const std::vector<neuralVal_t>& evolveFilterTaps);

            ~InputNeuron();

            void FeedInput(neuralVal_t input);
            
            //JSON GetJson()
        protected:
            neuralVal_t calcOutput();

            void postBackProbe();
            void postForwardProbe();
            
            void evolveSelf(neuralVal_t amount);
            
            void postPurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& toPurge);

            void postFlush();
        private:
            neuralVal_t m_nextInput;
    };
}

#endif
