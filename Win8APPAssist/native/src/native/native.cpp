// native.cpp
#include "pch.h"
#include "native.h"
#include "mimeapi.h"
#include "modifiedPpltasks.h"
#include "mimeutil.h"

using namespace native;
using namespace Platform;
using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

MimeParser::MimeParser()
{
	hasInline = false;
	hasAttach = false;
	MimeInit();
}

WF::Collections::IMap<String^, Object^>^ MimeParser::ParseHeader_Sync(const Array<uint8>^ buffer)
{
	std::string str = ArrayToStdString(buffer);

	// 测试代码
	//mapMail = _ParseHeader(str)->GetView();
	//auto task = create_task(this->BuildMail());
	//task.then([this](String^ ret){
	//	String^ mail = ret;
	//});

	return _ParseHeader(str);
}

String^ MimeParser::ParseBody_Sync(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset)
{
	return _ParseBody(buffer, sEncodeType, sCharset);
}

Array<uint8>^ MimeParser::ParseAttachment_Sync(const Array<uint8>^ buffer, String^ sEncodeType)
{
	return _ParseAttachment(buffer, sEncodeType);
}

String^ MimeParser::DecodeMBoxName_Sync(String^ name)
{
	return _DecodeMBoxName(name);
}

String^ MimeParser::EncodeMBoxName_Sync(String^ name)
{
	return _EncodeMBoxName(name);
}

String^ MimeParser::BufferToString_Sync(const Array<uint8>^ buffer)
{
	return _BufferToString(buffer);
}

String^ MimeParser::ArrayToString_Sync(const Array<uint8>^ buffer)
{
	return ArrayToString(buffer);
}

String^ MimeParser::BuildMail_Sync(WF::Collections::IMapView<String^, Object^>^ mapMail)
{
	return _BuildMail(mapMail);
}

String^ MimeParser::Base64Encode_Sync(String^ sText)
{
	return StdStringToString(Base64Encode(sText));
}

String^ MimeParser::Base64Decode_Sync(String^ sText)
{
	return StdStringToString(Base64Decode(sText));
}

//String^ MimeParser::Base64Encode_Sync(const Array<uint8>^ buffer)
//{
//	return StdStringToString(Base64Encode(buffer));
//}
//
//String^ MimeParser::Base64Decode_Sync(const Array<uint8>^ buffer)
//{
//	return StdStringToString( Base64Decode(buffer));
//}

String^ MimeParser::GenerateContentId_Sync()
{
	return StdStringToString(vmime::messageId::generateId().getId());
}

//WF::IAsyncOperationWithProgress<WF::Collections::IMap<String^, Object^>^, double>^
//	MimeParser::ParseHeader(const Array<uint8>^ buffer)
//{
//	std::string str = ArrayToStdString(buffer);
//	return create_async([this, &str](progress_reporter<double> reporter)-> WF::Collections::IMap<String^, Object^>^
//	{
//		return _ParseHeader(str);
//	});
//}
//
//WF::IAsyncOperationWithProgress<String^, double>^
//	MimeParser::ParseBody(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset)
//{
//	return create_async([this, &buffer, &sEncodeType, &sCharset](progress_reporter<double> reporter)-> String^
//	{
//		return _ParseBody(buffer, sEncodeType, sCharset);
//	});
//}
//
//WF::IAsyncOperationWithProgress<Array<uint8>^, double>^
//	MimeParser::ParseAttachment(const Array<uint8>^ buffer, String^ sEncodeType)
//{
//	return create_async([this, &buffer, &sEncodeType](progress_reporter<double> reporter)-> Array<uint8>^
//	{
//		return _ParseAttachment(buffer, sEncodeType);
//	});
//}
//
//WF::IAsyncOperationWithProgress<Array<String^>^, double>^
//	MimeParser::DecodeMBoxNames(const Array<String^>^ buffer)
//{
//	return create_async([this, &buffer](progress_reporter<double> reporter)-> Array<String^>^
//	{
//		return _DecodeMBoxNames(buffer);
//	});
//}
//
//WF::IAsyncOperationWithProgress<Array<String^>^, double>^
//	MimeParser::EncodeMBoxNames(const Array<String^>^ buffer)
//{
//	return create_async([this, &buffer](progress_reporter<double> reporter)-> Array<String^>^
//	{
//		return _EncodeMBoxNames(buffer);
//	});
//}
//
//
//WF::IAsyncOperationWithProgress<String^, double>^ MimeParser::DecodeMBoxName(String^ name)
//{
//	return create_async([this, &name](progress_reporter<double> reporter)-> String^
//	{
//		return _DecodeMBoxName(name);
//	});
//}
//
//
//WF::IAsyncOperationWithProgress<String^, double>^ MimeParser::EncodeMBoxName(String^ name)
//{
//	return create_async([this, &name](progress_reporter<double> reporter)-> String^
//	{
//		return _EncodeMBoxName(name);
//	});
//}
//
//WF::IAsyncOperationWithProgress<String^, double>^ MimeParser::BufferToString(const Array<uint8>^ buffer)
//{
//	return create_async([this, &buffer](progress_reporter<double> reporter)-> String^
//	{
//		return _BufferToString(buffer);
//	});
//}

#include "vmime/utility/encoder/b64Encoder.hpp"
using namespace vmime::utility::encoder;
namespace WFC = Windows::Foundation::Collections;

WF::IAsyncOperation<int>^ MimeParser::WriteInline_AsyncFunc(int nIndex)
{
	auto dummy = create_async([]()->int{
		return 0;
	});

#ifndef LOCAL_DEBUG
	auto mapInline = (WFC::PropertySet^)arrInline[nIndex];
#else
	Collections::Map<String^, String^>^ mapInline = (Collections::Map<String^, String^>^)arrInline[nIndex];
#endif
	if (!mapInline->HasKey("CONTENT"))
		return dummy;

	std::string sPath = StringToStdString((String^)mapInline->Lookup("CONTENT"));
	if (sPath.length() == 0)
		return dummy;

	auto task1 = create_task(StorageFolder::GetFolderFromPathAsync(StdStringToString(GetFileDirFromPath(sPath))));
	auto task2 = task1.then([this, sPath](StorageFolder^ folder) {
		String^ sFileName = StdStringToString(GetFileNameFromPath(sPath));
		return folder->CreateFileAsync(sFileName, CreationCollisionOption::OpenIfExists);
	});
	auto task3 = task2.then([this](StorageFile^ file) {
		return FileIO::ReadBufferAsync(file);
	});
	auto task4 = task3.then([this, mapInline](IBuffer^ buf) ->int {
		//IBuffer^ buf = getBuffer.get();
		DataReader^ dataReader = DataReader::FromBuffer(buf);

		vmime::ref <vmime::stringContentHandler> text = vmime::create<vmime::stringContentHandler>();
		vmime::encoding enc(vmime::encodingTypes::BASE64);

		//vmime::encoding enc(vmime::encodingTypes::BASE64);
		vmime::ref <b64Encoder> benc = vmime::create<b64Encoder>();
		try
		{
			std::string name;
			if (mapInline->HasKey("NAME"))
				name = StringToStdString((String^)mapInline->Lookup("NAME"));

			benc->getProperties().setProperty<int>("maxlinelength", 76);
			text->setData(benc->encode(dataReader, NULL), enc);
			std::string sContentType = GetContentTypeFromExt(name);

			if (!mapInline->HasKey("CONTENTID"))
				return 0;
			std::string id = StringToStdString((String^)mapInline->Lookup("CONTENTID"));

			m_Html->addObject(text, GetFormattedWord(name), id, enc,
				(sContentType.length() == 0) ? vmime::mediaType() : vmime::mediaType(sContentType));
			return 0;
		}
		catch (vmime::exceptions::no_encoder_available ex)
		{
			return 0;
		}
	});

	return create_async([task4](){
		return task4;
	});  
}

WF::IAsyncOperation<int>^ MimeParser::WriteAttach_AsyncFunc(int nIndex)
{
	auto dummy = create_async([]()->int{
		return 0;
	});

#ifndef LOCAL_DEBUG
	auto mapAttach = (WFC::PropertySet^)arr[nIndex];
#else
	Collections::Map<String^, String^>^ mapAttach = (Collections::Map<String^, String^>^)arr[nIndex];
#endif

	if (!mapAttach->HasKey("CONTENT"))
		return dummy;

	std::string sPath = StringToStdString((String^)mapAttach->Lookup("CONTENT"));
	if (sPath.length() == 0)
		return dummy;

	auto task1 = create_task(StorageFolder::GetFolderFromPathAsync(StdStringToString(GetFileDirFromPath(sPath))));
	auto task2 = task1.then([this, sPath](StorageFolder^ folder) {
		String^ sFileName = StdStringToString(GetFileNameFromPath(sPath));
		return folder->CreateFileAsync(sFileName, CreationCollisionOption::OpenIfExists);
	});
	auto task3 = task2.then([this](StorageFile^ file) {
		return FileIO::ReadBufferAsync(file);
	});
	auto task4 = task3.then([this, mapAttach](IBuffer^ buf) ->int {
		//IBuffer^ buf = getBuffer.get();
		DataReader^ dataReader = DataReader::FromBuffer(buf);

		vmime::ref <vmime::stringContentHandler> text = vmime::create<vmime::stringContentHandler>();
		vmime::encoding enc(vmime::encodingTypes::BASE64);

		//vmime::encoding enc(vmime::encodingTypes::BASE64);
		vmime::ref <b64Encoder> benc = vmime::create<b64Encoder>();
		try
		{
			std::string name;
			if (mapAttach->HasKey("NAME"))
				name = StringToStdString((String^)mapAttach->Lookup("NAME"));

			benc->getProperties().setProperty<int>("maxlinelength", 76);
			text->setData(benc->encode(dataReader, NULL), enc);
			std::string sContentType = GetContentTypeFromExt(name);

			vmime::ref <vmime::defaultAttachment> attach = vmime::create<vmime::defaultAttachment>(text,
				enc,
				(sContentType.length() == 0) ? vmime::mediaType() : vmime::mediaType(sContentType),
				vmime::NULL_TEXT,
				name.length() == 0 ? vmime::NULL_WORD : GetFormattedWord(name));

			attach->generateIn(m_Msg);
			return 0;
		}
		catch (vmime::exceptions::no_encoder_available ex)
		{
			return -1;
		}
	});

	return create_async([task4](){
		return task4;
	});  
}

task<String^> MimeParser::WriteBody_AsyncFunc()
{
	std::vector<task<int>> vecTask;
	if (hasInline)
	{
		for (int i=0; i< (int)arrInline->Length; i++)
		{
			task<int> tsk = create_task(WriteInline_AsyncFunc(i));
			vecTask.push_back(tsk);
		}
	}

	auto join =  when_all(vecTask.begin(), vecTask.end());
	return join.then([this](vector<int> vec) -> String^
	{
		if (hasAttach && m_Html->getPartCount() > 1)
		{
			// Set parent part (message) to "multipart/mixed"
			m_Msg->getHeader()->ContentType()->setValue
				(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_MIXED));

			vmime::ref <vmime::bodyPart> subPart = vmime::create <vmime::bodyPart>();
			m_Msg->getBody()->appendPart(subPart);

			subPart->getHeader()->ContentType()->setValue
				(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_ALTERNATIVE));

			m_Html->generateIn(m_Msg, subPart);
		}
		else
		{
			// Generate the text part(s) directly into the message
			m_Html->generateIn(m_Msg, m_Msg);

			if (hasAttach)
			{
				m_Msg->getHeader()->ContentType()->setValue
					(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_MIXED));
			}
			else if (m_Html->getPartCount() > 1)
			{
				m_Msg->getHeader()->ContentType()->setValue
					(vmime::mediaType(vmime::mediaTypes::MULTIPART, vmime::mediaTypes::MULTIPART_ALTERNATIVE));
			}
		}

		return "";
	}).then([this](String^ str)
	{
		std::vector<task<int>> vecTaskAttach;
		for (int i=0; i<(int)arr->Length; i++)
		{
			task<int> tsk = create_task(WriteAttach_AsyncFunc(i));
			vecTaskAttach.push_back(tsk);
		}

		auto join1 = when_all(vecTaskAttach.begin(), vecTaskAttach.end());
		return join1;
	}).then([this](std::vector<int> vec) -> String^
	{
		if (m_Msg->getBody()->getPartCount() == 1)
		{
			const vmime::bodyPart& part = *m_Msg->getBody()->getPartAt(0);
			vmime::ref <vmime::body> bodyCopy = part.getBody()->clone().dynamicCast <vmime::body>();
			const std::vector <vmime::ref <const vmime::headerField> > fields = part.getHeader()->getFieldList();
			for (std::vector <vmime::ref <const vmime::headerField> >::const_iterator it = fields.begin() ;
				it != fields.end() ; ++it)
			{
				*(m_Msg->getHeader()->getField((*it)->getName())) = **it;
			}

			m_Msg->getBody()->copyFrom(*bodyCopy);
		}

		return StdStringToString(m_Msg->generate());
	});
}

WF::IAsyncOperation<String^>^ MimeParser::BuildMail(WF::Collections::IMapView<String^, Object^>^ mapData)
{
	return create_async([this, mapData]()
	{
		m_Msg = vmime::create <vmime::message>();

		mapMail = mapData;
		// 先按指定顺序生成一些头部信息
		BuildCommonHeader(mapMail, m_Msg);

		// 构建其他头部信息
		WFC::IIterator<WFC::IKeyValuePair<String^, Object^>^>^ iter = mapMail->First();
		while (iter->HasCurrent)
		{
			WFC::IKeyValuePair<String^, Object^>^ cur = iter->Current;
			BuildFieldFromKeyAndValue(m_Msg, cur->Key, cur->Value);

			iter->MoveNext();
		}

		String^ err("");
		vmime::ref <vmime::stringContentHandler> text = vmime::create<vmime::stringContentHandler>();
		//vmime::encoding enc(/*vmime::encodingTypes::BASE64*/);
		m_Html = vmime::create<vmime::htmlTextPart>();

		Object^ obj;
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

		//mapMail->HasKey("CONTENT-HTML");
		if (!mapMail->HasKey("CONTENT-HTML"))
		{
			return create_task([err]() {
				return err;
			});
		}
		obj = mapMail->Lookup("CONTENT-HTML");
		if (!((String^)obj)->IsEmpty())
		{
			text->setData(StringToStdString((String^)obj));
			m_Html->setCharset(vmime::charsets::UTF_8);
			m_Html->setText(text);

			if (mapMail->HasKey("CONTENT-TEXT"))
			{
				obj = mapMail->Lookup("CONTENT-TEXT");
				if (!((String^)obj)->IsEmpty())
				{
					text->setData(StringToStdString((String^)obj));
					m_Html->setPlainText(text);
				}	
			}

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
		}

		return WriteBody_AsyncFunc();
	});
}