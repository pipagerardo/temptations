#ifndef _PGL_COMPRESOR_
#define _PGL_COMPRESOR_
#include "PGL_cabeceras.h"
#ifdef PGL_USAR_COMPRESOR
#include "PGL_contenedor.h"
// Linker: -lzlib
namespace PGL {

int Comprime(    const char* datos, size_t largo, Contenedor<char>& contenedor, int nivel );
int Descomprime( const char* datos, size_t largo, Contenedor<char>& contenedor );
// int DescomprimeGZIP( const char* datos, size_t largo, Contenedor<char>& contenedor );
	
}
#endif

#endif


