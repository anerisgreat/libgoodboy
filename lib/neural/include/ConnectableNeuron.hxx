#ifndef NEURAL_CONNECTABLE_NEURON_HXX
#define NEURAL_CONNECTABLE_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"

#include <memory>

namespace LibGoodBoy
{
    class ConnectableNeuron : public Neuron
    {
        public:
            ConnectableNeuron(
                    const std::vector<neuralVal_t>& p_outputFilterTaps,
                    const std::vector<neuralVal_t>& p_evolveFilterTaps);

            ~ConnectableNeuron();

            //JSON GetJson()
        protected:

            struct NeuralConnection
            {
                public:
                    std::shared_ptr<Neuron> ConnectedNeuron;

                    neuralVal_t Weight;
                    neuralVal_t Alpha;

                    NeuralConnection(
                        std::shared_ptr<Neuron>& p_connectedNeuron,
                        neuralVal_t p_weight,
                        neuralVal_t p_alpha)
                        :
                            Weight(p_weight),
                            Alpha(p_alpha),
                            ConnectedNeuron(
                                    std::shared_ptr<Neuron>(p_connectedNeuron))
                    {
                    }

            }

            neuralVal_t calcOutput();

            void postBackProbe();
            void postForwardProbe();
            
            void evolveSelf(neuralVal_t p_amount);
            
            void postPurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& p_toPurge);

            void postFlush();
        private:
    };
}

#endif
