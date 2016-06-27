#ifndef _coAes_H_
#define _coAes_H_

#include <string>

void coAesSetKey(const char* key);
void coAesEncript(const std::string& ins, std::string& outs);
void coAesDecript(const std::string& ins, std::string& outs);

//////////////////////////////////////////////////////////////////////////
// php 例子
/*
<?php

define("KEY", "11111111222222223333333344444444");

function CryptEncode($data)
{
	$iv = KEY;
	return base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_128, KEY, $data, MCRYPT_MODE_ECB, $iv));
}

function CryptDecode($data)
{
	$iv = KEY;
	return mcrypt_decrypt(MCRYPT_RIJNDAEL_128, KEY, base64_decode($data), MCRYPT_MODE_ECB, $iv);
}

?>
*/
//////////////////////////////////////////////////////////////////////////
#endif // _coAes_H_