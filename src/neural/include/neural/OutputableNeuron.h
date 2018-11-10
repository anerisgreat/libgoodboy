#include "libGoodBoyConfig.h"
#include "neural/Neuron.h"
#include "neural/NeuralConfig.h"

namespace PROJECT_NAMESPACE
{
    virtual class OutputableNeuron : virtual public Neuron
    {
        public:
            ConnectedNeuron(/*PREFILT_SHIFTREG,
                              FILTER,
                              POSTFILT_SHIFTREG,
                              ENDORPH_TAPS*/);
            ~ConnectedNeuron();

            void Connect(Neuron* neuronToConnect);

        protected:
            OUTP_TYPE calcOutput();
            void postResetOutput();
            void postResetProbe();
            void endorphinizeSelf();
            void postFlush();
        private:
            /*vector<NEURAL_CONNECTION>*/ m_input_connections_vec;
    }
}
