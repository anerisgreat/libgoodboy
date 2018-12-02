#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"

#include <vector>

#include <boost/circular_buffer.hpp>
#include <boost/uuid/uuid.hpp>


namespace PROJECT_NAMESPACE
{
    virtual class Neuron
    {
        public:
            //Constructor & Destructor____________________
            Neuron( std::vector<t_neural_val>* output_filter_taps_ptr,
                    std::vector<t_neural_val>* endorphinization_filter_taps_ptr
                    );
            ~Neuron();

            //Output______________________________________
            t_neural_val GetOutput();
            void ResetOutputFlag();

            //Endorphinization____________________________
            void Endorphinize(t_neural_val amount);
            void ResetEndorphiniztionFlag();
            t_neural_val CalcContribution();
            
            //Connection Management_______________________
            void PurgeFromOutput(std::vector<Neuron*>* neurons_to_purge);
            void OnConnectedToOutput(Neuron* connected);
            void OnRemovedFromOutput(Neuron* removed);

            //Probe_______________________________________
            void BackProbe();
            void ForwardProbe();
            bool GetWasFullyProbed();
            void ResetProbeFlag();

            //Flushing____________________________________
            void Flush();

            //Getting Properties__________________________
            boost::uuids:uuid GetUUID();
            //JSON GetJson();
            //STRING ToString();
        protected:
            virtual t_neural_val calcOutput() = 0;
            t_neural_val m_lastOutput;

            //Consider why this exists
            virtual void postBackProbe() = 0;
            virtual void postResetProbe() = 0;
            virtual void endorphinizeSelf(t_neural_val amount) = 0;
            virtual void postFlush() = 0;

            t_neural_val taps_circ_buff_inner(
                    std::vector<t_neural_val>* taps,
                    boost:circular_buffer<t_neural_val>* samps);

        private:
            bool m_checkedOutputFlag;
            bool m_forwardProbedFlag;
            bool m_backwardProbedFlag;
            bool m_endorphinizationFlag;

            boost::circular_buffer<t_neural_val> m_output_pre_filter_buffer;
            boost::circular_buffer<t_neural_val> m_output_post_filter_buffer;

            std::vector<t_neural_val>* m_output_filter_taps_ptr;
            std::vector<t_neural_val>* m_endorphinization_filter_taps_ptr;

            boost::uuids:uuid m_id;
            vector<Neuron*> m_output_connections_vec;
    };
}
