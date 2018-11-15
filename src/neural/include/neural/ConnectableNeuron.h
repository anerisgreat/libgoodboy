#include "libGoodBoyConfig.h"
#include "neural/Neuron.h"
#include "neural/NeuralConfig.h"

namespace PROJECT_NAMESPACE
{
    virtual class Connectable : virtual public Neuron
    {
        public:
            ConnectableNeuron(/*PREFILT_SHIFTREG,
                              FILTER,
                              POSTFILT_SHIFTREG,
                              ENDORPH_TAPS*/);
            ~ConnectableNeuron();

            void Connect(Neuron* neuronToConnect);

        protected:
            t_neural_val calcOutput();
            void postResetOutput();
            void postResetProbe();
            void endorphinizeSelf(t_neural_val amount);
            void postFlush();
        private:
            /*vector<NEURAL_CONNECTION>*/ m_input_connections_vec;
    }
}
