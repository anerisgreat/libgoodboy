#include "libGoodBoyConfig.h"
#include "neural/NeuralConfig.h"

namespace PROJECT_NAMESPACE
{
    class NeuralNet
    {  
        public:
            Neuralnet();
            //Neuralnet(JSON);
            ~NeuralNet();

            void SetInput(unsigned int n_input, t_neural_val input_val);
            t_neural_val GetOutput(unsigned int n_input);

            void RunIter();

            //JSON ToJson()
            //
            

    }
}
