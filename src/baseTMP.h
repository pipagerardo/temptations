#ifndef _BASE_TMP_
#define _BASE_TMP_
#include "PGL/PGL.h"
#include "basePGL.h"
#include "baldosas.h"
#include "mapa.h"
#include "enemigos.h"

#define DISPAROS_MAX	6
#define PROCESOS_MAX	8
#define ULTIMO_NIVEL    6

enum Enumera_Tipo_Movimiento { 
	ANDA, 
	VUELA, 
	NADA 
};

enum Enumera_Tipo_Disparo { 
	FLECHA, 
	FUEGO, 
	AGUA, 
	BOMERANG
};

class BaseTMP : public BasePGL {
protected:
	static Baldosas  m_baldosas;
	static Mapa		 m_mapa;
	static Enemigos  m_enemigos;
};

#endif

