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
#include <list>

namespace LibGoodBoy{

/*!
 * @class This class represents a neural network utilizing the GoodBoy learning
 *  technique.
 */
class GoodBoyNet{
    public:
        //Constructor & Destructor____________________
        /*!
         * @brief Constructs an empty GoodBoyNet
         *
         * @detail Constructs an empty GoodBoyNet with all the constant runtime
         *  parameters it needs.
         *
         * @param[in] p_outputFilterTaps UNUSED
         * @param[in] p_evolveFilterTaps UNUSED
         *
         * @param[in] p_degrFactor The degredation factor applied to all
         *  neurons. The higher the parameter, the less the network will retain.
         * @param[in] p_maxStartWeight The maximum valued weight a neuron's
         *  connection will hold.
         * @param[in] p_defaultAlpha The default alpha value any connection will
         *  start with. The higher this value, the longer new neurons will last.
         * @param[in] p_defaultGenerationFactor A value determining how likely
         *  the network is to generate new neurons. The higher the value, the
         *  more neurons will be generated at random.
         */
        GoodBoyNet(
                const std::vector<neuralVal_t>& p_outputFilterTaps,
                const std::vector<neuralVal_t>& p_evolveFilterTaps,

                neuralVal_t p_degrFactor,
                neuralVal_t p_maxStartWeight,
                neuralVal_t p_defaultAlpha,
                neuralVal_t p_defaultGenerationFactor,

                bool p_evolvingEnabled);
        /*!
         * @brief Constructs a GoodBoyNet based on a JSON
         *
         * @detail Constructs a GoodBoyNet as described by the inputted JSON.
         *
         * @param[in] p_json The JSON describing the network.
         */
        GoodBoyNet(json_t p_json);
        /*!
         * @brief Destructor for the GoodBoyNet class
         *
         * @detail Destructs a GoodBoyNet. There is no need for any cleanup
         *  before calling this function.
         */
        ~GoodBoyNet();

        //Runtime_____________________________________
        /*!
         * @brief Calculates the outputs of the network.
         *
         * @detail Given the updated inputs and last state of network,
         *  calculates the next output.
         */
        void Iter();
        /*!
         * @brief Evolves the network, creating neurons and changing
         *  connections.
         *
         * @detail This function changes the network On each call,
         *  the network creates new neurons and connects them based on previous
         *  activity. Connections are then changed based on the parameter.
         *
         * @param[in] p_amount Parameter determining the feedback from the
         *  environment. A positive value will retain connections, keeping
         *  current behavior. Negative feedback will degrade connections, aiding
         *  change in the network.
         */
        void Evolve(neuralVal_t p_amount);

        //Input/output handling_______________________
        /*!
         * @brief Creates input neurons in a group.
         *
         * @detail Creates an input group
         *
         * @param[in] p_groupName The name of the input group, used later to
         *  access the values.
         * @param[in] p_positions The positions of the input neurons in space.
         */
        void CreateInputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        /*!
         * @brief Appends new neurons to an existing input group.
         *
         * @detail Creates new input neurons and appends them to an existing
         *  input group.
         *
         * @param[in] p_groupName the name of the group that the neurons should
         *  be appended to.
         * @param[in] p_positions the positions of the new input neurons in
         *  space.
         */
        void AppendToInputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        /*!
         * @brief Sets the values of input neurons.
         *
         * @detail Sets the values of existing input neurons in a group to the
         *  values given. This is the way values are inputted into the network
         *  in real time.
         *
         * @param[in] p_groupName The name of the group to set the input values
         *  to.
         * @param[in] p_vals A vector containing the values to input to the
         *  neurons. The vector must have the same number of elements in it as
         *  neurons in the input group
         */
        void SetInputGroupValues(const std::string p_groupName,
                const std::vector<neuralVal_t>& p_vals);

        /*!
         * @brief Creates output neurons in a group.
         *
         * @detail Creates output neurons, being used to output the values of
         *  the network. The end results of the calculations in the network
         *  end up in these neurons.
         *
         * @param[in] p_groupName The name of the output group to which the
         *  neurons will be assigned.
         * @param[in] p_positions A vector containing the positions of the
         *  neurons in space.
         */
        void CreateOutputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        /*!
         * @brief Creates output neurons in a group.
         *
         * @detail Creates output neurons, being used to output the values of
         *  the network. The end results of the calculations in the network
         *  end up in these neurons.
         *
         * @param[in] p_groupName The name of the output group to which the
         *  neurons will be assigned.
         * @param[in] p_positions A vector containing the positions of the
         *  neurons in space.
         */
        void AppendToOutputGroup(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);

        /*!
         * @brief Retrives the output values of the network.
         *
         * @detail Given an output group, retrives the output values of the
         *  output neurons and copies them into a vector for use outside
         *  the network.
         *
         * @param[in] p_groupName The name of the output group to retrieve
         *  values from.
         * @param[out] p_outBuff A vector to copy the values to. This vector
         *  must have the same number of elements as the output group.
         */
        void GetOutputGroupValues(std::string p_groupName,
                std::vector<neuralVal_t>& p_outBuff) const;

        //Statistics__________________________________
        /*!
         * @brief Retrives the number of non input/output neurons.
         */
        neuralSize_t GetMidSize() const;

        //JSON________________________________________
        /*
         * @brief Returns a JSON representing the structure of the network.
         *
         * @detial Returns a JSON descibing all the neurons and connections in
         *  this network. The JSON also includes all parameters used to make
         *  this network.
         *
         * @return json_t representing the network itself.
         */
        json_t GetJSON() const;

        /*!
         * @brief Returns a JSON formatted string reperesnting the structure of
         *  the network.
         *
         * @detail Returns a string containing a JSON describing the
         *  neurons, connections and parameters dictating the behavior of this
         *  network.
         *
         * @return std::string containing the JSON describing the network.
         */
        std::string GetJSONString();

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

        //Runtime_____________________________________
        void adjustWeights(neuralVal_t p_amount);
        void cleanup(); //Erases all non-reachable neurons
        neuralSize_t numNeuronsToMake();
        void makeNewNeurons(neuralSize_t p_numNewNeurons);
        Neuron* getOutNeuron(neuralVal_t p_maxSelectionWeight);
        neuralVal_t calcOutNeuronWeight(Neuron* p_n);
        ConnectableNeuron* getRecvNeuron(Neuron* p_outNeuron);
        posscalar_t calcRecvNeuronWeight(
            const ConnectableNeuron* p_recvNeuron, 
            pos_t p_outPos);

        //Iterators___________________________________
        groupIterator<InputNeuron*> getInputNeuronIterator();
        groupIterator<ConnectableNeuron*> getOutputNeuronIterator();

        //Input/output________________________________
        void appendInputNeurons(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        void appendOutputNeurons(const std::string p_groupName,
                const std::vector<pos_t>& p_positions);
        ConnectableNeuron*  getOutputAtIndex(neuralSize_t p_index);
        neuralSize_t        getNumOutputs();
        InputNeuron* getInputAtIndex(neuralSize_t p_index);
        neuralSize_t        getNumInputs();

        //Members_____________________________________
        std::map<std::string, std::vector<InputNeuron*>>
            m_inputMap;
        std::map<std::string, std::vector<ConnectableNeuron*>>
            m_outputMap;

        std::list<ConnectableNeuron*> m_midNeurons;

        neuralConnectionPool_t m_connectionPool;
        connectableNeuronPool_t m_midNeuronPool;
        inputNeuronPool_t  m_inputNeuronPool;

        std::vector<neuralVal_t> m_outputFilterTaps;
        std::vector<neuralVal_t> m_evolveFilterTaps;

        neuralVal_t m_degrFactor;
        neuralVal_t m_maxStartWeight;
        neuralVal_t m_defaultAlpha;
        neuralVal_t m_generationFactor;
        bool m_evolvingEnabled;
};

}//End namespace

#endif
