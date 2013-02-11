/* Jatta - General Utility Library
 * Copyright (C) 2012-2013, Joshua Brookover and Amber Thrall
 * All rights reserved.
 */

#include "Thread.h"

static void runThread(Jatta::Thread*ptr)
{
    ptr->Main();
}

_JATTA_EXPORT Jatta::Thread::Thread()
{
    
}
_JATTA_EXPORT Jatta::Thread::~Thread()
{
    delete thread;
}

_JATTA_EXPORT void Jatta::Thread::Run()
{
    thread = new std::thread(runThread, this);
}

_JATTA_EXPORT void Jatta::Thread::Join()
{
    thread->join();
}
_JATTA_EXPORT void Jatta::Thread::Detach()
{
    thread->detach();
}