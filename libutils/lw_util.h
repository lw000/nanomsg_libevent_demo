#ifndef __LW_UTIL_H__
#define __LW_UTIL_H__

#include <vector>
#include <string>
#include <unordered_map>
#include <list>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#endif

#include <functional>

#ifdef _WIN32
#define lw_sleep(seconds) SleepEx(seconds * 1000, 1);
#else
#define lw_sleep(seconds) sleep(seconds);
#endif

struct KV
{
	char *k;
	char *v;
};

class KVQueryUrlValue
{
public:
	KVQueryUrlValue();
	~KVQueryUrlValue();

public:
	int parse(const char* data);
	char* find(const char* key);
	void each(std::function<void(KV*)> func);
	void reset();

private:
	std::list<KV*> _kv;
};

unsigned int lw_make_software_version(unsigned char major, unsigned char minor, unsigned short build);
void lw_software_version(unsigned int version, unsigned char &major, unsigned char &minor, unsigned short &build);
unsigned char lw_major_version(unsigned int version);
unsigned char lw_minor_version(unsigned int version);
unsigned short lw_build_version(unsigned int version);

unsigned long lw_hash_code(const char* c);

char * lw_strtok_r(char *s, const char *delim, char **state);

std::vector<std::string> split(const char* str, const char* pattern);

std::unordered_map<std::string, std::string> split_url_pragma_data(const char* str);

void trim(char* src, char* dest);
void trim_l(char* src, char* dest);
void trim_r(char* src, char* dest);

#if defined(_WIN32)

//UTF-8��GB2312ת��
char* utf8_to_gbk(const char* utf8);
//GB2312��UTF-8ת��
char* gbk_to_utf8(const char* gb2312);

std::wstring StringUtf8ToWideChar(const std::string& strUtf8);
std::string StringWideCharToUtf8(const std::wstring& strWideChar);
std::string UTF8StringToMultiByte(const std::string& strUtf8);

#endif

#if defined(_WIN32)
#define U2G(v)	utf8_to_gbk(v)
#define G2U(v)	gbk_to_utf8(v)
#else
#define U2G(v)
#define G2U(v)
#endif

#ifdef _WIN32
#ifndef __MINGW32__
#include <stdio.h>
#define snprintf(str, n, format, ...)  \
			_snprintf_s(str, n, _TRUNCATE, format, __VA_ARGS__)
#endif
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

#define lw_min(a,b) ((a)<(b)?(a):(b))

#define lw_max(a,b) ((a)>(b)?(a):(b))

extern double gettime(void);

#endif // !__LW_UTIL_H__
