#include "base/aes.h"
#include "coAes.h"

//////////////////////////////////////////////////////////////////////////
static void aes_encrypt_data( aes_context *ctx, const std::string& ins, std::string& outs)
{	
	int index = 0;
	int size = ins.size();
	int sizeN = size / 16 * 16;

	if (sizeN < size)
		sizeN += 16;

	outs = ins;
	outs.resize(sizeN);

	while (index < sizeN)
	{
		aes_encrypt(ctx, (unsigned char*)&outs[index], (unsigned char*)&outs[index]);
		index += 16;
	}
};

static void aes_decrypt_data( aes_context *ctx, const std::string& ins, std::string& outs)
{
	int index = 0;
	int size = ins.size();
	int sizeN = size / 16 * 16;

	if (sizeN < size)
		sizeN += 16;

	outs = ins;
	outs.resize(sizeN);

	while (index < sizeN)
	{
		aes_decrypt(ctx, (unsigned char*)&outs[index], (unsigned char*)&outs[index]);
		index += 16;
	}

	//去掉000
	int iZero = 0;
	for (int i = outs.size() - 1; i >= 0; --i)
	{
		if (outs[i]=='\0')
			iZero++;
		else
			break;
	}

	if (iZero > 0)
		outs = outs.substr(0, outs.size() - iZero);
};

//////////////////////////////////////////////////////////////////////////
aes_context __gAesContext;

void coAesSetKey(const char* key)
{
	unsigned char keyBuf[32];
	memset(keyBuf, 0, sizeof(keyBuf));
	strncpy((char*)keyBuf, (const char*)key, 32);
	aes_set_key(&__gAesContext, keyBuf, 256);
}

void coAesEncript(const std::string& ins, std::string& outs)
{
	aes_encrypt_data(&__gAesContext, ins, outs);
}

void coAesDecript(const std::string& ins, std::string& outs)
{
	aes_decrypt_data(&__gAesContext, ins, outs);
}
