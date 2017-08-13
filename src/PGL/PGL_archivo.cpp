#include "PGL_archivo.h"
#include "PGL_funciones.h"
namespace PGL {
	
Archivo::Archivo() {
    p_archivo = NULL; b_abierto = false;
}

Archivo::Archivo( const char *nombre, const char *modo ) {
    p_archivo = NULL; b_abierto = false;
    inicia( nombre, modo );
}

Archivo::Archivo( void *memoria, int ancho ) {
    p_archivo = NULL; b_abierto = false;
    inicia( memoria, ancho );
}

Archivo::Archivo( const void *memoria, int ancho ) {
    p_archivo = NULL; b_abierto = false;
    inicia( memoria, ancho );
}

Archivo::~Archivo() {
    quita();
}

bool Archivo::inicia( const char *nombre, const char *modo ) {
    if( b_abierto ) { if( !quita() ) return false; }
	p_archivo = SDL_RWFromFile( nombre, modo );
	if( p_archivo == NULL ) return false;
	b_abierto = true;
    return b_abierto;
}

bool Archivo::inicia( void *memoria, int ancho ) {
    if( b_abierto ) { if( !quita() ) return false; }
	p_archivo = SDL_RWFromMem( memoria, ancho );
	if( p_archivo == NULL ) return false;
	b_abierto = true;
    return b_abierto;
}

bool Archivo::inicia( const void *memoria, int ancho ) {
    if( b_abierto ) { if( !quita() ) return false; }
	p_archivo = SDL_RWFromConstMem( memoria, ancho );
	if( p_archivo == NULL ) return false;
	b_abierto = true;
    return b_abierto;
}

bool Archivo::quita() {
    if( !b_abierto ) return !b_abierto;
	if( SDL_RWclose( p_archivo ) != 0 ) return !b_abierto;
	p_archivo  = NULL;
	b_abierto  = false;
    return !b_abierto;
}

bool Archivo::esta_abierto() const { return  b_abierto; }
bool Archivo::esta_cerrado() const { return !b_abierto; }
bool Archivo::final() {
	if( esta_cerrado() ) return true;
	if( posicion() <= capacidad() ) return false;
	return true;
}
bool Archivo::no_final() { 
	return !final();
}

char Archivo::lee() {
	if( esta_cerrado() ) return '\0';
    char caracter;
	return SDL_RWread( p_archivo, &caracter, sizeof( char ), 1 ) != 1 ? '\0' : caracter;
}

bool Archivo::lee( char *cadena, size_t longitud ) {
	if( esta_cerrado() ) return false;
    if( ( cadena == NULL ) || ( longitud == 0 ) ) return false;
	cadena[0] = '\0';
	char caracter = '\0';
	size_t i;
	for( i = 0; i < longitud; i++ ) {
		caracter = lee();
		if( caracter == '\r' ) continue;
		if( caracter == '\n' ) break;
		if( caracter == '\0' ) break;
		cadena[i] = caracter;
	}
	cadena[i] = '\0';
	return ( i > 0 ) ? true : false;
}

bool Archivo::lee( Cadena &cadena ) {
	if( esta_cerrado() ) return false;
	cadena.borraDesde( 0 );
	char caracter = '\0';
	while( true ) {
		caracter = lee();
		if( caracter == '\r' ) continue;
		if( caracter == '\n' ) break;
		if( caracter == '\0' ) return false;
		cadena.concatena( caracter );
	}
	return true;
	// return ( cadena.dameLongitud() > 0 ) ? true : false;
}

// -----------------------------------------------------------------------------
//                         ESCRITURA EN MODO TEXTO
// -----------------------------------------------------------------------------

bool Archivo::escribe( char caracter ) {
	if( esta_cerrado() ) return '\0';
	return SDL_RWwrite( p_archivo, &caracter, sizeof( char ), 1 ) != 1 ? false : true;
}

bool Archivo::escribe( const char *cadena, size_t longitud ) {
	if( esta_cerrado() ) return false;
 	if( ( cadena == NULL ) || ( longitud == 0 ) ) return 0;
	return SDL_RWwrite( p_archivo, cadena, sizeof( char ), longitud ) != longitud ? false : true;
}

bool Archivo::escribe( Cadena &cadena ) {	
	if( esta_cerrado() ) return false;
	size_t longitud = cadena.dameLongitud();
 	if( longitud == 0 ) return 0;
	return SDL_RWwrite( p_archivo, cadena.dameCadena(), sizeof( char ), longitud ) != longitud ? false : true;	
}

// -----------------------------------------------------------------------------
//                        ENTRADA Y SALIDA FORMATEADA:
// -----------------------------------------------------------------------------

size_t Archivo::sprintf( const char *formato, ... ) {
	if( esta_cerrado() ) return 0;
	Cadena cadena;
    va_list va; va_start( va, formato );
	size_t resultado = cadena.vsprintf( formato, va );
	va_end( va );	
	if( !escribe( cadena ) ) resultado = 0;
    return resultado;
}

size_t Archivo::sprintf( size_t cantidad, const char *formato, ... ) {
	if( esta_cerrado() ) return 0;
	Cadena cadena;
    va_list va; va_start( va, formato );
	size_t resultado = cadena.vsprintf( cantidad, formato, va );
	va_end( va );	
	if( !escribe( cadena ) ) resultado = 0;
    return resultado;
}

size_t Archivo::sscanf( const char *formato, ... ) {
	if( esta_cerrado() ) return 0;
	Cadena cadena;
	if( !lee( cadena ) ) return 0;
    va_list va; va_start( va, formato );
    size_t resultado =  cadena.vsscanf( formato, va );
    va_end( va );
    return resultado;
}

// -----------------------------------------------------------------------------
//                      LECTURA Y ESCRITURA EN MODO BINARIO
// -----------------------------------------------------------------------------
size_t Archivo::lee( void *puntero, size_t ancho, size_t cantidad ) {
	if( esta_cerrado() ) return 0;
    return SDL_RWread( p_archivo, puntero, ancho, cantidad );
}

size_t Archivo::escribe( void *puntero, size_t ancho, size_t cantidad ) {
	if( esta_cerrado() ) return 0;
	return SDL_RWwrite( p_archivo, puntero, ancho, cantidad );
}

Uint8 Archivo::leeU8() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadU8( p_archivo );
}

Uint16 Archivo::leeLE16() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadLE16( p_archivo );
}

Uint16 Archivo::leeBE16() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadBE16( p_archivo );
}

Uint32 Archivo::leeLE32() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadLE32( p_archivo );
}

Uint32 Archivo::leeBE32() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadBE32( p_archivo );
}

Uint64 Archivo::leeLE64() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadLE64( p_archivo );
}

Uint64 Archivo::leeBE64() {
	if( esta_cerrado() ) return 0;
	return SDL_ReadBE64( p_archivo );
}

float Archivo::leeLEFloat() {
	if( esta_cerrado() ) return 0.0f;
	float  valor = 0;
	SDL_RWread( p_archivo, &valor, sizeof( valor ), 1 );
	return SDL_SwapFloatLE( valor );
}

float Archivo::leeBEFloat() {
	if( esta_cerrado() ) return 0.0f;
	float  valor = 0;
	SDL_RWread( p_archivo, &valor, sizeof( valor ), 1 );
	return SDL_SwapFloatBE( valor );
}

double Archivo::leeLEDouble() {
	if( esta_cerrado() ) return 0.0;
	double  valor = 0;
	SDL_RWread( p_archivo, &valor, sizeof( valor ), 1 );
	return SDL_SwapDoubleLE( valor );
}

double Archivo::leeBEDouble() {
	if( esta_cerrado() ) return 0.0;
	double  valor = 0;
	SDL_RWwrite( p_archivo, &valor, sizeof( valor ), 1 );
	return SDL_SwapDoubleBE( valor );
}

size_t Archivo::escribeU8( Uint8 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteU8( p_archivo, valor );
}

size_t Archivo::escribeLE16( Uint16 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteLE16( p_archivo, valor );
}

size_t Archivo::escribeBE16( Uint16 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteBE16( p_archivo, valor );
}

size_t Archivo::escribeLE32( Uint32 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteLE32( p_archivo, valor );
}

size_t Archivo::escribeBE32( Uint32 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteBE32( p_archivo, valor );
}

size_t Archivo::escribeLE64( Uint64 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteLE64( p_archivo, valor );
}

size_t Archivo::escribeBE64( Uint64 valor ) {
	if( esta_cerrado() ) return 0;
	return SDL_WriteBE64( p_archivo, valor );
}

size_t Archivo::escribeLEFloat( float valor ) {
	if( esta_cerrado() ) return 0.0f;
	const float swapped = SDL_SwapFloatLE( valor );
	return SDL_RWwrite( p_archivo, &swapped, sizeof( valor ), 1 );
}

size_t Archivo::escribeBEFloat( float valor ) {
	if( esta_cerrado() ) return 0.0f;
	const float swapped = SDL_SwapFloatBE( valor );
	return SDL_RWwrite( p_archivo, &swapped, sizeof( valor ), 1 );
}

size_t Archivo::escribeLEDouble( double valor ) {
	if( esta_cerrado() ) return 0.0;
	const double swapped = SDL_SwapDoubleLE( valor );
	return SDL_RWwrite( p_archivo, &swapped, sizeof( valor ), 1 );
}

size_t Archivo::escribeBEDouble( double valor ) {
	if( esta_cerrado() ) return 0.0;
	const double swapped = SDL_SwapDoubleBE( valor );
	return SDL_RWwrite( p_archivo, &swapped, sizeof( valor ), 1 );
}

// -----------------------------------------------------------------------------
//                             ACCESO ALEATORIO
// -----------------------------------------------------------------------------
long int Archivo::capacidad() {
	if( esta_cerrado() ) return 0;
    return SDL_RWsize( p_archivo );
}

long int Archivo::posicion() { 
	if( esta_cerrado() ) return 0;
	return SDL_RWtell( p_archivo );
}

void Archivo::al_inicio() {
	if( esta_cerrado() ) return;
	SDL_RWseek( p_archivo, 0, RW_SEEK_SET );
}

void Archivo::al_final() { 
	if( esta_cerrado() ) return;
	SDL_RWseek( p_archivo, 0, RW_SEEK_END );
}

bool Archivo::desplaza_desde_inicio( long int desplazamiento ) {
	if( esta_cerrado() ) return false;
	return SDL_RWseek( p_archivo, desplazamiento, RW_SEEK_SET )  < 0 ? false : true;
}

bool Archivo::desplaza_desde_final(  long int desplazamiento ) {
	if( esta_cerrado() ) return false;
    return SDL_RWseek( p_archivo, desplazamiento, RW_SEEK_END )  < 0 ? false : true;
}

bool Archivo::desplaza_desde_actual( long int desplazamiento ) {
	if( esta_cerrado() ) return false;
	return SDL_RWseek( p_archivo, desplazamiento, RW_SEEK_CUR ) < 0 ? false : true;
}

}

