#ifndef NEURAL_NEURON_HXX
#define NEURAL_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralGlobals.hxx"
#include "Resetable.hxx"

#include <vector>
#include <memory>
#include <list>

namespace LibGoodBoy
{
    class Neuron : public Resetable
    {
        public:
            //Constructor & Destructor____________________
            Neuron();
            ~Neuron();

            //Output______________________________________
            neuralVal_t GetOutput();
            void ResetOutputFlag();

            //Evolving____________________________________
            virtual void Evolve(neuralVal_t p_amount) = 0;
            neuralVal_t GetContribution();
            void ResetContributionFlag();

            neuralVal_t GetOutputSum();
            void ResetOutputSum();

            //Connection Management_______________________
            virtual void PurgeConnections(const std::list<Neuron*>& p_toPurge);

            void OnConnectedToOutput(Neuron* p_connected);
            void OnRemovedFromOutput(Neuron* p_removed);

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
            std::string jsonString();

            pos_t GetPosition() const;
            void SetPosition(pos_t p_pos);
            static posscalar_t GetNeuronDistance(const Neuron& p_a,
                    const Neuron& p_b);
            static pos_t GetNeuronAveragePosition(const Neuron& p_a,
                    const Neuron& p_b);
        protected:
            virtual neuralVal_t calcOutput() = 0;

            virtual void postBackProbe() = 0;
            virtual void postForwardProbe() = 0;

        private:
            bool m_checkedOutputFlag;
            bool m_forwardProbedFlag;
            bool m_backwardProbedFlag;
            bool m_evolveFlag;
            bool m_contributionFlag;

            neuralVal_t m_lastOutput;
            neuralVal_t m_lastContribution;
            neuralVal_t m_outputSum;

            std::list<Neuron*> m_outputConnectionsList;

            pos_t m_pos;

            const std::vector<neuralVal_t> m_outputFilterTaps;
            const std::vector<neuralVal_t> m_evolveFilterTaps;

            std::size_t m_evolveFilterLen;

            uuid_t m_uid;
    };
}

#endif
