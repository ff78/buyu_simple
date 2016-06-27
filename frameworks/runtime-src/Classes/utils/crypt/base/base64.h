#ifndef _base64_H_
#define _base64_H_

//http://www.cnblogs.com/hoodlum1980/archive/2012/05/28/2521500.html
#include <string>
size_t Base64_Decode(char *pDest, const char *pSrc, size_t srclen);
size_t Base64_Encode(char *pDest, const char *pSrc, size_t srclen);

#endif // _base64_H_