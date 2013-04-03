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

	};
}