/*********************************iBoxV300*********************************                                      
 
	                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)           
***************************************************************************/
#include "ST_string.h"
#include <stdio.h>



/**************************************
**函数功能：获取字符串长度
***************************************/
signed int st_strlen(char* str)
{
	int i = 0;
	if(str != NULL)
	{
		while(*str++)
		{
			i++;
		}
	}
	
	return i;
}

/**************************************************
**函数功能：比较两个字符串大小
**************************************************/
signed int st_strcmp(signed char *s,signed char *t)
{
	while(*s == *t)
	{
		if(*s == 0)
		{
			return(0);
		}
	    ++s;
	    ++t;
	}
	
	return (*s - *t);
}


/***************************************************
**函数功能：字符串拷贝
****************************************************/
signed char *st_strcpy(signed char *t,signed char *s)
{
	signed char *d;

	d = t;
	while(*s)
	{
	   *t++ = *s++;
	}
	*t = 0x0;			// Add string terminator

	return(d);
}


/***********************************************************************
**函数功能：可变长度字符串大小比较
************************************************************************/
signed int st_strncmp(signed char *dest,signed char *src,signed int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++) 
	{
		if(dest[i] != src[i])
		{
		    if(dest[i] > src[i])
			{
		        return 1;
		    }
			else
			{
		        return -1;
		    }
	    }
	}

	return 0;

}

/*****************************************************
**函数功能：判断字符大小写
******************************************************/
signed char st_toupper(signed char c)
{
	if(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))
	{
		c &= 0xDF;
	}
	
	return c;
}


/*********************************************************
**函数功能：字符串连接
**********************************************************/
signed char *st_strcat(signed char *s, signed char *t)
{
	signed char *d;
	
	d = s;
	--s;
	while(*++s);

	do 
	{
		*s = *t++;
	}
	while(*s++);

	return(d);
}


/*********************************************************************
**函数功能：可变长度内存拷贝
**********************************************************************/
signed int st_memcpy(signed char *dest, signed char *src, signed int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++)
	{
		dest[i] = src[i];
	}

	return(Len);
}


/***********************************************************************
**函数功能：设置长度为Len的内存块为byte
************************************************************************/
signed int st_memset(signed char *dest,signed char byte, signed int Len)
{
	signed int i = 0;

	for(i = 0; i < Len; i++)
	{
		dest[i] = byte;
	}

	return(Len);
}



/***********************************************************************
**函数功能：内存段内数据比较
************************************************************************/
signed int st_memcmp(signed char* dest, signed char* src, signed int Len)
{
	unsigned int i;

	for(i = 0; i < Len; i++)
	{
		if(dest[i] != src[i])
		{
		    if(dest[i] > src[i])
			{
		        return 1;
		    }
			else
			{
		        return -1;
		    }
	    }
	}
	
	return 0;
}
