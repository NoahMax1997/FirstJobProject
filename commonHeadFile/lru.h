#ifndef __LRU_H__
#define __LRU_H__

#include <map>
#include "base/log/logthread.h"
#include "time_utils.h"
#include "list.h"
#include "lock.h"

using std::map;
using std::pair;
using wtse::log::CTseLogger;

/*
 * LRU is thread-safe
 */
namespace leyi {
template<class T>
struct CacheNode {
    T data;
    bool status;
    int key;
    long last_visit;

    CacheNode(const T &_data, const bool _status, const int _key) {
        data = _data;
        status = _status;
        key = _key;
        last_visit = CTimeUtils::GetUnixTime();
    }

    ~CacheNode() {
        delete data;
    }
};

template<class T>
class LRU {
public:
    LRU(const LRU&) = delete;
    LRU& operator=(const LRU&) = delete;
    LRU() {};
    ~LRU() {};
    static const bool BUSY = true;
    static const bool IDLE = false;
    int Init(const int max_size, const long time_out, const int one_turn_max_clear_num, CTseLogger *logger);
    int GetData(int key, T *data);
    int ReleaseData(int key, T data); // 归还取出使用的cache数据
    int AddData(int key, const T &data, const bool status); // 增加新的cache数据
    void UpdateParam(const int max_size, const long time_out, const int one_turn_max_clear_num);
private:
    typedef list_node<CacheNode<T>*> LRUNode;
    typedef map<int, LRUNode*> MapCache;

    void ClearCache();

    MapCache m_map_cache;
    list<CacheNode<T>*> m_lru_cache;
    MutexLock m_lock;
    int m_max_size; // cache max size
    long m_time_out;
    int m_one_turn_max_clear_num;
    CTseLogger *m_logger;
};
}
#include "lru_impl.h"
#endif
