#pragma once

#include <string>
#include <collection.h>
#include "modifiedPpltasks.h"
#include "vmime/vmime.hpp"

using namespace Platform;
using namespace Platform::Collections;

namespace WF = Windows::Foundation;

namespace native
{
    public ref class MimeParser sealed
    {
	private:
		Windows::Storage::StorageFile^ sampleFile;
    public:
        MimeParser();

		WF::Collections::IMap<String^, Object^>^ ParseHeader_Sync(const Array<uint8>^ buffer);
		String^ ParseBody_Sync(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset);
		Array<uint8>^ ParseAttachment_Sync(const Array<uint8>^ buffer, String^ sEncodeType);
		String^ DecodeMBoxName_Sync(String^ name);
		String^ EncodeMBoxName_Sync(String^ name);
		String^ BufferToString_Sync(const Array<uint8>^ buffer);
		String^ ArrayToString_Sync(const Array<uint8>^ buffer);
		String^ BuildMail_Sync(WF::Collections::IMapView<String^, Object^>^ mapMail);

		// BASE64编解码
		String^ Base64Encode_Sync(String^ sText);
		String^ Base64Decode_Sync(String^ sText);
		//String^ Base64Encode_Sync(const Array<uint8>^ buffer);
		//String^ Base64Decode_Sync(const Array<uint8>^ buffer);

		String^ GenerateContentId_Sync();


		///* 头部解析
		//	@parameter buffer 头部二进制数据
		//	@return 头部键值对集合
		//*/
		//WF::IAsyncOperationWithProgress<WF::Collections::IMap<String^, Object^>^, double>^ ParseHeader(const Array<uint8>^ buffer);

		///* 正文解析
		//	@parameter buffer body数据
		//	@parameter sEncodeType 编码格式
		//	@return 解码后的body数据
		//*/
		//WF::IAsyncOperationWithProgress<String^, double>^ ParseBody(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset);

		///* 附件解析
		//	@parameter buffer body数据
		//	@parameter sEncodeType 编码格式
		//	@return 解码后的附件数据
		//*/

		//WF::IAsyncOperationWithProgress<Array<uint8>^, double>^ ParseAttachment(const Array<uint8>^ buffer, String^ sEncodeType);

		///* 邮件夹名称解码
		//	@parameter buffer 邮件夹名称数组
		//	@return 解码后的邮件夹名称数组
		//*/
		//WF::IAsyncOperationWithProgress<Array<String^>^, double>^ DecodeMBoxNames(const Array<String^>^ buffer);

		///* 邮件夹名称编码
		//	@parameter buffer 邮件夹名称数组
		//	@return 编码后的邮件夹名称数组
		//*/
		//WF::IAsyncOperationWithProgress<Array<String^>^, double>^ EncodeMBoxNames(const Array<String^>^ buffer);

		///* 邮件夹名称解码
		//	@parameter buffer 邮件夹名称数组
		//	@return 解码后的邮件夹名称数组
		//*/
		//WF::IAsyncOperationWithProgress<String^, double>^ DecodeMBoxName(String^ name);

		///* 邮件夹名称编码
		//	@parameter buffer 邮件夹名称数组
		//	@return 编码后的邮件夹名称数组
		//*/
		//WF::IAsyncOperationWithProgress<String^, double>^ EncodeMBoxName( String^ name);

		///* 把二进制数据转化为本地字符集编码
		//	@parameter buffer 二进制数据
		//	@return 经过本地字符集编码的字符串
		//*/
		//WF::IAsyncOperationWithProgress<String^, double>^ BufferToString(const Array<uint8>^ buffer);

		/* 写入单个inline信息到html
			@parameter html 构建html
		*/
		WF::IAsyncOperation<int>^ WriteInline_AsyncFunc(int nIndex);

		/* 写入单个附件信息到msg
			@parameter msg 邮件信息
		*/
		WF::IAsyncOperation<int>^ WriteAttach_AsyncFunc(int nIndex);

		/* 邮件的构建
			@parameter mapMail 存放邮件数据的map
			@return 构建完成的邮件全文
		*/
		WF::IAsyncOperation<String^>^ BuildMail(WF::Collections::IMapView<String^, Object^>^ mapData);

	private:
		/* 写入单个附件信息到msg
			@parameter msg 邮件信息
		*/
		Concurrency::task<String^> WriteBody_AsyncFunc();

		std::string m_sPath;
		vmime::ref <vmime::message> m_Msg;
		vmime::ref <vmime::htmlTextPart> m_Html;
		std::vector<vmime::ref <vmime::defaultAttachment>> vecAttach;

		bool hasInline;
		bool hasAttach;
		Array<Object^>^ arr;
		Array<Object^>^ arrInline;

		WF::Collections::IMapView<String^, Object^>^ mapMail;
    };
}