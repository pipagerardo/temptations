#ifndef _PGL_CADENA_
#define _PGL_CADENA_
#include "PGL_cabeceras.h"
namespace PGL {
	
class Cadena {
public:
	Cadena();
	Cadena( size_t capacidad );
	Cadena( size_t capacidad, size_t bloque );
	Cadena( const char*   texto );
	Cadena( const Cadena& texto );
	~Cadena();
	
	// El tamaño de los bloques de memoria. No hace nada hasta iniciar la cadena.
	void   tomaBloque( size_t bloque ); // Tamaño de bloque > 8 y < 1024, por defecto 64. Múltiplo de 8.
	size_t dameBloque() const;			// Retorna el tamaño de bloque de memoria. Múltiplo de 8.
	
	// Constructor y destructor de memoria.
	bool inicia( size_t capacidad );				// Reserva memoria en un multiplo de bloque.
	bool inicia( size_t capacidad, size_t bloque );	// Reserva memoria en un multiplo de bloque.
	void quita();									// Libera la memoria y restaura todos los valores.

	// La capacidad de memoria reservada es un multiplo de bloque.
	size_t      dameCapacidad() const;		    // Retorna la memoria reservada.
	size_t      dameLongitud() const;			// Retorna la longitud de la cadena alojada.
	const char* dameCadena()   const;			// Retorna la cadena.
	
	// Copiar datos a la cadena:
	size_t copia( char       caracter );
	size_t copia( const char*   texto );
	size_t copia( const Cadena& texto );
	
	// Concatenar datos a la cadena:
	size_t concatena( char       caracter );
	size_t concatena( const char*   texto );
	size_t concatena( const Cadena& texto );
	
	// sprintf y sscanf:
	size_t sprintf(  const char* formato, ... );
	size_t vsprintf( const char* formato, va_list va );
	size_t sprintf(  size_t cantidad, const char* formato, ... );
	size_t vsprintf( size_t cantidad, const char* formato, va_list va );
	size_t sscanf(   const char* formato, ... );
	size_t vsscanf(  const char* formato, va_list va );

	bool busca( char       caracter, size_t& posicion );
	bool busca( const char*   texto, size_t& posicion );
	bool busca( const Cadena& texto, size_t& posicion );
	
	void borraHasta( size_t posicion );
	void borraDesde( size_t posicion );
	
	// Estilo C++
	Cadena& operator = ( char       caracter );
	Cadena& operator = ( const char*   texto );
	Cadena& operator = ( const Cadena& texto );

	Cadena& operator << ( char       caracter );
	Cadena& operator << ( const char*   texto );
	Cadena& operator << ( const Cadena& texto );

	Cadena  operator + ( char       caracter );
	Cadena  operator + ( const char*   texto );
	Cadena  operator + ( const Cadena& texto );

	char&       operator[]( size_t posicion );
	const char& operator[]( size_t posicion ) const;

	bool operator == ( const char*   texto );
	bool operator == ( const Cadena& texto );

	bool operator != ( const char*   texto );
	bool operator != ( const Cadena& texto );
	
	bool parte( Cadena& cadena, const char *delimitadores );
	
private:
	size_t m_bloque;
	size_t m_capacidad;
	size_t m_longitud;
	char*  m_cadena;
};

}
#endif

