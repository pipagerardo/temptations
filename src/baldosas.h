#ifndef _BALDOSAS_
#define _BALDOSAS_
#include "PGL/PGL.h"
#include "basePGL.h"

struct Baldosas_Datos {
	int primera;	// Numero de la primera baldosa.
 	int ancho; 		// Ancho en pixels de baldosa.
	int alto;	    // Alto en pixels de baldosa.
	int cantidad; 	// El número de baldosas.  
	int espacio;	// La distancia en píxeles entre las baldosas en este juego de fichas.
	int margen;		// El margen alrededor de los azulejos en este juego de fichas.
	PGL::Imagen imagen;
};

union Baldosas_Propiedades {
	struct {
		Uint8 arriba    : 1;	// 1
		Uint8 abajo     : 1;	// 2
		Uint8 derecha   : 1;	// 4
		Uint8 izquierda : 1;	// 8
		Uint8 mata      : 1;	// 16
		Uint8 objeto    : 1;	// 32
		Uint8 animada   : 1;	// 64
		Uint8 invisible : 1;	// 128
	};
	Uint8 todas;
};

class Baldosas : public BasePGL {
public:
	
	Baldosas();
	~Baldosas();
	bool inicia();
	void quita();
	bool cambia();
	
	void tomaPosicion( int x, int y );
	void actualizaAnimacion();
	int pinta( Uint16 baldosa, int x, int y );
	
	bool arriba(    Uint16 baldosa );
	bool abajo(     Uint16 baldosa );
	bool derecha(   Uint16 baldosa );
	bool izquierda( Uint16 baldosa );
	bool mata(      Uint16 baldosa );
	bool invisible( Uint16 baldosa );
	bool objeto(    Uint16 baldosa );
		
private:
	size_t			m_total;	// El total de baldosas.
	size_t          m_cantidad;	// La cantidad de Baldosas_Datos.
	Baldosas_Datos *m_baldosas;
	Baldosas_Propiedades *m_propiedades;
	int             m_x;
	int             m_y;
	int				m_animacion;
	int				m_retardo;
	SDL_Rect 		m_srctiles;
	SDL_Rect 		m_destiles;
	size_t 			m_ancho;
};

#endif
