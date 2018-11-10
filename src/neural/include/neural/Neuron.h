#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"
namespace PROJECT_NAMESPACE
{
    virtual class Neuron
    {
        public:
            Neuron(/*PREFILT_SHIFTREG, 
                     FILTER, 
                     POSTFILT_SHIFTREG, 
                     ENDORPH_TAPS*/);
            ~Neuron();

            OUTP_TYPE GetOutput();
            void ResetOutputFlag();

            void Endorphinize(float val);
            void ResetEndorphiniztionFlag();

            void BackProbe();
            void ForwardProbe();
            bool GetWasFullyProbed();
            void ResetProbeFlag();

            void Flush();
            //NEURON_ID_TYPE GetUUID();
            //JSON GetJson() = 0;
            //STRING ToString();
        protected:
            virtual float calcOutput() = 0;

            virtual void postResetOutput() = 0;
            virtual void postResetProbe() = 0;
            virtual void endorphinizeSelf() = 0;
            virtual void postFlush() = 0;

            bool calcContribution();
        private:
            bool m_checkedOutputFlag;
            bool m_forwardProbedFlag;
            bool m_backwardProbedFlag;
            bool m_checkedEndorphinizationFlag;

            //NEURON_ID_TYPE UUID
            
            //VECTOR<NEURAL_CONNECTIONS> m_output_connections_vec 

            //FIR_TAPS m_output_filter_taps;
            //SHIFTREG m_output_shiftreg;

            //FIR_TAPS m_endorphinization_taps;
            //SHIFTREG m_output_history_shiftreg;
    };
}
