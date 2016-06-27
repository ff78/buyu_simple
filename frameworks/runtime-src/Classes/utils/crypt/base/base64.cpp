#include "base64.h"

typedef unsigned char byte;

//===================================
//    Base64 解码
//===================================
static byte Decode_GetByte(char c)
{
	if(c == '+')
		return 62;
	else if(c == '/')
		return 63;
	else if(c <= '9')
		return (byte)(c - '0' + 52);
	else if(c == '=')
		return 64;
	else if(c <= 'Z')
		return (byte)(c - 'A');
	else if(c <= 'z')
		return (byte)(c - 'a' + 26);
	return 64;
}

//===================================
//    Base64 编码
//===================================
char Encode_GetChar(byte num)
{
	static const char CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	return CHARS[num];
} // Encode_GetChar


//解码
size_t Base64_Decode(char *pDest, const char *pSrc, size_t srclen)
{
	byte input[4];
	size_t i, index = 0;
	for(i = 0; i < srclen; i += 4)
	{
		//byte[0]
		input[0] = Decode_GetByte(pSrc[i]);
		input[1] = Decode_GetByte(pSrc[i + 1]);
		pDest[index++] = (input[0] << 2) + (input[1] >> 4);

		//byte[1]
		if(pSrc[i + 2] != '=')
		{
			input[2] = Decode_GetByte(pSrc[i + 2]);
			pDest[index++] = ((input[1] & 0x0f) << 4) + (input[2] >> 2);
		}

		//byte[2]
		if(pSrc[i + 3] != '=')
		{
			input[3] = Decode_GetByte(pSrc[i + 3]);
			pDest[index++] = ((input[2] & 0x03) << 6) + (input[3]);
		}            
	}

	//null-terminator
	pDest[index] = 0;
	return index;
}

//编码
size_t Base64_Encode(char *pDest, const char *pSrc, size_t srclen)
{
	byte input[3], output[4];
	size_t i, index_src = 0, index_dest = 0;
	for(i = 0; i < srclen; i += 3)
	{
		//char [0]
		input[0] = pSrc[index_src++];
		output[0] = (byte)(input[0] >> 2);
		pDest[index_dest++] = Encode_GetChar(output[0]);

		//char [1]
		if(index_src < srclen)
		{
			input[1] = pSrc[index_src++];
			output[1] = (byte)(((input[0] & 0x03) << 4) + (input[1] >> 4));
			pDest[index_dest++] = Encode_GetChar(output[1]);
		}
		else
		{
			output[1] = (byte)((input[0] & 0x03) << 4);
			pDest[index_dest++] = Encode_GetChar(output[1]);
			pDest[index_dest++] = '=';
			pDest[index_dest++] = '=';
			break;
		}

		//char [2]
		if(index_src < srclen)
		{
			input[2] = pSrc[index_src++];
			output[2] = (byte)(((input[1] & 0x0f) << 2) + (input[2] >> 6));
			pDest[index_dest++] = Encode_GetChar(output[2]);
		}
		else
		{
			output[2] = (byte)((input[1] & 0x0f) << 2);
			pDest[index_dest++] = Encode_GetChar(output[2]);
			pDest[index_dest++] = '=';
			break;
		}

		//char [3]
		output[3] = (byte)(input[2] & 0x3f);
		pDest[index_dest++] = Encode_GetChar(output[3]);
	}
	//null-terminator
	pDest[index_dest] = 0;
	return index_dest;
}