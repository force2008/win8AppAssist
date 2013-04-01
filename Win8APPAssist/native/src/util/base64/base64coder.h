#pragma once
#include "../intType.h"

/*
*
*  ���������Ĺ�����
*  ����		Tintin Tintin2000@263.net
*  ����		2001-4-15
*  ����޸� 2001-4-19	
*
*/

namespace native
{

class CBase64Coder
{
public:
	static unsigned int m_LineWidth;						//ָ�������ÿ�еĳ��ȣ�ȱʡ��76
	static const char BASE64_ENCODE_TABLE[64];				//Base64�����
	static const unsigned int BASE64_DECODE_TABLE[256];		//Base64�����
	static const byte QUOTED_ENCODE_TABLE[256];	//Quoted�����
private:
	inline static char DecToHex(char B);		//ΪQuoted������������ַ�ת��
	inline static char HexToDec(char C);		//ΪQuoted������������ַ�ת��
public:
	static biguint Base64EncodeSize(biguint iSize);		//�����ļ�ʵ�ʳ��Ȼ�ȡ����Base64��ĳ���  
	static biguint Base64DecodeSize(biguint iSize);		//�����ѱ����ļ����Ȼ�ȡBase64�Ľ��볤��
	static biguint UUEncodeSize(biguint iSize);			//�����ļ�ʵ�ʳ��Ȼ�ȡUUCode�����ĳ���
	static biguint UUDecodeSize(biguint iSize);			//�����ѱ����ļ����Ȼ�ȡUUCode�����ĳ���
	static biguint QuotedEncodeSize(biguint iSize);		//����ʵ���ļ��ĳ��Ȼ�ȡQuoted����

	/*
	*  ��һ��Buffer����Base64����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� Base64EncodeSize(int) ����ȡ��
	*/
	static int base64_encode(const char *pSrc, unsigned int nSize, char *pDest);

	/*
	*  ��һ��Buffer����Base64����
	*	
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*		return	������ʵ�ʳ���
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� Base64DecodeSize(int) ����ȡ��
	*/
	static int  base64_decode(const char *pSrc, unsigned int nSize, char *pDest);

	/*
	*  ��һ��Buffer����UUCODE����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� UUEncodeSize(int) ����ȡ��
	*/
	static int UU_encode(char *pSrc, unsigned int nSize, char *pDest);
	
	/*
	*  ��һ��Buffer����UUCODE����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� UUDecodeSize(int) ����ȡ��
	*/
	static int UU_decode(char *pSrc, unsigned int nSize, char *pDest);
	/*
	*  ��һ��Buffer����Quoted-Printble����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*		return	������ʵ�ʳ���
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� QuotedDecodeSize(int) ����ȡ��
	*/
	static int quoted_encode(char *pSrc, unsigned int nSize, char *pDest);
	
	/*
	*  ��һ��Buffer����Quoted-Printble����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*		return	������ʵ�ʳ���
	*
	*	 ע������û���ṩ������볤�ȵķ��� ֱ��ʹ������Buffer�����Buffer�Ϳ�����
	*/
	static int quoted_decode(char *pSrc, unsigned int nSize, char *pDest);
	
};

}
