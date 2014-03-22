// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
//#define _DEBUG
#define _CRT_SECURE_NO_WARNINGS

//Exclude rarely used stuff from Windows headers
//#define WIN32_LEAN_AND_MEAN

#include <crtdbg.h>

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>

#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>

//Define using shared_ptr

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;

class GCC_noncopyable
{
private:  
      GCC_noncopyable(const GCC_noncopyable& x);  
      GCC_noncopyable& operator=(const GCC_noncopyable& x);  
public:  
      GCC_noncopyable() {}; // Default constructor  
};

//Fast delegate for the event manager

#include "3rdParty/FastDelegate/FastDelegate.h"
using fastdelegate::MakeDelegate;
