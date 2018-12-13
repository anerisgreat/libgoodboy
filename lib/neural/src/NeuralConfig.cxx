#include "NeuralConfig.hxx"

namespace LibGoodBoy{
    const std::string JSON_UID_KEY = "uid";
    const std::string JSON_OUTP_CONN_KEY = "output_connections";
    const std::string JSON_INP_CONN_KEY = "input_connections";

    const std::string JSON_NEURON_TYPE_KEY = "neuron_type";
    const std::string JSON_NEURON_TYPE_INPUT_VAL = "input_neuron";
    const std::string JSON_NEURON_TYPE_CONNECTABLE_VAL = "connectable_neuron";

    const std::string JSON_CONNECTION_WEIGHT_KEY = "weight";
    const std::string JSON_CONNECTION_ALPHA_KEY = "alpha";

    const std::string NET_INP_KEY = "inputs";
    const std::string NET_OUT_KEY = "outputs";
    const std::string NET_MID_KEY = "mid_neurons";
    const std::string NET_DEGR_FACTOR_KEY = "degredation_factor";
    const std::string NET_MAX_START_WEIGHT_KEY = "maximum_start_weight";
    const std::string NET_DEFAULT_ALPHA = "default_alpha";
    const std::string NET_GEN_FACTOR = "generation_factor";
    const std::string NET_EVOLVE_ENABLED = "evolve_enabled";
    const std::string NET_OUT_FILT_TAPS_KEY = "output_filter_taps";
    const std::string NET_EVOLVE_FILT_TAPS_KEY = "input_filter_taps";
}
