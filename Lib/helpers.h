#ifndef HELPERS_H
#define HELPERS_H

#include <stdarg.h>  // For va_start, etc.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define PERIOD "."


#define stringify( name ) # name

namespace stde
{
	inline static char* convert_str_to_char(const std::string _in)
	{
		char* _in_c = new char[_in.length() + 1];
		strcpy(_in_c, _in.c_str());
		return _in_c;
	}

	inline static wchar_t* convert_str_to_wchar(const std::string _in)
	{
		char* _in_c = convert_str_to_char(_in);
		wchar_t* wcstring = new wchar_t[_in.length()];
		size_t newsize = strlen(_in_c) + 1;
		size_t convertedChars = 0;

		mbstowcs_s(&convertedChars, wcstring, newsize, _in_c, _TRUNCATE);
		return wcstring;
	}

	inline static wchar_t* convert_char_to_wchar(const char* _in)
	{
		wchar_t* wcstring = new wchar_t[strlen(_in)];
		size_t newsize = strlen(_in) + 1;
		size_t convertedChars = 0;

		mbstowcs_s(&convertedChars, wcstring, newsize, _in, _TRUNCATE);
		return wcstring;
	}

	inline static std::string convert_char_to_str(const char* _in)
	{
		return std::string(_in);
	}

    static std::string string_format(const std::string fmt, ...)
    {
		int final_n, n = ((int)fmt.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) {
			formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
			strcpy(&formatted[0], fmt.c_str());
			va_start(ap, fmt);
			final_n = vsnprintf(&formatted[0], n, fmt.c_str(), ap);
			va_end(ap);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
		}
		return std::string(formatted.get());
    }


	/* Prepends t into s. Assumes s has enough space allocated
	** for the combined string.
	*/
	static void prepend(char* s, const char* t)
	{
		size_t len = strlen(t);
		size_t i;

		memmove(s + len, s, strlen(s) + 1);

		for (i = 0; i < len; ++i)
		{
			s[i] = t[i];
		}
	}
}


#define str2char stde::convert_str_to_char
#define char2wchar stde::convert_char_to_wchar
#define str2wchar stde::convert_str_to_wchar
#define char2str stde::convert_char_to_str
#define sformat stde::string_format


#endif // HELPERS_H