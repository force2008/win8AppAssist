/*
*		File		AESTool.h
*		Author		��Ǭ��
*		Email		qfliu@corp.netease.com
*       Copyright   (c) 2010-2012  ���׺����о�Ժ
*		Date		2010-08-04
*		Comment		AES���ܽ����㷨������
*/

#pragma once

#include "AES.h"
#include <string>

namespace native
{

/*
*	��������
*/
enum
{
	CIPHER_ENCRYPT = 0,					// encrypting ..
	CIPHER_DECRYPT = 1,					// decrypting ..
};

/*
*	����ʽ
*/
enum
{
	CIPHER_MODE_ECB = 0,				// ECB
	CIPHER_MODE_CBC = 1,				// CBC
	CIPHER_MODE_CFB = 2,				// CFB
};

/*
*	�����붨��
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
*	����
*/
#define     MAX_KEY_SIZE         64		//# of ASCII char's needed to represent a key
#define     MAX_IV_SIZE          16		//# bytes needed to represent an IV


/*
*	��Կ��Ϣ����
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
*	������������
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
	@brief		����ԭʼ��Կ���������/���ܲ�����������
	@param		[out] key			��Կ��Ϣ����
	@param		[in]  direction		����/����
	@param		[in]  keyLen		ԭʼ��Կ����
	@param		[in]  keyMaterial	ԭʼ��Կ����
	@return		�Ƿ�ɹ�
	*/ 
	
	static int AESMakeKey(KeyInstance* key, BYTE direction, int keyLen, char *keyMaterial);

	/*!
	@brief		��ʼ��������ʽ������IV
	@param		[out] cipher		������������
	@param		[in]  mode			������ʽ
	@param		[in]  IV			����IV
	@return		�������
	*/ 
	static int AESCipherInit(CipherInstance* cipher, BYTE mode, char* IV);

	/*!
	@brief		����С����(16�ֽ�����)
	@param		[in]  rk		 ��Կ����
	@param		[in]  Nr		 ������������
	@param		[in]  pt		 ����
	@param		[out] ct		 �����м�����(������ɺ�������)
	@return		void
	*/ 
	static void AESEncrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t pt[16], uint8_t ct[16]);

	/*!
	@brief		����С����(16�ֽ�����)
	@param		[in]  rk		 ��Կ����
	@param		[in]  Nr		 ������������
	@param		[out] pt		 �����м�����(������ɺ�������)
	@param		[in]  ct		 ����
	@return		void
	*/ 
	static void AESDecrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t ct[16], uint8_t pt[16]);

	/*!
	@brief		�������ݿ�(��������Ϊ16�ֽڵı���  �磺16,32,48...)
	@param		[in]  cipher	 ������������
	@param		[in]  key		 ��Կ��Ϣ����
	@param		[in]  input		 ��������
	@param		[in]  inputLen	 �������ݳ���
	@param		[out] outBuffer	 ���ܽ������
	@return		�Ѽ��ܵ����ݴ�С
	*/
	static int AESBlockEncrypt(CipherInstance* cipher, KeyInstance* key, BYTE* input, int inputLen, BYTE* outBuffer);

	/*!
	@brief		�������ݿ�(��������Ϊ16�ֽڵı���  �磺16,32,48...)
	@param		[in]  cipher	 ������������
	@param		[in]  key		 ��Կ��Ϣ����
	@param		[in]  input		 ��������
	@param		[in]  inputLen	 �������ݳ���
	@param		[out] outBuffer	 ���ܽ������
	@return		�Ѽ��ܵ����ݴ�С
	*/
	static int AESBlockDecrypt(CipherInstance* cipher, KeyInstance* key, BYTE* input, int inputLen, BYTE* outBuffer);


// AES+Base64ͨ�üӽ���
public:
	/*!
	@brief		ͨ�ü���(AES + Base64)
	@param		[in]  strSrc	 ԭʼ����
	@param		[out] strDest ���ܺ��ַ���
	@return		�Ƿ�ɹ�
	*/
	static BOOL AESBase64Encrypt(const std::string& strSrc, std::string& strDest);

	/*!
	@brief		ͨ�ý���(Base64 + AES)
	@param		[in]  strSrc	 ���ܺ��ַ���
	@param		[out] strDest ԭʼ����
	@return		�Ƿ�ɹ�
	*/
	static BOOL AESBase64Decrypt(const std::string& strSrc, std::string& strDest);
};

}