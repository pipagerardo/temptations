#ifndef _DIBUJA_
#define _DIBUJA_
#include "PGL/PGL.h"
#include "baseTMP.h"
#include "monje.h"

class Dibuja : public BaseTMP {
public:
	
	Dibuja();
	~Dibuja();
	bool inicia( int mapa );
	void quita();
	bool cambiaGraficos();
	
	void inicio();
	void pantalla( int posx, int posy );
	void final();
	void consola( const Monje& monje, int nivel, int pantalla );
	void escribe( const char* texto );
	
};

#endif
