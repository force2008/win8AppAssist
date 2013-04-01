/*
*		File		AES.h
*		Author		��Ǭ��
*		Email		qfliu@corp.netease.com
*       Copyright   (c) 2010-2012  ���׺����о�Ժ
*		Date		2010-08-04
*		Comment		AES���ܽ����㷨		����rijndael�㷨version3.0
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
	@brief		�������ԭʼ��ԿΪ����Կ
	@param		[out] rk		 ����Կ��Žṹ
	@param		[in]  cipherKey	 ԭʼ��Կ����
	@param		[in]  keyBits	 ԭʼ��Կ����
	@return		���������Ӧ����
	*/ 
	static int MakeKeyEnc(uint32_t rk[/*4*(Nr + 1)*/], const uint8_t cipherKey[], int keyBits);

	/*!
	@brief		�������ԭʼ��ԿΪ����Կ
	@param		[out] rk		 ����Կ��Žṹ
	@param		[in]  cipherKey	 ԭʼ��Կ����
	@param		[in]  keyBits	 ԭʼ��Կ����
	@return		���������Ӧ����
	*/ 
	static int MakeKeyDec(uint32_t rk[/*4*(Nr + 1)*/], const uint8_t cipherKey[], int keyBits);

	/*!
	@brief		����
	@param		[in]  rk		 ��Կ����
	@param		[in]  Nr		 ������������
	@param		[in]  pt		 ����
	@param		[out] ct		 �����м�����(������ɺ�������)
	@return		void
	*/ 
	static void Encrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t pt[16], uint8_t ct[16]);

	/*!
	@brief		����
	@param		[in]  rk		 ��Կ����
	@param		[in]  Nr		 ������������
	@param		[out] pt		 �����м�����(������ɺ�������)
	@param		[in]  ct		 ����
	@return		void
	*/ 
	static void Decrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr, const uint8_t ct[16], uint8_t pt[16]);
};

}