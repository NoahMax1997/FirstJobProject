#ifndef __LEYI_LIST_IMPL__
#define __LEYI_LIST_IMPL__

namespace leyi {
template<class T>
list_node<T>* list<T>::push_front(const T &data) {
    list_node<T> *node = new list_node<T>(data);
    if (m_len == 0) {
        m_head = node;
        m_tail = node;
    } else {
        m_head->pre = node;
        node->next = m_head;
        m_head = node;
    }
    m_len++;
    return node;
}

template<class T>
void list<T>::push_front(list_node<T>* node) {
    if (m_len == 0) {
        m_head = node;
        m_tail = node;
    } else {
        m_head->pre = node;
        node->next = m_head;
        m_head = node;
    }
    m_len++;
    return;
}

template<class T>
list_node<T>* list<T>::push_back(const T &data) {
    list_node<T> *node = new list_node<T>(data);
    if (m_len == 0) {
        m_head = node;
        m_tail = node;
    } else {
        m_tail->next = node;
        node->pre = m_tail;
        m_tail = node;
    }
    m_len++;
    return node;
}

template<class T>
void list<T>::push_back(list_node<T>* node) {
    if (m_len == 0) {
        m_head = node;
        m_tail = node;
    } else {
        m_tail->next = node;
        node->pre = m_tail;
        m_tail = node;
    }
    m_len++;
    return;
}

template<class T>
list_node<T>* list<T>::pop_front() {
    if (m_head == NULL) {
        return NULL;
    }
    list_node<T> *ret = m_head;
    remove(m_head);
    return ret;
}

template<class T>
list_node<T>* list<T>::pop_back() {
    if (m_tail == NULL) {
        return NULL;
    }
    list_node<T> *ret = m_tail;
    remove(m_tail);
    return ret;
}

template<class T>
void list<T>::insert(list_node<T> *old_node, list_node<T> *new_node) {
    // new_node->pre and next should be NULL
    new_node->pre = old_node;
    if (m_tail == old_node) {
        m_tail = new_node;
        old_node->next = new_node;
    } else {
        old_node->next->pre = new_node;
        new_node->next = old_node->next->pre;
        old_node->next = new_node;
    }
}

template<class T>
void list<T>::remove(list_node<T> *node) {
    if (m_head == node) {
        m_head = node->next;
    }
    if (m_tail == node) {
        m_tail = node->pre;
    }
    if (node->next != NULL) {
        node->next->pre = node->pre;
    }
    if (node->pre != NULL) {
        node->pre->next = node->next;
    }
    node->next = NULL;
    node->pre = NULL;
    m_len--;
}

template<class T>
void list<T>::erase(list_node<T> *node) {
    remove(node);
    delete node;
}
}
#endif
