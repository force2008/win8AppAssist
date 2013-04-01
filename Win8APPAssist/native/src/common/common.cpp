#include "pch.h"
#include "common.h"
#include <Windows.h>

std::wstring common_s2ws(const std::string& s)
{
	const char* _Source = s.c_str();
	size_t _Dsize = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);
	wchar_t *_Dest = new wchar_t[_Dsize];
	MultiByteToWideChar(CP_ACP, 0, _Source, -1, _Dest, _Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	return result;
}

std::string common_ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}