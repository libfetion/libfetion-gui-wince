#ifndef _LINK_OPENSSL_H_
#define _LINK_OPENSSL_H_

#ifdef __cplusplus
extern "C" {
#endif

char* __cdecl strerror(int errnum);
void __cdecl abort(void);
char * __cdecl getenv(char *envvar);


	#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif