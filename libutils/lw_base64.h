#ifndef __lw_base64_h__
#define __lw_base64_h__

#include <string>
#include <functional>

class Base64 final
{
public:
	Base64();
	~Base64();

public:
	int encrypt(const char* filename, std::function<void(char * out, unsigned int len)> func);
	int encrypt(unsigned char *data, unsigned int data_len, std::function<void(char * out, unsigned int len)> func);
	
	int decrypt(const char* filename, std::function<void(unsigned char * out, unsigned int len)> func);
	int decrypt(unsigned char *data, unsigned int data_len, std::function<void(unsigned char * out, unsigned int len)> func);
};


#endif	// __lw_base64_h__
