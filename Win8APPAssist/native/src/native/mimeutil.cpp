#include <pch.h>
#include "windows.h"
#include "vmime/constants.hpp"
#include "vmime/contentTypeField.hpp"
#include "vmime/contentDispositionField.hpp"
#include "vmime/charset.hpp"
#include "mimeutil.h"
#include <collection.h>

//#define LOCAL_DEBUG

namespace WFC = Windows::Foundation::Collections;
namespace WF = Windows::Foundation;

wstring s2ws(const string& s, UINT nCodePage)
{
    const char* _Source = s.c_str();
    size_t _Dsize = MultiByteToWideChar(nCodePage, 0, _Source, -1, NULL, 0);
    wchar_t *_Dest = new wchar_t[_Dsize];
    MultiByteToWideChar(nCodePage, 0, _Source, -1, _Dest, _Dsize);
    wstring result = _Dest;
    delete []_Dest;
    return result;
}

std::string ws2s(const std::wstring& ws)
{
	int nLen = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, NULL, 0, NULL, NULL); 
    if (nLen<= 0) return std::string(""); 
    char* pszDst = new char[nLen]; 
    if (NULL == pszDst) return std::string(""); 
    WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, pszDst, nLen, NULL, NULL); 
    pszDst[nLen -1] = 0; 
    std::string strTemp(pszDst); 
    delete [] pszDst; 
    return strTemp; 
}

UINT GetCodePageByString(const std::string& str)
{
	if (str.compare(vmime::charsets::UTF_8) == 0)
		return 65001;
	else if (str.compare(vmime::charsets::GB2312) == 0)
		return 936;
	else if (str.compare(vmime::charsets::BIG5) == 0)
		return 950;

	else return 65001;
}

String^ ArrayToString(const Array<uint8>^ buffer)
{
	std::string str((char*)(&buffer[0]), buffer->Length);
	std::wstring ws = s2ws(str, GetCodePageByString(FuzzyCharsetIdentification(str, vmime::charset::getLocaleCharset().getName())));
	String^ result = ref new String(ws.c_str());
	std::wstring s = result->Data();
	return result;
}

String^ StdStringToString(const string& s)
{
	std::wstring ws = s2ws(s);
	return ref new String(ws.c_str(), ws.length());
}

std::string StringToStdString(String^ s)
{
	std::string str = ws2s(s->Data());
	return str;
}

std::string ArrayToStdString(const Array<uint8>^ buffer)
{
	std::string str((char*)(&buffer[0]), buffer->Length);
	return str;
}

Array<uint8>^ StringToArray(String^ str)
{
	std::string s = ws2s(str->Data());
	return ref new Platform::Array<uint8>((uint8*)s.data(), s.size());
}

Array<uint8>^ StdStringToArray(const std::string& str)
{
	return ref new Platform::Array<uint8>((uint8*)str.data(), str.size());
}

/*
UTF-8��һ�ֶ��ֽڱ�����ַ�������ʾһ��Unicode�ַ�ʱ����������1��������ֽڣ��ڱ�ʾ���й��ɣ�

1�ֽڣ�0xxxxxxx
2�ֽڣ�110xxxxx 10xxxxxx
3�ֽڣ�1110xxxx 10xxxxxx 10xxxxxx
4�ֽڣ�11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

http://www.diybl.com/course/3_program/vc/vc_js/20090220/156034.html
*/
bool IsUTF8Charset(const std::string& src)
{
	unsigned char* start = (unsigned char*)src.c_str();
	unsigned char* end = (unsigned char*)src.c_str() + src.length();
	while(start < end)
	{
		if (*start < 0x80)
			start++;
		else if (*start < (0xC0))
			return false;
		else if (*start < (0xE0))
		{
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80)
				return false;
			start += 2;
		}
		else if (*start < (0xF0))
		{
			if (start >= end - 2)
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
				return false;
			start += 3;
		}
		else
			return false;
	}
	return true;
}

bool IsUTF8Charset_table(const std::string& src)
{
	static const int utf8_map[256] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0
	};

	int i, j, k;
	int length = src.length();
	const unsigned char* stream = (const unsigned char*)src.c_str();

	for (i = 0; i < length; )
	{
		k = utf8_map[stream[i]];
		if (k == 0)
			break;
		for (j = 1; j < k; j++)
			if (utf8_map[stream[i + j]] != 0)
				return false;
		i += j;
	}
	return i == (int)src.length();
}

/*
����λ�ֽڡ�ʹ����0x81-0xFE������λ�ֽڡ�ʹ����0x40-0x7E����0xA1-0xFE
lead bytes of byte pairs in range 0xA1-0xF9 followed by trail bytes in the range 0x40-0xFE.

http://zh.wikipedia.org/w/index.php?title=����a&variant=zh-hans
http://interscript.sourceforge.net/interscript/doc/en_big5_0001.html
*/
bool IsBig5Charset(const std::string& src)
{
	unsigned char* start = (unsigned char*)src.c_str();
	unsigned char* end = (unsigned char*)src.c_str() + src.length();
	while(start < end)
	{
		if (0x00 <= *start && *start <= 0x7F)
		{
			// ����ascii
			start ++;
			continue;
		}
		if (! (0xA1 <= *start && *start <= 0xF9))
			return false;
		start ++;
		if (! ((0x40 <= *start && *start <= 0x7E) || (0xA1 <= *start && *start <= 0xFE)))
			return false;
		start ++;
	}
	return true;
}

/*
����λ�ֽڡ�ʹ����0xA1-0xF7����01-87�������ż���0xA0��������λ�ֽڡ�ʹ����0xA1-0xFE����01-94����0xA0��
����һ�����ִ�16����ʼ���������ġ���λ�ֽڡ��ķ�Χ��0xB0-0xF7������λ�ֽڡ��ķ�Χ��0xA1-0xFE

http://zh.wikipedia.org/wiki/GB_2312

�ַ���һ�ֽں�˫�ֽڱ��룬00�C7F��Χ����һλ����ASCII����һ�£��˷�Χ���ϸ���˵��96�����ֺ�32�����Ʒ��š�
֮���˫�ֽ��У�ǰһ�ֽ���˫�ֽڵĵ�һλ��
������˵��һ�ֽڵķ�Χ��81�CFE��Ҳ���ǲ���80��FF�����ڶ��ֽڵ�һ����������40�CFE������������80�CFE��

http://zh.wikipedia.org/zh-cn/GBK
*/
#define DeInRange(x,a,b) (x)>=(a)&&(x)<=(b)
bool IsGB2312Charset(const std::string& src)
{
	unsigned char* start = (unsigned char*)src.c_str();
	unsigned char* end = (unsigned char*)src.c_str() + src.length();
	while(start < end)
	{
		if (0x00 <= *start && *start <= 0x7F)
		{
			// ����ascii
			start ++;
			continue;
		}
		//ˮ׼ GBK/1	A1�CA9	A1�CFE	846	717
		//ˮ׼ GBK/2	B0�CF7	A1�CFE	6,768	6,763
		//ˮ׼ GBK/3	81�CA0	40�CFE (7F����)	6,080	6,080
		//ˮ׼ GBK/4	AA�CFE	40�CA0 (7F����)	8,160	8,160
		//ˮ׼ GBK/5	A8�CA9	40�CA0 (7F����)	192	166
		if (start + 2 > end)
			return false;
		if (!(DeInRange(*(start+1),0xa1,0xfe)&&(DeInRange(*start,0xb0,0xf7)||DeInRange(*start,0xa1,0xa9)))&&
			!(DeInRange(*start,0x81,0xa0)&&*(start+1)!=0x7f&&DeInRange(*(start+1),0x40,0xfe))&&
			!(DeInRange(*(start+1),0x40,0xa0)&&*(start+1)!=0x7f&&(DeInRange(*start,0xaa,0xfe)||DeInRange(*start,0xa8,0xa9))))
		{
			return false;
		}
		start += 2;

		//if (! (0x81 <= *start && *start <= 0xF7))
		//	return false;
		//start ++;
		//if (! (0x40 <= *start && *start <= 0xFE))
		//	return false;
		//start ++;
	}
	return start == end;
}


/*
lead bytes of byte pairs in range 0x21-0x7D followed by trail bytes in the range 0x21-0x7E

http://interscript.sourceforge.net/interscript/doc/en_iscr_0002.html
http://interscript.sourceforge.net/interscript/doc/en_ksc5601_1987_0001.html
*/
bool IsKSC_5601_1987Charset(const std::string& src)
{
	unsigned char* start = (unsigned char*)src.c_str();
	unsigned char* end = (unsigned char*)src.c_str() + src.length();
	while(start < end)
	{
		if (0x00 <= *start && *start <= 0x7F)
		{
			// ����ascii
			start ++;
			continue;
		}
		if (! (0x21 <= *start && *start <= 0x7D))
			return false;
		start ++;
		if (! (0x21 <= *start && *start <= 0x7E))
			return false;
		start ++;
	}
	return true;
}

/*
ShiftJis is mapped with three tables: a single linear table covering the range A0 to BF corresponding to Kana script, 
and two rectangular tables with lead bytes in the ranges 81 to 9F and E0 to FC and trail bytes in the range 40 to FC.
The bytes FD, FE, and FF are not used.

http://en.wikipedia.org/wiki/Shift_JIS
http://interscript.sourceforge.net/interscript/doc/en_shiftjis_0001.html
*/
bool IsShift_JISCharset(const std::string& src)
{
	unsigned char* start = (unsigned char*)src.c_str();
	unsigned char* end = (unsigned char*)src.c_str() + src.length();
	while(start < end)
	{
		if (0x00 <= *start && *start <= 0x7F)
		{
			// ����ascii
			start ++;
			continue;
		}
		if (0xA0 <= *start && *start <= 0xBF)
		{
			start ++;
			continue;
		}
		if (! ((0x81 <= *start && *start <= 0x9F) || (0xE0 <= *start && *start <= 0xFC)))
			return false;
		start ++;
		if (! (0x40 <= *start && *start <= 0xFC))
			return false;
		start ++;
	}
	return true;
}

/*
��λ1�Х��ȡ� 0x21��0x7e 
��λ1�Х��ȡ� 0x21��0x7e 

http://charset.7jp.net/jis.html
http://en.wikipedia.org/wiki/ISO/IEC_2022
*/
bool IsISO_2022_JPCharset(const std::string& src)
{
	unsigned char* start = (unsigned char*)src.c_str();
	unsigned char* end = (unsigned char*)src.c_str() + src.length();
	while(start < end)
	{
		if (0x00 <= *start && *start <= 0x7F)
		{
			// ����ascii
			start ++;
			continue;
		}
		if (! (0x21 <= *start && *start <= 0x7e))
			return false;
		start ++;
	}
	return true;
}

std::string FuzzyCharsetIdentification(const std::string& src, const std::string& sDefaultCharset)
{
	if (IsGB2312Charset(src))
		return vmime::charsets::GB2312;
	if (IsUTF8Charset_table(src))
		return vmime::charsets::UTF_8;
	if (IsBig5Charset(src))
		return vmime::charsets::BIG5;
	if (IsKSC_5601_1987Charset(src))
		return vmime::charsets::KS_C_5601_1987;
	if (IsShift_JISCharset(src))
		return vmime::charsets::SHIFT_JIS;
	if (IsISO_2022_JPCharset(src))
		return vmime::charsets::ISO_2022_JP;

	return sDefaultCharset;
}

void ProcessMailbox(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					vmime::ref <vmime::mailbox>& mb,
					vmime::charset& ch)
{
	auto mapAddrs = ref new Collections::Map<String^, String^>();

	std::string value = GetCorrectText(mb->getName(), ch, hdr);

	String^ str = StdStringToString(value);

	mapAddrs->Insert("NAME", StdStringToString(value));
	mapAddrs->Insert("ADDR", StdStringToString(mb->getEmail()));
	mapHeader.insert(std::pair<String^, Object^>(StdStringToString(name), mapAddrs));
}

void ProcessMailboxList(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					vmime::ref <vmime::mailboxList>& mblist,
					vmime::charset& ch)
{
	if (mblist->getMailboxCount() == 0)
		return;

	auto arr =  ref new Array<Object^>(mblist->getMailboxCount());
	for (int i=0; i<mblist->getMailboxCount(); i++)
	{
		vmime::ref <vmime::mailbox>& mb = mblist->getMailboxAt(i);
		auto mapMailbox = ref new Collections::Map<String^, String^>();

		std::string value = GetCorrectText(mb->getName(), ch, hdr);

		mapMailbox->Insert("NAME", StdStringToString(value));
		mapMailbox->Insert("ADDR", StdStringToString(mb->getEmail()));
		arr[i] = mapMailbox;
	}

	mapHeader.insert(std::pair<String^, Object^>(StdStringToString(name), arr));
}

void ProcessDateTime(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					vmime::ref <vmime::datetime> & date)
{
	ProcessDateTime(hdr, mapHeader, name, *date.get());
}

void ProcessDateTime(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					const vmime::datetime & date)
{
	Windows::Foundation::DateTime TimeStamp;
	auto cal = ref new Windows::Globalization::Calendar();
	cal->Year = date.getYear();
	cal->Month = date.getMonth();
	cal->Day = date.getDay();
	cal->Minute = date.getMinute();
	cal->Second = date.getSecond();

	//workaround
	int hour = date.getHour();
	if (cal->NumberOfHoursInThisPeriod == 24) {
		cal->Hour = hour;
	} else {
		cal->Period = 1 + hour / 12; // 1->am, 2->pm
		int periodHour = hour % 12;
		if (periodHour == 0) {
			periodHour = 12; // amend 0:00 to 12:00 am; 12:00 to 12:00 pm
		}
		cal->Hour = periodHour;
	}
	TimeStamp = cal->GetDateTime();
	mapHeader.insert(std::pair<String^, Object^>(StdStringToString(name), TimeStamp));
}

std::string& ToUpperCase(std::string & s)
{
	transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}

bool HasAsciiChar(const std::string& s)
{
	std::string::size_type n = vmime::utility::stringUtils::findFirstNonASCIIchar(s.begin(), s.end());
	return (n != std::string::npos);
}

vmime::word GetFormattedWord(const std::string& s)
{
	vmime::word wName(s, vmime::charset(vmime::charsets::UTF_8));
	if (HasAsciiChar(s))
		wName.setBuffer("=?UTF-8?B?"+Base64Encode(StdStringToString(s))+"?=");
	return wName;
}

std::string GetCorrectText(const vmime::text& txt, vmime::charset& ch, vmime::ref <vmime::header> & hdr)
{
	// �ж��Ƿ�ÿ���ֶζ�û��charset��Ϣ
	bool bNoCharset = true;
	for (int i=0; i<txt.getWordCount(); i++)
	{
		if (txt.getWordAt(i)->getCharset().getName().compare(vmime::charsets::US_ASCII) != 0)
			bNoCharset = false;
	}

	try 
	{
		if (!txt.isEmpty() && bNoCharset)
		{
			vmime::ref <vmime::text> t = vmime::create<vmime::text>(txt.getWholeBuffer(), /*chSrc*/
						FuzzyCharsetIdentification(txt.getWholeBuffer(), vmime::charset::getLocaleCharset().getName()));
			return t->getConvertedText(ch);
		}
		else
		{
			return txt.getConvertedText(ch);
		}
	}
	catch (vmime::exceptions::charset_conv_error)
	{
		vmime::ref <vmime::text> t = vmime::create<vmime::text>(txt.getWholeBuffer(), vmime::charset::getLocaleCharset());
		return t->getConvertedText(ch);
	}
}

std::string GetCorrectText(vmime::ref <vmime::text> txt, vmime::charset& ch, vmime::ref <vmime::header> & hdr)
{
	// �ж��Ƿ�ÿ���ֶζ�û��charset��Ϣ
	bool bNoCharset = true;
	for (int i=0; i<txt->getWordCount(); i++)
	{
		if (txt->getWordAt(i)->getCharset().getName().compare(vmime::charsets::US_ASCII) != 0)
			bNoCharset = false;
	}

	try
	{
		if (!txt->isEmpty() && bNoCharset)
		{
			vmime::ref <vmime::text> t = vmime::create<vmime::text>(txt->getWholeBuffer(), /*chSrc*/
						FuzzyCharsetIdentification(txt->getWholeBuffer(), vmime::charset::getLocaleCharset().getName()));
			return t->getConvertedText(ch);
		}
		else
		{
			return txt->getConvertedText(ch);
		}
	}
	catch (vmime::exceptions::charset_conv_error)
	{
		vmime::ref <vmime::text> t = vmime::create<vmime::text>(txt->getWholeBuffer(), vmime::charset::getLocaleCharset());
		return t->getConvertedText(ch);
	}
}

std::string GetCorrectText(const vmime::word& wd, vmime::charset& ch)
{
	if (wd.getCharset().getName().compare(vmime::charsets::US_ASCII) == 0)
	{
		vmime::ref <vmime::text> t = vmime::create<vmime::text>(wd.getBuffer(),
				FuzzyCharsetIdentification(wd.getBuffer(), vmime::charset::getLocaleCharset().getName()));
			return t->getConvertedText(ch);
	}
	else
	{
		return wd.getConvertedText(ch);
	}
}

#include "vmime/utility/encoder/b64Encoder.hpp"
using namespace vmime::utility::encoder;

std::string Base64Encode(const char* c, int nLength)
{
	vmime::ref <b64Encoder> enc = vmime::create<b64Encoder>();
	enc->getProperties().setProperty<int>("maxlinelength", -1);
	
	std::string outbuf;
	vmime::utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc->encode(c, nLength, out, NULL);
	}
	catch (vmime::exceptions::no_encoder_available ex)
	{
		return "";
	}

	return outbuf;
}

std::string Base64Encode(const std::string& sText)
{
	vmime::encoding enc(vmime::encodingTypes::BASE64);
	enc.getEncoder()->getProperties().setProperty<int>("maxlinelength", -1);
	vmime::utility::inputStreamStringAdapter in(sText.c_str(), 0, sText.length());
	
	std::string outbuf;
	vmime::utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc.getEncoder()->encode(in, out, NULL);
	}
	catch (vmime::exceptions::no_encoder_available ex)
	{
		return "";
	}

	return outbuf;
}

std::string Base64Decode(const std::string& sText)
{
	vmime::encoding enc(vmime::encodingTypes::BASE64);
	enc.getEncoder()->getProperties().setProperty<int>("maxlinelength", -1);
	vmime::utility::inputStreamStringAdapter in(sText.c_str(), 0, sText.length());
	
	std::string outbuf;
	vmime::utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc.getEncoder()->decode(in, out, NULL);
	}
	catch (vmime::exceptions::no_encoder_available ex)
	{
		return "";
	}

	return outbuf;
}

std::string Base64Encode(String^ sText)
{
	std::string str = StringToStdString(sText);
	return Base64Encode(str);
}

std::string Base64Decode(String^ sText)
{
	std::string str = StringToStdString(sText);
	return Base64Decode(str);
}

std::string Base64Encode(const Array<uint8>^ buffer)
{
	vmime::encoding enc(vmime::encodingTypes::BASE64);
	enc.getEncoder()->getProperties().setProperty<int>("maxlinelength", -1);
	vmime::utility::inputStreamStringAdapter in((char*)&buffer[0], 0, buffer->Length-1);
	
	std::string outbuf;
	vmime::utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc.getEncoder()->encode(in, out, NULL);
	}
	catch (vmime::exceptions::no_encoder_available ex)
	{
		return "";
	}

	return outbuf;
}

std::string Base64Decode(const Array<uint8>^ buffer)
{
	vmime::encoding enc(vmime::encodingTypes::BASE64);
	enc.getEncoder()->getProperties().setProperty<int>("maxlinelength", -1);
	vmime::utility::inputStreamStringAdapter in((char*)&buffer[0], 0, buffer->Length-1);
	
	std::string outbuf;
	vmime::utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc.getEncoder()->decode(in, out, NULL);
	}
	catch (vmime::exceptions::no_encoder_available ex)
	{
		return "";
	}

	return outbuf;
}

const unsigned int BASE64_DECODE_TABLE[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, //  00 -  07
	255, 255, 255, 255, 255, 255, 255, 255, //  08 -  15
	255, 255, 255, 255, 255, 255, 255, 255, //  16 -  23
	255, 255, 255, 255, 255, 255, 255, 255, //  24 -  31
	255, 255, 255, 255, 255, 255, 255, 255, //  32 -  39
	255, 255, 255,  62, 63,  255, 255, 255, //  40 -  47    ע�⣬63ֵ�ɵ�47��ֲ��44
	52,  53,  54,  55,  56,  57,  58,  59, //  48 -  55
	60,  61, 255, 255, 255, 255, 255, 255, //  56 -  63
	255,   0,   1,   2,   3,   4,   5,   6, //  64 -  71
	7,   8,   9,  10,  11,  12,  13,  14, //  72 -  79
	15,  16,  17,  18,  19,  20,  21,  22, //  80 -  87
	23,  24,  25, 255, 255, 255, 255, 255, //  88 -  95
	255,  26,  27,  28,  29,  30,  31,  32, //  96 - 103
	33,  34,  35,  36,  37,  38,  39,  40, // 104 - 111
	41,  42,  43,  44,  45,  46,  47,  48, // 112 - 119
	49,  50,  51, 255, 255, 255, 255, 255, // 120 - 127
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255 };

const char BASE64_ENCODE_TABLE[64] = {
	65,  66,  67,  68,  69,  70,  71,  72,   // 00 - 07
	73,  74,  75,  76,  77,  78,  79,  80,   // 08 - 15
	81,  82,  83,  84,  85,  86,  87,  88,   // 16 - 23
	89,  90,  97,  98,  99, 100, 101, 102,   // 24 - 31
	103, 104, 105, 106, 107, 108, 109, 110,  // 32 - 39
	111, 112, 113, 114, 115, 116, 117, 118,  // 40 - 47
	119, 120, 121, 122,  48,  49,  50,  51,  // 48 - 55
	52,  53,  54,  55,  56,  57,  43,  44    // 56 - 63 ע�����һ����47(/)��Ϊ��44(,)
};

std::wstring GetMid(const std::wstring & str, int nStart, int nEnd)
{
	return std::wstring(str.data()+nStart, nEnd - nStart);
}

bool toModifiedUTF7(const std::wstring& text, std::string& sDest)
{
	sDest.clear();
	//��wstring���ַ����ֽ���ߵ�
	//CString csSrcInter;
	PWCHAR pBuffer = new WCHAR[text.length()];
	//PWCHAR pBuffer = csSrcInter.GetBufferSetLength(csSrc.GetLength());
	for (int i = 0; i < (int)text.length(); ++i)
		pBuffer[i] = (text[i] << 8) | (text[i] >> 8);

	//��������buffer�Ĵ�С
	int nLen = text.length();
	int nLastLen = nLen * 2 % 3;
	int nDestLen = nLen * 2 / 3 * 4;
	if (nLastLen > 0)
		nDestLen += nLastLen + 1;
	sDest.resize((UINT)nDestLen);

	//base64����
	const unsigned char* pSrc = (const unsigned char*)(LPCTSTR)pBuffer;
	const unsigned char* pCur = pSrc;
	const unsigned char* pEnd = pSrc + nLen * 2;
	const unsigned char* pScanEnd = pSrc + (nLen * 2 - nLastLen);
	char* pDest = (char*)sDest.data();
	while(pCur < pScanEnd)
	{
		//��ȡ
		UINT treBytes = 0;		//���ֽڵ�����
		treBytes |= *pCur++;
		treBytes <<= 8;
		treBytes |= *pCur++;
		treBytes <<= 8;
		treBytes |= *pCur++;

		//����
		UINT forBytes = 0;		//���ֽ�����
		forBytes = BASE64_ENCODE_TABLE[treBytes & 0x3F] << 24;
		treBytes >>= 6;
		forBytes |= BASE64_ENCODE_TABLE[treBytes & 0x3F] << 16;
		treBytes >>= 6;
		forBytes |= BASE64_ENCODE_TABLE[treBytes & 0x3F] << 8;
		treBytes >>= 6;
		forBytes |= BASE64_ENCODE_TABLE[treBytes & 0x3F];

		//�洢
		*(UINT*)pDest = forBytes;
		pDest += 4;
	}

	//β��
	if(pEnd - pScanEnd == 1)
	{
		UINT singleByte = *pCur;
		singleByte <<= 4;
		*pDest++ = BASE64_ENCODE_TABLE[(singleByte >> 6) & 0x3F];
		*pDest++ = BASE64_ENCODE_TABLE[singleByte & 0x03F];
	}
	else if(pEnd - pScanEnd == 2)
	{
		UINT twoBytes = *pCur++;
		twoBytes <<= 8;
		twoBytes |= *pCur++;
		twoBytes <<= 2;
		*pDest++ = BASE64_ENCODE_TABLE[(twoBytes >> 12) & 0x3F];
		*pDest++ = BASE64_ENCODE_TABLE[(twoBytes >> 6 ) & 0x3F];
		*pDest++ = BASE64_ENCODE_TABLE[twoBytes & 0x3F];
	}

	delete [] pBuffer;

	return true;
}

bool fromModifiedUTF7(const std::string& text, std::wstring& ws)
{
	//��������buffer�Ĵ�С
	UINT nSrcLen = text.length();
	UINT nLastLen = nSrcLen % 4;			//���ȳ�4��1�ǲ��Ϸ���
	int nDestLen = nSrcLen / 4 * 3;
	if (nLastLen != 0)
		nDestLen += nLastLen - 1;

	PWCHAR pDestBuffer = new WCHAR[nDestLen / 2+1];

	//base64����
	unsigned char* pDest = (unsigned char*)pDestBuffer;
	const char* pSrc = text.data();
	const char* pCur = pSrc;
	const char* pEnd = pSrc + nSrcLen;
	const char* pScanEnd = pSrc + nSrcLen - nLastLen;
	while( pCur < pScanEnd)
	{
		//���ĸ��ֽڣ�ͳһת�������ֽڵ�ԭʼ��Ϣ
		UINT treBytes = 0;
		UINT nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		treBytes |= nRes << 18;
		nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		treBytes |= nRes << 12;
		nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		treBytes |= nRes << 6;
		nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		treBytes |= nRes;

		//�洢
		*pDest++ = (treBytes >> 16) & 0xFF;
		*pDest++ = (treBytes >> 8)  & 0xFF;
		*pDest++ = (treBytes) & 0xFF;
	}

	//β��
	if (pEnd - pScanEnd == 2)
	{
		UINT singleByte = 0;
		UINT nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		singleByte |= nRes << 6;
		nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		singleByte |= nRes;

		*pDest++ = singleByte >> 4;
	}
	else if (pEnd - pScanEnd == 3)
	{
		UINT twoBytes = 0;
		UINT nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		twoBytes |= nRes << 12;
		nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		twoBytes |= nRes << 6;
		nRes = BASE64_DECODE_TABLE[*pCur++];
		if (nRes == 0xFF)
			return false;
		twoBytes |= nRes;

		*pDest++ = (twoBytes >> 10) & 0xFF;
		*pDest++ = (twoBytes >> 2)  & 0xFF;
	}

	// ��wstring���ֽڵߵ�
	for (int i = 0; i < nDestLen / 2; ++i)
		pDestBuffer[i] = (pDestBuffer[i] << 8) | (pDestBuffer[i] >> 8);

	pDestBuffer[nDestLen/2] = 0;
	ws = std::wstring(pDestBuffer);
	delete [] pDestBuffer;
	return true;
}

std::string Imap4SingleMBoxNameNet2Host(const std::string& sName)
{
	std::wstring out;
	//'&' shift to Modified Base64, '-' shift back to US-ASCII
	BOOL bAsciiing = true;
	int nBeginPos = -1;					//��Ҫ����base64�������ʼλ��
	for( UINT i = 0; i < sName.size(); i++)
	{
		char c = sName[i];
		if ( c == '&' && bAsciiing)
		{
			bAsciiing = false;
			nBeginPos = i;
		}
		else if ( c == '-' && !bAsciiing)
		{
			UINT nLen = i - nBeginPos - 1;
			if ( nLen <= 0)
			{
				out += L'&';			//ֻ�� '&-',����� '&' ����
			}
			else
			{
				std::wstring ws;
				BOOL bRet = fromModifiedUTF7(sName.substr(nBeginPos + 1, nLen), ws);
				//TODO : bRet FALSE
				out += ws;
			}
			bAsciiing = true;
			nBeginPos = -1;
		}
		else if (bAsciiing)
		{
			out += (WCHAR)c;
		}
	}

	return ws2s(out);
}

std::string Imap4SingleMBoxNameHost2Net(const std::string& sName)
{
		//0x20-0x7e��ascii��ʽ����������0x26(&)�������ֽڵ�&-����
	bool bUsingAscii = true;
	int nBeginPos = -1;					//��Ҫbase64��������ݿ�ʼλ��

	std::string out;
	std::wstring ws = s2ws(sName);
	for (int i = 0; i < (int)ws.length(); i++)
	{
		WCHAR c = ws.data()[i];
		BOOL bAscii = c >= 0x0020 && c <= 0x007e;
		if (bAscii)
		{
			if(!bUsingAscii)
			{//��ascii�Ѿ���������Base64����
				out.push_back('&');
				std::string sRes;
				toModifiedUTF7(GetMid(ws, nBeginPos, i), sRes);
				out.append(sRes);
				out.push_back('-');
				bUsingAscii = true;
				nBeginPos = -1;
			}

			if (c == L'&')
				out.append("&-");
			else
				out.push_back((char)(c & 0xFF));
		}
		else if (bUsingAscii)
		{
			nBeginPos = i;
			bUsingAscii = false;
		}
	}

	//�������û�б���ķ�ascii
	if (!bUsingAscii && nBeginPos != -1)
	{
		out.push_back('&');
		std::string sRes;
		toModifiedUTF7(GetMid(ws, nBeginPos, ws.length()/* - nBeginPos*/), sRes);
		out.append(sRes);
		out.push_back('-');
	}

	return out;
}

std::string GetContentTypeFromExt(const std::string& sName)
{
	int nPos = sName.rfind('.');
	if (nPos == -1)
		return "";

	std::string sExt = sName.substr(nPos, sName.length() - nPos);

	if (sExt.compare(".jpg") == 0
		|| sExt.compare(".jpeg") == 0
		|| sExt.compare(".jpe") == 0)
	{
		return "image/jpeg";
	}
	else if (sExt.compare(".png") == 0)
	{
		return "image/x-png";
	}
	else if (sExt.compare(".au") == 0
		|| sExt.compare(".snd") == 0)
	{
		return "audio/basic";
	}
	else if (sExt.compare(".wav") == 0)
	{
		return "audio/wav";
	}
	else if (sExt.compare(".mpeg") == 0
		|| sExt.compare(".mpg") == 0
		|| sExt.compare(".mpe") == 0)
	{
		return "video/mpeg";
	}
	else if (sExt.compare(".avi") == 0)
	{
		return "video/x-msvideo";
	}
	else if (sExt.compare(".movie") == 0)
	{
		return "video/x-sgi-movie";
	}
	else if (sExt.compare(".rtf") == 0)
	{
		return "application/rtf";
	}
	else if (sExt.compare(".pdf") == 0)
	{
		return "application/pdf";
	}
	else if (sExt.compare(".doc") == 0)
	{
		return "application/vnd.framemaker";
	}
	else if (sExt.compare(".tar") == 0)
	{
		return "application/x-tar";
	}
	else if (sExt.compare(".zip") == 0)
	{
		return "application/zip";
	}
	else if (sExt.compare(".txt") == 0
		|| sExt.compare(".c") == 0
		|| sExt.compare(".cc") == 0
		|| sExt.compare(".cpp") == 0
		|| sExt.compare(".h") == 0
		|| sExt.compare(".pl") == 0)
	{
		return "text/plain";
	}
	else if (sExt.compare(".htm") == 0
		|| sExt.compare(".html") == 0)
	{
		return "text/html";
	}

	return "";
}

std::string GetFileDirFromPath(const std::string sPath)
{
	int nIdx = sPath.rfind('\\');
	return sPath.substr(0, nIdx);
}

std::string GetFileNameFromPath(const std::string sPath)
{
	int nIdx = sPath.rfind('\\');
	return sPath.substr(nIdx+1, sPath.length()-nIdx-1);
}

std::string WordToString(vmime::word w, vmime::charset ch)
{
	if (w.getCharset().getName().compare(vmime::charsets::US_ASCII)  == 0)
		return w.getBuffer();
	else
		return w.getConvertedText(ch);
}

std::string WordToString(vmime::ref <vmime::word> w, vmime::charset ch)
{
	if (w->getCharset().getName().compare(vmime::charsets::US_ASCII)  == 0)
		return w->getBuffer();
	else
		return w->getConvertedText(ch);
}

namespace vu = vmime::utility;

void FillHeaderMap(vmime::ref <vmime::header> & hdr, std::map<String^, Object^> & mapHeader)
{
	vmime::charset ch = vmime::charset(vmime::charsets::UTF_8);

	// ���ʼ�����������
	//mapHeader.insert(std::pair<String^, Object^>("CONTENT-HTML", "�������"));
	//mapHeader.insert(std::pair<String^, Object^>("CONTENT-TEXT", "����SB"));
	//auto arr = ref new Array<Object^>(2);
	//auto mapAttach = ref new Collections::Map<String^, String^>();

	//std::string s = "����.jpg";
	//std::string sutf8;
	//vmime::charset chSrc = vmime::charset(vmime::charset::getLocaleCharset());
	//vmime::charset chDst = vmime::charset(vmime::charsets::UTF_8);
	//vmime::charsetConverter cC(chSrc, chDst);
	//cC.convert(s, sutf8);

	//mapAttach->Insert("NAME", StdStringToString(sutf8));
	////mapAttach->Insert("CONTENT", StdStringToArray("helloworld!"));
	//mapAttach->Insert("CONTENT", "F:\\Users\\���\\Documents\\abc.eml");

	//auto mapAttach1 = ref new Collections::Map<String^, String^>();
	//mapAttach1->Insert("NAME", StdStringToString(sutf8));
	//mapAttach1->Insert("CONTENT", "F:\\Users\\���\\Documents\\abcd.eml");
	//arr[0] = mapAttach;
	//arr[1] = mapAttach1;
	//mapHeader.insert(std::pair<String^, Object^>("ATTACHMENT-ARRAY", arr));

	//auto arrInline = ref new Array<Object^>(2);
	//auto mapInline = ref new Collections::Map<String^, String^>();
	//s = "fsdf.txt";
	//chSrc = vmime::charset(vmime::charset::getLocaleCharset());
	//chDst = vmime::charset(vmime::charsets::UTF_8);
	//cC.convert(s, sutf8);

	//mapInline->Insert("NAME", StdStringToString(sutf8));
	////mapInline->Insert("CONTENT", StdStringToArray("Goodbyeworld!"));
	//mapInline->Insert("CONTENT", "F:\\Users\\���\\Documents\\11.eml");
	//mapInline->Insert("CONTENTID", StdStringToString(vmime::messageId::generateId().getId()));
	//arrInline[0] = mapInline;
	//auto mapInline1 = ref new Collections::Map<String^, String^>();
	//mapInline1->Insert("NAME", StdStringToString(sutf8));
	//mapInline1->Insert("CONTENT", "F:\\Users\\���\\Documents\\123.eml");
	//mapInline1->Insert("CONTENTID", StdStringToString(vmime::messageId::generateId().getId()));
	//arrInline[1] = mapInline1;
	//mapHeader.insert(std::pair<String^, Object^>("INLINE-ARRAY", arrInline));

	for (int i=0; i<hdr->getFieldCount(); i++)
	{
		vmime::string name = hdr->getFieldAt(i)->getName();
		vmime::string nameUpperCase = name;
		transform(nameUpperCase.begin(), nameUpperCase.end(), nameUpperCase.begin(), ::toupper);

		if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::FROM))) == 0)
		{
			vmime::ref <vmime::mailbox> mb = hdr->From()->getValue().dynamicCast<vmime::mailbox>();
			ProcessMailbox(hdr, mapHeader, nameUpperCase, mb, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::TO))) == 0)
		{
			vmime::ref <vmime::mailboxList> mblist = hdr->To()->getValue().dynamicCast<vmime::addressList>()->toMailboxList();
			ProcessMailboxList(hdr, mapHeader, nameUpperCase, mblist, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CC))) == 0)
		{
			vmime::ref <vmime::mailboxList> mblist = hdr->Cc()->getValue().dynamicCast<vmime::addressList>()->toMailboxList();
			ProcessMailboxList(hdr, mapHeader, nameUpperCase, mblist, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::BCC))) == 0)
		{
			vmime::ref <vmime::mailboxList> mblist = hdr->Bcc()->getValue().dynamicCast<vmime::addressList>()->toMailboxList();
			ProcessMailboxList(hdr, mapHeader, nameUpperCase, mblist, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::DATE))) == 0)
		{
			vmime::ref <vmime::datetime> date = hdr->Date()->getValue().dynamicCast<vmime::datetime>();
			ProcessDateTime(hdr, mapHeader, nameUpperCase, date);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::SUBJECT))) == 0)
		{
			vmime::ref <vmime::text > txt = hdr->Subject()->getValue().dynamicCast<vmime::text>();
			std::string subject = GetCorrectText(txt, ch, hdr);	
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(subject)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::MESSAGE_ID))) == 0)
		{
			std::string messageid = hdr->MessageId()->getValue().dynamicCast<vmime::messageId>()->getId();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(messageid)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CONTENT_TYPE))) == 0)
		{
			vmime::ref <vmime::mediaType> mediatype = hdr->ContentType()->getValue().dynamicCast<vmime::mediaType>();
			std::string contenttype = mediatype->getType() + "/" + mediatype->getSubType();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(contenttype)));

			// ��ȡcharset
			vmime::ref <vmime::contentTypeField> field =
				hdr->getField(vmime::fields::CONTENT_TYPE).dynamicCast <vmime::contentTypeField>();
			try
			{
				vmime::ref <vmime::parameter> prm = field->findParameter("charset");
				vmime::word value = prm->getValue();
				mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase+"-CHARSET"), StdStringToString(value.getBuffer())));
			}
			catch (vmime::exceptions::no_such_parameter ex)
			{

			}
			try
			{
				vmime::ref <vmime::parameter> prm = field->findParameter("name");
				vmime::word value = prm->getValue();
					
				mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase+"-NAME"), StdStringToString(/*WordToString(value, ch)*/
									GetCorrectText(value, ch))));
			}
			catch (vmime::exceptions::no_such_parameter ex)
			{
				continue;
			}
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::IN_REPLY_TO))) == 0)
		{
			vmime::ref <vmime::messageIdSequence> inreplyto = hdr->InReplyTo()->getValue().dynamicCast<vmime::messageIdSequence>();
			auto arrMid = ref new Array<String^>(inreplyto->getMessageIdCount());
			if (inreplyto->getMessageIdCount() == 0)
				continue;
			for (int i=0; i<inreplyto->getMessageIdCount(); i++)
				arrMid[i] = StdStringToString(inreplyto->getMessageIdAt(i)->getId());
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), arrMid));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::RECEIVED))) == 0)
		{
			//vmime::ref <vmime::relay> recv = hdr->Re()->getValue().dynamicCast<vmime::relay>();
			// ��Ϊ�����ж���ֶΣ�������Ҫ���������ӿ�
			std::vector <vmime::ref <vmime::headerField>> vecRecvs = hdr->findAllFields(name);
			if (vecRecvs.size() <= 0)
				continue;
			vmime::ref <vmime::relay> recv = vecRecvs[0]->getValue().dynamicCast<vmime::relay>();
			ProcessDateTime(hdr, mapHeader, nameUpperCase, recv->getDate());
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::ORGANIZATION))) == 0)
		{
			std::string org = hdr->Organization()->getValue().dynamicCast<vmime::text>()->getConvertedText(ch);
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(org)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::MIME_VERSION))) == 0)
		{
			std::string version = hdr->MimeVersion()->getValue().dynamicCast<vmime::text>()->getConvertedText(ch);
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(version)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::RETURN_PATH))) == 0)
		{
			vmime::ref <vmime::path> returnpath = hdr->ReturnPath()->getValue().dynamicCast<vmime::path>();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(returnpath->getLocalPart() + "@" + returnpath->getDomain())));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CONTENT_TRANSFER_ENCODING))) == 0)
		{
			vmime::ref <vmime::encoding> encode = hdr->ContentTransferEncoding()->getValue().dynamicCast<vmime::encoding>();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(encode->getName())));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CONTENT_DESCRIPTION))) == 0)
		{
			std::string description = hdr->ContentDescription()->getValue().dynamicCast<vmime::text>()->getConvertedText(ch);
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(description)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CONTENT_DISPOSITION))) == 0)
		{
			vmime::ref <vmime::contentDisposition> cttdispostion = hdr->ContentDisposition()->getValue().dynamicCast<vmime::contentDisposition>();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(cttdispostion->getName())));

			// ��ȡ��������
			vmime::ref <vmime::contentDispositionField> field =
				hdr->getField(vmime::fields::CONTENT_DISPOSITION).dynamicCast <vmime::contentDispositionField>();
			try
			{
				vmime::word name = field->getFilename();
				mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase+"-NAME"),
						StdStringToString(/*name.getConvertedText(ch)*/GetCorrectText(name, ch))));
			}
			catch (vmime::exceptions::no_such_parameter ex)
			{
				continue;
			}
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CONTENT_ID))) == 0)
		{
			std::string contentid = hdr->ContentId()->getValue().dynamicCast<vmime::messageId>()->getId();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(contentid)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::CONTENT_LOCATION))) == 0)
		{
			std::string location = hdr->ContentLocation()->getValue().dynamicCast<vmime::text>()->getConvertedText(ch);
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(location)));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::REFERENCES))) == 0)
		{
			vmime::ref <vmime::messageIdSequence> reference = hdr->References()->getValue().dynamicCast<vmime::messageIdSequence>();
			if (reference->getMessageIdCount() == 0)
				continue;
			auto arrMid = ref new Array<String^>(reference->getMessageIdCount());
			for (int i=0; i<reference->getMessageIdCount(); i++)
				arrMid[i] = StdStringToString(reference->getMessageIdAt(i)->getId());
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), arrMid));
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::ORIGINAL_MESSAGE_ID))) == 0)
		{
			std::string origmessageid = hdr->OriginalMessageId()->getValue().dynamicCast<vmime::messageId>()->getId();
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(origmessageid)));
		}
		//else if (vu::stringUtils::isStringEqualNoCase(nameUpperCase, vmime::fields::DISPOSITION))
		//{
		//	vmime::ref <vmime::disposition> disp = hdr->Disposition()->getValue().dynamicCast<vmime::disposition>();
		//	
		//}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::DISPOSITION_NOTIFICATION_TO))) == 0)
		{
			vmime::ref <vmime::mailboxList> notif = hdr->DispositionNotificationTo()->getValue().dynamicCast<vmime::mailboxList>();
			ProcessMailboxList(hdr, mapHeader, nameUpperCase, notif, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::SENDER))) == 0)
		{
			vmime::ref <vmime::mailbox> mb = hdr->Sender()->getValue().dynamicCast<vmime::mailbox>();
			ProcessMailbox(hdr, mapHeader, nameUpperCase, mb, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::REPLY_TO))) == 0)
		{
			vmime::ref <vmime::mailbox> mb = hdr->ReplyTo()->getValue().dynamicCast<vmime::mailbox>();
			ProcessMailbox(hdr, mapHeader, nameUpperCase, mb, ch);
		}
		else if (nameUpperCase.compare(ToUpperCase(std::string(vmime::fields::DELIVERED_TO))) == 0)
		{
			vmime::ref <vmime::mailbox> mb = hdr->DeliveredTo()->getValue().dynamicCast<vmime::mailbox>();
			ProcessMailbox(hdr, mapHeader, nameUpperCase, mb, ch);
		}
		else
		{
			vmime::ref <vmime::headerField> field = hdr->getFieldAt(i);
			vmime::ref <vmime::headerFieldValue> headervalue = field->getValue();
			//vmime::string value = headervalue.dynamicCast<vmime::text>()->getConvertedText(ch);
			vmime::string value = GetCorrectText(headervalue.dynamicCast<vmime::text>(), ch, hdr);
			mapHeader.insert(std::pair<String^, Object^>(StdStringToString(nameUpperCase), StdStringToString(value)));
		}
	}
}

void AppendMailboxListField(vmime::ref <vmime::header>& hdr,
							vmime::headerFieldFactory* hfFactory,
							const std::string::value_type * fieldType,
							Object^ obj)
{
	if (!obj)
		return;

	Array<Object^>^ arr;
#ifndef LOCAL_DEBUG
	((WF::IPropertyValue^)obj)->GetInspectableArray(&arr);
#else
	arr = (Array<Object^>^)obj;
#endif

	vmime::ref <vmime::headerField> field = hfFactory->create(fieldType);
	vmime::ref <vmime::mailboxList> recipients = vmime::create <vmime::mailboxList>();

	int length = arr->Length;
	for (int i=0; i<(int)length; i++)
	{
		std::string name;
		std::string addr;

#ifndef LOCAL_DEBUG
		auto element = arr[i];
		auto mapField = (WFC::PropertySet^)element;
#else
		auto mapField = (Collections::Map<String^, String^>^)arr[i];
#endif
		if (mapField->HasKey("NAME"))
			name = StringToStdString((String^)mapField->Lookup("NAME"));
		if (mapField->HasKey("ADDR"))
			addr = StringToStdString((String^)mapField->Lookup("ADDR"));

		if (addr.length() == 0)
			continue;
		vmime::text tName(name, vmime::charset(vmime::charsets::UTF_8));
		recipients->appendMailbox(vmime::create<vmime::mailbox>(tName, addr));
	}

	if (recipients->getMailboxCount() == 0)
		return;

	field->setValue(recipients);
	hdr->appendField(field);
}

void AppendMailboxField(vmime::ref <vmime::header>& hdr,
							vmime::headerFieldFactory* hfFactory,
							const std::string::value_type * fieldType,
							Object^ obj)
{
	if (!obj)
		return;
#ifndef LOCAL_DEBUG
	auto mapField = (WFC::PropertySet^)obj;
#else
	auto mapField = (Collections::Map<String^, String^>^)obj;
#endif

	std::string name;
	std::string addr;
	if (mapField->HasKey("NAME"))
		name = StringToStdString((String^)mapField->Lookup("NAME"));
	if (mapField->HasKey("ADDR"))
		addr = StringToStdString((String^)mapField->Lookup("ADDR"));

	if (addr.length() == 0)
		return;

	vmime::ref <vmime::headerField> field = hfFactory->create(fieldType);
	field->setValue(vmime::create <vmime::mailbox>(vmime::text(name, vmime::charset(vmime::charsets::UTF_8)), addr));
	hdr->appendField(field);
}

void AppendMsgIdRefsField(vmime::ref <vmime::header>& hdr,
							vmime::headerFieldFactory* hfFactory,
							const std::string::value_type * fieldType,
							Object^ obj)
{
	if (!obj)
		return;
	vmime::ref <vmime::headerField> field = hfFactory->create(fieldType);
	vmime::ref <vmime::messageIdSequence> value = vmime::create<vmime::messageIdSequence>();

	Array<String^>^ arr;
#ifndef LOCAL_DEBUG
	((WF::IPropertyValue^)obj)->GetStringArray(&arr);
#else
	arr = (Array<String^>^)obj;
#endif

	for (int i=0; i<(int)arr->Length; i++)
	{
		std::string id = StringToStdString(arr[i]);
		if (id.length() == 0)
			continue;
		vmime::ref<vmime::messageId> msgId = vmime::create<vmime::messageId>(id);
		value->appendMessageId(msgId);
	}

	if (value->getMessageIdCount() == 0)
		return;

	field->setValue(value);
	hdr->appendField(field);
}

void BuildFieldFromKeyAndValue(vmime::ref <vmime::message>& msg, String^ key, Object^ obj)
{
	// messageid��contentid����mime����
	vmime::ref <vmime::header> hdr = msg->getHeader();
	vmime::ref <vmime::body> bdy = msg->getBody();
	vmime::headerFieldFactory* hfFactory = vmime::headerFieldFactory::getInstance();

	std::string sKey = StringToStdString(key);

	if (sKey.compare(ToUpperCase(std::string(vmime::fields::IN_REPLY_TO))) == 0)
	{
		AppendMsgIdRefsField(hdr, hfFactory, vmime::fields::IN_REPLY_TO, obj);
	}
	else if (sKey.compare(ToUpperCase(std::string(vmime::fields::SENDER))) == 0)
	{
		AppendMailboxField(hdr, hfFactory, vmime::fields::SENDER, obj);
	}
	else if (sKey.compare(ToUpperCase(std::string(vmime::fields::DELIVERED_TO))) == 0)
	{
		AppendMailboxField(hdr, hfFactory, vmime::fields::DELIVERED_TO, obj);
	}
	// ����ʱ��������Щ�ֶ�
	else if (sKey.compare(ToUpperCase(std::string(vmime::fields::RECEIVED))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::MIME_VERSION))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CONTENT_TRANSFER_ENCODING))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CONTENT_DESCRIPTION))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CONTENT_DISPOSITION))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CONTENT_ID))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CONTENT_LOCATION))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::ORIGINAL_MESSAGE_ID))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::DISPOSITION))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CONTENT_TYPE))) == 0)
	{
	}
	// ��Ϊ��Ԥ��д����Щͷ����������������˵�
	else if (sKey.compare(ToUpperCase(std::string(vmime::fields::FROM))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::TO))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::CC))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::BCC))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::DATE))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::SUBJECT))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::MESSAGE_ID))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::REFERENCES))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::REPLY_TO))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::ORGANIZATION))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::RETURN_PATH))) == 0
		|| sKey.compare(ToUpperCase(std::string(vmime::fields::DISPOSITION_NOTIFICATION_TO))) == 0)
	{
	}
	else if (sKey.compare("ATTACHMENT-ARRAY") == 0
		|| sKey.compare("INLINE-ARRAY") == 0
		|| sKey.compare("CONTENT-TEXT") == 0
		|| sKey.compare("CONTENT-HTML") == 0)
	{
	}
	else
	{
		vmime::ref <vmime::headerField> field = hfFactory->create(sKey, StringToStdString((String^)obj));
		hdr->appendField(field);
	}
}

void BuildCommonHeader(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail, vmime::ref <vmime::message>& msg)
{
	vmime::ref <vmime::header> hdr = msg->getHeader();
	vmime::ref <vmime::body> bdy = msg->getBody();
	vmime::headerFieldFactory* hfFactory = vmime::headerFieldFactory::getInstance();

	String^ key;

	// ���DATE�ֶ�
	vmime::ref <vmime::headerField> dateField = hfFactory->create(vmime::fields::DATE);
	dateField->setValue(vmime::datetime::now());
	hdr->appendField(dateField);

	key = StdStringToString(ToUpperCase(std::string(vmime::fields::FROM)));
	if (mapMail->HasKey(key))
	{
		AppendMailboxField(hdr, hfFactory, vmime::fields::FROM, mapMail->Lookup(key));
	}
	key = StdStringToString(ToUpperCase(std::string(vmime::fields::TO)));
	if (mapMail->HasKey(key))
	{
		AppendMailboxListField(hdr, hfFactory, vmime::fields::TO, mapMail->Lookup(key));
	}
	key = StdStringToString(ToUpperCase(std::string(vmime::fields::CC)));
	if (mapMail->HasKey(key))
	{
		AppendMailboxListField(hdr, hfFactory, vmime::fields::CC, mapMail->Lookup(key));
	}
	key = StdStringToString(ToUpperCase(std::string(vmime::fields::BCC)));
	if (mapMail->HasKey(key))
	{
		AppendMailboxListField(hdr, hfFactory, vmime::fields::BCC, mapMail->Lookup(key));
	}
	key = StdStringToString(ToUpperCase(std::string(vmime::fields::SUBJECT)));
	if (mapMail->HasKey(key))
	{
		if (!((String^)mapMail->Lookup(key))->IsEmpty())
		{
			vmime::ref <vmime::headerField> subjectField = hfFactory->create(vmime::fields::SUBJECT);

			subjectField->setValue(vmime::word(StringToStdString((String^)mapMail->Lookup(key)), vmime::charset(vmime::charsets::UTF_8)));
			hdr->appendField(subjectField);	
		}
	}
	key = StdStringToString(ToUpperCase(std::string(vmime::fields::REPLY_TO)));
	if (mapMail->HasKey(key))
	{
		AppendMailboxField(hdr, hfFactory, vmime::fields::REPLY_TO, mapMail->Lookup(key));	
	}

	// ���message-id�ֶ�
	vmime::ref <vmime::headerField> field = hfFactory->create(vmime::fields::MESSAGE_ID);
	vmime::messageId value = vmime::messageId::generateId();
	field->setValue(value);
	hdr->appendField(field);

	key = StdStringToString(ToUpperCase(std::string(vmime::fields::REFERENCES)));
	if (mapMail->HasKey(key))
	{
		AppendMsgIdRefsField(hdr, hfFactory, vmime::fields::REFERENCES, mapMail->Lookup(key));
	}

	// ���MIME-VERSION��
	msg->getHeader()->MimeVersion()->setValue(std::string(vmime::SUPPORTED_MIME_VERSION));
}

void BuildBody(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail, vmime::ref <vmime::message>& msg)
{
	vmime::ref <vmime::stringContentHandler> text = vmime::create<vmime::stringContentHandler>();
	vmime::encoding enc(vmime::encodingTypes::BASE64);
	vmime::ref <vmime::htmlTextPart> html = vmime::create<vmime::htmlTextPart>();
	if (!mapMail->HasKey("CONTENT-HTML"))
		return;
	Object^ obj = mapMail->Lookup("CONTENT-HTML");
	if (!((String^)obj)->IsEmpty())
	{
		text->setData(Base64Encode((String^)obj), enc);
		html->setCharset(vmime::charsets::UTF_8);
		html->setText(text);

		if (mapMail->HasKey("CONTENT-TEXT"))
		{
			obj = mapMail->Lookup("CONTENT-TEXT");
			if (!((String^)obj)->IsEmpty())
			{
				text->setData(Base64Encode((String^)obj), enc);
				html->setPlainText(text);
			}	
		}

		bool hasInline = false;
		Array<Object^>^ arrInline;
		if (mapMail->HasKey("INLINE-ARRAY"))
		{
			obj = mapMail->Lookup("INLINE-ARRAY");
#ifndef LOCAL_DEBUG
			((WF::IPropertyValue^)obj)->GetInspectableArray(&arrInline);
#else
			arrInline = (Array<Object^>^)obj;
#endif
			if (arrInline->Length != 0)
				hasInline = true;
		}

		if (hasInline)
		{
			for (int i=0; i<(int)arrInline->Length; i++)
			{
				Collections::Map<String^, Object^>^ mapAttach = (Collections::Map<String^, Object^>^)arrInline[i];
				std::string name;
				if (mapAttach->HasKey("NAME"))
					name = StringToStdString((String^)mapAttach->Lookup("NAME"));

				// TODO(lixianliang): modify content type
#ifndef LOCAL_DEBUG
				text->setData(Base64Encode((String^)mapAttach->Lookup("CONTENT")), enc);
#else
				text->setData(Base64Encode(ArrayToString((Array<uint8>^)mapAttach->Lookup("CONTENT"))), enc);
#endif
				std::string sContentType = GetContentTypeFromExt(name);

				if (!mapAttach->HasKey("CONTENTID"))
					return;
				std::string id = StringToStdString((String^)mapAttach->Lookup("CONTENTID"));

				html->addObject(text, GetFormattedWord(name), id, enc,
					(sContentType.length() == 0) ? vmime::mediaType() : vmime::mediaType(sContentType));
			}
		}
	}

	bool hasAttach = false;
	Array<Object^>^ arr;
	if (mapMail->HasKey("ATTACHMENT-ARRAY"))
	{
		obj = mapMail->Lookup("ATTACHMENT-ARRAY");
#ifndef LOCAL_DEBUG
		((WF::IPropertyValue^)obj)->GetInspectableArray(&arr);
#else
		arr = (Array<Object^>^)obj;
#endif
		if (arr->Length != 0)
			hasAttach = true;
	}

	if (hasAttach && html->getPartCount() > 1)
	{
		// Set parent part (message) to "multipart/mixed"
		msg->getHeader()->ContentType()->setValue
			(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_MIXED));

		vmime::ref <vmime::bodyPart> subPart = vmime::create <vmime::bodyPart>();
		msg->getBody()->appendPart(subPart);

		subPart->getHeader()->ContentType()->setValue
			(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_ALTERNATIVE));

		html->generateIn(msg, subPart);
	}
	else
	{
		// Generate the text part(s) directly into the message
		html->generateIn(msg, msg);

		if (hasAttach)
		{
			msg->getHeader()->ContentType()->setValue
				(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_MIXED));
		}
		else if (html->getPartCount() > 1)
		{
			msg->getHeader()->ContentType()->setValue
				(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_ALTERNATIVE));
		}
	}

	// Generate the attachments
	if (hasAttach)
	{
		for (int i=0; i<(int)arr->Length; i++)
		{
			Collections::Map<String^, Object^>^ mapAttach = (Collections::Map<String^, Object^>^)arr[i];
			std::string name;
			if (mapAttach->HasKey("NAME"))
				name = StringToStdString((String^)mapAttach->Lookup("NAME"));
			
			// TODO(lixianliang): modify content type
#ifndef LOCAL_DEBUG
			text->setData(Base64Encode((String^)mapAttach->Lookup("CONTENT")), enc);
#else
			text->setData(Base64Encode(ArrayToString((Array<uint8>^)mapAttach->Lookup("CONTENT"))), enc);
#endif
			std::string sContentType = GetContentTypeFromExt(name);
			vmime::ref <vmime::defaultAttachment> attach = vmime::create<vmime::defaultAttachment>(text,
						enc,
						(sContentType.length() == 0) ? vmime::mediaType() : vmime::mediaType(sContentType),
						vmime::NULL_TEXT,
						name.length() == 0 ? vmime::NULL_WORD : GetFormattedWord(name));

			attach->generateIn(msg);
		}
	}

	if (msg->getBody()->getPartCount() == 1)
	{
		const vmime::bodyPart& part = *msg->getBody()->getPartAt(0);
		vmime::ref <vmime::body> bodyCopy = part.getBody()->clone().dynamicCast <vmime::body>();
		const std::vector <vmime::ref <const vmime::headerField> > fields = part.getHeader()->getFieldList();
		for (std::vector <vmime::ref <const vmime::headerField> >::const_iterator it = fields.begin() ;
		     it != fields.end() ; ++it)
		{
			*(msg->getHeader()->getField((*it)->getName())) = **it;
		}

		msg->getBody()->copyFrom(*bodyCopy);
	}
}