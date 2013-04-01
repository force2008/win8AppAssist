#pragma once
#include "../intType.h"

/*
*
*  编码与解码的功能类
*  作者		Tintin Tintin2000@263.net
*  日期		2001-4-15
*  最后修改 2001-4-19	
*
*/

namespace native
{

class CBase64Coder
{
public:
	static unsigned int m_LineWidth;						//指定编码后每行的长度，缺省是76
	static const char BASE64_ENCODE_TABLE[64];				//Base64编码表
	static const unsigned int BASE64_DECODE_TABLE[256];		//Base64解码表
	static const byte QUOTED_ENCODE_TABLE[256];	//Quoted编码表
private:
	inline static char DecToHex(char B);		//为Quoted编码操作进行字符转换
	inline static char HexToDec(char C);		//为Quoted解码操作进行字符转换
public:
	static biguint Base64EncodeSize(biguint iSize);		//根据文件实际长度获取编码Base64后的长度  
	static biguint Base64DecodeSize(biguint iSize);		//根据已编码文件长度获取Base64的解码长度
	static biguint UUEncodeSize(biguint iSize);			//根据文件实际长度获取UUCode编码后的长度
	static biguint UUDecodeSize(biguint iSize);			//根据已编码文件长度获取UUCode解码后的长度
	static biguint QuotedEncodeSize(biguint iSize);		//根据实际文件的长度获取Quoted编码

	/*
	*  对一段Buffer进行Base64编码
	*
	*	 	pSrc	输入Buffer
	*		nSize	Buffer长度
	*		pDest	输出缓冲
	*
	*	 注: 输出Buffer的长度可以使用 Base64EncodeSize(int) 方法取得
	*/
	static int base64_encode(const char *pSrc, unsigned int nSize, char *pDest);

	/*
	*  对一段Buffer进行Base64解码
	*	
	*	 	pSrc	输入Buffer
	*		nSize	Buffer长度
	*		pDest	输出缓冲
	*		return	解码后的实际长度
	*
	*	 注: 输出Buffer的长度可以使用 Base64DecodeSize(int) 方法取得
	*/
	static int  base64_decode(const char *pSrc, unsigned int nSize, char *pDest);

	/*
	*  对一段Buffer进行UUCODE编码
	*
	*	 	pSrc	输入Buffer
	*		nSize	Buffer长度
	*		pDest	输出缓冲
	*
	*	 注: 输出Buffer的长度可以使用 UUEncodeSize(int) 方法取得
	*/
	static int UU_encode(char *pSrc, unsigned int nSize, char *pDest);
	
	/*
	*  对一段Buffer进行UUCODE解码
	*
	*	 	pSrc	输入Buffer
	*		nSize	Buffer长度
	*		pDest	输出缓冲
	*
	*	 注: 输出Buffer的长度可以使用 UUDecodeSize(int) 方法取得
	*/
	static int UU_decode(char *pSrc, unsigned int nSize, char *pDest);
	/*
	*  对一段Buffer进行Quoted-Printble编码
	*
	*	 	pSrc	输入Buffer
	*		nSize	Buffer长度
	*		pDest	输出缓冲
	*		return	编码后的实际长度
	*
	*	 注: 输出Buffer的长度可以使用 QuotedDecodeSize(int) 方法取得
	*/
	static int quoted_encode(char *pSrc, unsigned int nSize, char *pDest);
	
	/*
	*  对一段Buffer进行Quoted-Printble解码
	*
	*	 	pSrc	输入Buffer
	*		nSize	Buffer长度
	*		pDest	输出缓冲
	*		return	解码后的实际长度
	*
	*	 注：这里没有提供计算解码长度的方法 直接使用输入Buffer作输出Buffer就可以了
	*/
	static int quoted_decode(char *pSrc, unsigned int nSize, char *pDest);
	
};

}
