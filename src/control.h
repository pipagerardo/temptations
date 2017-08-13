#ifndef _CONTROL_
#define _CONTROL_
#include "PGL/PGL.h"

union Tecla {
	struct {	// Las teclas como campo de bits:
		Uint32 arriba    : 1;
		Uint32 abajo     : 1;
		Uint32 derecha   : 1;
		Uint32 izquierda : 1;
		Uint32 fuego_1   : 1;
		Uint32 fuego_2   : 1;
		Uint32 fuego_3   : 1;
		Uint32 fuego_4   : 1;
		Uint32 quitar    : 1;
		Uint32 escapar   : 1;
		Uint32 pantalla  : 1;
		Uint32 graficos  : 1;
		Uint32 info      : 1;
		Uint32 pausa     : 1;
		Uint32 lenguaje  : 1;
		Uint32 musica    : 1;
		Uint32 activa    : 1; // SDL_WINDOWEVENT_ENTER
		Uint32 desactiva : 1; // SDL_WINDOWEVENT_LEAVE
		Uint32 mueve 	 : 1; // SDL_WINDOWEVENT_MOVED
		Uint32 cambia	 : 1; // SDL_WINDOWEVENT_RESIZED
		Uint32 maximiza  : 1; // SDL_WINDOWEVENT_MAXIMIZED
		Uint32 minimiza	 : 1; // SDL_WINDOWEVENT_MINIMIZED
        Uint32 restaura	 : 1; // SDL_WINDOWEVENT_RESTORED
		Uint32 f1        : 1;
		Uint32 f2        : 1;
        Uint32 f3	     : 1;
		Uint32 f4        : 1;
		Uint32 f5        : 1;
		Uint32 sinuso_1  : 1;
		Uint32 sinuso_2  : 1;
		Uint32 sinuso_3  : 1;
		Uint32 sinuso_4  : 1;
	};
	Uint32 todas; 	// Todas las teclas como un número entero sin signo:
};

#ifdef TACTIL
struct Tactil {
	SDL_TouchID id;
#ifdef TACTIL_RATON
	bool  raton;
#endif
	float zoom;
	int xpad;
	int ypad;
	int rmax;
	int rmed;
	int rmin;
	int rbot;
	int xb1;
	int xb2;
	Uint8 alfa;
	int x;
	int y;
};
#endif

class Control {
public:
	
	// Contrucción y destrucción:
	Control();
	~Control();
	bool inicia( int ancho, int alto );
	bool inicia( const PGL::Ventana &ventana );
	void quita();

	// Funciones de trabajo:
	void leeEstado();	// Lectura inmediata del estado del teclado y joystick.

	// Retornos de teclas:
	bool arriba() const;
	bool abajo() const;
	bool derecha() const;
	bool izquierda() const;
	bool fuego_1() const;
	bool fuego_2() const;
	bool fuego_3() const;
	bool fuego_4() const;
	bool fuego_5() const;
	bool quitar() const;	// Cerrar ventana.
	bool escapar() const;	// ESC
	bool pantalla() const;	// Pantalla Completa/Ventana.
	bool graficos() const;	// Cambiar gráficos.
	bool info() const;		// Mostrar información.
	bool pausa() const;
	bool lenguaje() const;	// Cambiar el Idioma del juego.
	bool musica() const;	// Quitar poner música.
	
	// Retornos de ventana:
	bool activa()    const; // SDL_WINDOWEVENT_ENTER
	bool desactiva() const; // SDL_WINDOWEVENT_LEAVE
	bool mueve()     const; // SDL_WINDOWEVENT_MOVED
	bool cambia()    const; // SDL_WINDOWEVENT_RESIZED
	bool maximiza()  const; // SDL_WINDOWEVENT_MAXIMIZED
	bool minimiza()  const; // SDL_WINDOWEVENT_MINIMIZED
    bool restaura()  const; // SDL_WINDOWEVENT_RESTORED

	bool f1() const;
	bool f2() const;
	bool f3() const;
	bool f4() const;
	bool f5() const;
	
	bool salir() const;
	bool derecha_y_arriba()   const;
	bool derecha_y_abajo()    const;
	bool izquierda_y_arriba() const;
	bool izquierda_y_abajo()  const;
	
	bool dispara()	const; // espacio || fuego_1 || fuego_3
	bool salta()	const; // fuego_2 || fuego_4;

	// Usar después de un evento mueve();
	int  dameX()     const;
	int  dameY()     const;

	// Usar después de un evento cambia();
	int   dameAncho() const;
	int   dameAlto()  const;
	Tecla dameTecla() const;

#ifdef TACTIL
	Tactil dameTactil() const;
	bool   hayTactil() const;
	void   quitaTactil();
	void   agrandaTactil();
	void   encogeTactil();
	void   transparentaTactil();
	void   opacaTactil();
	void   tomaZoom( float zoom );
	void   tomaAlfa( Uint8 alfa );
#endif
	
private:
#ifdef TACTIL
	void actualizaTactil();
	void leeTactil( float xf, float yf );	
	Tactil			m_tactil;
	float 			m_radianes[8];
#endif
	int				m_x;
	int				m_y;
	int 		    m_ancho;
	int 			m_alto;
	Tecla        	m_tecla;
	PGL::Ventana*   m_ventana;
	SDL_Joystick*	m_joystick;
	SDL_GameController* m_gamecontroller;
};

#endif

