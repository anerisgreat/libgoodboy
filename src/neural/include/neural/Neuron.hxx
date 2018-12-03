#ifndef NEURAL_NEURON_HXX
#define NEURAL_NEURON_HXX

#include "libGoodBoyConfig.hxx"
#include "neural/NeuralConfig.hxx"

#include <vector>
#include <memory>
#include <list>

#include <boost/circular_buffer.hpp>
#include <boost/uuid/uuid.hpp>

namespace lib_good_boy::neural
{
    class Neuron
    {
        public:
            //Constructor & Destructor____________________
            Neuron( const std::vector<neuralVal_t>& outputFilterTaps,
                    const std::vector<neuralVal_t>& evolveFilterTaps);
            ~Neuron();

            //Output______________________________________
            neuralVal_t GetOutput();
            void ResetOutputFlag();

            //Evolving____________________________________
            void Evolve(neuralVal_t amount);
            void ResetEvolveFlag();
            neuralVal_t GetContribution() const;
            
            //Connection Management_______________________
            void PurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& toPurge);

            void OnConnectedToOutput(const std::shared_ptr<Neuron> connected);
            void OnRemovedFromOutput(const std::shared_ptr<Neuron> removed);

            //Probe_______________________________________
            void BackProbe();
            void ForwardProbe();
            bool GetWasFullyProbed() const;
            void ResetProbeFlag();

            //Flushing____________________________________
            void Flush();

            //Properties__________________________________
            boost::uuids::uuid GetUUID();
            //JSON GetJson();
            //STRING ToString();
        protected:
            virtual neuralVal_t calcOutput() = 0;

            virtual void postBackProbe() = 0;
            virtual void postForwardProbe() = 0;
            
            virtual void evolveSelf(neuralVal_t amount) = 0;
            
            virtual void postPurgeConnections(
                    const std::list<std::shared_ptr<Neuron>>& toPurge) = 0;

            virtual void postFlush() = 0;

            neuralVal_t tapsCircBuffInner(
                    const std::vector<neuralVal_t>& taps,
                    const boost::circular_buffer<neuralVal_t>& samps) const;

        private:
            bool m_checkedOutputFlag;
            bool m_forwardProbedFlag;
            bool m_backwardProbedFlag;
            bool m_evolveFlag;

            std::list<std::shared_ptr<Neuron>> m_outputConnectionsList;

            boost::circular_buffer<neuralVal_t> m_outputPreFilterBuffer;
            boost::circular_buffer<neuralVal_t> m_outputPostFilterBuffer;

            const std::vector<neuralVal_t>& m_outputFilterTaps;
            const std::vector<neuralVal_t>& m_evolveFilterTaps;

            boost::uuids::uuid m_id;
    };
}

#endif
