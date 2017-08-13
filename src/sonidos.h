#ifndef _SONIDOS_
#define _SONIDOS_
#include "PGL/PGL.h"

class Sonidos {
public:
	Sonidos();
	~Sonidos();
	
	bool inicia( bool sonido_mejorado );
	void quita();
	bool cambia();
	void pausa();
	void continua();

	void anda();
	void andaPara();
	void salta( int y );
	void saltaPara();
	void cogerObjeto();
	void nuevoObjeto();
	void muerte();
	void diparo();
	void disparoFin();
	void acabose();
	
private:
	PGL::Sonido *m_sonido;
	bool m_anda;
	bool m_salta;
	size_t m_numero_salto;
	bool m_sonidos_mejorados;
};

#endif
