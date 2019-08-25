#ifndef KEYED_GROUP_COLLECTION_HXX
#define KEYED_GROUP_COLLECTION_HXX

#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <stdio.h>

namespace LibGoodBoy{

template <class T> class KeyedGroupCollection{

    typedef typename std::vector<T>                     groupVec_t;
    typedef typename groupVec_t::iterator               groupVecIter_t;
    typedef typename std::map<std::string, groupVec_t>  groupMap_t;
    typedef typename groupMap_t::iterator               groupMapIter_t;

    private:
        groupMap_t m_groupMap;

    public:
        class iterator: public std::iterator<std::forward_iterator_tag, T*>{
            private:
                groupMap_t*     m_map;
                groupMapIter_t  m_mapIter;
                groupVecIter_t  m_vecIter;

                void incrementIterator(){
                    if(m_mapIter != m_map->end()){
                        ++m_vecIter;
                        if(m_vecIter == m_mapIter->second.end()){
                            ++m_mapIter;
                            if(m_mapIter != m_map->end()){
                                m_vecIter = m_mapIter->second.begin();
                            }
                        }
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

                groupMap_t GetMap() const{
                    return m_map;
                }

            public:
                iterator(groupMap_t& p_map)
                    :
                        m_map(&p_map)
                {
                    m_mapIter = p_map.begin();
                    if(m_mapIter != p_map.end()){
                        m_vecIter = m_mapIter->second.begin();
                    }
                }

                iterator(const iterator& p_other)
                    :
                        m_mapIter(p_other.GetMapIter()),
                        m_vecIter(p_other.GetVecIter()),
                        m_map(p_other.GetMap())
                {}

                iterator(groupMap_t& p_map,
                        bool p_isEnd)
                    :
                        m_map(&p_map)
                {
                    if(!p_isEnd){
                        m_mapIter = p_map.begin();
                        if(m_mapIter != p_map.end()){
                            m_vecIter = m_mapIter->second.begin();
                        }
                    }
                    else{
                        m_mapIter = p_map.end();
                    }
                }

                iterator& operator++(){
                    incrementIterator();
                    return *this;
                }

                bool operator == (const iterator& p_other) const{
                    return this->m_mapIter == p_other.GetMapIter() && \
                        ( (this->m_mapIter == m_map->end()) || \
                        (this->m_vecIter == p_other.GetVecIter()) );
                }

                bool operator != (const iterator& p_other) const{
                    return !(this->m_mapIter == p_other.GetMapIter() && \
                        ((this->m_mapIter == m_map->end()) || \
                        (this->m_vecIter == p_other.GetVecIter())));

                    if(this->m_mapIter != p_other.GetMapIter())
                        return true;
                    if(this->m_vecIter != p_other.GetVecIter())
                        return true;
                    return false;
                }

                T& operator*() {return *m_vecIter;}
        };

        iterator begin(){ return iterator(m_groupMap); }
        iterator end(){ return iterator(m_groupMap, true); }

        KeyedGroupCollection(){}

        groupVec_t operator [](std::string key) const{
            return m_groupMap[key];
        }

        groupVec_t& operator [](std::string key){
            return m_groupMap[key];
        }

        void RemoveGroup(const std::string p_groupName){
            delete(m_groupMap[p_groupName]);
            m_groupMap.remove(p_groupName); //TODO: check
        }


};

}//End namespace

#endif

