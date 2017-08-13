#ifndef _DISPAROS_
#define _DISPAROS_
#include "PGL/PGL.h"
#include "baseTMP.h"
#include "monje.h"

struct Disparos_Tiro {
	bool activo;
	float x;
	float y;
	Enumera_Tipo_Disparo	tipo;
	Enumera_Direccion 		direccion;
};

class Disparos : public BaseTMP {
public:
	Disparos();
	~Disparos();
	bool inicia();
	void quita();
	void limpia();
	void dispara( const Monje& monje );
	void mueve();
	void dibuja();

private:
	friend class Procesos;
	size_t         m_cantidad;
	Disparos_Tiro *m_tiro;
	int            m_cuenta;
	int            m_oculto;
	void objetosEspeciales( size_t i, Uint16 baldosa, int px, int py );
};

#endif
