#ifndef NEURAL_INPUT_NEURON_HXX
#define NEURAL_INPUT_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"

namespace LibGoodBoy
{
    class InputNeuron : public Neuron
    {
        public:
            InputNeuron(const std::vector<neuralVal_t>& p_outputFilterTaps,
                        const std::vector<neuralVal_t>& p_evolveFilterTaps);

            ~InputNeuron();

            void FeedInput(neuralVal_t p_input);
            
            //JSON GetJson()
        protected:
            neuralVal_t calcOutput();

            void postBackProbe();
            void postForwardProbe();
            
            void evolveSelf(neuralVal_t p_amount);
            
            void postPurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& p_toPurge);

            void postReset();
        private:
            neuralVal_t m_nextInput;
    };
}

#endif
