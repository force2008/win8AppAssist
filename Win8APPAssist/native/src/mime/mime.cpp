// mime.cpp
#include "pch.h"
#include "mime.h"
#include "common.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include "mimeapi.h"


#include "vmime/vmime.hpp"
#include "vmime/platforms/windowsHandler.hpp"

using namespace native;
using namespace Platform;

Mime::Mime()
{
	vmime::platform::setHandler<vmime::platforms::windows::windowsHandler>();
}

//std::string ws2s(const std::wstring& ws)
//{
//    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
//    setlocale(LC_ALL, "chs");
//    const wchar_t* _Source = ws.c_str();
//    size_t _Dsize = 2 * ws.size() + 1;
//    char *_Dest = new char[_Dsize];
//    memset(_Dest,0,_Dsize);
//    wcstombs(_Dest,_Source,_Dsize);
//    std::string result = _Dest;
//    delete []_Dest;
//    setlocale(LC_ALL, curLocale.c_str());
//    return result;
//}

template <class T> 
std::string ToString(T value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}


std::string getDate(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
	std::stringstream ss;
	ss << nYear << ":" << nMonth << ":" << nDay << ":" << nHour << ":" << nMinute << ":" << nSecond;
	return ss.str();
}

String^ Mime::Test(String^ text)
{
	//// VMime initialization
	//vmime::platform::setHandler<vmime::platforms::windows::windowsHandler>();

	//vmime::messageBuilder mb;

	//// Fill in the basic fields
	//mb.setExpeditor(vmime::mailbox("me@somewhere.com"));

	//vmime::addressList to;
	//to.appendAddress(vmime::create <vmime::mailbox>("you@elsewhere.com"));

	//mb.setRecipients(to);

	//vmime::addressList bcc;
	//bcc.appendAddress(vmime::create <vmime::mailbox>("you-bcc@nowhere.com"));

	//mb.setBlindCopyRecipients(bcc);

	//mb.setSubject(vmime::text("My first message generated with vmime::messageBuilder"));

	//// Message body
	//mb.getTextPart()->setText(vmime::create <vmime::stringContentHandler>(
	//		"I'm writing this short text to test message construction " \
	//		"using the vmime::messageBuilder component."));

	//// Construction
	//vmime::ref <vmime::message> msg = mb.construct();

	//vmime::utility::outputStreamAdapter out(std::cout);
	//msg->generate(out);

	//String^ str = ref new String(s2ws(mb.getSubject().getWholeBuffer()).c_str());
	//String^ str = ref new String();
	//str = "test";
//	return str;

	// Read data from file
	// Read data from file
	//FILE *fp;
	//fp = fopen("F:\\Users\\晨炜\\Documents\\haha.txt","rb");
	//int nErr = ::GetLastError();

	//std::ifstream file("E:\\TestFolder\\abc.eml");																																																																																																																																																																																																																																																									
	////file.open("E:\\TestFolder\\abc.eml");
	//if (!file.is_open())
	//{
	//	std::cout << "Error opening file";
	//	return "";
	//}
	//char buf[1024] = "";
	//file.read(buf, 1024);
	//vmime::utility::inputStreamAdapter is( file );
	//vmime::string data;
	//vmime::utility::outputStreamStringAdapter os(data);
	//vmime::utility::bufferedStreamCopy(is, os);

	// Actually parse the message
	
	//vmime::string data = ws2s(text->Data());
	//vmime::ref <vmime::message> msg = vmime::create <vmime::message>();
	//msg->parse(data);
	//vmime::ref <vmime::header> hdr = msg->getHeader();
	//vmime::ref <vmime::body> bdy = msg->getBody();
	//// Now, you can extract some of its components
	//vmime::charset ch(vmime::charsets::UTF_8);
	///*std::cout << "The subject of the message is : "
	//		<< hdr->Subject()->getValue().dynamicCast <vmime::text>()->getConvertedText(ch)
	//		<< std::endl << "It was sent by: "
	//		<< hdr->From()->getValue().dynamicCast <vmime::mailbox>()->getName().getConvertedText(ch)
	//		<< " (email: " << hdr->From()->getValue().get() << ")" << std :: endl;*/

	//vmime::ref <vmime::datetime> temp = hdr->Date()->getValue().dynamicCast<vmime::datetime>();
	//vmime::datetime* pDate = temp.get();

	//vmime::ref <vmime::addressList> addresses = hdr->To()->getValue().dynamicCast<vmime::addressList>();

	//vmime::string str = "Subject: " + hdr->Subject()->getValue().dynamicCast<vmime::text>()->getConvertedText(ch) + "\n"
	//	+ "Sent by: " + hdr->From()->getValue().dynamicCast<vmime::mailbox>()->getName().getConvertedText(ch) + "\n"
	//	+ "To: " + addresses->toMailboxList()->getMailboxAt(0)->getEmail() + "\n"
	//	+ "MessageId: " + hdr->MessageId()->getValue().dynamicCast<vmime::messageId>()->getId() + "\n"
	//	+ "Date: " + getDate(pDate->getYear(),pDate->getMonth(),pDate->getDay(),pDate->getHour(),pDate->getMinute(),pDate->getSecond()) + "\n"
	//	+ "ContentType: " + hdr->ContentType()->getValue().dynamicCast<vmime::mediaType>()->getType() + "\n";
	//			//+ " (email: " + hdr->From()->getValue().get() + ")";
	//std::wstringstream ws;
	//ws << str.c_str();
	return "";
}