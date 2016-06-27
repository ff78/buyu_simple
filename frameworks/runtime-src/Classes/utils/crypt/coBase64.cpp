#include "base/base64.h"
#include "coBase64.h"
// 编码
void coBase64Encode(std::string& outs, const std::string& ins)
{
	int size = ins.size();
	size = size/3*4 + (size%3 == 0 ? 0 : 4);
	outs.resize(size);

	Base64_Encode(&outs[0], &ins[0], ins.size());
}

// 解码
void coBase64Decode(std::string& outs, const std::string& ins)
{
	outs.resize(ins.size());
	size_t sl = Base64_Decode(&outs[0], &ins[0], ins.size());
	outs.resize(sl);
}

