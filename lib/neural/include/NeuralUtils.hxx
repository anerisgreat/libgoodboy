#ifndef NEURAL_UTILS_HXX
#define NEURAL_UTILS_HXX

#include "libGoodBoyConfig.hxx"
#include <type_traits>

namespace LibGoodBoy{
    template<class T> inline T RandInRange(T min, T max){
        static_assert(
                std::is_arithmetic<T>::value,
                "Value must be arithmetic!");
        return min + static_cast <T> (rand()) 
            / ( static_cast <T> (RAND_MAX/(max-min)));
    }
    
    template<class T> inline T Sigmoid(T p_a){
        static_assert(
                std::is_floating_point<T>::value,
                "Value must be floating point!");
        return p_a / (p_a + abs(p_a));
    }
}

#endif
