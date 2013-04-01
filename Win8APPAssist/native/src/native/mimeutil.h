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

// 全部转大写
std::string& ToUpperCase(std::string & s);

// 是否含有ASCII字符
bool HasAsciiChar(const std::string& s);

// 获得经过编码和格式化的文件名
vmime::word GetFormattedWord(const std::string& s);

// 模糊获取charset
std::string FuzzyCharsetIdentification(const std::string& src, const std::string& sDefaultCharset);

// 把ascii编码的word改成本地编码格式
std::string GetCorrectText(const vmime::text& txt, vmime::charset& ch, vmime::ref <vmime::header> & hdr);
std::string GetCorrectText(vmime::ref <vmime::text> txt, vmime::charset& ch, vmime::ref <vmime::header> & hdr);
std::string GetCorrectText(const vmime::word& wd, vmime::charset& ch);

// 获取实际编码
//std::string GetRealCharset(const std::string& sText);

// base64编解码
std::string Base64Encode(const char* c, int nLength);
std::string Base64Encode(const std::string& sText);
std::string Base64Decode(const std::string& sText);
std::string Base64Encode(String^ sText);
std::string Base64Decode(String^ sText);
std::string Base64Encode(const Array<uint8>^ buffer);
std::string Base64Decode(const Array<uint8>^ buffer);

// IMAP邮件夹名称转换
std::string Imap4SingleMBoxNameNet2Host(const std::string& sName);
std::string Imap4SingleMBoxNameHost2Net(const std::string& sName);

// 获取content-type
std::string GetContentTypeFromExt(const std::string& sName);

// 获取文件路径
std::string GetFileDirFromPath(const std::string sPath);

// 获取文件名称
std::string GetFileNameFromPath(const std::string sPath);

// 填充头部map
void FillHeaderMap(vmime::ref <vmime::header> & hdr, std::map<String^, Object^> & mapHeader);
// 写入邮件头部信息到msg
void BuildFieldFromKeyAndValue(vmime::ref <vmime::message>& msg, String^ key, Object^ obj);
// 写入常用的头部信息到msg
void BuildCommonHeader(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail, vmime::ref <vmime::message>& msg);
// 写入正文及附件信息到msg
void BuildBody(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail, vmime::ref <vmime::message>& msg);