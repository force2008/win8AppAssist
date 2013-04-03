// mime.cpp
#include "pch.h"
#include "util.h"


namespace native
{
Util::Util()
{
}

void Util::ExceptionForCallStack()
{
	throw ref new Platform::COMException(0x80cdcdcd, "ExceptionForCallStackNative");
}

bool Util::IsInDebugMode()
{
#ifdef NDEBUG
	return false;
#else
	return true;
#endif
}
}