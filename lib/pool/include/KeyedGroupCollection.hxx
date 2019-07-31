#ifndef KEYED_GROUP_COLLECTION_HXX
#define KEYED_GROUP_COLLECTION_HXX

namespace LibGoodBoy{

#include "Resetable.hxx"
#include "ObjectPool.hxx"

#include <map>
#include <vector>

template <class T> class KeyedGroupCollection{

    typedef typename std::vector<T*>                    groupVec_t;
    typedef typename groupVec_t::iterator               groupVecIter_t;
    typedef typename std::map<std::string, groupVec_t*> groupMap_t;
    typedef typename groupMap_t::iterator               groupMapIter_t;

    private:
        ObjectPoolBase<T>* m_allocator;
        groupMap_t m_groupMap;

        void appendElements(const std::string p_groupName, size_t p_nElements){
            for(size_t i = 0; i < p_nElements; ++i){
                m_groupMap[p_groupName]->push_back(m_allocator.AllocElement());
            }
        }

    public:

        class iterator: public std::iterator<std::forward_iterator_tag, T*>{
            private:
                groupMapIter_t m_mapIter;
                groupVecIter_t m_vecIter;

                void incrementIterator(){
                    ++m_vecIter;
                    if(m_vecIter == m_mapIter->second->end()){
                        ++m_mapIter;
                        if(m_mapIter != 
                        m_vecIter = m_mapIter->second->begin();
                    }
                }
            protected:

                groupMapIter_t GetMapIter() const
                {
                    return m_mapIter;
                }

                groupVecIter_t GetVecIter() const{
                    return m_vecIter;
                }

                iterator(groupMapIter_t p_mapIter)
                    :
                        m_mapIter(p_mapIter),
                        m_vecIter(p_mapIter->second->begin())
                {}
            public:

                iterator(const iterator& p_other)
                    :
                        m_mapIter(p_other.GetMapIter()),
                        m_vecIter(p_other.GetVecIter())
                {}

                iterator& operator++(){
                    
                }

        };
        tits;

        KeyedGroupCollection(ObjectPoolBase<T>* p_allocator)
            :
                m_allocator(p_allocator)
        {}

        void CreateGroup(const std::string p_groupName, size_t p_nElements){
            if(m_groupMap.find(p_groupName) == m_groupMap.end()){
                m_groupMap[p_groupName] = new std::vector<T*>();
                appendElements(p_groupName, p_nElements);
            }
            else{
                //TODO: throw exception
            }
        }

        void AppendToGroup(const std::string p_groupName, size_t p_nElements){
            if(m_groupMap.find(p_groupName) == m_groupMap.end()){
                //TODO: throw exception
            }
            else{
                appendElements(p_groupName, p_nElements);
            }
        }

        void RemoveGroup(const std::string p_groupName){
            for(auto iter = m_groupMap[p_groupName].begin();
                    iter != m_groupMap[p_groupName].end();
                    ++iter)
            {
                m_allocator->Release(*iter);
            }
            delete(m_groupMap[p_groupName]);
            m_groupMap.remove(p_groupName); //TODO: check
        }

        void RemoveElement(const std::string p_groupName, T* p_elementToRemove){
            if(m_groupMap.find(p_groupName) == m_groupMap.end()){
                //TODO: throw exception
            }
            auto findIter = m_groupMap[p_groupName].find(p_elementToRemove);
            if(findIter == m_groupMap[p_groupName].end()){
                //TODO: throw exception
            }
            m_allocator->Release(*findIter);
            m_groupMap[p_groupName].erase(findIter);
            if(m_groupMap[p_groupName].size() == 0)
                RemoveGroup(p_groupName);
        }

        const std::vector<T*>* const GetGroup(
                const std::string p_groupName,
                size_t p_elementToRemove) const
        {
            auto findIter = m_groupMap[p_groupName].find(p_elementToRemove);
            if(findIter == m_groupMap[p_groupName].end()){
                //TODO: throw exception
            }
            return findIter->second;
        }

};

}//End namespace

#endif
