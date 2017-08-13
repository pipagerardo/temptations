#ifndef _PGL_COLOR_
#define _PGL_COLOR_
#include "PGL_cabeceras.h"
namespace PGL {
	
enum COLOR {
	TRANSPARENTE,
	NEGRO,
	VERDE,
	VERDE_CLARO,
	AZUL_OSCURO,
	AZUL_CLARO,
	ROJO_OSCURO,
	CIAN,			// Mezcla de Verde y Azul.
	ROJO,
	ROJO_CLARO,
	AMARILLO_OSCURO,
	AMARILLO_CLARO,
	VERDE_OSCURO,
	MAGENTA,		// Mezcla de Azul y Rojo.
	GRIS,
	BLANCO
};

class Color {
public:

	// Construcción y destrucción:
	Color(); 	// 0, 0, 0, 255
	Color( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa = 255 );
	Color( COLOR color );
	Color( const Color& color );
	Color( const char*  html  );
	~Color();
	
	void  inicia( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa = 255 );
	void  inicia( COLOR color );
	void  inicia( const Color& color );
	void  inicia( const char*  html  ); // "#aabbcc" "#aabbccdd"
	void  quita();

	// Funciones de trabajo:
	void  tomaRojo(  Uint8 rojo  );
	void  tomaVerde( Uint8 verde );
	void  tomaAzul(  Uint8 azul  );
	void  tomaAlfa(  Uint8 alfa  );

	// Retornos:
	Uint8 dameRojo()  const;
	Uint8 dameVerde() const;
	Uint8 dameAzul()  const;
	Uint8 dameAlfa()  const;

	Color* operator &();
	Color& operator = ( const Color& color );
		
	// Retornos estáticos retorna los colores de la paleta:
	static Uint8 dameRojo(  COLOR color );
	static Uint8 dameVerde( COLOR color );	
	static Uint8 dameAzul(  COLOR color );
	static Uint8 dameAlfa(  COLOR color );
	
private:
	Uint8			m_rgba[4];
	static Uint8	m_paleta[16][4];
};

}
#endif


