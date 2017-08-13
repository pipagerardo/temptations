#ifndef _MAPA_
#define _MAPA_
#include "PGL/PGL.h"
#include "basePGL.h"

struct Mapa_Datos {
	int   x;
	int   y;
	int   ancho;
	int   alto;
	Uint8 alfa;
	bool  visible;
	PGL::Matriz< Uint16 > dato;
};

class Mapa : public BasePGL {
public:
	 Mapa();
	~Mapa();
	bool inicia();
	void quita();
	
	size_t dameMapas() const;
	Uint16 dame( size_t mapa, int x, int y ) const;
	void   toma( size_t mapa, int x, int y, Uint16 dato );
	int    dameAncho( size_t mapa ) const;
	int    dameAlto(  size_t mapa ) const;
	PGL::Color dameColor()          const;
	void 	tomaColor( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa = 255 );

private:
	size_t      m_cantidad;
	Mapa_Datos *m_mapa;
	PGL::Color  m_fondo; 
};

#endif
