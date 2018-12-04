#ifndef NEURAL_FLUSHABLE_HXX
#define NEURAL_FLUSHABLE_HXX

#include "libGoodBoyConfig.hxx"

namespace LibGoodBoy
{
    class Resetable
    {
        public:
            virtual void Reset() = 0;
    };
}

#endif
