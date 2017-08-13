#ifndef _PGL_FUNCIONES_
#define _PGL_FUNCIONES_
#include "PGL_cabeceras.h"

// -----------------------------------------------------------------------------
// Array:
// -----------------------------------------------------------------------------
// #define SDL_arraysize( array )    	(sizeof(array)/sizeof(array[0]))
// #define SDL_TABLESIZE( table )    	SDL_arraysize(table)
// void SDL_qsort(
//		void *base, 
//		size_t nmemb, 
//		size_t size, 
//		int (*compare) (const void *, const void *)
// );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CAST:
// -----------------------------------------------------------------------------
// #define SDL_reinterpret_cast(type, expression) 	reinterpret_cast<type>(expression)
// #define SDL_static_cast(type, expression) 		static_cast<type>(expression)
// #define SDL_const_cast(type, expression) 		const_cast<type>(expression)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// FAMILIA DE FUNCIONES MALLOC:
// -----------------------------------------------------------------------------
// void* SDL_malloc( size_t size );
// void* SDL_calloc( size_t nmemb, size_t size) ;
// void* SDL_realloc( void *mem, size_t size );
// void  SDL_free( void *mem );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// MEMORIA:
// -----------------------------------------------------------------------------
// void* SDL_memset( void *dst, int c, size_t len );
// void* SDL_zero( x ); 	// SDL_memset(&(x), 0, sizeof((x)))
// void* SDL_zerop( x );	// SDL_memset((x), 0, sizeof(*(x)))
// void* SDL_memset4( void *dst, Uint32 val, size_t dwords );
// void* SDL_memcpy(  void *dst, const void *src, size_t len );
// void* SDL_memmove( void *dst, const void *src, size_t len );
// int   SDL_memcmp( const void *s1, const void *s2, size_t len );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CADENAS:
// -----------------------------------------------------------------------------
// size_t SDL_strlen( const char *str );	// Retorna la longitud de la cadena sin el caracter nulo.
// size_t SDL_strlcpy( char *dst, const char *src, size_t maxlen );	// Copia una cadena en otra.
// size_t SDL_utf8strlcpy( char *dst, const char *src, size_t dst_bytes );
// size_t SDL_strlcat( char *dst, const char *src, size_t maxlen ); 	// Contatena cadenas.
// char* SDL_strdup( const char *str );	// Duplica una cadena, luego hay que borrar la con SDL_free.
// char* SDL_strrev( char *str ); 		// Invierte la cadena.
// char* SDL_strupr( char *str );		// Convierte la cadena a mayúsculas.
// char* SDL_strlwr( char *str );		// Convierte la cadena en minúsculas.
// char* SDL_strchr(  const char *str, int c );	// Busca la primera aparación de un caracter.
// char* SDL_strrchr( const char *str, int c );	// Busca la última aparación de un caracter.
// char* SDL_strstr(  const char *haystack, const char *needle ); // Busca la primera aparición de una cadena.
   char* SDL_strrstr( const char* haystack, const char* needle ); // Busca la última aparición de una cadena.
// int SDL_strcmp(  const char *str1, const char *str2 ); 					// Compara dos cadenas, sensitivo a las mayusculas.
// int SDL_strncmp( const char *str1, const char *str2, size_t maxlen );	// Compara dos cadenas, sensitivo a las mayúsculas.
// int SDL_strcasecmp(  const char *str1, const char *str2 ); 				// Compara dos cadenas, no sensitivo a las mayúsculas.
// int SDL_strncasecmp( const char *str1, const char *str2, size_t len );	// Compara dos cadenas, no sensitivo a las mayúsculas.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CORTAR CADENAS POR PASOS:
// -----------------------------------------------------------------------------
// char texto[] =  "Una cadena\tde ,,muestras\ny algunas   muestras más";
// const char* pasos   = " ,\t\n";
// char* muestra  = NULL;
// Error( "texto: %s\n", texto );
// muestra = SDL_strtok( texto, pasos );
// while( muestra != NULL )  {
//    SDL_Log( "\t%s\n", muestra );
//    muestra = SDL_strtok( NULL, pasos );
// }
// -----------------------------------------------------------------------------
char* SDL_strtok( register char *str, register const char *delimiters );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// FAMILIA DE FUNCIONES STRING PRINT-SCAN FORMAT:
// -----------------------------------------------------------------------------
// int SDL_sscanf( const char *text, const char *fmt, ... );
// int SDL_vsscanf( const char *text, const char *fmt, va_list ap );
// int SDL_snprintf( char *text, size_t maxlen, const char *fmt, ... );
// int SDL_vsnprintf( char *text, size_t maxlen, const char *fmt, va_list ap );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// DE NUMERO A CADENA:
// -----------------------------------------------------------------------------
// char* SDL_itoa( int value, char *str, int radix );
// char* SDL_uitoa( unsigned int value, char *str, int radix );
// char* SDL_ltoa( long value, char *str, int radix );
// char* SDL_ultoa( unsigned long value, char *str, int radix );
// char* SDL_lltoa( Sint64 value, char *str, int radix );
// char* SDL_ulltoa( Uint64 value, char *str, int radix );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// DE CADENA A NUMERO:
// -----------------------------------------------------------------------------
// int  SDL_atoi( const char *str );
// long SDL_strtol( const char *str, char **endp, int base );
// unsigned long SDL_strtoul( const char *str, char **endp, int base );
// Sint64 SDL_strtoll( const char *str, char **endp, int base );
// Uint64 SDL_strtoull( const char *str, char **endp, int base );
// double SDL_atof( const char *str );
// double SDL_strtod( const char *str, char **endp );
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// DE CADENA A NÚMERO CON CONFIRMACIÓN : ( Si falla la lectura retorna false )
// -----------------------------------------------------------------------------
namespace PGL {
SDL_bool STRtoI( const char* str, int &number );
SDL_bool STRtoUI( const char* str, unsigned int &number );
SDL_bool STRtoL( const char* str, long &number );
SDL_bool STRtoUL( const char* str, unsigned long &number );
SDL_bool STRtoLL( const char* str, long long &number );
SDL_bool STRtoULL( const char* str, unsigned long long &number );
SDL_bool STRtoF( const char* str, float &number );
SDL_bool STRtoD( const char* str, double &number );
SDL_bool STRtoLD( const char* str, long double &number );
}
// -----------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// MATEMÁTICAS:
// ---------------------------------------------------------------------------------
// #define M_PI    3.14159265358979323846264338327950288   
// int SDL_abs(int x);
// double SDL_acos(double x);
// double SDL_asin(double x);
// double SDL_atan(double x);
// double SDL_atan2(double x, double y);
// double SDL_ceil(double x);
// double SDL_copysign( double x, double y );	// retorna x con el signo de y.
// double SDL_cos(double x);
// float  SDL_cosf(float x);
// double SDL_fabs(double x);			// retorna el flotante en positivo.
// double SDL_floor(double x);
// double SDL_log(double x);
// double SDL_pow(double x, double y);
// double SDL_scalbn( double x, int n ); // Multiplica el valor de punto flotante por argumeno n FLT_RADIX elevado a potencia exp
// double SDL_sin(double x);
// float  SDL_sinf(float x);
// double SDL_sqrt(double x);
// ---------------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Seno:   ( cateto opuesto / hipotenusa )
// ------------------------------------------------------------------------
// Coseno: ( cateto contiguo / hipotenusa )
// ------------------------------------------------------------------------
// Tangente: ( cateto opuesto / cateto contiguo ) == ( seno / coseno )
// ------------------------------------------------------------------------
namespace PGL {
// ------------------------------------------------------------------------
// Números aleatorios:
// ------------------------------------------------------------------------
void   Semilla();
void   Semilla( Uint32 tiempo );
int    Aleatorio( int   min, int   max );
float  Aleatorio( float min, float max ); 
// ------------------------------------------------------------------------
// De Radianes a Grados y Viceversa:
// ------------------------------------------------------------------------
// Las funciones trigonométricas retornan el ángulo en radianes.
// La circunferencia completa en   grados = 360º
// La circunferencia completa en radianes = 2 * Pi
//      Conclusión:       360 grados == 2 * Pi radianes
//                        180 grados == Pi radianes
//     Un radián son 57.2958 grados. Un grado son 0.0174533 radianes.
// ------------------------------------------------------------------------
float Grados( float radianes );
float Radianes( float grados );
// ------------------------------------------------------------------------
// Hipotenusa:  ( h^2 = a^2 + b^2 )
// ------------------------------------------------------------------------
float Hipotenusa( float a, float b );
// ------------------------------------------------------------------------
// Distancia entre dos Puntos 2D: distancia = Hipotenusa( y2 - y1, x2 - x1 );
// ------------------------------------------------------------------------
float Distancia2P( float x1, float y1, float x2, float y2 );
// ------------------------------------------------------------------------
// Ángulo entre dos Puntos 2D: radianes = Atan2( y2 - y1, x2 - x1 );
// ------------------------------------------------------------------------
float Angulo2P( float x1, float y1, float x2, float y2 );
// ------------------------------------------------------------------------
// Igualdad entre dos Puntos 2D: P1( x1, y1 ) == P2( x2, y2 )
// ------------------------------------------------------------------------
bool Igualdad2P( float x1, float y1, float x2, float y2 );
// ------------------------------------------------------------------------
// Ecuación de la Recta entre dos Puntos 2D: P1( x1, y1 ), P2( x2, y2 )
//    m = ( y2 - y1 ) / ( x2 - y2 );    // tangente de alfa.
//    b = y1 - ( m * x1 );              // punto de corte eje vertical Y
//    y = ( m * x ) + b;				// Si x = 0; y = b;		
//    x = ( y - b ) / m;				// Si y = 0; x = -b / m;
// ------------------------------------------------------------------------
bool Recta2P( float x1, float y1, float x2, float y2, float &m, float &b );
// ------------------------------------------------------------------------
// Distancia ente un Punto y una Recta 2D: P( x, y ) R( m, b )
//     distancia = ( ( m * x ) - y + b ) / Raiz2( m * m + 1 );
// ------------------------------------------------------------------------
float Distancia1P1R( float x, float y, float m, float b );
// ------------------------------------------------------------------------
// Recta (Perpendicular) ente un Punto y una Recta 2D: P( x, y ) R( m, b )
//     mp = -m / 2;
//     bp = y - ( mp * x );
// ------------------------------------------------------------------------
void Recta1P1R( float x, float y, float m, float &mp, float &bp );
// ------------------------------------------------------------------------
// Punto de curce entre dos Rectas 2D: R1( m1, b1 ), R2( m2, b2 )
//     if( m1 != m2 ) {
//         x = ( b2 - b1 ) / ( m1 - m2 );
//         y =  x * m2 + b2
//     }
// ------------------------------------------------------------------------
bool Punto2R( float m1, float b1, float m2, float b2, float &x, float &y );
// ------------------------------------------------------------------------
// Angulo entre dos Rectas 2D: R1( m1, b1 ), R2( m2, b2 )
//     Atan( ( m2 - m1 ) / ( 1 + ( m2 * m1 ) ) );
// ------------------------------------------------------------------------
float Angulo2R( float m1, float m2 );
// ------------------------------------------------------------------------
// Distancia entre dos Rectas Paraleas 2D: R1( m1, b1 ), R2( m2, b2 )
// ------------------------------------------------------------------------
float Distancia2R( float m1, float b1, float m2, float b2 );


}
// -----------------------------------------------------------------------------
// BASE64:
// -----------------------------------------------------------------------------
namespace PGL {
int Base64decode_len( const char *bufcoded );
int Base64decode( char *bufplain, const char *bufcoded );
int Base64encode_len( int len );
int Base64encode( char *encoded, const char *string, int len );
}
// -----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Mientras no se implemente la funcion inline double SDL_SwapDouble( double x );
// ----------------------------------------------------------------------------
#ifndef SDL_SwapDoubleLE
inline double SDL_SwapDouble( double x ) {
 	union {
		double f;
 		Uint64 ui64;
 	} swapper;
 	swapper.f = x;
	swapper.ui64 = SDL_Swap64( swapper.ui64 );
 	return swapper.f;
};
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SDL_SwapDoubleLE(X)  (X)
#define SDL_SwapDoubleBE(X)  SDL_SwapDouble(X)
#else
#define SDL_SwapDoubleLE(X)  SDL_SwapDouble(X)
#define SDL_SwapDoubleBE(X)  (X)
#endif
#endif
// ----------------------------------------------------------------------------

#endif

