#ifndef NEURAL_CONFIG_HXX
#define NEURAL_CONFIG_HXX

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>

namespace LibGoodBoy{
    typedef float neuralVal_t;
    typedef nlohmann::json json_t;
    typedef boost::uuids::uuid uuid_t;

    extern const std::string JSON_UID_KEY;
    extern const std::string JSON_OUTP_CONN_KEY;

    extern const std::string JSON_NEURON_TYPE_KEY;
    extern const std::string JSON_NEURON_TYPE_INPUT_VAL;

    extern const std::string JSON_CONNECTION_WEIGHT_KEY;
    extern const std::string JSON_CONNECTION_ALPHA_KEY;
}

#endif
