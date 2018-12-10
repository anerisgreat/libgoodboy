#ifndef NEURAL_CONNECTABLE_NEURON_HXX
#define NEURAL_CONNECTABLE_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Neuron.hxx"
#include "NeuralConnection.hxx"
#include "ObjectPool.hxx"

#include <memory>
#include <list>

namespace LibGoodBoy
{
    class ConnectableNeuron : public Neuron
    {
        public:
            ConnectableNeuron(
                    const std::vector<neuralVal_t>& p_outputFilterTaps,
                    const std::vector<neuralVal_t>& p_evolveFilterTaps,
                    ObjectPool<NeuralConnection>& p_connectionPool,
                    neuralVal_t p_degrFactor,
                    neuralVal_t p_maxStartWeight,
                    neuralVal_t p_defaultAlpha);

            ~ConnectableNeuron();

            void Evolve(neuralVal_t p_amount);

            void PurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& p_toPurge);

            void Reset();

            json_t GetJSON() const;

            void Connect(std::shared_ptr<Neuron>& p_toConnect);
            void Connect(std::shared_ptr<Neuron>& p_toConnect,
                    neuralVal_t p_weight,
                    neuralVal_t p_alpha);
        protected:

            neuralVal_t calcOutput();

            void postBackProbe();
            void postForwardProbe();

        private:
            std::list<std::weak_ptr<NeuralConnection>> m_inConnectionList;
            ObjectPool<NeuralConnection>& m_connectionPool;

            neuralVal_t m_degrFactor;
            neuralVal_t m_maxStartWeight;
            neuralVal_t m_defaultAlpha;

            void releaseAllInputs();
    };
}

#endif
