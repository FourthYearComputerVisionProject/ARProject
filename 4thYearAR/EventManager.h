/*
 * EventManager.h
 * Header file for EventManager.cpp
 */

#pragma once

#include "stdafx.h"
#include <list>

#include "IEventListener.h"

/*
 * Type defs for event type and a list of event listeners and a list of events.
 * Pointers because C++ hates dealing with pure virtual classes that aren't pointer references.
 * Go figure...
 */
typedef std::list<IEventListener*> IEventListenerList;
typedef std::list<BaseEvent*> BaseEventList;
typedef long EventType;

/*
 * Maximum number of queues to store events in.
 * This should always be greater then 1 or else when processEvents is called,
 * the queue that is being processed will also be receiving new events (which is BAD)
 */
#define NUM_OF_QUEUES 2

/*
 * Definition of what the constant for infinite processing time, which can be passed to
 * the processEvents function
 */
#define INFINITE_TIME -1

/*
 * Defintion of an event manager.
 */
class EventManager {
private:
	/*
	 * The current queue that is active.
	 */
	int activeQueue;
	/*
	 * The mapping of event listeners to the events they are listening to.
	 */
	std::map<EventType, IEventListenerList*>* eventListeners;
	/*
	 * The queues the events go into.
	 * WARNING: These queues are partially thread safe. It is not recommended to use methods
	 * that use these queues from any thread that is not the once calling processEvents.
	 * Methods who use these queues are:
	 * - queueEvent
	 * - removeEvent
	 * - processEvents
	 * FIXME Make these queues thread safe
	 */
	BaseEventList eventQueues[NUM_OF_QUEUES];
public:
	/*
	 * Get the global event manager.
	 */
	static EventManager* getGlobal();
	/*
	 * Constructor
	 * Argument global: Whether or not this is the global event listener, defaults to false
	 */
	EventManager(bool isGlobal = false);
	/*
	 * Add a listener to the manager.
	 * Argument type: The type of event to listen to.
	 * Argument listener: The listener to add.
	 * Return: True if the event listener was added, false otherwise.
	 */
	bool addListener(EventType type, IEventListener* listener);
	/*
	 * Remove a listener to the manager.
	 * Argument type: The type of event to listen to.
	 * Argument listener: The listener to remove.
	 * Return: True if the event listener was removed, false otherwise.
	 */
	bool removeListener(EventType type, IEventListener* listener);
	/*
	 * Fire an event, bypassing the queues.
	 * Argument evt: The event to fire.
	 * Return: True if the event was processed, false otherwise.
	 */
	bool fireEvent(BaseEvent* evt);
	/*
	 * Queue an event to be processed.
	 * Argument evt: The event to queue.
	 * Return: True if the event was queued, false otherwise.
	 */
	bool queueEvent(BaseEvent* evt);
	/*
	 * Remove an event from the queue.
	 * Argument type: The event type to remove.
	 * Argument allOf: Whether or not to remove all events of the given type, or just
	 * the first, defaults to false.
	 * Return: True if at least one event was removed.
	 */
	bool removeEvent(EventType type, bool allOf = false);
	/*
	 * Process the events in the queue.
	 * Argument maxTime: The maximum number of milliseconds to take processing events.
	 */
	bool processEvents(long maxTime);
};