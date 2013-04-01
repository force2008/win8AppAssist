#pragma once

#include "collection.h"
#include "vmime/vmime.hpp"

using namespace std;
using namespace Platform;

wstring s2ws(const string& s, UINT nCodePage=CP_UTF8);

std::string ws2s(const std::wstring& ws);

String^ ArrayToString(const Array<uint8>^ buffer);

String^ StdStringToString(const string& s);

std::string StringToStdString(String^ s);

std::string ArrayToStdString(const Array<uint8>^ buffer);

Array<uint8>^ StringToArray(String^ str);

Array<uint8>^ StdStringToArray(const std::string& str);

void ProcessMailbox(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					vmime::ref <vmime::mailbox>& mb,
					vmime::charset& ch);

void ProcessMailboxList(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					vmime::ref <vmime::mailboxList>& mblist,
					vmime::charset& ch);

void ProcessDateTime(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					vmime::ref <vmime::datetime> & date);

void ProcessDateTime(vmime::ref <vmime::header> & hdr,
					std::map<String^, Object^> & mapHeader,
					vmime::string & name,
					const vmime::datetime & date);

// ȫ��ת��д
std::string& ToUpperCase(std::string & s);

// �Ƿ���ASCII�ַ�
bool HasAsciiChar(const std::string& s);

// ��þ�������͸�ʽ�����ļ���
vmime::word GetFormattedWord(const std::string& s);

// ģ����ȡcharset
std::string FuzzyCharsetIdentification(const std::string& src, const std::string& sDefaultCharset);

// ��ascii�����word�ĳɱ��ر����ʽ
std::string GetCorrectText(const vmime::text& txt, vmime::charset& ch, vmime::ref <vmime::header> & hdr);
std::string GetCorrectText(vmime::ref <vmime::text> txt, vmime::charset& ch, vmime::ref <vmime::header> & hdr);
std::string GetCorrectText(const vmime::word& wd, vmime::charset& ch);

// ��ȡʵ�ʱ���
//std::string GetRealCharset(const std::string& sText);

// base64�����
std::string Base64Encode(const char* c, int nLength);
std::string Base64Encode(const std::string& sText);
std::string Base64Decode(const std::string& sText);
std::string Base64Encode(String^ sText);
std::string Base64Decode(String^ sText);
std::string Base64Encode(const Array<uint8>^ buffer);
std::string Base64Decode(const Array<uint8>^ buffer);

// IMAP�ʼ�������ת��
std::string Imap4SingleMBoxNameNet2Host(const std::string& sName);
std::string Imap4SingleMBoxNameHost2Net(const std::string& sName);

// ��ȡcontent-type
std::string GetContentTypeFromExt(const std::string& sName);

// ��ȡ�ļ�·��
std::string GetFileDirFromPath(const std::string sPath);

// ��ȡ�ļ�����
std::string GetFileNameFromPath(const std::string sPath);

// ���ͷ��map
void FillHeaderMap(vmime::ref <vmime::header> & hdr, std::map<String^, Object^> & mapHeader);
// д���ʼ�ͷ����Ϣ��msg
void BuildFieldFromKeyAndValue(vmime::ref <vmime::message>& msg, String^ key, Object^ obj);
// д�볣�õ�ͷ����Ϣ��msg
void BuildCommonHeader(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail, vmime::ref <vmime::message>& msg);
// д�����ļ�������Ϣ��msg
void BuildBody(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail, vmime::ref <vmime::message>& msg);