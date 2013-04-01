#pragma once

#include <string>
#include <collection.h>

using namespace Platform;

namespace wbxml
{
    public ref class WBXMLParser sealed
    {
	public:
		WBXMLParser();
		static String^ wbxml2xml(const Array<uint8>^ buffer);
		static Array<uint8>^ xml2wbxml(String^ sXml);
	};
}