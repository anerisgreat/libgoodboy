#ifndef NEURAL_OBJECT_POOL_HXX
#define NEURAL_OBJECT_POOL_HXX

#include "libGoodBoyConfig.hxx"
#include "Resetable.hxx"

#include <memory>
#include <vector>

namespace LibGoodBoy{

    template <class T> class ObjectPool{
        static_assert(
            (std::is_base_of<Resetable, T>::value),
            "T in ObjectPool must be a descendant of LibGoodBoy::Resetable"
        );

        private:

            template<class U> struct BoolPtrPair{
                public:
                    std::shared_ptr<U> Ptr;
                    bool Used;

                    BoolPtrPair(const std::shared_ptr<T>& p_ptr)
                        :
                            Ptr(p_ptr),
                            Used(false)
                    {}
            };

            std::shared_ptr<T> (*m_allocFunc)();
            std::vector<BoolPtrPair<T>> m_pool;
            typename std::vector<BoolPtrPair<T>>::iterator m_iter;

        public:
            ObjectPool(std::shared_ptr<T> (*p_allocFunc)())
                :
                    m_allocFunc(p_allocFunc),
                    m_pool(std::vector<BoolPtrPair<T>>())
            {}

            ~ObjectPool();

            std::shared_ptr<T> AllocElement(){
                bool found = false;

                do{
                    typename std::vector<T>::size_type checks = 0;
                    typename std::vector<T>::size_type maxChecks 
                        = m_pool.size();

                    do{
                        if(!(m_iter->Used)){
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
                                i < amountToAdd; ++i){
                            m_pool.emplace_back((*m_allocFunc)());
                        }
                        
                        m_iter = m_pool.begin() + prevSize;
                        found = true;
                    }
                }while(!found);

                return (*m_iter).Ptr;
            }

            void Release(const std::shared_ptr<T>& p_elementPtr){
                auto iter = m_pool.begin();
                bool found = false;
                while(!found && iter != m_pool.end()){
                    if((*iter).Ptr == p_elementPtr){
                        found = true;
                        (*iter).Used=false;
                        (*iter).Ptr->Reset();
                    }
                }
            }
    };
}

#endif
