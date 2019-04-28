#ifndef NEURAL_CONFIG_HXX
#define NEURAL_CONFIG_HXX

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>
#include <vector>
#include <valarray>

namespace LibGoodBoy{
    //TYPEDEFS
    typedef float neuralVal_t;
    typedef std::size_t neuralSize_t;
    typedef nlohmann::json json_t;
    typedef boost::uuids::uuid uuid_t;
    typedef double posscalar_t;
    typedef std::valarray<posscalar_t> pos_t;

    //METHODS

    template<class T> inline T RandInRange(T min, T max);
    template<class T> inline T Sigmoid(T p_a);
    template<class T> inline T ReLU(T p_a);
    template<class T> inline T ApproxSigmoid(T p_a);

    extern posscalar_t GetDistance(const pos_t& p_ca, const pos_t& p_cb);
    extern pos_t GetAveragePosition(const pos_t& p_ca, const pos_t& p_cb);

    //CONSTS
    extern const std::string JSON_UID_KEY;
    extern const std::string JSON_OUTP_CONN_KEY;
    extern const std::string JSON_INP_CONN_KEY;

    extern const std::string JSON_NEURON_TYPE_KEY;
    extern const std::string JSON_NEURON_TYPE_INPUT_VAL;
    extern const std::string JSON_NEURON_TYPE_CONNECTABLE_VAL;

    extern const std::string JSON_CONNECTION_WEIGHT_KEY;
    extern const std::string JSON_CONNECTION_ALPHA_KEY;

    extern const std::string JSON_POS_KEY;

    extern const std::string NET_INP_KEY;
    extern const std::string NET_OUT_KEY;
    extern const std::string NET_MID_KEY;
    extern const std::string NET_DEGR_FACTOR_KEY;
    extern const std::string NET_MAX_START_WEIGHT_KEY;
    extern const std::string NET_DEFAULT_ALPHA;
    extern const std::string NET_GEN_FACTOR;
    extern const std::string NET_EVOLVE_ENABLED;
    extern const std::string NET_OUT_FILT_TAPS_KEY;
    extern const std::string NET_EVOLVE_FILT_TAPS_KEY;
}

#endif
