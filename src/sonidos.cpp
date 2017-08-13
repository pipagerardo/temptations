#include "sonidos.h"

Sonidos::Sonidos() {
	m_sonido = NULL;
	m_anda = false;
	m_salta = false;
	m_numero_salto = 0;
	m_sonidos_mejorados = false;
}

Sonidos::~Sonidos() {
	quita();
}
	
bool Sonidos::inicia( bool sonido_mejorado ) {
	if( m_sonido != NULL ) quita();
	m_sonido = new PGL::Sonido[ 13 ];
	m_sonidos_mejorados = !sonido_mejorado;

	// Reservamos grupos de canales canales:
	Mix_GroupChannels( 0, 3, -1 );
	if( Mix_ReserveChannels( 4 ) != 4 )  { // -1
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Error: Mix_ReserveChannels( 4 )" );
	#endif
	}
	if( !Mix_GroupChannels( 4, 7, 0 ) ) {	// 0 Disparos
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Error: Mix_GroupChannels( 4, 7, 0 )" );
	#endif
	}
	if( !Mix_GroupChannels( 8, 9,  1 ) ) {	// 1 fin disparos
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Error: Mix_GroupChannels( 8, 9,  1 )" );
	#endif
	}
	if( !Mix_GroupChannel( 10, 2 ) ) {		// 2 Andar
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Error: Mix_GroupChannel( 10, 2 )" );
	#endif
	}
	if( !Mix_GroupChannel( 11, 3 ) ) {		// 3 Saltar
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Error: Mix_GroupChannel( 11, 3 )" );
	#endif
	}
	
	return cambia();	
}

void Sonidos::quita() {
	if( m_sonido != NULL ) {
		delete[] m_sonido;
		m_sonido = NULL;
		m_anda = false;
	}
}

bool Sonidos::cambia() {
	m_sonidos_mejorados = !m_sonidos_mejorados;
	if( m_sonidos_mejorados ) {
		if( !m_sonido[0].inicia(  "sonidos/anda_mejorado.ogg", 64 ) ) return false;
		if( !m_sonido[1].inicia(  "sonidos/cogerobjeto_mejorado.ogg", 64 ) ) return false;
		if( !m_sonido[2].inicia(  "sonidos/nuevoobjeto_mejorado.ogg", 64 ) ) return false;
		if( !m_sonido[3].inicia(  "sonidos/muerte_mejorado.ogg", 64 ) ) return false;
		if( !m_sonido[4].inicia(  "sonidos/disparo_mejorado.ogg", 64 ) ) return false;
		if( !m_sonido[5].inicia(  "sonidos/findisparo_mejorado.ogg", 64 ) ) return false;
		if( !m_sonido[6].inicia(  "sonidos/salto1_mejorado.ogg", 32 ) ) return false;
		if( !m_sonido[7].inicia(  "sonidos/salto2_mejorado.ogg", 32 ) ) return false;
		if( !m_sonido[8].inicia(  "sonidos/salto3_mejorado.ogg", 32 ) ) return false;
		if( !m_sonido[9].inicia(  "sonidos/salto4_mejorado.ogg", 32 ) ) return false;
		if( !m_sonido[10].inicia( "sonidos/salto5_mejorado.ogg", 32 ) ) return false;
		if( !m_sonido[11].inicia( "sonidos/salto6_mejorado.ogg", 32 ) ) return false;
		if( !m_sonido[12].inicia( "sonidos/acabose_mejorado.ogg", 64 ) ) return false;
	} else {
		if( !m_sonido[0].inicia(  "sonidos/anda.ogg", 64 ) ) return false;
		if( !m_sonido[1].inicia(  "sonidos/cogerobjeto.ogg", 64 ) ) return false;
		if( !m_sonido[2].inicia(  "sonidos/nuevoobjeto.ogg", 64 ) ) return false;
		if( !m_sonido[3].inicia(  "sonidos/muerte.ogg", 64 ) ) return false;
		if( !m_sonido[4].inicia(  "sonidos/disparo.ogg", 64 ) ) return false;
		if( !m_sonido[5].inicia(  "sonidos/findisparo.ogg", 64 ) ) return false;
		if( !m_sonido[6].inicia(  "sonidos/salto1.ogg", 32 ) ) return false;
		if( !m_sonido[7].inicia(  "sonidos/salto2.ogg", 32 ) ) return false;
		if( !m_sonido[8].inicia(  "sonidos/salto3.ogg", 32 ) ) return false;
		if( !m_sonido[9].inicia(  "sonidos/salto4.ogg", 32 ) ) return false;
		if( !m_sonido[10].inicia( "sonidos/salto5.ogg", 32 ) ) return false;
		if( !m_sonido[11].inicia( "sonidos/salto6.ogg", 32 ) ) return false;
		if( !m_sonido[12].inicia( "sonidos/acabose.ogg", 64 ) ) return false;
	}	
	return true;
}

void Sonidos::pausa() {
	Mix_Pause( -1 );
}

void Sonidos::continua() {
	Mix_Resume( -1 ); 
}
	
void Sonidos::cogerObjeto() {
	m_sonido[1].reproduce( -1, 0 );
}

void Sonidos::nuevoObjeto() {
	m_sonido[2].reproduce( -1, 0 );
}

void Sonidos::acabose() {
	m_sonido[12].reproduce( -1, 0 );
}

void Sonidos::muerte() {
	m_sonido[3].reproduce( -1, 0 );
}

void Sonidos::diparo() {
	m_sonido[4].reproduce( 0, 0 );
}

void Sonidos::disparoFin() {
	m_sonido[5].reproduce( 1, 0 );
}

void Sonidos::anda() {
	if( !m_anda ) {
		m_sonido[0].reproduce( 2, -1 );
		m_anda = true;
	}
}
void Sonidos::andaPara() {
	if( m_anda ) {
		m_sonido[0].para();
		m_anda = false;
	}
}

void Sonidos::salta( int y ) {
	if( !m_salta ) {
		if		( y < 40  ) m_numero_salto = 11;
		else if ( y < 56  ) m_numero_salto = 10;
		else if ( y < 72  ) m_numero_salto = 9;
		else if ( y < 88  ) m_numero_salto = 8;
		else if ( y < 104 ) m_numero_salto = 7;
		else 		        m_numero_salto = 6;	
		m_sonido[ m_numero_salto ].reproduce( 3, 0 );
		m_salta = true;
	}

}

void Sonidos::saltaPara() {
	if( m_salta ) {
		m_sonido[ m_numero_salto ].para();
		m_salta = false;
	}
}

