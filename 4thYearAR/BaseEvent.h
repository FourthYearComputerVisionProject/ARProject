/*
 * BaseEvent.h
 * Header file for BaseEvent.cpp
 * A BaseEvent is the base-level event that all other events implement.
 */

#pragma once

#include "stdafx.h"

#include "EventTypes.h"

/*
 * Static definition of no source for an event.
 */
#define NO_EVENT_SOURCE -1

/*
 * BaseEvent definition
 */
class BaseEvent {
protected:
	/*
	 * The type of the event.
	 */
	long type;
	/*
	 * The time at which the event was fired, in clock cycles
	 * since program start.
	 * To translate to seconds, use fireTime * CLOCKS_PER_SECOND
	 * To translate to milliseconds, use fireTime * CLOCKS_PER_SECOND * 1000
	 */
	clock_t fireTime;
	/*
	 * The source of the event.
	 */
	long source;
public:
	/*
	 * Constructor.
	 * Argument eventType: The type of this event.
	 * Argument eventSource: The source of this event, which defaults to NO_SOURCE.
	 */
	BaseEvent(long eventType, long eventSource = NO_EVENT_SOURCE) { type = eventType; source = eventSource; fireTime = clock(); }
	/*
	 * Get the type of this event.
	 */
	long getType() { return type; }
	/*
	 * Get the time at which the event was fired.
	 */
	clock_t getFireTime() { return fireTime; }
	/*
	 * Get the source of this event.
	 */
	long getSource() { return source; }
};