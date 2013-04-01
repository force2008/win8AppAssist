// wbxml.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "wbxml.h"
#include "wbxml_defines.h"
#include "wbxml_errors.h"
#include "wbxml_conv.h"

using namespace Platform;
using namespace wbxml;

std::wstring s2ws(const std::string& s, UINT nCodePage = CP_UTF8);

std::wstring s2ws(const std::string& s, UINT nCodePage)
{
    const char* _Source = s.c_str();
    size_t _Dsize = MultiByteToWideChar(nCodePage, 0, _Source, -1, NULL, 0);
    wchar_t *_Dest = new wchar_t[_Dsize];
    MultiByteToWideChar(nCodePage, 0, _Source, -1, _Dest, _Dsize);
    std::wstring result = _Dest;
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

String^ StdStringToString(const std::string& s)
{
	std::wstring ws = s2ws(s);
	return ref new String(ws.c_str(), ws.length());
}

std::string StringToStdString(String^ s)
{
	std::string str = ws2s(s->Data());
	return str;
}

WBXMLParser::WBXMLParser()
{

}

String^ WBXMLParser::wbxml2xml(const Array<uint8>^ buffer)
{
	WB_UTINY *xml = NULL;
	WB_ULONG xml_len = 0;
	WBXMLError result = WBXML_OK;
	WBXMLConvWBXML2XML *conv = NULL;
	String^ ret;

	result = wbxml_conv_wbxml2xml_create(&conv);
	if (result != WBXML_OK)
	{
		goto clean_up;
	}

	result = wbxml_conv_wbxml2xml_run(conv, &buffer[0], buffer->Length, &xml, &xml_len);
    if (result == WBXML_OK) {
		std::string str((char*)xml, xml_len);
		ret = StdStringToString(str);
    }

	if (xml != NULL)
		free(xml);

clean_up:
    if (conv != NULL)
        wbxml_conv_wbxml2xml_destroy(conv);

	return ret;
}

Array<uint8>^ WBXMLParser::xml2wbxml(String^ sXml)
{
	std::string xml = StringToStdString(sXml);
	WB_UTINY *wbxml = NULL;
	WB_ULONG wbxml_len = 0;
	WBXMLError result = WBXML_OK;
	WBXMLConvXML2WBXML *conv = NULL;
	Array<uint8>^ ret;

	result = wbxml_conv_xml2wbxml_create(&conv);
	if (result != WBXML_OK)
	{
		goto clean_up;
	}

	wbxml_conv_xml2wbxml_disable_public_id(conv);

	result = wbxml_conv_xml2wbxml_run(conv, (unsigned char*)xml.c_str(), xml.size(), &wbxml, &wbxml_len);
    if (result == WBXML_OK) {
		ret =  ref new Array<uint8>(wbxml, wbxml_len);
    }

	if (wbxml != NULL)
		free(wbxml);

clean_up:
    if (conv != NULL)
        wbxml_conv_xml2wbxml_destroy(conv);

	return ret;
}