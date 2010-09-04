
#include "stdafx.h"
#include "WMLF.h"
#include "link_openssl.h"

time_t __cdecl time(time_t * t32)
{
	__time64_t t64;
	time_t r = (time_t)_time64(&t64);
	if (t32)
		*t32 = (time_t)t64;
	return r;
}



char* __cdecl strerror(int errnum)
{
	return "Unknown Error";
}

void __cdecl abort(void)
{
	int * i = NULL;
	*i = 1;
	return;
}

char * __cdecl getenv(char *envvar)
{
	return NULL;
}
