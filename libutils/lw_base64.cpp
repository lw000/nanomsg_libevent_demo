#include "lw_base64.h"
#include "FileUtils.h"
#include "Data.h"

extern "C"
{
	#include "./base64.h"
}

Base64::Base64()
{

}

Base64::~Base64()
{
}

int Base64::encrypt(const char* filename, std::function<void(char * out, unsigned int len)> func)
{
	Data d = FileUtils::getInstance()->getDataFromFile(filename);
	return this->encrypt(d.getBytes(), d.getSize(), func);
}

int Base64::encrypt(unsigned char *data, unsigned int data_len, std::function<void(char * out, unsigned int len)> func)
{
	unsigned int len;
	char* out;
	len = base64Encode(data, data_len, &out);
	func(out, len);
	free(out);
	return 0;
}

int Base64::decrypt(const char* filename, std::function<void(unsigned char * out, unsigned int len)> func)
{
	Data d = FileUtils::getInstance()->getDataFromFile(filename);
	return this->decrypt(d.getBytes(), d.getSize(), func);
}

int Base64::decrypt(unsigned char *data, unsigned int data_len, std::function<void(unsigned char * out, unsigned int len)> func)
{
	unsigned int len;
	unsigned char* out;
	len = base64Decode(data, data_len, &out);
	out[len] = 0;
	func(out, len);
	free(out);
	return 0;
}


