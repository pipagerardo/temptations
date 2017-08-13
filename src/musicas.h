#ifndef _MUSICAS_
#define _MUSICAS_
#include "PGL/PGL.h"

class Musicas {
public:
	Musicas();
	~Musicas();
	
	bool inicia( bool musica_mejorada, int nivel );
	void quita();
	bool cambia();
	void reproduce( int veces = -1 );
	void para();
	void pausa();
	void continua();
	
private:
	PGL::Musica m_musica;
	bool m_mejorada;
	int m_nivel;
};

#endif
