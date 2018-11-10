#include "libGoodBoyConfig.h"

namespace PROJECT_NAMESPACE
{
    class NeuralConnection
    {
        public:
            Neuron();
            ~Neuron();

            float GetOutput(unsigned char nIter);
            void Endorphinize(float val) = 0;

            String GetJson() = 0;

        private:
            virtual float calcOutput(nIter) = 0;
    };
}
