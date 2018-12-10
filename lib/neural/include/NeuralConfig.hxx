#ifndef NEURAL_CONFIG_HXX
#define NEURAL_CONFIG_HXX

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>
#include <vector>

namespace LibGoodBoy{
    typedef float neuralVal_t;
    typedef std::size_t neuralSize_t;
    typedef nlohmann::json json_t;
    typedef boost::uuids::uuid uuid_t;

    extern const std::string JSON_UID_KEY;
    extern const std::string JSON_OUTP_CONN_KEY;
    extern const std::string JSON_INP_CONN_KEY;

    extern const std::string JSON_NEURON_TYPE_KEY;
    extern const std::string JSON_NEURON_TYPE_INPUT_VAL;
    extern const std::string JSON_NEURON_TYPE_CONNECTABLE_VAL;

    extern const std::string JSON_CONNECTION_WEIGHT_KEY;
    extern const std::string JSON_CONNECTION_ALPHA_KEY;

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
