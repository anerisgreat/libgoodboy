#ifndef NEURAL_NEURON_HXX
#define NEURAL_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Resetable.hxx"

#include <vector>
#include <memory>
#include <list>

#include <boost/circular_buffer.hpp>
#include <boost/uuid/uuid.hpp>

namespace LibGoodBoy
{
    class Neuron : public Resetable, public std::enable_shared_from_this<Neuron>
    {
        public:
            //Constructor & Destructor____________________
            Neuron( const std::vector<neuralVal_t>& p_outputFilterTaps,
                    const std::vector<neuralVal_t>& p_evolveFilterTaps);
            ~Neuron();

            //Output______________________________________
            neuralVal_t GetOutput();
            void ResetOutputFlag();

            //Evolving____________________________________
            void Evolve(neuralVal_t p_amount);
            void ResetEvolveFlag();
            neuralVal_t GetContribution();
            
            //Connection Management_______________________
            void PurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& p_toPurge);

            void OnConnectedToOutput(
                    const std::shared_ptr<Neuron>& p_connected);
            void OnRemovedFromOutput(
                    const std::shared_ptr<Neuron>& p_removed);

            //Probe_______________________________________
            void BackProbe();
            void ForwardProbe();
            bool GetWasFullyProbed() const;
            void ResetProbeFlag();

            //Reseting____________________________________
            void Reset();

            //Properties__________________________________
            boost::uuids::uuid GetUUID();
            //JSON GetJson();
            //STRING ToString();
        protected:
            virtual neuralVal_t calcOutput() = 0;

            virtual void postBackProbe() = 0;
            virtual void postForwardProbe() = 0;
            
            virtual void evolveSelf(neuralVal_t p_amount) = 0;
            
            virtual void postPurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& p_toPurge) = 0;

            virtual void postReset() = 0;

            neuralVal_t tapsCircBuffInner(
                    const std::vector<neuralVal_t>& p_taps,
                    const boost::circular_buffer<neuralVal_t>& p_samps) const;

        private:
            bool m_checkedOutputFlag;
            bool m_forwardProbedFlag;
            bool m_backwardProbedFlag;
            bool m_evolveFlag;
            bool m_contributionFlag;

            neuralVal_t m_lastContribution;

            std::list<std::weak_ptr<Neuron>> m_outputConnectionsList;

            boost::circular_buffer<neuralVal_t> m_outputPreFilterBuffer;
            boost::circular_buffer<neuralVal_t> m_outputPostFilterBuffer;

            const std::vector<neuralVal_t>& m_outputFilterTaps;
            const std::vector<neuralVal_t>& m_evolveFilterTaps;

            boost::uuids::uuid m_id;
    };
}

#endif