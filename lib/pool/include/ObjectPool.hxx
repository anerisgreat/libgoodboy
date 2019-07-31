#ifndef OBJECT_POOL_HXX
#define OBJECT_POOL_HXX

#include "Resetable.hxx"

#include <memory>
#include <vector>
#include <tuple>

namespace LibGoodBoy{

template <class T> class ObjectPoolBase{
    protected:
        virtual std::tuple<bool, std::shared_ptr<T>> makeNewElement() = 0;

    private:
        std::vector<std::tuple<bool, std::shared_ptr<T>>> m_pool;
        typename std::vector<std::tuple<bool, std::shared_ptr<T>>>::iterator
            m_iter;

        void advanceIterator(){
            ++m_iter;
            if(m_iter == m_pool.end()){
                m_iter = m_pool.begin();
            }
        }

    public:
        ObjectPoolBase() : 
                m_pool(std::vector<std::tuple<bool, std::shared_ptr<T>>>())
        {}

        ~ObjectPoolBase(){};

        T* AllocElement(){
            bool found = false;
            typename std::vector<T>::size_type poolSizeAtBegin 
                = m_pool.size();

            do{
                typename std::vector<T>::size_type checks = 0;

                while(!found && checks < poolSizeAtBegin){
                    if(!std::get<0>(*m_iter)){
                        found = true;
                    }
                    else{
                        advanceIterator();
                    }

                    ++checks;
                }

                if(!found){
                    typename std::vector<T>::size_type amountToAdd 
                        = poolSizeAtBegin; 
                    if(amountToAdd == 0){
                        amountToAdd = 2;
                    }
                    for(typename std::vector<T>::size_type i = 0; 
                            i < amountToAdd; ++i)
                    {
                        m_pool.emplace_back(makeNewElement());
                    }

                    m_iter = m_pool.begin() + poolSizeAtBegin;
                    found = true;
                }
            }while(!found);

            std::get<0>(*m_iter) = true;

            std::shared_ptr<T>& retElement = std::get<1>(*m_iter);

            advanceIterator();
            return retElement.get();
        }

        void Release(const T* p_elementPtr){
            auto iter = m_pool.begin();
            bool found = false;
            while(!found && iter != m_pool.end()){
                if(std::get<1>(*iter).get() == p_elementPtr){
                    found = true;
                    std::get<0>(*iter)=false;
                    std::get<1>(*iter)->Reset();
                }
                ++iter;
            }
        }

        std::size_t PoolSize(){
            return m_pool.size();
        }
};

template <class T, typename... T_args> class ObjectPool 
    : public ObjectPoolBase<T>
{
    private:
        std::tuple<T_args...> m_argTuple;
    protected:
        template<size_t... Is> struct index_sequence;

        template<std::size_t... Is>
        std::tuple<bool, std::shared_ptr<T>> makeNewElement(
                std::index_sequence<Is...>)
        {
            return std::make_tuple<bool, std::shared_ptr<T>>(
                    false, 
                    std::make_shared<T>(std::get<Is>(m_argTuple)...));
        }

        std::tuple<bool, std::shared_ptr<T>> makeNewElement(){
            return makeNewElement(std::index_sequence_for<T_args...>());
        }
    public:
        ObjectPool(T_args... p_args)
            :
                ObjectPoolBase<T>(),
                m_argTuple(std::tuple<T_args...>(p_args...))
        {}

        ~ObjectPool(){};
};

template <class T > class ObjectPool<T, void> : public ObjectPoolBase<T>{
    protected:
        std::tuple<bool, std::shared_ptr<T>> makeNewElement(){
            return std::make_tuple<bool, std::shared_ptr<T>>(
                    false,
                    std::make_shared<T>());
        }
    public:
        ObjectPool():ObjectPoolBase<T>(){}
        ~ObjectPool(){};
};

}//End namespace

#endif
