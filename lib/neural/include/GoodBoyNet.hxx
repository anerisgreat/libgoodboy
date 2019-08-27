#ifndef NEURAL_GOOD_BOY_NET_HXX
#define NEURAL_GOOD_BOY_NET_HXX

#include "libGoodBoyConfig.hxx"
#include "NeuralGlobals.hxx"
#include "Neuron.hxx"
#include "ConnectableNeuron.hxx" 
#include "InputNeuron.hxx"
#include "InputNeuronPool.hxx"
#include "ConnectableNeuronPool.hxx"

#include <vector>
#include <memory>
#include <list>

namespace LibGoodBoy{

class GoodBoyNet{
    public:
        //Constructor & Destructor____________________
        GoodBoyNet(
                const std::vector<neuralVal_t>& p_outputFilterTaps,
                const std::vector<neuralVal_t>& p_evolveFilterTaps,

                neuralVal_t p_degrFactor,
                neuralVal_t p_maxStartWeight,
                neuralVal_t p_defaultAlpha,
                neuralVal_t p_defaultGenerationFactor,

                bool p_evolvingEnabled);
        ~GoodBoyNet();

        //Runtime_____________________________________
        void Iter();

        //Input/output handling_______________________
        void CreateInputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        void AppendToInputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        void SetInputGroupValues(const std::string p_groupName,
                const std::vector<neuralVal_t>& p_vals);

        void CreateOutputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        void AppendToOutputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        void GetOutputGroupValues(std::string p_groupName,
                std::vector<neuralVal_t>& p_outBuff) const;

        //Statistics__________________________________
        neuralSize_t GetMidSize() const;

        //JSON________________________________________
        json_t GetJSON() const;
        std::string GetJSONString();

        void Evolve(neuralVal_t p_amount);

    private:
        //Helper_classes______________________________
        template <class T> class groupIterator{
            typedef typename std::vector<T>                     groupVec_t;
            typedef typename groupVec_t::iterator               groupVecIter_t;
            typedef typename std::map<std::string, groupVec_t>  groupMap_t;
            typedef typename groupMap_t::iterator               groupMapIter_t;
            private:
                //Members
                groupVecIter_t  m_vecIter;
                groupMapIter_t  m_mapIter;
                groupMap_t*     m_map;

                //Methods
                void incrementIterator(){
                    if(m_mapIter != m_map->end()){
                        ++m_vecIter;
                        if(m_vecIter == m_mapIter->second.end()){
                            ++m_mapIter;
                            if(m_mapIter != m_map->end()){
                                m_vecIter = m_mapIter->second.begin();
                            }
                        }
                    }
                }

            protected:
                //Protected getters/setters
                groupMapIter_t GetMapIter() const{return m_mapIter;}
                groupVecIter_t GetVecIter() const{return m_vecIter;}
                groupMap_t GetMap() const{return m_map;}

            public:
                //Constructors
                groupIterator(groupMap_t& p_map) : m_map(&p_map) {
                    m_mapIter = p_map.begin();
                    if(m_mapIter != p_map.end()){
                        m_vecIter = m_mapIter->second.begin();
                    }
                }

                groupIterator(const groupIterator& p_other) :
                    m_mapIter(p_other.GetMapIter()),
                    m_vecIter(p_other.GetVecIter()),
                    m_map(p_other.GetMap())
                {}

                //Public methods
                bool IsEnd(){return m_mapIter == m_map->end();}

                //Operators
                groupIterator& operator++(){
                    incrementIterator();
                    return *this;
                }
                bool operator == (const groupIterator& p_other) const{
                    return this->m_mapIter == p_other.GetMapIter() && \
                        ( (this->m_mapIter == m_map->end()) || \
                        (this->m_vecIter == p_other.GetVecIter()) );
                }
                bool operator != (const groupIterator& p_other) const{
                    return !(this->m_mapIter == p_other.GetMapIter() && \
                        ((this->m_mapIter == m_map->end()) || \
                        (this->m_vecIter == p_other.GetVecIter())));
                }
                T& operator*() {return *m_vecIter;}
        };

        //Methods_____________________________________
        void adjustWeights(neuralVal_t p_amount);
        void cleanup(); //Erases all non-reachable neurons

        groupIterator<InputNeuron*> getInputNeuronIterator();
        groupIterator<ConnectableNeuron*> getOutputNeuronIterator();

        neuralSize_t numNeuronsToMake();
        void makeNewNeurons(neuralSize_t p_numNewNeurons);

        void appendInputNeurons(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        void appendOutputNeurons(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        Neuron* getOutNeuron(neuralVal_t p_maxSelectionWeight);
        neuralVal_t calcOutNeuronWeight(Neuron* p_n);
        ConnectableNeuron* getRecvNeuron(Neuron* p_outNeuron);
        posscalar_t calcRecvNeuronWeight(
            const ConnectableNeuron* p_recvNeuron, 
            pos_t p_outPos);

        ConnectableNeuron*  getOutputAtIndex(neuralSize_t p_index);
        neuralSize_t        getNumOutputs();
        InputNeuron* getInputAtIndex(neuralSize_t p_index);
        neuralSize_t        getNumInputs();

        void calcOutputs();

        //Members_____________________________________
        std::map<std::string, std::vector<InputNeuron*>>
            m_inputMap;
        std::map<std::string, std::vector<ConnectableNeuron*>>
            m_outputMap;

        std::list<ConnectableNeuron*> m_midNeurons;

        neuralConnectionPool_t m_connectionPool;
        connectableNeuronPool_t m_midNeuronPool;
        inputNeuronPool_t  m_inputNeuronPool;

        const std::vector<neuralVal_t> m_outputFilterTaps;
        const std::vector<neuralVal_t> m_evolveFilterTaps;

        neuralVal_t m_degrFactor;
        neuralVal_t m_maxStartWeight;
        neuralVal_t m_defaultAlpha;
        neuralVal_t m_generationFactor;
        bool m_evolvingEnabled;
};

}//End namespace

#endif
