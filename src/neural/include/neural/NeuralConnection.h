#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"

namespace PROJECT_NAMESPACE
{
    virtual class NeuralConnection
    {
        public:
            NeuralConnection(Neuron* neuronToConnect, Neuron* parent);
            ~NeuralConnection();

            void Endorphinize(t_neural_val amount);
            t_neural_val GetWeightedOutput();
        private:
            t_neural_val m_weight;
            t_neural_val inertia;
    }
}
