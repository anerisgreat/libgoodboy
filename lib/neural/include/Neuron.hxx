#ifndef NEURAL_NEURON_HXX
#define NEURAL_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralConfig.hxx"
#include "Resetable.hxx"

#include <vector>
#include <memory>
#include <list>

#include <boost/circular_buffer.hpp>

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
            virtual void Evolve(neuralVal_t p_amount) = 0;
            neuralVal_t GetContribution();
            void ResetContributionFlag();

            //Connection Management_______________________
            virtual void PurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& p_toPurge);

            void OnConnectedToOutput(
                    const std::shared_ptr<Neuron>& p_connected);
            void OnRemovedFromOutput(
                    const std::shared_ptr<Neuron>& p_removed);

            //Probe_______________________________________
            void BackProbe();
            void ForwardProbe();
            bool GetWasFullyProbed() const;
            virtual void ResetProbeFlag();

            //Reseting____________________________________
            virtual void Reset();

            //Properties__________________________________
            uuid_t GetUID();
            virtual json_t GetJSON() const;
            //STRING ToString();
        protected:
            virtual neuralVal_t calcOutput() = 0;

            virtual void postBackProbe() = 0;
            virtual void postForwardProbe() = 0;

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

            std::size_t m_evolveFilterLen;

            uuid_t m_uid;
    };
}

#endif
