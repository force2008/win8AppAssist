#include "base64.h"
#include "base64coder.h"

namespace native
{

std::string CBase64::Base64Encode(const std::string& sSrc)
{
	std::string sDest;
	int nLen = CBase64Coder::Base64EncodeSize(sSrc.length());
	sDest.resize(nLen);
	nLen = CBase64Coder::base64_encode(sSrc.data(), sSrc.length(), (char*)sDest.data());
	sDest.resize(nLen);
	return sDest;
}

std::string CBase64::Base64Decode(const std::string& sSrc)
{
	std::string sDest;
	int nLen = CBase64Coder::Base64DecodeSize(sSrc.length());
	sDest.resize(nLen);
	nLen = CBase64Coder::base64_decode(sSrc.data(), sSrc.length(), (char*)sDest.data());
	sDest.resize(nLen);
	return sDest;
}

}