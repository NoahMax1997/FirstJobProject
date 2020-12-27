#ifndef __RANK_LIST_H__
#define __RANK_LIST_H__
#include <vector>
#include "base/log/logthread.h"

using std::map;
using std::pair;
using wtse::log::CTseLogger;

/*
 * RankList is not thread-safe
 * 不存在并列排名
 * cmp(a, b) = 0 --> a和b是同一个人
 * cmp(a, b) < 0 --> b排名靠前
 * cmp(a, b) > 0 --> a排名靠前
 */
namespace leyi {
template<class T, class C>
class RankList {
public:
    RankList(const RankList&) = delete;
    RankList& operator=(const RankList&) = delete;
    RankList(const int max_size, CTseLogger *logger) {
        m_max_size = max_size;
        m_logger = logger;
        m_rank_list.clear();
        m_seq = 0;
        last_update_time=CTimeUtils::GetUnixTime();
    }
    ~RankList();
    bool Update(T *data);
    T* operator[] (int i) {
        return m_rank_list[i];
    }
    int size() {
        return m_rank_list.size();
    }
    long GetSeq() {
        ++m_seq;
        return m_seq;
    }
    void PushBack(T *data) {
        T *node = new T();
        *node = *data;
        m_rank_list.push_back(node);
    }
    int GetMaxSize() {
        return m_max_size;
    }
    long getUpdateTime(){
        return last_update_time;
    }
    void setUpdateTime(long time){
        last_update_time=time;
    }
private:
    std::vector<T*> m_rank_list;
    C m_cmp;
    int m_max_size; // rank max size
    long m_seq; // 更新次数
    long last_update_time;
    CTseLogger *m_logger;
};
}
#include "rank_list_impl.h"
#endif
