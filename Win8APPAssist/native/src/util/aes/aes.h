/*
*		File		AES.h
*		Author		刘乾发
*		Email		qfliu@corp.netease.com
*       Copyright   (c) 2010-2012  网易杭州研究院
*		Date		2010-08-04
*		Comment		AES加密解密算法		基于rijndael算法version3.0
*/

#pragma once
#include "..\intType.h"

namespace native
{

#define AES_MAXKB		(256/8)
#define AES_MAXNR		14

class AES
{
public:
	AES();
	virtual ~AES();

public:
	/*!
	@brief		处理加密原始密钥为轮密钥
	@param		[out] rk		 轮密钥存放结构
	@param		[in]  cipherKey	 原始密钥数据
	@param		[in]  keyBits	 原始密钥长度
	@return		加密所需对应轮数
	*/ 
	static int MakeKeyEnc(uint32_t rk[/*4*(Nr + 1)*/], const uint8_t cipherKey[], int keyBits);

	/*!
	@brief		处理解密原始密钥为轮密钥
	@param		[out] rk		 轮密钥存放结构
	@param		[in]  cipherKey	 原始密钥数据
	@param		[in]  keyBits	 原始密钥长度
	@return		解密所需对应轮数
	*/ 
	static int MakeKeyDec(uint32_t rk[/*4*(Nr + 1)*/], const uint8_t cipherKey[], int keyBits);

	/*!
	@brief		加密
	@param		[in]  rk		 密钥数据
	@param		[in]  Nr		 加密所需轮数
	@param		[in]  pt		 明文
	@param		[out] ct		 加密中间数据(加密完成后存放密文)
	@return		void
	*/ 
	static void Encrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t pt[16], uint8_t ct[16]);

	/*!
	@brief		解密
	@param		[in]  rk		 密钥数据
	@param		[in]  Nr		 解密所需轮数
	@param		[out] pt		 解密中间数据(解密完成后存放明文)
	@param		[in]  ct		 密文
	@return		void
	*/ 
	static void Decrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t ct[16], uint8_t pt[16]);
};

}