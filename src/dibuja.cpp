#include "dibuja.h"

Dibuja::Dibuja() {

}

Dibuja::~Dibuja() {
	quita();
}

bool Dibuja::inicia( int mapa ) {
	
	quita();
	bool retorno;
	
	// Iniciamos m_xml:
	switch( mapa ) {
		case 0:  retorno = m_xml.inicia( m_ruta.dameCadena(), "inicio.tmx" ); break;
		case 1:  retorno = m_xml.inicia( m_ruta.dameCadena(), "mapa1.tmx"  ); break;	
		case 2:  retorno = m_xml.inicia( m_ruta.dameCadena(), "mapa2.tmx"  ); break;	
		case 3:  retorno = m_xml.inicia( m_ruta.dameCadena(), "mapa3.tmx"  ); break;	
		case 4:  retorno = m_xml.inicia( m_ruta.dameCadena(), "mapa4.tmx"  ); break;
		case 5:  retorno = m_xml.inicia( m_ruta.dameCadena(), "mapa5.tmx"  ); break;
		case 6:  retorno = m_xml.inicia( m_ruta.dameCadena(), "final.tmx"  ); break;	
		default: retorno = false; break;
	}
	if( !retorno ) {
		m_xml.quita();
		return retorno;
	}
	
	m_mapa_ancho    = (int)m_xml.map.width;
	m_mapa_alto     = (int)m_xml.map.height;
	m_baldosa_ancho = (int)m_xml.map.tilewidth;
	m_baldosa_alto  = (int)m_xml.map.tileheight;
	
	// Antes de liberar el mapa xml:
	if( !m_baldosas.inicia() ) return false;
	if( !m_mapa.inicia()     ) return false;
	if( !m_enemigos.inicia() ) return false;
	
	// Liberamos el mapa xml:
	m_xml.quita();	

	m_pantalla_x     = 0;
	m_pantalla_y     = 0;
	m_pantalla_ancho = 32;
	m_pantalla_alto  = 16;
		
	m_consola_x     = 0;
	m_consola_y     = 0;
	m_consola_ancho = 32;
	m_consola_alto  = 8;
	
	m_consola_y     = m_pantalla_alto * m_baldosa_alto;
	m_consola_posy  = m_mapa_alto - m_consola_alto;

	m_pantalla_w = m_pantalla_x + ( m_pantalla_ancho * m_baldosa_ancho );
	m_pantalla_h = m_pantalla_y + ( m_pantalla_alto  * m_baldosa_alto  );

	m_anchoLog = m_pantalla_ancho * m_baldosa_ancho;
	m_altoLog = ( m_pantalla_alto + m_consola_alto ) * m_baldosa_alto;
	m_ventana.medidasLogicas( m_anchoLog, m_altoLog );

	return true;
}

void Dibuja::quita() {
	// TMP:
	m_mapa.quita();
	m_baldosas.quita();
	m_enemigos.quita();
}

bool Dibuja::cambiaGraficos() {
	m_graficos_mejorados = !m_graficos_mejorados;
	if( m_graficos_mejorados ) {
		if( m_nivel == 4 ) m_mapa.tomaColor( 64, 114, 176 );
		if( !m_sprites.inicia( m_ventana, "graficos/sprites_nenefranz.png" ) ) return false;
	} else {
		if( m_nivel == 4 ) m_mapa.tomaColor( 27, 170, 35 );
		if( !m_sprites.inicia( m_ventana, "graficos/sprites.png" ) ) return false;
	}
	return m_baldosas.cambia();
}

void Dibuja::inicio() {
	static int cuenta = 0;
	static int incy = 0;
	int ingles = ( m_ingles ) ? m_pantalla_ancho : 0;
	m_ventana.tomaColor( m_mapa.dameColor() );
	m_ventana.limpia();
	m_baldosas.tomaPosicion( m_pantalla_x, m_pantalla_y );
	for( int y = 0; y < 20; ++y ) {
	for( int x = 0; x < m_pantalla_ancho; ++x ) {
		Uint16 baldosa = m_mapa.dame( 0, x + ingles, y );
		m_baldosas.pinta( baldosa, x, y );
	}
	}
	for( int y = 20; y < 24; ++y ) {
	for( int x = 0; x < m_pantalla_ancho; ++x ) {
		Uint16 baldosa = m_mapa.dame( 0, x + ingles, y + incy );
		m_baldosas.pinta( baldosa, x, y );
	}
	}
	++cuenta;
	if( cuenta > 400 ) { cuenta = 0; incy += 4; }
	if( incy > 24 ) incy = 0;	
}

void Dibuja::pantalla( int posx, int posy ) {
	m_ventana.tomaColor( m_mapa.dameColor() );
	m_ventana.limpia();	
	m_baldosas.tomaPosicion( m_pantalla_x, m_pantalla_y );
	for( register int y = 0; y < m_pantalla_alto ; ++y ) {
	for( register int x = 0; x < m_pantalla_ancho; ++x ) {
		m_baldosas.pinta( m_mapa.dame( 0, x + posx, y + posy ), x, y );
	}
	}
	m_baldosas.actualizaAnimacion();
}

void Dibuja::final() {
	int ingles = ( m_ingles ) ? m_pantalla_ancho : 0;
	m_ventana.tomaColor( m_mapa.dameColor() );
	m_ventana.limpia();	
	m_baldosas.tomaPosicion( m_pantalla_x, m_pantalla_y );
	for( int y = 0; y < m_pantalla_alto ; ++y ) {
	for( int x = 0; x < m_pantalla_ancho; ++x ) {
		Uint16 baldosa = m_mapa.dame( 0, x + ingles, y + m_posy );
		m_baldosas.pinta( baldosa, x, y );
	}
	}
}

void Dibuja::consola( const Monje& monje, int nivel, int pantalla ) {
	int ingles = ( m_ingles ) ? m_consola_ancho : 0;
	m_baldosas.tomaPosicion( m_consola_x, m_consola_y );
	for( register int y = 0; y < m_consola_alto;  ++y ) {
	for( register int x = 0; x < m_consola_ancho; ++x ) {
		m_baldosas.pinta( m_mapa.dame( 0, x + ingles, y + m_consola_posy ), x, y );
	}
	}
	
	SDL_Rect origen  = { 64, 128, 8, 8 };
	SDL_Rect destino = { m_consola_x + 16 , m_consola_y + 40, 8, 8 };
	switch( monje.dameTipoDisparo() ) {
		case FLECHA: 	break;
		case FUEGO:		origen.y += 8; 	break;
		case AGUA: 		origen.x +=16;	break;
		case BOMERANG: 	origen.x +=16; origen.y += 8; break; 
	}
	for( register Uint8 i = 0; i < monje.damePotenciaDisparo(); i ++ ) {
		m_sprites.copia( &origen, &destino );
		destino.x += m_baldosa_ancho;
	}

	if( monje.dameVidas() < 10 ) {
		m_baldosas.pinta( monje.dameVidas() + 93, 8, 3 );
	} else {
		Uint8 decenas = ( monje.dameVidas() / 10 ) ;
		m_baldosas.pinta( decenas + 93, 8, 3 );
		Uint8 unidades = monje.dameVidas() - ( decenas * 10 );					
		m_baldosas.pinta( unidades + 93, 9, 3 );
	}

	m_baldosas.pinta( pantalla + 93, 29, 3 );
	m_baldosas.pinta( nivel    + 93, 27, 5 );
}

void Dibuja::escribe( const char* texto ) {
	m_baldosas.tomaPosicion( m_pantalla_x, m_pantalla_y );
	size_t len = SDL_strlen( texto );
	int y = ( m_pantalla_alto + m_consola_alto ) >> 1;
	int x = ( m_pantalla_ancho - len ) >> 1;
	for( register size_t i = 0; i < len; i++ ) {
		if( texto[i] == ' ' ) {
			++x; continue;
		}
		if(  texto[i] < 'A' ) m_baldosas.pinta( (Uint16)texto[i] + 45, x, y );
		else m_baldosas.pinta( (Uint16)texto[i] + 1, x, y );
		++x;
	}
}


