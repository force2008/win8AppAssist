/*
*		File		AESTool_Comm.cpp
*		Author		刘乾发
*		Email		qfliu@corp.netease.com
*       Copyright   (c) 2010-2012  网易杭州研究院
*		Date		2010-09-6
*		Comment		密码专用加/解密模块
*		Remark		该文件不可被修改！
*/


#include "aes.h"
#include "aesTool.h"
#include "../../native/mimeutil.h"

#define KEY_LEN		128
#define KEY_BYTE	(KEY_LEN/8)

static const uint8_t tk0[32] = {
	0x03, 0xfa, 0xcd, 0xd4,
	0xa9, 0x73, 0x6e, 0xf1,
	0x3c, 0x6a, 0x87, 0x3b,
	0xcd, 0xb9, 0xf2, 0xff,
	0x05, 0x29, 0xf3, 0x7c,
	0xff, 0xcc, 0x22, 0x81,
	0x1e, 0x2b, 0xaa, 0xdd,
	0xc7, 0x02, 0x6a, 0x10,
};

static const uint8_t tk1[32] = {
	0xaf, 0xf4, 0xee, 0xcc,
	0x21, 0x42, 0x65, 0x7f,
	0x00, 0x4c, 0x1a, 0x47,
	0x5a, 0x88, 0xff, 0xb7,
	0x3b, 0x46, 0x84, 0xa4,
	0xbb, 0xca, 0xef, 0x61,
	0x63, 0x9a, 0xb4, 0xc2,
	0xff, 0xa1, 0x7d, 0xbb,
};

static const uint8_t tk2[32] = {
	0xf3, 0xa4, 0x3c, 0x6a,
	0x88, 0xf0, 0x2a, 0xaa,
	0x4c, 0x30, 0x3b, 0xbc,
	0xa2, 0xd5, 0xfc, 0xcf,
	0xee, 0x37, 0x71, 0x55,
	0x8f, 0x2a, 0xcc, 0x47,
	0x3a, 0x41, 0x3d, 0x2c,
	0x76, 0xff, 0x2d, 0xa9,
};

static const uint8_t tk3[32] = {
	0x1c, 0x4c, 0x29, 0x82,
	0x32, 0x3e, 0xad, 0xc0,
	0x7e, 0xcb, 0x6d, 0x0f,
	0x42, 0xc7, 0xe3, 0x2d,
	0xe7, 0x8a, 0xcc, 0xa1,
	0xdd, 0xd3, 0xad, 0x9a,
	0x24, 0x10, 0xfc, 0xef,
	0xe2, 0xe4, 0x77, 0x08,
};

void MakeCipherKey(BYTE key[320])
{	
	memset(key, 0, sizeof(key));

	uint32_t uValue = 0;
	for (int i=0; i<KEY_BYTE; ++i)
	{
		int nShift = i % 4;
		int nSubOffSet = i/4;

		switch (nShift)
		{
		case 0:
			{
				uValue = (tk0[0] << nSubOffSet) + tk1[1];
			}
			break;
		case 1:
			{
				uValue = (tk0[1] << nSubOffSet) + tk1[2];
			}
			break;
		case 2:
			{
				uValue = (tk0[2] << nSubOffSet) + tk1[3];
			}
			break;
		case 3:
		default:
			{
				uValue = (tk0[3] << nSubOffSet) + tk1[0];
			}		
			break;
		}

		sprintf((char*)&key[2*i], "%02X", uValue);
	}
}


namespace native
{

BOOL AESTool::AESBase64Encrypt(const std::string& strSrc, std::string& strDest)
{
	// 确保输入长度能整除16
	int nLen = strSrc.size();
	int nOffset = nLen % 16;
	if (nOffset > 0)
		nLen += (16-nOffset);

	BYTE* pt = new BYTE[nLen+1];
	BYTE* ct = new BYTE[nLen+1];
	memset(pt, 0, nLen+1);
	memset(ct, 0, nLen+1);

	memcpy(pt, strSrc.c_str(), strSrc.size());

	// 生成密钥:
	BYTE key[320];
	MakeCipherKey(key);

	// AES加密	
	KeyInstance keyInst;
	CipherInstance cpInst;		
	AESTool::AESCipherInit(&cpInst, CIPHER_MODE_CBC, NULL);
	AESTool::AESMakeKey(&keyInst, CIPHER_ENCRYPT, KEY_LEN, (char*)key);
	AESTool::AESBlockEncrypt(&cpInst, &keyInst, pt, nLen*8, ct);

	// Base64加密
	//std::string strCyper;
	//strCyper.append((const char*)ct, nLen);

	strDest.clear();
	//strDest = Base64Encode(strCyper);
	strDest = Base64Encode((const char*)ct, nLen);

	delete []pt;
	delete []ct;

	return TRUE;
}


BOOL AESTool::AESBase64Decrypt(const std::string& strSrc, std::string& strDest)
{
	// Base64解密
	std::string strTemp;
	strTemp = Base64Decode(strSrc);

	// 确保输入长度能整除16
	int nLen = strTemp.size();
	int nOffset = nLen % 16;
	if (nOffset > 0)
		nLen += (16-nOffset);

	BYTE* pt = new BYTE[nLen+1];
	BYTE* ct = new BYTE[nLen+1];
	memset(pt, 0, nLen+1);
	memset(ct, 0, nLen+1);

	memcpy(ct, strTemp.c_str(), strTemp.size());

	// 生成密钥:
	BYTE key[320];
	MakeCipherKey(key);

	// AES解密
	KeyInstance keyInst;
	CipherInstance cpInst;
	AESTool::AESCipherInit(&cpInst, CIPHER_MODE_CBC, NULL);
	AESTool::AESMakeKey(&keyInst, CIPHER_DECRYPT, KEY_LEN, (char*)key);
	AESTool::AESBlockDecrypt(&cpInst, &keyInst, ct, nLen*8, pt);

	// 最终字符串
	strDest.clear();
	strDest.append((const char*)pt, nLen);

	delete []pt;
	delete []ct;

	return TRUE;
}

}