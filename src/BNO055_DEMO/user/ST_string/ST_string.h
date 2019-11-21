#ifndef _ST_STRING_H_
#define _ST_STRING_H_



signed int st_strlen(char* str);
signed int st_strcmp(signed char *s,signed char *t);
signed char *st_strcpy(signed char *t,signed char *s);
signed int st_strncmp(signed char *dest,signed char *src,signed int Len);
signed char st_toupper(signed char c);
signed char *st_strcat(signed char *s, signed char *t);
signed int st_memcpy(signed char *dest, signed char *src, signed int Len);
signed int st_memset(signed char *dest,signed char byte, signed int Len);
signed int st_memcmp(signed char* dest, signed char* src, signed int Len);


#endif
