#ifndef NEURAL_OBJECT_POOL_HXX
#define NEURAL_OBJECT_POOL_HXX

#include "libGoodBoyConfig.hxx"
#include "Resetable.hxx"
#include "InstanceFactory.hxx"

#include <memory>
#include <vector>
#include <tuple>

namespace LibGoodBoy{
    template <class T, typename... T_args> class ObjectPool{
        static_assert(
            (std::is_base_of<Resetable, T>::value),
            "T in ObjectPool must be a descendant of LibGoodBoy::Resetable"
        );

        private:
            template<size_t... Is> struct index_sequence;
            using BoolPtrPair = std::tuple<bool, std::shared_ptr<T>>;

            std::tuple<T_args...> m_argTuple;
            std::vector<BoolPtrPair> m_pool;
            typename std::vector<BoolPtrPair>::iterator m_iter;

            template<std::size_t... Is>
            BoolPtrPair makeNewElement(std::index_sequence<Is...>){
                T* newMember = new T(std::get<Is>(m_argTuple)...);
                return std::make_tuple<bool, std::shared_ptr<T>>(
                        false, 
                        std::make_shared<T>(std::get<Is>(m_argTuple)...));
            }

            BoolPtrPair makeNewElement(){
                return makeNewElement(std::index_sequence_for<T_args...>());
            }

        public:
            ObjectPool(T_args... p_args)
                :
                    m_pool(std::vector<BoolPtrPair>()),
                    m_argTuple(std::make_tuple<T_args...>(p_args...)),
                    m_iter(m_pool.begin())
            {}

            ~ObjectPool();

            std::shared_ptr<T> AllocElement(){
                bool found = false;

                do{
                    typename std::vector<T>::size_type checks = 0;
                    typename std::vector<T>::size_type maxChecks 
                        = m_pool.size();

                    do{
                        if(!std::get<0>(*m_iter)){
                            found = true;
                        }
                        else{
                            ++m_iter;
                            if(m_iter == m_pool.end()){
                                m_iter = m_pool.begin();
                            }
                        }

                    }while(!found && checks < maxChecks);

                    if(!found){
                        typename std::vector<T>::size_type amountToAdd 
                            = m_pool.size();
                        typename std::vector<T>::size_type prevSize
                            = m_pool.size();
                        if(amountToAdd == 0){
                            amountToAdd = 2;
                        }
                        for(typename std::vector<T>::size_type i = 0; 
                                i < amountToAdd; ++i)
                        {
                            m_pool.emplace_back(makeNewElement());
                        }

                        m_iter = m_pool.begin() + prevSize;
                        found = true;
                    }
                }while(!found);

                std::get<0>(*m_iter) = true;

                return std::get<1>(*m_iter);
            }

            void Release(const std::shared_ptr<T>& p_elementPtr){
                auto iter = m_pool.begin();
                bool found = false;
                while(!found && iter != m_pool.end()){
                    if(std::get<1>(*iter) == p_elementPtr){
                        found = true;
                        std::get<0>(*iter)=false;
                        std::get<1>(*iter)->Reset();
                    }
                }
            }
    };
}

#endif
