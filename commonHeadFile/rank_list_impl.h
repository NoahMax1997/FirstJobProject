#ifndef __RANK_LIST_IMPL__
#define __RANK_LIST_IMPL__

namespace leyi {
template<class T, class C>
RankList<T,C>::~RankList() {
    for (auto iter = m_rank_list.begin(); iter != m_rank_list.end(); ++iter) {
        delete *iter;
    }
    m_rank_list.clear();
}

template<class T, class C>
bool RankList<T,C>::Update(T *data) {
    TINT32 cnt_size = m_rank_list.size();
    if (cnt_size > 0 && m_cmp(m_rank_list[cnt_size-1], data) > 0 && cnt_size >= m_max_size) {
        // rank榜满了
        return false;
    }
    T *node = NULL;
    for (auto iter = m_rank_list.begin(); iter != m_rank_list.end(); ++iter) {
        if (m_cmp(*iter, data) == 0) {
            node = *iter;
            m_rank_list.erase(iter);
            break;
        }
    }
    if (node == NULL) {
        if (cnt_size < m_max_size) {
            node = new T();
        } else {
            node = m_rank_list[cnt_size - 1];
            m_rank_list.erase(m_rank_list.begin() + (cnt_size - 1));
        }
    }
    // 模板T需要重载=运算符，实现深拷贝
    *node = *data;
    bool insert_flag = false;
    for (auto iter = m_rank_list.begin(); iter != m_rank_list.end(); ++iter) {
        if (m_cmp(*iter, data) < 0) {
            m_rank_list.insert(iter, node);
            insert_flag = true;
            break;
        }
    }
    if (insert_flag == false) {
        m_rank_list.push_back(node);
    }
    return true;
}
}  // end namespace leyi
#endif
