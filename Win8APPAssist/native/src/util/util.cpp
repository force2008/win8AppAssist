// mime.cpp
#include "pch.h"
#include "util.h"
#include "aes\aesTool.h"
#include "mimeutil.h"

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

String^ Util::passEncode(String^ strPlaint)
{
	std::string strEncryp;
	AESTool::AESBase64Encrypt(StringToStdString(strPlaint), strEncryp);
	return StdStringToString(strEncryp);
}

String^ Util::passDecode(String^ strEncryp)
{
	std::string strPlaint;
	AESTool::AESBase64Decrypt(StringToStdString(strEncryp), strPlaint);
	return StdStringToString(strPlaint);
}

String^ Util::newGuid() {
	GUID guid;
	CoCreateGuid(&guid);
	return ((Guid)guid).ToString();
}
}