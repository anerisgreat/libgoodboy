#ifndef INSTANCE_FACTORY_HXX
#define INSTANCE_FACTORY_HXX

#include "libGoodBoyConfig.hxx"

#include <memory>

namespace LibGoodBoy
{
    template<class T> class InstanceFactory
    {
        public:
            virtual std::shared_ptr<T> MakeNewInstance() = 0;
    };
}

#endif
