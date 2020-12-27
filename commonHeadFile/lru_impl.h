#ifndef __LRU_IMPL__
#define __LRU_IMPL__
#include "lock.h"
namespace leyi {
template<class T>
int LRU<T>::Init(const int max_size, const long time_out,
              const int one_turn_max_clear_num, CTseLogger *logger) {
    if (logger == NULL) {
        return -1;
    }
    m_logger = logger;
    if (max_size <= 0 || time_out <= 0 || one_turn_max_clear_num <= 0) {
        TSE_LOG_ERROR(m_logger, ("cache size[%d] time_out[%ld] "
                                 "one_turn_max_clear_num[%d]",
                                 max_size, time_out, one_turn_max_clear_num));
        return -1;
    }
    m_max_size = max_size;
    m_time_out = time_out;
    m_one_turn_max_clear_num = one_turn_max_clear_num;
    m_map_cache.clear();
    return 0;
}

template<class T>
int LRU<T>::GetData(int key, T *data) {
    GuardMutexLock guard(&m_lock);
    guard.GetLock();
    *data = NULL;
    if (m_map_cache.end() == m_map_cache.find(key)) {
        return 0;
    }
    LRUNode *lru_node = m_map_cache[key];
    CacheNode<T> *cache_node = lru_node->data;
    if (IDLE != cache_node->status) {
        TSE_LOG_INFO(m_logger, ("key[%d] is busy", key));
        return -1;
    } else {
        cache_node->status = BUSY;
        cache_node->last_visit = CTimeUtils::GetUnixTime();
        m_lru_cache.remove(lru_node);
        m_lru_cache.push_front(lru_node);
        *data = cache_node->data;
        return 0;
    }
}

template<class T>
int LRU<T>::ReleaseData(int key, T data) {
    GuardMutexLock guard(&m_lock);
    guard.GetLock();
    if (m_map_cache.end() == m_map_cache.find(key)) {
        return -1;
    }
    LRUNode *lru_node = m_map_cache[key];
    CacheNode<T> *cache_node = lru_node->data;
    if (data != cache_node->data) {
        return -1;
    }
    if (BUSY != cache_node->status) {
        return -1;
    }
    cache_node->status = IDLE;
    return 0;
}

template<class T>
int LRU<T>::AddData(int key, const T &data, bool status) {
    GuardMutexLock guard(&m_lock);
    guard.GetLock();
    if (m_map_cache.end() != m_map_cache.find(key)) {
        return -1;
    }
    // CacheNode<T> *cache_node = new CacheNode<T>(data, IDLE, key);
    CacheNode<T> *cache_node = new CacheNode<T>(data, status, key);
    LRUNode *lru_node = m_lru_cache.push_front(cache_node);
    m_map_cache[key] = lru_node;
    ClearCache();
    return 0;
}

template<class T>
void LRU<T>::ClearCache() {
    long cnt_time = CTimeUtils::GetUnixTime();
    int cnt_num = 0;
    while (m_max_size < m_lru_cache.size() && cnt_num < m_one_turn_max_clear_num) {
        LRUNode *lru_node = m_lru_cache.pop_back();
        CacheNode<T> *cache_node = lru_node->data;
        if (cache_node->status != IDLE ||
            cache_node->last_visit + m_time_out >= cnt_time) {
            m_lru_cache.push_back(lru_node);
            break;
        }
        TSE_LOG_INFO(m_logger, ("clear cache key[%d] last_visit[%d] time_dis[%d]",
                                cache_node->key, cache_node->last_visit,
                                cache_node->last_visit + m_time_out - cnt_time));
        m_map_cache.erase(cache_node->key);
        delete cache_node;
        delete lru_node;
        cnt_num++;
    }
    TSE_LOG_INFO(m_logger, ("cache size[%d]", m_lru_cache.size()));
}

template<class T>
void LRU<T>::UpdateParam(const int max_size, const long time_out, const int one_turn_max_clear_num) {
    GuardMutexLock guard(&m_lock);
    guard.GetLock();
    m_max_size = max_size;
    m_time_out = time_out;
    m_one_turn_max_clear_num = one_turn_max_clear_num;
    return;
}
}  // end namespace leyi
#endif
