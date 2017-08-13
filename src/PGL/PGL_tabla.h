// ----------------------------------------------------------------------------
// EJEMPLOS DE USO:
// ----------------------------------------------------------------------------
// Crear una tabla:
//		size_t ancho = 10;
//		Tabla< TIPO > tabla( ancho );
//
// Guardar, leer datos en la Tabla:
// 		(TIPO) tabla[ ancho ] = dato;
//
// Destruir la Tabla:
//		tabla.quita();
// ----------------------------------------------------------------------------
// Crear una tabla de dos dimensiones o matriz.
//		size_t alto = 5, ancho = 3;
//		Tabla< Tabla< TIPO > > matriz( alto );
//		for( size_t i = 0; i < matriz.dameLongitud(); ++i ) 
//			matriz[ i ].inicia( ancho );
//
// Guardar, leer datos en la matriz:
//		(Dato) matriz[ alto ][ ancho ] = dato;
//
// Destruir la Matriz;
//		for( size_t i = 0; i < matriz.dameLongitud(); ++i ) 
//			matriz[ i ].quita();
//		matriz.quita();
// ----------------------------------------------------------------------------
#ifndef _PGL_TABLA_
#define _PGL_TABLA_
#include "PGL_cabeceras.h"
namespace PGL {
	
template <class TIPO>
class Tabla {
public:

Tabla() {
	numero_de_elementos = 0;
	puntero_tipo = NULL;
}

Tabla( size_t num_elementos ) {
	numero_de_elementos = 0;
	puntero_tipo = NULL;
	this->inicia( num_elementos );
}

Tabla( size_t num_elementos, TIPO valor ) {
	numero_de_elementos = 0;
	puntero_tipo = NULL;
	this->inicia( num_elementos, valor );
}

Tabla( const Tabla &tabla ) {
	numero_de_elementos = 0;
	puntero_tipo = NULL;
	this->inicia( tabla.numero_de_elementos );
	this->copia( tabla );
}

~Tabla() {
	this->quita();
}

bool inicia( size_t num_elementos ) {
	if( puntero_tipo  != NULL ) return false;
	if( num_elementos == 0    ) return false;
	numero_de_elementos = num_elementos;
	puntero_tipo = new TIPO[ numero_de_elementos ];
	if( puntero_tipo == NULL ) {
		numero_de_elementos = 0;
		return false;
	}
	return true;
}

bool inicia( size_t num_elementos, TIPO valor ) {
	if( !inicia( num_elementos ) ) return false;
	for( size_t i = 0; i < numero_de_elementos; ++i )
		puntero_tipo[ i ] = valor;
	return true;
}

void quita() {
	if( puntero_tipo == NULL ) return;
	if( numero_de_elementos == 0 ) return;
	numero_de_elementos = 0;
	delete[] puntero_tipo;
	puntero_tipo = NULL;
}

bool copia( const Tabla &tabla ) {
 	size_t cantidad;
	if( numero_de_elementos > tabla.numero_de_elementos ) cantidad = tabla.numero_de_elementos;
	else                                                  cantidad = numero_de_elementos;
	return SDL_memcpy( 
		(void*)puntero_tipo, 
		// numero_de_elementos * sizeof( TIPO ), 
		(const void*)tabla.puntero_tipo, 
		cantidad * sizeof( TIPO ) 
	);
}

// Copia parte de una tabla a otra, destino por defecto es cero y es la posici�n
// de destino desde donde empezar a copiar la tabla:
bool copia( const Tabla &tabla, size_t inicio, size_t final, size_t destino = 0 ) {
	size_t cantidad, max_destino, max_origen;
	max_destino = numero_de_elementos - destino;
	max_origen  = final - inicio + 1;
	if( max_destino > max_origen ) cantidad = max_origen;
	else                           cantidad = max_destino;
	return SDL_memcpy( 
		(void*)(puntero_tipo+destino), 
		// max_destino * sizeof( TIPO ), 
		(const void*)(tabla.puntero_tipo+inicio), 
		cantidad * sizeof( TIPO )
	);
}

TIPO& operator[]( size_t posicion ) {
	if ( posicion >= numero_de_elementos )
		return puntero_tipo[ numero_de_elementos - 1 ];
	return puntero_tipo[ posicion ];
}

TIPO operator[]( size_t posicion ) const {
	if ( posicion >= numero_de_elementos )
		return puntero_tipo[ numero_de_elementos - 1 ];
	return puntero_tipo[ posicion ];
}

size_t dameLongitud() const {
	return numero_de_elementos;
}

// Ordena la tabla usando una funci�n comparadora que hay que suministrar:
// Prototipo de la funci�n de comparar:
//	int comparar( const void* arg1, const void* arg2 ) {
//		if( *( TIPO *)arg1 < *( TIPO *)arg2 ) return -1;
//		if( *( TIPO *)arg1 > *( TIPO *)arg2 ) return  1;
//    	return  0;
//	}
void ordena( int (*comparar)(const void*, const void*) ) {
	SDL_qsort( (void*)puntero_tipo, numero_de_elementos, sizeof( TIPO ), comparar );
}

// Busca el elemento base retornando su posici�n, si hay varias coincidencias
// retorna la que le parezca. Si no lo encuentra retorna el n�mero de elementos.
// Requiere que la tabla est� ordenada.
size_t busca( TIPO base, int (*comparar)(const void*, const void*) ) {
	TIPO *p_busca = NULL;
	p_busca = (TIPO*)bsearch( (const void*)&base, (const void*)puntero_tipo, numero_de_elementos, sizeof( TIPO ), comparar );
	if ( p_busca ) return ( p_busca - puntero_tipo );
	               return ( numero_de_elementos    );
}

private:
	size_t  numero_de_elementos;
	TIPO    *puntero_tipo;
};

}
#endif

