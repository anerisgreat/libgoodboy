#include "libGoodBoyConfig.h"

namespace PROJECT_NAMESPACE
{
    class InputNeuron : public Neuron
    {
        public:
            Neuron();
            ~Neuron();

            float GetOutput(unsigned char nIter);
            void Endorphinize(float val) = 0;

            String GetJson() = 0;

        protected:
            OUTP_TYPE calcOutput();
        private:
            OUTP_TYPE calcOutput();
            OUTP_TYPE mLatestOutputss;
    };
}
