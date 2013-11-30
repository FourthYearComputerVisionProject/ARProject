#ifndef EVENT_TEMPLATES_H
#define EVENT_TEMPLATES_H

#include "stdafx.h"

//Minor fix to shared_ptr

template <class Type>
shared_ptr<Type> MakeStrongPtr(weak_ptr<Type> pWeakPtr)
{
	if(!pWeakPtr.expired())
	{
		return shared_ptr<Type>(pWeakPtr);
	}
	else
	{
		return shared_ptr<Type>();
	}
}

template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void)
{
	return new SubType;
}

template <class BaseType, class IdType>
class GenericObjectFactory
{
	typedef BaseClass* (*ObjectCreationFunction)(void);
	std::map<IdType, ObjectCreationFunction> m_creationFunctions;
public:
	template <class SubClass>
	bool Register(IdType id)
	{
		auto findIt = m_creationFunctions.find(id);
		if(findIt == m_creationFunctions.end())
		{
			m_creationFunctions[id] = & GenericObjectCreationFunction<BaseClass, SubClass>;
			return true;
		}
		return false;
	}

	BaseClass* Create(IdType id)
	{
		auto findIt = m_creationFunctions.find(id);
		if(findIt != m_creationFunctions.end())
		{
			ObjectCreationFunction pFunc = findIt->second;
			return pFunc();
		}
	}
};

#endif