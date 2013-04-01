#include <pch.h>
#include <iostream>
#include "mimeapi.h"
#include "mimeutil.h"

#include "vmime/constants.hpp"
#include "vmime/vmime.hpp"
#include "vmime/platforms/windowsHandler.hpp"

using namespace vmime;

void MimeInit()
{
	platform::setHandler<vmime::platforms::windows::windowsHandler>();
}

Collections::Map<String^, Object^>^ _ParseHeader(const std::string& data)
{
	//vmime::string data = ArrayToStdString(buffer);
	vmime::ref <message> msg = vmime::create <message>();
	msg->parse(data);
	vmime::ref <header> hdr = msg->getHeader();

	std::map<String^, Object^> mapHeader;
	FillHeaderMap(hdr, mapHeader);

	// 测试用
	//String^ out = _BuildMail(ref new Collections::MapView<String^, Object^>(mapHeader));

	return ref new Collections::Map<String^, Object^>(mapHeader);
}



String^ _ParseBody(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset)
{
	// 如果sEncodeType为空，则默认是“7bit”，如果sCharset为空，则默认是本地代码页字符集
	encoding enc(StringToStdString(sEncodeType));
	if (sEncodeType->IsEmpty())
		enc.setName("7bit");
	utility::inputStreamStringAdapter in((char*)&buffer[0], 0, buffer->Length);
	std::string outbuf;
	utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc.getEncoder()->decode(in, out, NULL);
	}
	catch (exceptions::no_encoder_available ex)
	{
		return "";
	}

	// 如果编码一样，则略过编码转化步骤

	// 兼容“utf8”
	std::string sSrc = ToUpperCase(StringToStdString(sCharset));
	std::string sDst = charset(charsets::UTF_8).getName();

	if (sSrc.length() == 0)
		sSrc = FuzzyCharsetIdentification(outbuf, charset(charsets::UTF_8).getName());

	if (ToUpperCase(sSrc).compare(ToUpperCase(sDst)) == 0)
		return StdStringToString(outbuf);

	std::string ret;
	charset chSrc = charset(sSrc);
	charset chDst = charset(sDst);
	vmime::charsetConverter cC(chSrc, chDst);

	try
	{
		cC.convert(outbuf, ret);
	}
	catch (exceptions::charset_conv_error)
	{
		return "";
	}

	return StdStringToString(ret);
}


Array<uint8>^ _ParseAttachment(const Array<uint8>^ buffer, String^ sEncodeType)
{
	encoding enc(StringToStdString(sEncodeType));
	utility::inputStreamStringAdapter in((char*)&buffer[0], 0, buffer->Length);
	std::string outbuf;
	utility::outputStreamStringAdapter out(outbuf);

	try
	{
		enc.getEncoder()->decode(in, out, NULL);
	}
	catch (exceptions::no_encoder_available ex)
	{
		return ref new Array<uint8>(0);
	}

	return StdStringToArray(outbuf);
}


Array<String^>^ _DecodeMBoxNames(const Array<String^>^ buffer)
{
    vector<String^> vec(buffer->begin(), buffer->end());
	auto arr = ref new Array<String^>(vec.size());

	for (int i=0; i<(int)vec.size(); i++)
		arr[i] = _DecodeMBoxName(vec[i]);

	return arr;
}


Array<String^>^ _EncodeMBoxNames(const Array<String^>^ buffer)
{
    vector<String^> vec(buffer->begin(), buffer->end());
	auto arr = ref new Array<String^>(vec.size());

	for (int i=0; i<(int)vec.size(); i++)
		arr[i] = _EncodeMBoxName(vec[i]);

	return arr;
}


String^ _DecodeMBoxName(String^ name)
{
	std::string mbName =  Imap4SingleMBoxNameNet2Host(StringToStdString(name));
	return StdStringToString(mbName);
}


String^ _EncodeMBoxName(String^ name)
{
	std::string mbName = StringToStdString(name);
	return StdStringToString(Imap4SingleMBoxNameHost2Net(mbName));
}

String^ _BufferToString(const Array<uint8>^ buffer)
{
	std::string ret;
	std::string outbuf = ArrayToStdString(buffer);
	charset chSrc = charset::getLocaleCharset();
	charset chDst = charset(charsets::UTF_8);
	vmime::charsetConverter cC(chSrc, chDst);

	try
	{
		cC.convert(outbuf, ret);
	}
	catch (exceptions::charset_conv_error)
	{
		return StdStringToString(outbuf);
	}

	return StdStringToString(ret);
}

namespace WFC = Windows::Foundation::Collections;

String^ _BuildMail(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail)
{
	vmime::ref <vmime::message> msg = vmime::create <vmime::message>();

	// 先按指定顺序生成一些头部信息
	BuildCommonHeader(mapMail, msg);

	// 构建其他头部信息
	WFC::IIterator<WFC::IKeyValuePair<String^, Object^>^>^ iter = mapMail->First();
	while (iter->HasCurrent)
	{
		WFC::IKeyValuePair<String^, Object^>^ cur = iter->Current;
		BuildFieldFromKeyAndValue(msg, cur->Key, cur->Value);

		iter->MoveNext();
	}

	// 添加BODY
	BuildBody(mapMail, msg);

	return StdStringToString(msg->generate());
}