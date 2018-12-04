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
                    neuralVal_t p_degrFactor);

            ~ConnectableNeuron();

            void Connect(std::shared_ptr<Neuron>& p_toConnect);

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
            std::list<std::weak_ptr<NeuralConnection>> m_inConnectionList;
            ObjectPool<NeuralConnection>& m_connectionPool;

            neuralVal_t m_degrFactor;

            static inline neuralVal_t sigmoid(neuralVal_t p_a);
            void releaseAllConnections();
            void removeConnection(
                    const std::shared_ptr<NeuralConnection> p_toRemove);
    };
}

#endif