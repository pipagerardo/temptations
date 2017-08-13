#include "PGL_funciones.h"

// -----------------------------------------------------------------------------
// CADENAS:
// -----------------------------------------------------------------------------
char* SDL_strrstr( const char* haystack, const char* needle ) {
	char* sz_r = NULL;
	if ( !haystack || !needle[0] ) return sz_r;
	char* sz_p = NULL;
	do {
	    sz_p = (char*)SDL_strstr( haystack, needle );
	    if ( sz_p ) { sz_r = sz_p; haystack = sz_p + 1; }
	} while( sz_p );
	return sz_r;
}
/*
char* SDL_strtok( char* str, const char* delimiters ) {
	return strtok( str, delimiters );
}
*/
char* SDL_strtok( register char *str, register const char *delimiters ) {
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;
	if( str == NULL && (str = last) == NULL) return (NULL);
	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *str++;
	for (spanp = (char *)delimiters; (sc = *spanp++) != 0;) {
		if (c == sc) goto cont;
	}
	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = str - 1;
	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *str++;
		spanp = (char *)delimiters;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0) str = NULL;
				else        str[-1] = 0;
				last = str;
				return (tok);
			}
		} while (sc != 0);
	}
}


// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// DE CADENA A NÚMERO CON CONFIRMACIÓN : ( Si falla la lectura retorna false )
// -----------------------------------------------------------------------------
namespace PGL {
	
SDL_bool STRtoI( const char* str, int &number ) {
    char *p;
	number = (int)SDL_strtol( str, &p, 10 );
    if( number != 0     ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoUI( const char* str, unsigned int &number ) {
    char *p;
    number = (unsigned int)SDL_strtoul( str, &p, 10 );
    if( number != 0     ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoL( const char* str, long &number )  {
    char *p;
	number = SDL_strtol( str, &p, 10 );
    if( number != 0     ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoUL( const char* str, unsigned long &number )  {
    char *p;
    number = SDL_strtoul( str, &p, 10 );
    if( number != 0     ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoLL( const char* str, long long &number )  {
    char *p;
	number = SDL_strtoll( str, &p, 10 );
    if( number != 0     ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoULL( const char* str, unsigned long long &number )  {
    char *p;
	number = SDL_strtoull( str, &p, 10 );
    if( number != 0     ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoF( const char* str, float &number ) {
    char *p;
	number = (float)SDL_strtod( str, &p );
    if( number != 0.0F  ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoD( const char* str, double &number ) {
    char *p;
	number = SDL_strtod( str, &p );
    if( number != 0.0   ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}
SDL_bool STRtoLD( const char* str, long double &number ) {
    char *p;
	number = (long double)SDL_strtod( str, &p );
    if( number != 0.0L  ) return SDL_TRUE;
    if( p == str        ) return SDL_FALSE;
    if( *(p - 1) == '0' ) return SDL_TRUE;
    if( *p != '\0'      ) return SDL_FALSE;
    return SDL_TRUE;
}

}
// -----------------------------------------------------------------------------

// ------------------------------------------------------------------------
namespace PGL {
void   Semilla() { srand( SDL_GetTicks() /*GetTickCount()*/ ); } 
void   Semilla( Uint32 tiempo ) { srand( tiempo ); } 
int    Aleatorio( int   min, int   max ) { return min + ( rand() % ( max - min + 1 ) ) ; }
float  Aleatorio( float min, float max ) { return min + ( (float)rand() / RAND_MAX * ( max - min ) ) ; }
float Grados( float radianes ) { return ( 180.0f * radianes ) / M_PI; }
float Radianes( float grados ) { return ( M_PI * grados ) / 180.0f; }
float Hipotenusa( float a, float b ) { return SDL_sqrt( (a * a) + (b * b) ); }
float Distancia2P( float x1, float y1, float x2, float y2 ) { return Hipotenusa( y2 - y1, x2 - x1 ); }
float Angulo2P( float x1, float y1, float x2, float y2 ) { return SDL_atan2( y2 - y1, x2 - x1 ); }
bool Igualdad2P( float x1, float y1, float x2, float y2 ) { 
	if ( y2 != y1 ) return false; 
  	if ( x2 != x1 ) return false; 
   	return true; 
}
bool Recta2P( float x1, float y1, float x2, float y2, float &m, float &b ) {
    if( Igualdad2P( x1, y1, x2, y2 ) ) return false;
    if( x2 == x1 ) m = 0;
    else m = ( y2 - y1 ) / ( x2 - x1 ); 
    b = y1 - ( m * x1 ); 
    return true;
}
float Distancia1P1R( float x, float y, float m, float b ) { 
    return ( ( m * x ) - y + b ) / SDL_sqrt( m * m + 1.0f ); 
}
void Recta1P1R( float x, float y, float m, float &mp, float &bp ) { 
    mp = -m / 2.0f; 
    bp = y - ( mp * x ); 
}
bool Punto2R( float m1, float b1, float m2, float b2, float &x, float &y ) { 
    if( m1 == m2 ) return false; 
    x = ( b2 - b1 ) / ( m1 - m2 ); 
	y =  x * m2 + b2; 
	return true; 
}
float Angulo2R( float m1, float m2 ) { 
	return SDL_atan( ( m2 - m1 ) / ( 1.0f + ( m2 * m1 ) ) );
}
float Distancia2R( float m1, float b1, float m2, float b2 ) {
    if( m1 == m2 ) return ( b1 - b2 ) / SDL_sqrt( 1.0f + ( m1 * m2 ) );
    return 0.0f; 
}
}
// ------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// BASE64:
// -----------------------------------------------------------------------------
namespace PGL {
	
static const unsigned char pr2six[256] = { // ASCII table
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode_len( const char *bufcoded ) {
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;
    bufin = (const unsigned char *) bufcoded;
    while( pr2six[*(bufin++)] <= 63 );
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    return nbytesdecoded + 1;
}

int Base64decode( char *bufplain, const char *bufcoded ) {
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;
    bufin = (const unsigned char *) bufcoded;
    while( pr2six[*(bufin++)] <= 63 );
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;
    while( nprbytes > 4 ) {
		*(bufout++) = (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    	*(bufout++) = (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    	*(bufout++) = (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    	bufin += 4;
    	nprbytes -= 4;
    }
    // Note: (nprbytes == 1) would be an error, so just ingore that case
    if (nprbytes > 1) {
    *(bufout++) = (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
    *(bufout++) = (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
    *(bufout++) = (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }
    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len( int len ) {
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode( char *encoded, const char *string, int len ) {
    int i;
    char *p;
    p = encoded;
    for( i = 0; i < len - 2; i += 3 ) {
    	*p++ = basis_64[(string[i] >> 2) & 0x3F];
    	*p++ = basis_64[((string[i] & 0x3) << 4) |
                    ((int) (string[i + 1] & 0xF0) >> 4)];
    	*p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                    ((int) (string[i + 2] & 0xC0) >> 6)];
    	*p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
    	*p++ = basis_64[(string[i] >> 2) & 0x3F];
    	if (i == (len - 1)) {
        	*p++ = basis_64[((string[i] & 0x3) << 4)];
        	*p++ = '=';
    	} else {
        	*p++ = basis_64[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        	*p++ = basis_64[((string[i + 1] & 0xF) << 2)];
    	}
    	*p++ = '=';
    }
    *p++ = '\0';
    return p - encoded;
}

}
// -----------------------------------------------------------------------------


