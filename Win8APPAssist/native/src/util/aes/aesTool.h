/*
*		File		AESTool.h
*		Author		刘乾发
*		Email		qfliu@corp.netease.com
*       Copyright   (c) 2010-2012  网易杭州研究院
*		Date		2010-08-04
*		Comment		AES加密解密算法工具类
*/

#pragma once

#include "AES.h"
#include <string>

namespace native
{

/*
*	处理类型
*/
enum
{
	CIPHER_ENCRYPT = 0,					// encrypting ..
	CIPHER_DECRYPT = 1,					// decrypting ..
};

/*
*	处理方式
*/
enum
{
	CIPHER_MODE_ECB = 0,				// ECB
	CIPHER_MODE_CBC = 1,				// CBC
	CIPHER_MODE_CFB = 2,				// CFB
};

/*
*	错误码定义
*/
#define     BAD_KEY_DIR          -1		//  Key direction is invalid, e.g., unknown value
#define     BAD_KEY_MAT          -2		//  Key material not of correct length
#define     BAD_KEY_INSTANCE     -3		//  Key passed is not valid
#define     BAD_CIPHER_MODE      -4		//  Params struct passed to AESCipherInit invalid
#define     BAD_CIPHER_STATE     -5		//  Cipher in wrong state (e.g., not initialized)
#define     BAD_BLOCK_LENGTH     -6
#define     BAD_CIPHER_INSTANCE  -7
#define     BAD_DATA             -8		//  Data contents are invalid, e.g., invalid padding
#define     BAD_OTHER            -9		//  Unknown error

/*
*	其他
*/
#define     MAX_KEY_SIZE         64		//# of ASCII char's needed to represent a key
#define     MAX_IV_SIZE          16		//# bytes needed to represent an IV


/*
*	密钥信息数据
*/
typedef struct
{
    BYTE  direction;					// Key used for encrypting or decrypting
    int   keyLen;						// Length of the key
    char  keyMaterial[MAX_KEY_SIZE+1];	// Raw key data in ASCII, e.g., user input or KAT values
	int   Nr;							// key-length-dependent number of rounds
	uint32_t   rk[4*(AES_MAXNR + 1)];   // key schedule */
	uint32_t   ek[4*(AES_MAXNR + 1)];   // CFB key schedule (encryption only)
} KeyInstance;


/*
*	操作集合数据
*/
typedef struct
{
    BYTE  mode;							// MODE_ECB, MODE_CBC, or MODE_CFB
    BYTE  IV[MAX_IV_SIZE];				// A possible Initialization Vector for ciphering
} CipherInstance;


class AESTool
{
public:
	AESTool();
	virtual ~AESTool();

public:
	/*!
	@brief		处理原始密钥，计算加密/解密操作所需轮数
	@param		[out] key			密钥信息数据
	@param		[in]  direction		加密/解密
	@param		[in]  keyLen		原始密钥长度
	@param		[in]  keyMaterial	原始密钥数据
	@return		是否成功
	*/ 
	
	static int AESMakeKey(KeyInstance* key, BYTE direction, int keyLen, char *keyMaterial);

	/*!
	@brief		初始化操作方式及向量IV
	@param		[out] cipher		操作集合数据
	@param		[in]  mode			操作方式
	@param		[in]  IV			向量IV
	@return		操作结果
	*/ 
	static int AESCipherInit(CipherInstance* cipher, BYTE mode, char* IV);

	/*!
	@brief		加密小数据(16字节以内)
	@param		[in]  rk		 密钥数据
	@param		[in]  Nr		 加密所需轮数
	@param		[in]  pt		 明文
	@param		[out] ct		 加密中间数据(加密完成后存放密文)
	@return		void
	*/ 
	static void AESEncrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t pt[16], uint8_t ct[16]);

	/*!
	@brief		解密小数据(16字节以内)
	@param		[in]  rk		 密钥数据
	@param		[in]  Nr		 解密所需轮数
	@param		[out] pt		 解密中间数据(解密完成后存放明文)
	@param		[in]  ct		 密文
	@return		void
	*/ 
	static void AESDecrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t ct[16], uint8_t pt[16]);

	/*!
	@brief		加密数据块(输入内容为16字节的倍数  如：16,32,48...)
	@param		[in]  cipher	 操作集合数据
	@param		[in]  key		 密钥信息数据
	@param		[in]  input		 明文数据
	@param		[in]  inputLen	 明文数据长度
	@param		[out] outBuffer	 加密结果数据
	@return		已加密的数据大小
	*/
	static int AESBlockEncrypt(CipherInstance* cipher, KeyInstance* key, BYTE* input, int inputLen, BYTE* outBuffer);

	/*!
	@brief		解密数据块(输入内容为16字节的倍数  如：16,32,48...)
	@param		[in]  cipher	 操作集合数据
	@param		[in]  key		 密钥信息数据
	@param		[in]  input		 密文数据
	@param		[in]  inputLen	 密文数据长度
	@param		[out] outBuffer	 解密结果数据
	@return		已加密的数据大小
	*/
	static int AESBlockDecrypt(CipherInstance* cipher, KeyInstance* key, BYTE* input, int inputLen, BYTE* outBuffer);


// AES+Base64通用加解密
public:
	/*!
	@brief		通用加密(AES + Base64)
	@param		[in]  strSrc	 原始明文
	@param		[out] strDest 加密后字符串
	@return		是否成功
	*/
	static BOOL AESBase64Encrypt(const std::string& strSrc, std::string& strDest);

	/*!
	@brief		通用解密(Base64 + AES)
	@param		[in]  strSrc	 加密后字符串
	@param		[out] strDest 原始明文
	@return		是否成功
	*/
	static BOOL AESBase64Decrypt(const std::string& strSrc, std::string& strDest);
};

}