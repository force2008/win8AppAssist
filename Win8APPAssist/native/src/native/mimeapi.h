#pragma once

#include <collection.h>

using namespace Platform;


/* mime初始化
*/

void MimeInit();


/* 头部解析
	@parameter buffer 头部二进制数据
	@return 头部键值对集合
*/

Collections::Map<String^, Object^>^ _ParseHeader(const std::string& data);

/* 正文解析
	@parameter buffer body数据
	@parameter sEncodeType 编码格式
	@return 解码后的body数据
*/

String^ _ParseBody(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset);

/* 附件解析
	@parameter buffer body数据
	@parameter sEncodeType 编码格式
	@return 解码后的附件数据
*/

Array<uint8>^ _ParseAttachment(const Array<uint8>^ buffer, String^ sEncodeType);

/* 邮件夹名称解码
	@parameter buffer 邮件夹名称数组
	@return 解码后的邮件夹名称数组
*/

Array<String^>^ _DecodeMBoxNames(const Array<String^>^ buffer);

/* 邮件夹名称编码
	@parameter buffer 邮件夹名称数组
	@return 编码后的邮件夹名称数组
*/

Array<String^>^ _EncodeMBoxNames(const Array<String^>^ buffer);


/* 单个邮件夹名称解码
	@parameter name 邮件夹名称数组
	@return 解码后的邮件夹名称数组
*/
String^ _DecodeMBoxName(String^ name);

/* 单个邮件夹名称编码
	@parameter name 邮件夹名称数组
	@return 编码后的邮件夹名称数组
*/
String^ _EncodeMBoxName(String^ name);

/* 把二进制数据转化为本地字符集编码
	@parameter buffer 二进制数据
	@return 经过本地字符集编码的字符串
*/
String^ _BufferToString(const Array<uint8>^ buffer);

/* 邮件的构建
	@parameter mapMail 存放邮件数据的map
	@return 构建完成的邮件全文
*/
String^ _BuildMail(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail);