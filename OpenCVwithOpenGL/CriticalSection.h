#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include "stdafx.h"
#include <windows.h>
 
class CriticalSection : public GCC_noncopyable
{
public:
	CriticalSection()
	{ 
		InitializeCriticalSection( &m_cs ); 
	}
 
	~CriticalSection()
	{
		DeleteCriticalSection( &m_cs ); 
	}
 
	void Lock()
    { 
		EnterCriticalSection( &m_cs ); 
	}

	void Unlock()
    { 
		LeaveCriticalSection( &m_cs ); 
	}
protected:
    mutable CRITICAL_SECTION m_cs;
};

class ScopedCriticalSection : public GCC_noncopyable
{
public:
	ScopedCriticalSection( CriticalSection & csResource)
		: m_csResource( csResource)
	{
		m_csResource.Lock();
	}
  
	~ScopedCriticalSection()
	{
		m_csResource.Unlock();
	}
private:
       CriticalSection & m_csResource;
};

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
	CriticalSection m_cs;
	HANDLE m_dataPushed;
public:
	concurrent_queue()
	{
		m_dataPushed = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

    void push(Data const& data)
    {
		{
			ScopedCriticalSection locker(m_cs);
			the_queue.push(data);
		}
		PulseEvent(m_dataPushed);
    }

    bool empty() const
    {
		ScopedCriticalSection locker(m_cs);
        return the_queue.empty();
    }

    bool try_pop(Data& popped_value)
    {
		ScopedCriticalSection locker(m_cs);
        if(the_queue.empty())
        {
            return false;
        }
        popped_value=the_queue.front();
        the_queue.pop();
        return true;
    }

    void wait_and_pop(Data& popped_value)
    {
		ScopedCriticalSection locker(m_cs);
        while(the_queue.empty())
        {
			WaitForSingleObject(m_dataPushed);
        }
        popped_value=the_queue.front();
        the_queue.pop();
    }
};

#endif