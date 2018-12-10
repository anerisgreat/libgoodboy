#ifndef NEURAL_INPUT_NEURON_HXX
#define NEURAL_INPUT_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"

#include <vector>

namespace LibGoodBoy
{
    class InputNeuron : public Neuron
    {
        public:
            InputNeuron(const std::vector<neuralVal_t>& p_outputFilterTaps,
                        const std::vector<neuralVal_t>& p_evolveFilterTaps);

            ~InputNeuron();

            void Evolve(neuralVal_t p_amount);

            void FeedInput(neuralVal_t p_input);

            void Reset();

            json_t GetJSON() const;
        protected:
            neuralVal_t calcOutput();

            void postBackProbe();
            void postForwardProbe();

        private:
            neuralVal_t m_nextInput;
    };
}

#endif
