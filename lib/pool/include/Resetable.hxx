#ifndef NEURAL_FLUSHABLE_HXX
#define NEURAL_FLUSHABLE_HXX

#include "libGoodBoyConfig.hxx"

namespace LibGoodBoy
{
    template <class T> class Resetable
    {
        public:
            virtual void Flush() = 0;
    }
}

#endif
