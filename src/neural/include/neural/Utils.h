#pragma once

#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"

namespace PROJECT_NAMESPACE
{
    class InputNeuron : public Neuron
    {
        public:
            InputNeuron();
            ~InputNeuron();

            void SetOutput(OUTP_TYPE next_input);
            
            //JSON GetJson()
        protected:
            OUTP_TYPE calcOutput();
            void postResetOutput();
            void postRestProbe();
            void endorphinizeSelf();
            void postFlush();
        private:
            OUTP_TYPE calcOutput();
            OUTP_TYPE mLatestOutputss;
    };
}
