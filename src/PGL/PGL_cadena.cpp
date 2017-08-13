#include "PGL_cadena.h"
namespace PGL {
#define CADENA_BLOQUE 64

Cadena::Cadena() {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
}

Cadena::Cadena( size_t capacidad ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	inicia( capacidad );
}

Cadena::Cadena( size_t capacidad, size_t bloque ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	inicia( capacidad, bloque );
}

Cadena::Cadena( const char* texto ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	copia( texto );
}

Cadena::Cadena( const Cadena& texto ) {
	m_bloque    = CADENA_BLOQUE;
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
	copia( texto );
}
	
Cadena::~Cadena() {
	quita();
}

void Cadena::tomaBloque( size_t bloque )  { 
	m_bloque = bloque;
	if( m_bloque < 8    ) m_bloque = CADENA_BLOQUE;
	if( m_bloque > 1024 ) m_bloque = 1024;
	size_t resto = m_bloque % 8;
	if( resto ) m_bloque += resto;
}

size_t Cadena::dameBloque() const { 
	return m_bloque;
}
		
bool Cadena::inicia( size_t capacidad ) {
	if( capacidad == 0 ) return false;
	size_t posicion = m_capacidad;
	m_capacidad     = capacidad;
	size_t resto    = m_capacidad % m_bloque;
	if( resto ) m_capacidad += ( m_bloque - resto );
	// Ahora m_capacidad es la nueva y posición la vieja.
	if     ( m_cadena == NULL        ) m_cadena = (char*)SDL_malloc(  m_capacidad );
	else if( posicion != m_capacidad ) m_cadena = (char*)SDL_realloc( m_cadena, m_capacidad );
	if( m_cadena == NULL ) {
		quita();
		return false;
	}
	// Se ha incrementado la memoria, asi que la limpiamos.
	if( posicion < m_capacidad ) SDL_memset( m_cadena + posicion, 0, m_capacidad - posicion );
	return true;
}

bool Cadena::inicia( size_t capacidad, size_t bloque ){
	tomaBloque( bloque );
	return inicia( capacidad );
}

void Cadena::quita() {
	if( m_cadena != NULL ) SDL_free( m_cadena );
	m_bloque    = CADENA_BLOQUE; 
	m_capacidad = 0;
	m_longitud  = 0;
	m_cadena    = NULL;
}

size_t      Cadena::dameCapacidad() const { return m_capacidad; }	
size_t      Cadena::dameLongitud()  const { return m_longitud;  }
const char* Cadena::dameCadena()    const { return ( m_cadena != NULL ) ? (const char *)m_cadena : ""; }
	
size_t Cadena::copia( char caracter ) {
	if( caracter == '\0' ) return 0;
	if( 2 > m_capacidad ) {
		if( !inicia( 2 ) ) return 0;
	}
	m_cadena[0] = caracter;
	m_cadena[1] = '\0';
	m_longitud  = 1;
	return m_longitud;
}

size_t Cadena::copia( const char* texto ) {
	if( texto == NULL ) return 0;
	m_longitud = SDL_strlen( texto ) + 1;
	if( m_longitud > m_capacidad ) {
		if( !inicia( m_longitud ) ) return 0;
	}
	m_longitud = SDL_strlcpy( m_cadena, texto, m_capacidad );
	return m_longitud;
}

size_t Cadena::copia( const Cadena& texto ) {
	if( texto.m_longitud == 0 ) return 0;
	m_longitud = texto.m_longitud + 1;
	if( m_longitud > m_capacidad ) {
		if( !inicia( m_longitud ) ) return 0;
	}
	m_longitud = SDL_strlcpy( m_cadena, texto.m_cadena, m_capacidad );
	return m_longitud;
}

size_t Cadena::concatena( char caracter ) {
	if( caracter == '\0' ) return 0;
	if( 2 > ( m_capacidad - m_longitud ) ) {
		if( !inicia( m_capacidad + 2 ) ) return 0;
	}
	m_cadena[   m_longitud ] = caracter;
	m_cadena[ ++m_longitud ] = '\0';
	return m_longitud;
}

size_t Cadena::concatena( const char* texto ) {
	if( texto == NULL ) return 0;
	size_t longitud = SDL_strlen( texto ) + 1;
	size_t libre    = m_capacidad - m_longitud;
	if( longitud > libre ) {
		if( !inicia( m_capacidad + longitud ) ) return 0;
	}
	m_longitud = SDL_strlcat( m_cadena, texto, m_capacidad );
	return m_longitud;
}

size_t Cadena::concatena( const Cadena& texto ) {
	if( texto.m_longitud == 0 ) return 0;
	size_t longitud = texto.m_longitud + 1;
	size_t libre    = m_capacidad - m_longitud;
	if( longitud > libre ) {
		if( !inicia( m_capacidad + longitud ) ) return 0;
	}
	m_longitud = SDL_strlcat( m_cadena, texto.m_cadena, m_capacidad );
	return m_longitud;
}
	
size_t Cadena::Cadena::sprintf( const char* formato, ... ) {
	if( formato == NULL ) return 0;
   	va_list va; va_start( va, formato );
   	m_longitud = (size_t)SDL_vsnprintf( m_cadena, m_capacidad, formato, va );
	va_end( va );
   	return m_longitud;
}

size_t Cadena::vsprintf( const char* formato, va_list va ) {
	// if( ( formato == NULL ) || ( va == NULL ) ) return 0;
	if( formato == NULL ) return 0;
   	m_longitud = (size_t)SDL_vsnprintf( m_cadena, m_capacidad, formato, va );
   	return m_longitud;
}

size_t Cadena::sprintf( size_t cantidad, const char* formato, ... ) {
	if( ( cantidad == 0 ) || ( formato == NULL ) ) return 0;
    va_list va; va_start( va, formato );
 	inicia( cantidad );
    m_longitud = (size_t)SDL_vsnprintf( m_cadena, cantidad, formato, va );
	va_end( va );
    return m_longitud;
}

size_t Cadena::vsprintf( size_t cantidad, const char* formato, va_list va ) {
	// if( ( formato == NULL ) || ( cantidad == 0 ) || ( va == NULL ) ) return 0;
	if( ( formato == NULL ) || ( cantidad == 0 ) ) return 0;
 	inicia( cantidad );
    m_longitud = (size_t)SDL_vsnprintf( m_cadena, cantidad, formato, va );
    return m_longitud;
}

size_t Cadena::sscanf( const char* formato, ... ) {
	if( ( formato == NULL ) || ( m_longitud == 0 ) ) return 0;
    va_list va; va_start( va, formato );
	int retorno = SDL_vsscanf( (const char *)m_cadena, formato, va );
	va_end( va );
    return (size_t)retorno;
}

size_t Cadena::vsscanf( const char* formato, va_list va ) {
	// if( ( formato == NULL ) || ( m_longitud == 0 ) || ( va == NULL ) ) return 0;
	if( ( formato == NULL ) || ( m_longitud == 0 ) ) return 0;
	int retorno = SDL_vsscanf( (const char *)m_cadena, formato, va );
    return (size_t)retorno;
}

bool Cadena::busca( char caracter, size_t& posicion ) {
	if( m_longitud == 0 ) return false;
	char* retorno = SDL_strchr( (const char*)m_cadena, caracter );
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}
	
bool Cadena::busca( const char* texto, size_t& posicion  ) {
	if( m_longitud == 0 ) return false;
	if( texto == NULL   ) return false;
	char* retorno = SDL_strstr( (const char*)m_cadena, texto );
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

bool Cadena::busca( const Cadena& texto, size_t& posicion  ) {
	if( m_longitud       == 0 ) return false;
	if( texto.m_longitud == 0 ) return false;
	char* retorno = SDL_strstr( (const char*)m_cadena, texto.m_cadena );
	if( retorno != NULL ) {
		posicion = (size_t)( retorno - m_cadena );
		return true;
	}
	posicion = m_longitud + 1;
	return false;
}

void Cadena::borraHasta( size_t posicion ) {
	if( posicion > m_capacidad ) { quita(); return; }
	// m_longitud = SDL_strlcpy( m_cadena, m_cadena + posicion, m_capacidad );
	SDL_strlcpy( m_cadena, m_cadena + posicion, m_capacidad );
	m_longitud -= posicion;
}

void Cadena::borraDesde( size_t posicion ) {
	if( posicion > m_capacidad ) return;
	SDL_memset( m_cadena + posicion, 0, m_capacidad - posicion );
	m_longitud = posicion;
}

Cadena& Cadena::operator = ( char       caracter ) { this->copia( caracter ); return *this; }
Cadena& Cadena::operator = ( const char*   texto ) { this->copia( texto );    return *this; }
Cadena& Cadena::operator = ( const Cadena& texto ) { this->copia( texto );    return *this; }

Cadena& Cadena::operator << ( char       caracter ) { this->concatena( caracter ); return *this; }
Cadena& Cadena::operator << ( const char*   texto ) { this->concatena( texto );    return *this; }
Cadena& Cadena::operator << ( const Cadena& texto ) { this->concatena( texto );    return *this; }

Cadena  Cadena::operator + ( char       caracter ) { Cadena cadena( *this ); cadena << caracter; return cadena; }
Cadena  Cadena::operator + ( const char*   texto ) { Cadena cadena( *this ); cadena << texto; return cadena; }
Cadena  Cadena::operator + ( const Cadena& texto ) { Cadena cadena( *this ); cadena << texto; return cadena; }	

char&       Cadena::operator[]( size_t posicion )       { return ( posicion < m_longitud ) ? m_cadena[ posicion ] : m_cadena[ m_longitud-1 ]; }
const char& Cadena::operator[]( size_t posicion ) const { return ( posicion < m_longitud ) ? m_cadena[ posicion ] : m_cadena[ m_longitud-1 ]; }

bool Cadena::operator == ( const char*   texto ) { return ( SDL_strcasecmp( m_cadena, texto ) == 0 ); }
bool Cadena::operator == ( const Cadena& texto ) { return ( SDL_strcasecmp( m_cadena, texto.m_cadena ) == 0 ); }

bool Cadena::operator != ( const char*   texto ) { return ( SDL_strcasecmp( m_cadena, texto ) != 0 ); }
bool Cadena::operator != ( const Cadena& texto ) { return ( SDL_strcasecmp( m_cadena, texto.m_cadena ) != 0 ); }


bool Cadena::parte( Cadena& cadena, const char *delimitadores ) {
	static size_t inicio = 0;
	// Ponemos '\0' en todas las coincidencias con los delimitadores:
	if( inicio == 0 ) {
		size_t longitud = SDL_strlen( delimitadores );
		for( size_t i = 0; i < m_longitud; i++ ) {
			for( size_t j = 0; j < longitud; j++ ) {
				if( m_cadena[i] == delimitadores[j] ) {
					m_cadena[i] = '\0';
					break;
				}
			}
		}		
	}
	// Saltamos los '\0' hasta encontrar un tozo de cadena:
	size_t longitud = 0;
	for( ; inicio < m_longitud; ++inicio ) {
		longitud = SDL_strlen( m_cadena + inicio );
		if( longitud > 0 ) break;
	}
	// Hemos acabado:
	if( inicio >= m_longitud ) {
		inicio = 0;
		quita();
		return false;
	}
	// Copiamos el trozo de cadena:	
	cadena.copia( (const char*)(m_cadena + inicio) );
	inicio += longitud + 1;
	return true;
}

}

