#include "PGL_tiempo.h"
namespace PGL {

Uint32 _getTicks() {
	Uint32 ticks = SDL_GetTicks();
	/* 
	* Since baseticks!=0 is used to track initialization
	* we need to ensure that the tick count is always >0 
	* since SDL_GetTicks may not have incremented yet and
	* return 0 depending on the timing of the calls.
	*/
	return (ticks == 0) ? 1 : ticks;
}

Retardo::Retardo( Uint32 hz ) {
	inicia( hz );
}

void Retardo::inicia( Uint32 hz ) {
	if     ( hz < 1   ) m_hz = 1;
	else if( hz > 200 ) m_hz = 200;
	else 				m_hz = hz;
	m_cuenta = 0;
	m_ratio  = ( 1000.0f / (float)m_hz );
	m_base  = _getTicks();
	m_ultimo  = m_base;
}

Uint32 Retardo::espera() {
	Uint32 actual, destino, el_retardo, tiempo_pasado = 0;
	m_cuenta++;
	actual = _getTicks();
	tiempo_pasado = actual - m_ultimo;
	m_ultimo = actual;
	destino = m_base + (Uint32) ((float) m_cuenta * m_ratio);
	if( actual <= destino ) {
		el_retardo = destino - actual;
		SDL_Delay( el_retardo );
	} else {
		m_cuenta = 0;
		m_base = _getTicks();
	}
	return tiempo_pasado;
}

Uint32 Retardo::dameHz() const {
	return m_hz;
}
float  Retardo::dameRatio() const {
	return m_ratio;
}

/*

Retardo::Retardo( int hz ) {
	inicia( hz ); 
}
void Retardo::inicia( int hz ) {
	m_fotogramasSegundo = hz;
	m_milisegundosFotograma = 1000 / m_fotogramasSegundo;
	m_tiempoInicial = SDL_GetTicks();
	m_tiempoFinal   = m_tiempoInicial;


	m_cuentaInicial    = SDL_GetPerformanceCounter();
	m_cuentaFinal      = m_cuentaInicial;
	m_cuentaPorSegundo = SDL_GetPerformanceFrequency();


}
void Retardo::espera() {

	m_cuentaFinal = m_cuentaInicial;
	m_cuentaInicial = SDL_GetPerformanceCounter();
	Uint32 tiempo = ((  m_cuentaInicial - m_cuentaFinal ) * 1000 ) / m_cuentaPorSegundo;	 
	if( tiempo < m_milisegundosFotograma ) {
		SDL_Delay( m_milisegundosFotograma - tiempo );
		SDL_LOG( "%u",  m_milisegundosFotograma - tiempo );
	} else {
		SDL_LOG( "OPS" );
	}

	
	m_tiempoFinal   = m_tiempoInicial;
	m_tiempoInicial = SDL_GetTicks();
	Uint32 tiempo = m_tiempoInicial - m_tiempoFinal;
	if( tiempo < m_milisegundosFotograma ) {
		Uint32 retardo = m_milisegundosFotograma - tiempo;
		if( retardo > 9 ) SDL_Delay( retardo );
		SDL_LOG( "%u",  retardo );
	} else {
		SDL_LOG( "OPS" );
	}
	

}
*/
	
}

