#pragma once
using namespace Platform;

namespace native
{
	public ref class Util sealed
	{
	public:
		Util();
		void ExceptionForCallStack();
		bool IsInDebugMode();

		String^ passEncode(String^ strPlaint);
		String^ passDecode(String^ strEncryp);
		static String^ newGuid();
	};
}