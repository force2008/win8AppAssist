#pragma once
#include <string>

namespace native
{

class CBase64
{
public:
	static std::string Base64Encode(const std::string& sSrc);
	static std::string Base64Decode(const std::string& sSrc);
};

}