#ifndef NEURAL_UTILS_HXX
#define NEURAL_UTILS_HXX

#include "libGoodBoyConfig.hxx"
#include <type_traits>
#include <cstdlib>

namespace LibGoodBoy{
    template<class T> inline T RandInRange(T min, T max){
        static_assert(
                std::is_arithmetic<T>::value,
                "Value must be arithmetic!");
        return min + static_cast <T> (std::rand()) 
            / ( static_cast <T> (RAND_MAX/(max-min)));
    }

    template<class T> inline T Sigmoid(T p_a){
        static_assert(
                std::is_floating_point<T>::value,
                "Value must be floating point!");
        return p_a / (1 + abs(p_a));
    }

    template<class T> inline T ReLU(T p_a){
        static_assert(
                std::is_floating_point<T>::value,
                "Value must be floating point!");
        if(p_a < 0){
            return 0;
        }
        return p_a;

    template<class T> inline T ApproxSigmoid(T p_a){
        static_assert(
                std::is_floating_point<T>::value,
                "Value must be floating point!");
        if(p_a > 1){
            return 1;
        }else if(p_a < -1){
            return -1;
        }else{
            return p_a;
        }
    }
}

#endif
