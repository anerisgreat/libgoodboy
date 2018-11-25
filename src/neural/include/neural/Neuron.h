#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"

#include <vector>

namespace PROJECT_NAMESPACE
{
    virtual class Neuron
    {
        public:
            Neuron(
                    
                    unsigned int fitlers_len);
            ~Neuron();

            t_neural_val GetOutput();
            void ResetOutputFlag();

            void Endorphinize(t_neural_val amount);
            void ResetEndorphiniztionFlag();
            t_neural_val CalcContribution();

            void BackProbe();
            void ForwardProbe();
            bool GetWasFullyProbed();
            void ResetProbeFlag();

            void Flush();
            void RemoveAllConnections();
            
            void OnConnectedToOutput();
            void OnRemovedFromOutput();

            boost::uuids:uuid GetUUID();

            //JSON GetJson();
            //STRING ToString();
        protected:
            virtual t_neural_val calcOutput() = 0;
            t_neural_val m_lastOutput;

            //Consider why this exists
            virtual void postResetOutput() = 0;
            virtual void postResetProbe() = 0;
            virtual void endorphinizeSelf(t_neural_val amount) = 0;
            virtual void postFlush() = 0;

        private:
            void reset();

            bool m_checkedOutputFlag;
            bool m_forwardProbedFlag;
            bool m_backwardProbedFlag;
            bool m_endorphinizationFlag;

            boost::circular_buffer<t_neural_val> m_output_pre_filter_buffer;
            boost::circular_buffer<t_neural_val> m_output_post_filter_buffer;
            unsigned int m_filters_len;
            t_neural_val* m_output_filter_taps;
            t_neural_val* m_endorphinization_filter_taps;

            boost::uuids:uuid m_id;
            vetor<NEURAL_CONNECTIONS> m_output_connections_vec 
    };
}
