#ifndef _JUEGO_
#define _JUEGO_
#include "PGL/PGL.h"
#include "baseTMP.h"
#include "dibuja.h"
#include "monje.h"
#include "disparos.h"
#include "procesos.h"

struct Configuracion {
	bool graficos_mejorados;
#ifndef PGL_ANDROID // En android siempre es en pantalla completa y con el mismo tamaño.
	bool pantalla_completa;	
	int  pantalla_ancho; 
	int  pantalla_alto; 
#endif
	bool musica;
	bool ingles;
#ifdef TACTIL
	float zoom;
	Uint8 alfa;
#endif
};

class Juego : public BaseTMP {
public:
	
	Juego();
	~Juego();

	bool inicia();
	void quita();
	void juega();

private:
	void rutaConfiguracion(); 
	void leeConfiguracion();
	void guardaConfiguracion();
	bool iniciaNivel( int numero );	// Hay que mejorarlo...
	bool leeControl();
	bool mueve();
	bool escribe( const char* texto );
	void pausa();
#ifdef TACTIL
	void dibujaTactil();
#endif
	bool pantallaCarga();	// Pantalla de Carga.
	bool pantallaInicio();	// Pantalla de Inicio.
	bool pantallaFinal();	// Pantalla final.
	bool muerteNoni();
	Dibuja     m_dibuja;
	Monje      m_noni;
	Disparos   m_disparos;
	Procesos   m_procesos;
	char* 	   m_rutaConfiguracion;
	bool       m_unavez;
	int        m_cuenta_revive;
	int        m_parpadeo;
	int		   m_cuenta;
};

#endif

