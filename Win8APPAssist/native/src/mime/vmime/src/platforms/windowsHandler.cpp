//
// VMime library (http://vmime.sourceforge.net)
// Copyright (C) 2002-2009 Vincent Richard <vincent@vincent-richard.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Linking this library statically or dynamically with other modules is making
// a combined work based on this library.  Thus, the terms and conditions of
// the GNU General Public License cover the whole combination.
//

#include "vmime/platforms/windowsHandler.hpp"
#include "vmime/config.hpp"

#include <time.h>
#include <locale.h>
#include <process.h>
#include <windows.h>  // for winnls.h

#ifdef VMIME_HAVE_MLANG_H
#   include <mlang.h>
#endif


namespace vmime {
namespace platforms {
namespace windows {


windowsHandler::windowsHandler()
{
}


windowsHandler::~windowsHandler()
{
}


unsigned int windowsHandler::getUnixTime() const
{
	return static_cast <unsigned int>(::time(NULL));
}


const vmime::datetime windowsHandler::getCurrentLocalTime() const
{
	const time_t t(::time(NULL));

	// Get the local time
#if defined(_REENTRANT) && defined(localtime_r)
	tm local;
	::localtime_r(&t, &local);
#else
	tm local = *::localtime(&t);  // WARNING: this is not thread-safe!
#endif

	// Get the UTC time
#if defined(_REENTRANT) && defined(gmtime_r)
	tm gmt;
	::gmtime_r(&t, &gmt);
#else
	tm gmt = *::gmtime(&t);  // WARNING: this is not thread-safe!
#endif

	// "A negative value for tm_isdst causes mktime() to attempt
	//  to determine whether Daylight Saving Time is in effect
	//  for the specified time."
	local.tm_isdst = -1;
	gmt.tm_isdst = -1;

	// Calculate the difference (in seconds)
	const int diff = (const int)(::mktime(&local) - ::mktime(&gmt));

	// Return the date
	return vmime::datetime(local.tm_year + 1900, local.tm_mon + 1, local.tm_mday,
					local.tm_hour, local.tm_min, local.tm_sec, diff / 60);  // minutes needed
}


const vmime::charset windowsHandler::getLocaleCharset() const
{
#ifdef VMIME_HAVE_MLANG_H
	char szCharset[256];

	CoInitialize(NULL);
	{
		IMultiLanguage* pMultiLanguage;
		CoCreateInstance(
			CLSID_CMultiLanguage,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IMultiLanguage,
			(void**)&pMultiLanguage);

		UINT codePage = GetACP();
		MIMECPINFO cpInfo;
		pMultiLanguage->GetCodePageInfo(codePage, &cpInfo);

		int nLengthW = lstrlenW(cpInfo.wszBodyCharset) + 1;

		WideCharToMultiByte(codePage, 0, cpInfo.wszBodyCharset, nLengthW, szCharset, sizeof(szCharset), NULL, NULL );

		pMultiLanguage->Release();

	}
	CoUninitialize();

	return vmime::charset(szCharset);
#else // VMIME_HAVE_MLANG_H
	vmime::string ch = vmime::charsets::GB2312; // default

	// 强制指定代码页为utf-8	 add by qfliu@corp.netease.com  2012.7.22
	UINT code = 936;
	CPINFOEX info;
	if (GetCPInfoEx(CP_ACP, 0, &info))
		code = info.CodePage;

	switch (code)
	{
	case 37: ch = vmime::charsets::IBM037; break;
	case 437: ch = vmime::charsets::IBM437; break;
	case 500: ch = vmime::charsets::IBM500; break;
	case 708: ch = vmime::charsets::ASMO_708; break;
	case 720: ch = vmime::charsets::DOS_720; break;
	case 737: ch = vmime::charsets::IBM737; break;
	case 775: ch = vmime::charsets::IBM775; break;
	case 850: ch = vmime::charsets::IBM850; break;
	case 852: ch = vmime::charsets::IBM852; break;
	case 855: ch = vmime::charsets::IBM855; break;
	case 857: ch = vmime::charsets::IBM857; break;
	case 858: ch = vmime::charsets::IBM00858; break;
	case 860: ch = vmime::charsets::IBM860; break;
	case 861: ch = vmime::charsets::IBM861; break;
	case 862: ch = vmime::charsets::DOS_862; break;
	case 863: ch = vmime::charsets::IBM863; break;
	case 864: ch = vmime::charsets::IBM864; break;
	case 865: ch = vmime::charsets::IBM865; break;
	case 866: ch = vmime::charsets::CP866; break;
	case 869: ch = vmime::charsets::IBM869; break;
	case 870: ch = vmime::charsets::IBM870; break;
	case 874: ch = vmime::charsets::WINDOWS_874; break;
	case 875: ch = vmime::charsets::CP875; break;
	case 932: ch = vmime::charsets::SHIFT_JIS; break;
	//case 936: ch = vmime::charsets::GB2312; break;
	case 936: ch = vmime::charsets::GB18030; break;
	case 949: ch = vmime::charsets::KS_C_5601_1987; break;
	case 950: ch = vmime::charsets::BIG5; break;
	case 1026: ch = vmime::charsets::IBM1026; break;
	case 1047: ch = vmime::charsets::IBM01047; break;
	case 1140: ch = vmime::charsets::IBM01140; break;
	case 1141: ch = vmime::charsets::IBM01141; break;
	case 1142: ch = vmime::charsets::IBM01142; break;
	case 1143: ch = vmime::charsets::IBM01143; break;
	case 1144: ch = vmime::charsets::IBM01144; break;
	case 1145: ch = vmime::charsets::IBM01145; break;
	case 1146: ch = vmime::charsets::IBM01146; break;
	case 1147: ch = vmime::charsets::IBM01147; break;
	case 1148: ch = vmime::charsets::IBM01148; break;
	case 1149: ch = vmime::charsets::IBM01149; break;
	case 1200: ch = vmime::charsets::UTF_16; break;
	case 1201: ch = vmime::charsets::UNICODEFFFE; break;
	case 1250: ch = vmime::charsets::WINDOWS_1250; break;
	case 1251: ch = vmime::charsets::WINDOWS_1251; break;
	case 1252: ch = vmime::charsets::WINDOWS_1252; break;
	case 1253: ch = vmime::charsets::WINDOWS_1253; break;
	case 1254: ch = vmime::charsets::WINDOWS_1254; break;
	case 1255: ch = vmime::charsets::WINDOWS_1255; break;
	case 1256: ch = vmime::charsets::WINDOWS_1256; break;
	case 1257: ch = vmime::charsets::WINDOWS_1257; break;
	case 1258: ch = vmime::charsets::WINDOWS_1258; break;
	case 1361: ch = vmime::charsets::JOHAB; break;
	case 10000: ch = vmime::charsets::MACINTOSH; break;
	case 10001: ch = vmime::charsets::X_MAC_JAPANESE; break;
	case 10002: ch = vmime::charsets::X_MAC_CHINESETRAD; break;
	case 10003: ch = vmime::charsets::X_MAC_KOREAN; break;
	case 10004: ch = vmime::charsets::X_MAC_ARABIC; break;
	case 10005: ch = vmime::charsets::X_MAC_HEBREW; break;
	case 10006: ch = vmime::charsets::X_MAC_GREEK; break;
	case 10007: ch = vmime::charsets::X_MAC_CYRILLIC; break;
	case 10008: ch = vmime::charsets::X_MAC_CHINESESIMP; break;
	case 10010: ch = vmime::charsets::X_MAC_ROMANIAN; break;
	case 10017: ch = vmime::charsets::X_MAC_UKRAINIAN; break;
	case 10021: ch = vmime::charsets::X_MAC_THAI; break;
	case 10029: ch = vmime::charsets::X_MAC_CE; break;
	case 10079: ch = vmime::charsets::X_MAC_ICELANDIC; break;
	case 10081: ch = vmime::charsets::X_MAC_TURKISH; break;
	case 10082: ch = vmime::charsets::X_MAC_CROATIAN; break;
	case 12000: ch = vmime::charsets::UTF_32; break;
	case 12001: ch = vmime::charsets::UTF_32BE; break;
	case 20000: ch = vmime::charsets::X_CHINESE_CNS; break;
	case 20001: ch = vmime::charsets::X_CP20001; break;
	case 20002: ch = vmime::charsets::X_CHINESE_ETEN; break;
	case 20003: ch = vmime::charsets::X_CP20003; break;
	case 20004: ch = vmime::charsets::X_CP20004; break;
	case 20005: ch = vmime::charsets::X_CP20005; break;
	case 20105: ch = vmime::charsets::X_IA5; break;
	case 20106: ch = vmime::charsets::X_IA5_GERMAN; break;
	case 20107: ch = vmime::charsets::X_IA5_SWEDISH; break;
	case 20108: ch = vmime::charsets::X_IA5_NORWEGIAN; break;
	case 20127: ch = vmime::charsets::US_ASCII; break;
	case 20261: ch = vmime::charsets::X_CP20261; break;
	case 20269: ch = vmime::charsets::X_CP20269; break;
	case 20273: ch = vmime::charsets::IBM273; break;
	case 20277: ch = vmime::charsets::IBM277; break;
	case 20278: ch = vmime::charsets::IBM278; break;
	case 20280: ch = vmime::charsets::IBM280; break;
	case 20284: ch = vmime::charsets::IBM284; break;
	case 20285: ch = vmime::charsets::IBM285; break;
	case 20290: ch = vmime::charsets::IBM290; break;
	case 20297: ch = vmime::charsets::IBM297; break;
	case 20420: ch = vmime::charsets::IBM420; break;
	case 20423: ch = vmime::charsets::IBM423; break;
	case 20424: ch = vmime::charsets::IBM424; break;
	case 20833: ch = vmime::charsets::X_EBCDIC_KOREANEXTENDED; break;
	case 20838: ch = vmime::charsets::IBM_THAI; break;
	case 20866: ch = vmime::charsets::KOI8_R; break;
	case 20871: ch = vmime::charsets::IBM871; break;
	case 20880: ch = vmime::charsets::IBM880; break;
	case 20905: ch = vmime::charsets::IBM905; break;
	case 20924: ch = vmime::charsets::IBM00924; break;
	case 20932: ch = vmime::charsets::EUC_JP; break;
	case 20936: ch = vmime::charsets::X_CP20936; break;
	case 20949: ch = vmime::charsets::X_CP20949; break;
	case 21025: ch = vmime::charsets::CP1025; break;
	case 21866: ch = vmime::charsets::KOI8_U; break;
	case 28591: ch = vmime::charsets::ISO_8859_1; break;
	case 28592: ch = vmime::charsets::ISO_8859_2; break;
	case 28593: ch = vmime::charsets::ISO_8859_3; break;
	case 28594: ch = vmime::charsets::ISO_8859_4; break;
	case 28595: ch = vmime::charsets::ISO_8859_5; break;
	case 28596: ch = vmime::charsets::ISO_8859_6; break;
	case 28597: ch = vmime::charsets::ISO_8859_7; break;
	case 28598: ch = vmime::charsets::ISO_8859_8; break;
	case 28599: ch = vmime::charsets::ISO_8859_9; break;
	case 28603: ch = vmime::charsets::ISO_8859_13; break;
	case 28605: ch = vmime::charsets::ISO_8859_15; break;
	case 29001: ch = vmime::charsets::X_EUROPA; break;
	case 38598: ch = vmime::charsets::ISO_8859_8_I; break;
	case 50220: ch = vmime::charsets::ISO_2022_JP; break;
	case 50221: ch = vmime::charsets::CSISO2022JP; break;
	case 50222: ch = vmime::charsets::ISO_2022_JP2; break;
	case 50225: ch = vmime::charsets::ISO_2022_KR; break;
	case 50227: ch = vmime::charsets::X_CP50227; break;
	case 51932: ch = vmime::charsets::EUC_JP2; break;
	case 51936: ch = vmime::charsets::EUC_CN; break;
	case 51949: ch = vmime::charsets::EUC_KR; break;
	case 52936: ch = vmime::charsets::HZ_GB_2312; break;
	case 54936: ch = vmime::charsets::GB18030; break;
	case 57002: ch = vmime::charsets::X_ISCII_DE; break;
	case 57003: ch = vmime::charsets::X_ISCII_BE; break;
	case 57004: ch = vmime::charsets::X_ISCII_TA; break;
	case 57005: ch = vmime::charsets::X_ISCII_TE; break;
	case 57006: ch = vmime::charsets::X_ISCII_AS; break;
	case 57007: ch = vmime::charsets::X_ISCII_OR; break;
	case 57008: ch = vmime::charsets::X_ISCII_KA; break;
	case 57009: ch = vmime::charsets::X_ISCII_MA; break;
	case 57010: ch = vmime::charsets::X_ISCII_GU; break;
	case 57011: ch = vmime::charsets::X_ISCII_PA; break;
	case 65000: ch = vmime::charsets::UTF_7; break;
	case 65001: ch = vmime::charsets::UTF_8; break;
	}

	return (vmime::charset(ch));
#endif
}

unsigned int windowsHandler::getProcessId() const
{
	return (static_cast <unsigned int>(::GetCurrentProcessId()));
}

} // posix
} // platforms
} // vmime
