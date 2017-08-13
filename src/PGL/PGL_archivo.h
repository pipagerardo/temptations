#ifndef _PGL_ARCHIVO_
#define _PGL_ARCHIVO_
#include "PGL_cabeceras.h"
#include "PGL_cadena.h"
namespace PGL {
	
class Archivo {
public:
	
// -----------------------------------------------------------------------------
//                        CONSTRUCTORES Y DESTRUCTOR
// -----------------------------------------------------------------------------
Archivo();
Archivo( const char *nombre, const char *modo );
Archivo( void       *memoria, int ancho );
Archivo( const void *memoria, int ancho );
~Archivo();       

// -----------------------------------------------------------------------------
//                     ABRIR Y CERRAR UN ARCHIVO                            
// -----------------------------------------------------------------------------
bool inicia( const char *nombre, const char *modo ); // Abre un archivo y retorna true si est� abierto.
bool inicia( void       *memoria, int ancho );
bool inicia( const void *memoria, int ancho );
bool quita();                                     // Cierra un archivo y retorna true si est� cerrado.
// "r"  leer: El archivo debe existir.
// "w"  escribir: Crear un archivo vac�o para las operaciones de salida. Si ya existe un archivo con el mismo nombre, su contenido se descartan y el archivo se trata como un nuevo archivo vac�o.
// "a"  anexar: Escribir al final de un archivo. Operaciones de salida siempre escriben datos al final del archivo, expandi�ndolo. Operaciones de reposicionamiento (fseek, fsetpos, rebobinado) se ignoran. Se crea el archivo si no existe.
// "r+" leer / escribir: Abrir un archivo de actualizaci�n (tanto para la entrada y salida). El archivo debe existir.
// "w+" escribir / leer: Crear un archivo vac�o y abrirlo para la actualizaci�n (tanto para la entrada y salida). Si ya existe un archivo con el mismo nombre de su contenido se descartan y el archivo se trata como un nuevo archivo vac�o.
// "a+" anexar / leer: Abrir un archivo de actualizaci�n (tanto para la entrada y salida) con todas las operaciones de salida de la escritura de datos al final del archivo. Operaciones de reposicionamiento (fseek, fsetpos, rebobinado) afecta las pr�ximas operaciones de entrada, pero las operaciones de salida de mover la posici�n de nuevo al final del archivo. Se crea el archivo si no existe.
// "b"  Modo binario: "rb", "wb", "ab", "r+b", "w+b" y "a+b"
// -----------------------------------------------------------------------------
//	                       FUNCIONES �TILES                                
// -----------------------------------------------------------------------------
bool esta_abierto() const; // Retorna true si est� abierto y false si est� cerrado.
bool esta_cerrado() const; // Retorna true si est� cerrado y false si est� abierto.
bool final(); 			   // Retorna true  cuando se ha llegado al final del archivo.
bool no_final(); 		   // Retorna false cuando se ha llegado al final del archivo.

// -----------------------------------------------------------------------------
//                        LECTURA EN MODO TEXTO                             
// -----------------------------------------------------------------------------
char lee();
bool lee( char   *cadena, size_t longitud );
bool lee( Cadena &cadena );
 
// -----------------------------------------------------------------------------
//                       ESCRITURA EN MODO TEXTO                           
// -----------------------------------------------------------------------------
bool escribe( char        caracter );
bool escribe( const char *cadena, size_t longitud );
bool escribe( Cadena     &cadena );

// -----------------------------------------------------------------------------
//                       ENTRADA Y SALIDA FORMATEADA:                       
// -----------------------------------------------------------------------------
// Se recomienda poner '\n' al final de formato. 
// Retornan el n�mero de datos leidos o escritos. 
size_t sprintf( const char *formato, ... ); 
size_t sprintf( size_t cantidad, const char *formato, ... ); 
size_t sscanf(  const char *formato, ... ); 

// -----------------------------------------------------------------------------
//                  LECTURA Y ESCRITURA EN MODO BINARIO                    
// -----------------------------------------------------------------------------
// char lee_caracter();                     // Lee un caracter tipo char.
// char escribe_caracter( char caracter );  // Escribe un car�cter tipo char.
size_t lee(     void *puntero, size_t ancho, size_t cantidad );
size_t escribe( void *puntero, size_t ancho, size_t cantidad );
// Leer y escribir registros de longitud fija:
// Los par�metros son: 
//   puntero   Puntero a la zona de memoria donde leer o escribir los datos.
//   ancho     El ancho de cada registro en bytes. 
//   cantidad  La cantidad de registros a leer o escribir.
// El valor de retorno: es el n�mero de registros le�dos o escritos.

Uint8  leeU8();
Uint16 leeLE16();
Uint16 leeBE16();
Uint32 leeLE32();
Uint32 leeBE32();
Uint64 leeLE64();
Uint64 leeBE64();
float  leeLEFloat();
float  leeBEFloat();
double leeLEDouble();
double leeBEDouble();
	
size_t escribeU8(   Uint8  valor );
size_t escribeLE16( Uint16 valor );
size_t escribeBE16( Uint16 valor );
size_t escribeLE32( Uint32 valor );
size_t escribeBE32( Uint32 valor );
size_t escribeLE64( Uint64 valor );
size_t escribeBE64( Uint64 valor );
size_t escribeLEFloat(  float  valor );
size_t escribeBEFloat(  float  valor ); 
size_t escribeLEDouble( double valor );
size_t escribeBEDouble( double valor ); 

// -----------------------------------------------------------------------------
//                           ACCESO ALEATORIO                             
// -----------------------------------------------------------------------------
long int capacidad();		// Retorna el tama�o en bytes del archivo.
long int posicion();		// Retorna la posici�n actual dentro del archivo.
void al_inicio();			// Se posiciona al incio del archivo.
void al_final();			// Se posiciona al final del archivo.
bool desplaza_desde_inicio( long int desplazamiento );
bool desplaza_desde_final(  long int desplazamiento );
bool desplaza_desde_actual( long int desplazamiento );

private:
	SDL_RWops *p_archivo;
	bool       b_abierto;
};

}
#endif


