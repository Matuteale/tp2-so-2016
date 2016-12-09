#include"string.h"

/* devuelve la longitud de un string */
int strlen(const char *s)
{
	int i = 0;
	while(s[i] != 0)
	{
		i++;
	}
	return i;
}

/* compara dos strings */
int strcmp(char * s1, char * s2)
{
	int i = 0, c = 0;
	while(c == 0)
	{
		c = s1[i] - s2 [i];
		i++;
	}
	return c;
}

/* copia un string de una posicion de memoria a otra */
void strcpy(char * dest, const char * source)
{
	int i = 0;
	while ((dest[i] = source[i]) != '\0')
	{
		i++;
	}
}

/* pasa los caracteres upper case de un string a lower case */
char * lowerString(char * s)
{
       while(*s != 0)
        {
                if(*s >= 'A' && *s <= 'Z')
                        *s = *s - 'A' + 'a';
                s++;
        }
		return 0;
}

/* saca los espacios al comienzo y al final de un string */
void trim(char* s)
{
	int  i = 0, j = 0, aux=-1;


	while ( s[i] != 0 && s[i] == ' ')
		    i++;

	/* o termino la cadena o encontro algo distinto a espacio
	   Si termino la cadena la trunco a cadena nula */
	if( s[i] == 0 )
		s[0] = 0;

	while( s[i] != 0)
	{
		if (s[i] == ' ')
			aux = i;
		else if (s[i] != ' ')
		{
		   if (aux == -1)
		           s[j++] = s[i];
		   else
		   {
		           while(aux < i + 1)
		                   s[j++] = s[aux++];
		           aux=-1;
		   }
		}
		i++;
	}
	if (j != 0)
		s[j]=s[i];
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void reverse(char * str)
{
    /* skip null */
    if (str == 0)
    {
        return;
    }

    /* skip empty string */
    if (*str == 0)
    {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}

/* devuelve el valor ascii lower case de una letra */
unsigned char tolower(unsigned char c)
{

	if(c >= 'A' && c <= 'Z')
			c = c - 'A' + 'a';
	return c;
}
