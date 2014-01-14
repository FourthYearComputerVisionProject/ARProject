/*
 * IEventListener.h
 * Header file containing the interface for event listeners.
 */

#pragma once

#include "stdafx.h"
#include "BaseEvent.h"

/*
 * Interface for event listeners.
 */
class IEventListener {
public:
	/*
	 * Handle an event.
	 * Argument evt: The event to handle.
	 */
	virtual void handleEvent(BaseEvent* evt) = 0;
};