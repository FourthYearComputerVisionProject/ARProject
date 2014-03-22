#ifndef EVENT_MANAGER_IMPL_H
#define EVENT_MANAGER_IMPL_H

#include "EventManager.h"

const unsigned int EVENT_MANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
	//Useful typedefs
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<IEventDataPtr> EventQueue;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[EVENT_MANAGER_NUM_QUEUES];
	int m_activeQueue;

	ThreadSafeEventQueue m_realtimeEventQueue;
public:
	explicit EventManager(const char* pName, bool setAsGlobal);
	virtual ~EventManager(void);

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& eventType);
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& eventType);

	virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const;

	//Do we need these?

	virtual bool VQueueEvent(const IEventDataPtr& pEvent);
	virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent);
	virtual bool VAbortEvent(const EventType& type, bool allOf = false);

	virtual bool VUpdate(unsigned long maxTimeMilli = kINFINITE);
};

#endif