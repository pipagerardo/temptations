#ifndef _PGL_CONTENEDOR_
#define _PGL_CONTENEDOR_
#include "PGL_cabeceras.h"
namespace PGL {
	
template <class TIPO>
class Contenedor {
public:

Contenedor()  { m_longitud = 0; m_datos = NULL; }
~Contenedor() { quita(); }

bool mete( const TIPO* datos, const size_t cantidad ) {
	size_t nuevaLongitud = m_longitud + cantidad;
	TIPO*  nuevosDatos   = new TIPO[ nuevaLongitud ];
	if( nuevosDatos == NULL ) return false; // Fallo de memoria.
	bool retorno;
	// Si hay datos viejos los copiamos al principio de los nuevos datos.
	if( m_datos != NULL ) {
		retorno = SDL_memcpy( (void*)nuevosDatos, (void*)m_datos, m_longitud * sizeof( TIPO ) );
        delete[] m_datos;
        if( !retorno ) return retorno;
    }
    // Copiamos los nuevos datos.
    retorno = SDL_memcpy( (void*)&nuevosDatos[ m_longitud ], (void*)datos, cantidad * sizeof( TIPO ) );
    m_datos     = nuevosDatos;
    m_longitud += cantidad;
    return retorno;
};

TIPO   operator[] ( size_t posicion ) const	{ return m_datos[ posicion ]; }
TIPO&  operator[] ( size_t posicion )       { return m_datos[ posicion ]; }
TIPO*  operator() (void) const 				{ return m_datos; }
TIPO*  operator() (size_t posicion ) const  { return &m_datos[ posicion ]; }
size_t dameLongitud() const 				{ return m_longitud; }

void quita() {
	m_longitud = 0;
	if( m_datos != NULL ) delete[] m_datos;
	m_datos = NULL;
}
 
private: 
	TIPO*  m_datos;
	size_t m_longitud;
};

}
#endif

