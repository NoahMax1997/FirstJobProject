#ifndef __LEYI_LIST_H__
#define __LEYI_LIST_H__

namespace leyi {
template<class T>
struct list_node {
    T data;
    list_node<T> *next;
    list_node<T> *pre;

    list_node<T> (const T &_data) {
        data = _data;
        next = NULL;
        pre = NULL;
    }
    ~list_node<T> () {}
};

template<class T>
class list {
public:
    list(const list&) = delete;
    list& operator=(const list&) = delete;
    list() {
        m_len = 0;
        m_head = NULL;
        m_tail = NULL;
    };
    ~list() {
        list_node<T> *i = m_head;
        list_node<T> *tmp;
        while (m_len > 0) {
            tmp = i->next;
            delete i;
            m_len--;
            i = tmp;
        }
    };

    int size() { return m_len; }
    bool empty() { return m_len == 0; }
    list_node<T>* push_front(const T &data);
    list_node<T>* push_back(const T &data);
    void push_front(list_node<T>* node);
    void push_back(list_node<T>* node);
    list_node<T>* pop_front();
    list_node<T>* pop_back();
    void insert(list_node<T> *old_node, list_node<T> *new_node);
    void remove(list_node<T> *node);
    void erase(list_node<T> *node);
private:
    int m_len;
    list_node<T> *m_head;
    list_node<T> *m_tail;
};
}

#include "list_impl.h"
#endif
