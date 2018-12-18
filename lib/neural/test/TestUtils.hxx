#include "NeuralConfig.hxx"

namespace LibGoodBoy{
    namespace Test{
        std::vector<neuralVal_t> GetConnectionFilter(){
            std::vector<neuralVal_t> connFilt;
            connFilt.push_back(0.1);
            connFilt.push_back(0.2);
            connFilt.push_back(0.4);
            connFilt.push_back(0.2);
            connFilt.push_back(0.1);

            return connFilt;
        }

        std::vector<neuralVal_t> GetEvFilter(){
            std::vector<neuralVal_t> evFilt;
            for(char i = 2; i < 100; i++){
                evFilt.push_back(2/((float)i*(float)i));
            }

            return evFilt;
        }

        const static neuralVal_t DEFAULT_DEGR_FACTOR = 0.1;
        const static neuralVal_t DEFAULT_MAX_START_WEIGHT = 1;
        const static neuralVal_t DEFAULT_DEFAULT_ALPHA = 2;
    }
}
