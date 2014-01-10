//Replace with #pragma once ?
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "stdafx.h"
#include "CriticalSection.h"
#include "templates.h"
#include "3rdParty/FastDelegate/FastDelegate.h"

//Forward declarations and typedefs

class IEventData;

typedef unsigned long EventType;
typedef shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;
typedef concurrent_queue<IEventDataPtr> ThreadSafeEventQueue;

//Macros for event registration

extern GenericObjectFactory<IEventData, EventType> g_eventFactory;
#define REGISTER_EVENT(eventClass) g_eventFactory.Register(eventClass::sk_EventType);
#define CREATE_EVENT(eventType) g_eventFactory.Create(eventType);

class IEventData
{
public:
	virtual ~IEventData(void) {}
	virtual const EventType& VGetEventType(void) const = 0;
	//Timestamp of event creation
	virtual float GetTimeStamp(void) const = 0;
	//Timestamp of when the event is no longer valid
	virtual float GetExpireTime(void) const = 0;
	virtual IEventDataPtr VCopy(void) const = 0;
	virtual const char* GetName(void) const = 0;
};

class BaseEventData : public IEventData
{
	const float m_timeStamp;
	const float m_expireTime;
public:
	explicit BaseEventData(const float timeStamp = 0.0f, const float expireTime = 0.0f) :
	m_timeStamp(timeStamp), m_expireTime(expireTime) { }
	virtual const EventType& VGetEventType(void) const = 0;

	float GetTimeStamp(void) const {return m_timeStamp;}
	float GetExpireTime(void) const {return m_expireTime;}
};

class IEventManager
{
public:
	//What is "infinity"?
	enum eConstants {kINFINITE = 0xffffffff};
	explicit IEventManager(const char* pName, bool setAsGlobal);
	virtual ~IEventManager(void);

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;
	virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const = 0;

	//Do we need these?

	virtual bool VQueueEvent(const IEventDataPtr& pEvent) = 0;
	virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent) = 0;
	virtual bool VAbortEvent(const EventType& type, bool allOf = false) = 0;
	virtual bool VUpdate(unsigned long maxTimeMilli = kINFINITE) = 0;

	//Global getter

	static IEventManager* Get(void);
};

#endif