#pragma once

#include <collection.h>

using namespace Platform;


/* mime��ʼ��
*/

void MimeInit();


/* ͷ������
	@parameter buffer ͷ������������
	@return ͷ����ֵ�Լ���
*/

Collections::Map<String^, Object^>^ _ParseHeader(const std::string& data);

/* ���Ľ���
	@parameter buffer body����
	@parameter sEncodeType �����ʽ
	@return ������body����
*/

String^ _ParseBody(const Array<uint8>^ buffer, String^ sEncodeType, String^ sCharset);

/* ��������
	@parameter buffer body����
	@parameter sEncodeType �����ʽ
	@return �����ĸ�������
*/

Array<uint8>^ _ParseAttachment(const Array<uint8>^ buffer, String^ sEncodeType);

/* �ʼ������ƽ���
	@parameter buffer �ʼ�����������
	@return �������ʼ�����������
*/

Array<String^>^ _DecodeMBoxNames(const Array<String^>^ buffer);

/* �ʼ������Ʊ���
	@parameter buffer �ʼ�����������
	@return �������ʼ�����������
*/

Array<String^>^ _EncodeMBoxNames(const Array<String^>^ buffer);


/* �����ʼ������ƽ���
	@parameter name �ʼ�����������
	@return �������ʼ�����������
*/
String^ _DecodeMBoxName(String^ name);

/* �����ʼ������Ʊ���
	@parameter name �ʼ�����������
	@return �������ʼ�����������
*/
String^ _EncodeMBoxName(String^ name);

/* �Ѷ���������ת��Ϊ�����ַ�������
	@parameter buffer ����������
	@return ���������ַ���������ַ���
*/
String^ _BufferToString(const Array<uint8>^ buffer);

/* �ʼ��Ĺ���
	@parameter mapMail ����ʼ����ݵ�map
	@return ������ɵ��ʼ�ȫ��
*/
String^ _BuildMail(Windows::Foundation::Collections::IMapView<String^, Object^>^ mapMail);