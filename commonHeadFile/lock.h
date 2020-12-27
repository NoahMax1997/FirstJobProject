#ifndef __LOCK_H__
#define __LOCK_H__

#include <pthread.h>

#pragma pack(1)

class MutexLock
{
private:
    pthread_mutex_t m_lock;

public:
    MutexLock()
    {
        pthread_mutex_init(&m_lock, NULL);
    }

    ~MutexLock()
    {
        pthread_mutex_destroy(&m_lock);
    }

    int GetLock()
    {
        pthread_mutex_lock(&m_lock);
        return 0;
    }

    int UnLock()
    {
        pthread_mutex_unlock(&m_lock);
        return 0;
    }
};

class ReadWriteLock
{
private:
    pthread_rwlock_t m_lock;
    pthread_rwlockattr_t m_lock_attr;

public:
    ReadWriteLock()
    {
        pthread_rwlockattr_init(&m_lock_attr);
        /* write first */
        pthread_rwlockattr_setkind_np(&m_lock_attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
        pthread_rwlock_init(&m_lock, &m_lock_attr);
    }

    ~ReadWriteLock()
    {
        pthread_rwlock_destroy(&m_lock);
    }

    int GetReadLock()
    {
        pthread_rwlock_rdlock(&m_lock);
        return 0;
    }

    int GetWriteLock()
    {
        pthread_rwlock_wrlock(&m_lock);
        return 0;
    }

    int UnLock()
    {
        pthread_rwlock_unlock(&m_lock);
        return 0;
    }
};

class GuardReadWriteLock
{
private:
    ReadWriteLock *m_lock;
    bool is_lock;

public:
    GuardReadWriteLock (ReadWriteLock *lock)
    {
        is_lock = false;
        m_lock = lock;
    }

    ~GuardReadWriteLock ()
    {
        if (is_lock == true)
        {
            m_lock->UnLock();
        }
    }

    void GetReadLock ()
    {
        if (is_lock == true)
        {
            m_lock->UnLock();
        }
        is_lock = true;
        m_lock->GetReadLock();
    }

    void GetWriteLock ()
    {
        if (is_lock == true)
        {
            m_lock->UnLock();
        }
        is_lock = true;
        m_lock->GetWriteLock();
    }

    void UnLock()
    {
        if (is_lock == true)
        {
            m_lock->UnLock();
            is_lock = false;
        }
    }
};

#pragma pack()
#endif
