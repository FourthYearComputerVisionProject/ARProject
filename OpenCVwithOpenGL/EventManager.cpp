#include "stdafx.h"
#include "EventManager.h"

static IEventManager* g_pEventManager = NULL;
GenericObjectFactory<IEventData, EventType> g_eventFactory;

IEventManager* IEventManager::Get(void)
{
	_ASSERTE(g_pEventManager);
	return g_pEventManager;
}

IEventManager::IEventManager(const char* pName, bool setAsGlobal)
{
	if(setAsGlobal)
	{
		if(g_pEventManager)
		{
			ERROR("Global event manager already exists");
			delete g_pEventManager;
		}
		g_pEventManager = this;
	}
}

IEventManager::~IEventManager(void)
{
	if(g_pEventManager == this)
	{
		g_pEventManager = NULL;
	}
}