#include "stdafx.h"
#include "EventManagerImpl.h"

EventManager::EventManager(const char* pName, bool setAsGlobal)
	: IEventManager(pName, setAsGlobal)
{
	m_activeQueue = 0;
}

EventManager::~EventManager(void)
{
	//Do nothing
}

bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, const EventType& eventType)
{
	EventListenerList& eventListenerList = m_eventListeners[eventType];
	for(auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
	{
		if(eventDelegate == (*it))
		{
			//Double register
			return false;
		}
	}
	eventListenerList.push_back(eventDelegate);
	return true;
}

bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& eventType)
{
	bool success = false;
	auto findIt = m_eventListeners.find(eventType);
	if(findIt != m_eventListeners.end())
	{
		EventListenerList& listeners = findIt->second;
		for(auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if(eventDelegate == (*it))
			{
				listeners.erase(it);
				success = true;
				break;
			}
		}
	}
	return success;
}

bool EventManager::VTriggerEvent(const IEventDataPtr& pEvent) const
{
	bool processed = false;

	auto findIt = m_eventListeners.find(pEvent->VGetEventType());
	if(findIt != m_eventListeners.end())
	{
		const EventListenerList& eventListenerList = findIt->second;
		for(EventListenerList::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
		{
			EventListenerDelegate listener = (*it);
			listener(pEvent);
			processed = true;
		}
	}
	return processed;
}

//Do we need these?

bool EventManager::VQueueEvent(const IEventDataPtr& pEvent)
{
	//Bound checking
	_ASSERTE(m_activeQueue >= 0);
	_ASSERTE(m_activeQueue < EVENT_MANAGER_NUM_QUEUES);

	if(!pEvent)
	{
		ERROR("Invalid event");
		return false;
	}

	auto findIt = m_eventListeners.find(pEvent->VGetEventType());
	if(findIt != m_eventListeners.end())
	{
		m_queues[m_activeQueue].push_back(pEvent);
		return true;
	}
	else
	{
		return false;
	}
}

bool EventManager::VThreadSafeQueueEvent(const IEventDataPtr& pEvent)
{
	m_realtimeEventQueue.push(pEvent);
	return true;
}

bool EventManager::VAbortEvent(const EventType& eventType, bool allOf)
{
	//Bound checking
	_ASSERTE(m_activeQueue >= 0);
	_ASSERTE(m_activeQueue < EVENT_MANAGER_NUM_QUEUES);

	bool success = false;
	EventListenerMap::iterator findIt = m_eventListeners.find(eventType);
	if(findIt != m_eventListeners.end())
	{
		//There are listeners listening for this event
		EventQueue& eventQueue = m_queues[m_activeQueue];
		auto it = eventQueue.begin();
		while(it != eventQueue.end())
		{
			//Auto-advance the iterator to avoid invalidating it when removing items
			auto thisIt = it;
			++it;

			if((*thisIt)->VGetEventType == eventType)
			{
				eventQueue.erase(thisIt);
				success = true;
				if(!allOf)
				{
					break;
				}
			}
		}
	}
	return success;
}

bool EventManager::VUpdate(unsigned long maxTimeMilli)
{
	unsigned long currentMilli = GetTickCount();
	unsigned long maxMilli = ((maxTimeMilli == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currentMilli + maxTimeMilli));
	IEventDataPtr pRealtimeEvent;
	while(m_realtimeEventQueue.try_pop(pRealtimeEvent))
	{
		VQueueEvent(pRealtimeEvent);

		currentMilli = GetTickCount();
		if(maxMilli != IEventManager::kINFINITE)
		{
			if(currentMilli >= maxMilli)
			{
				ERROR("Realtime queue is being spammed");
			}
		}
	}

	int queueToProcess = m_activeQueue;
	m_activeQueue = (m_activeQueue + 1) % EVENT_MANAGER_NUM_QUEUES;
	m_queues[m_activeQueue].clear();

	while(!m_queues[queueToProcess].empty())
	{
		IEventDataPtr pEvent = m_queues[queueToProcess].front();
		m_queues[queueToProcess].pop_front();

		const EventType& eventType = pEvent->VGetEventType();

		auto findIt = m_eventListeners.find(eventType);
		if(findIt != m_eventListeners.end())
		{
			const EventListenerList& eventListeners = findIt->second;
			for(auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
			{
				EventListenerDelegate listener = (*it);
				listener(pEvent);
			}
		}

		currentMilli = GetTickCount();
		if(maxMilli != IEventManager::kINFINITE && currentMilli >= maxMilli)
		{
			break;
		}
	}

	bool queueFlushed = (m_queues[queueToProcess].empty());
	if(!queueFlushed)
	{
		while(!m_queues[queueToProcess].empty())
		{
			IEventDataPtr pEvent = m_queues[queueToProcess].back();
			m_queues[queueToProcess].pop_back();
			m_queues[m_activeQueue].push_front(pEvent);
		}
	}
	return queueFlushed;
}