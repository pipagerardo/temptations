#ifndef _PGL_TIEMPO_
#define _PGL_TIEMPO_
#include "PGL_cabeceras.h"

namespace PGL {

class Retardo {
public:
	Retardo( Uint32 hz = 50 );
	void   inicia( Uint32 hz = 50 );
	Uint32 espera();
	Uint32 dameHz() const;
	float  dameRatio() const;
private:
	Uint32 m_hz;
	Uint32 m_cuenta;
	float  m_ratio;
	Uint32 m_base;
	Uint32 m_ultimo;	
};
	
class Tiempo {
public:
	

	
private:

};

}
#endif

// void SDL_Delay( Uint32 ms ); // Espera milisegundos.
// bool SDL_TICKS_PASSED( A, B ); // Retorna TRUE si A > B
// Uint32 SDL_GetTicks();	// Retorna los milisegundos, baja resolución.
// Uint64 SDL_GetPerformanceCounter();	 // Retorna pulsos en alta resolución.
// Uint64 SDL_GetPerformanceFrequency(); // Retorna los pulsos por segundo.

/*
Uint32 callback( Uint32 interval, void *param ) {
	// void *param; // Parámetro pasado por el usuario.
	return interval;
}
SDL_TimerID SDL_AddTimer(
	Uint32            interval,	// Intervalo en milisegundos.
	SDL_TimerCallback callback,
    void*             param		// Parámetro pasado por el usuario.
);
SDL_bool SDL_RemoveTimer( SDL_TimerID id );
*/
