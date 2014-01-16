/*
 * EventManager.cpp
 * Implementation of an event manager.
 */

#include "stdafx.h"

#include "EventManager.h"

static EventManager* globalEventManager;

EventManager::EventManager(bool isGlobal)
{
	eventListeners = new std::map<EventType, IEventListenerList*>();
	if(isGlobal)
	{
		if(globalEventManager)
		{
			delete globalEventManager;
		}
		globalEventManager = this;
	}
	activeQueue = 0;
}

EventManager* EventManager::getGlobal()
{
	return globalEventManager;
}

bool EventManager::addListener(EventType type, IEventListener* listener)
{
	IEventListenerList* list = (*eventListeners)[type];
	if(list == NULL)
	{
		list = new IEventListenerList();
		(*eventListeners)[type] = list;
	}
	for(auto it = list->begin(); it != list->end(); ++it)
	{
		if(listener == (*it))
		{
			//Double register, ignore
			return false;
		}
	}
	list->push_back(listener);
	return true;
}

bool EventManager::removeListener(EventType type, IEventListener* listener)
{
	auto findIt = eventListeners->find(type);
	if(findIt != eventListeners->end())
	{
		IEventListenerList* list = findIt->second;
		for(auto it = list->begin(); it != list->end(); ++it)
		{
			if(listener == (*it))
			{
				list->erase(it);
				return true;
			}
		}
	}
	return false;
}

bool EventManager::fireEvent(BaseEvent* evt)
{
	bool handled = false;
	EventType type = evt->getType();
	auto findIt = eventListeners->find(type);
	if(findIt == eventListeners->end())
	{
		return false;
	}
	IEventListenerList* list = findIt->second;
	for(auto it = list->begin(); it != list->end(); ++it)
	{
		(*it)->handleEvent(evt);
		handled = true;
	}
	return handled;
}

bool EventManager::queueEvent(BaseEvent* evt)
{
	EventType type = evt->getType();
	auto findIt = eventListeners->find(type);
	if(findIt == eventListeners->end())
	{
		return false;
	}
	eventQueues[activeQueue].push_back(evt);
	return true;
}

bool EventManager::removeEvent(EventType type, bool allOf)
{
	bool removed = false;
	for(auto it = eventQueues[activeQueue].begin(); it != eventQueues[activeQueue].end(); ++it)
	{
		if((*it)->getType() == type)
		{
			eventQueues[activeQueue].erase(it);
			removed = true;
			if(!allOf)
			{
				break;
			}
		}
	}
	return removed;
}

bool EventManager::processEvents(long maxTime)
{
	clock_t currentTime = clock();
	clock_t maxDiff = (maxTime == INFINITE_TIME)?(INFINITE_TIME):(maxTime / (1000 / CLOCKS_PER_SEC));
	clock_t maxT = (maxDiff == INFINITE_TIME)?(INFINITE_TIME):(currentTime + maxDiff);
	int processingQueue = activeQueue;
	activeQueue = ++activeQueue % NUM_OF_QUEUES;
	BaseEventList list = eventQueues[processingQueue];
	while((maxTime == INFINITE_TIME || currentTime < maxT) && !list.empty())
	{
		BaseEvent* evt = list.front();
		list.pop_front();
		if(!fireEvent(evt))
		{
			//FIXME Event didn't fire, what do we do?
		}
	}
	bool success = list.empty();
	if(!success)
	{
		while(!list.empty())
		{
			BaseEvent* evt = list.back();
			list.pop_back();
			eventQueues[activeQueue].push_front(evt);
		}
	}
	return success;
}