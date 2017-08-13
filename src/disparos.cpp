#include "disparos.h"

Disparos::Disparos() {
	m_cantidad = DISPAROS_MAX;
	m_tiro = NULL;
	m_cuenta = 0;
	m_oculto = 0;
}

Disparos::~Disparos() {
	quita();
}

bool Disparos::inicia() {
	if( m_tiro != NULL ) quita();
	m_tiro = new Disparos_Tiro[ m_cantidad ];
	if( m_tiro == NULL ) return false;
#ifdef PGL_DEPURANDO
	else SDL_LOG( "m_tiro = new Disparos_Tiro[ %u ];", (unsigned int)m_cantidad );
#endif
	for( size_t i = 0; i < m_cantidad; i++ ) {
		m_tiro[i].activo = false;
		m_tiro[i].x = 0.0f;
		m_tiro[i].y = 0.0f;
		m_tiro[i].tipo = FLECHA;
		m_tiro[i].direccion = DERECHA;
	}
	m_cuenta = 0;
    m_oculto = 0;
	return true;
}

void Disparos::quita() {
	if( m_tiro != NULL ) {
		delete[] m_tiro;
	#ifdef PGL_DEPURANDO
		SDL_LOG( "delete[] m_tiro;" );
	#endif
		m_tiro = NULL;
	}
}

void Disparos::limpia() {
	m_cuenta = 0;
	m_oculto = 0;
	if( m_tiro == NULL ) return;
	for( size_t i = 0; i < m_cantidad; i++ ) m_tiro[i].activo = false;
}

void Disparos::dispara( const Monje& monje ) {
	if( monje.dameDireccion() == FRENTE ) return;
	if( m_cuenta != 0 ) {
		--m_cuenta; 
		return;
	}
	for( register size_t i = 0; i < monje.damePotenciaDisparo(); i++ ) {
		if( m_tiro[i].activo ) continue;
		m_sonidos.diparo();
		m_tiro[i].activo = true;
		if( monje.dameDireccion() == IZQUIERDA ) m_tiro[i].x = monje.dameXfloat();
		else m_tiro[i].x = monje.dameXfloat() + 8;
		m_tiro[i].y = monje.dameYfloat() + 8;
		m_tiro[i].tipo = monje.dameTipoDisparo() ;
		m_tiro[i].direccion = monje.dameDireccion();
		m_cuenta = 4;
		break;
	}	
}

void Disparos::mueve() {
	for( register size_t i = 0; i < m_cantidad; i++ ) {
		if( !m_tiro[i].activo ) continue;
		int px = (int)m_tiro[i].x;
		if( m_tiro[i].direccion == DERECHA ){
			m_tiro[i].x += 3;
			px += m_baldosa_ancho;
		} else {
			m_tiro[i].x -= 3;
		}
		px /= m_baldosa_ancho;
		int xrel = (int)m_tiro[i].x - ( m_posx * m_baldosa_ancho );
		if( ( xrel < m_pantalla_x ) ||  ( xrel > m_pantalla_w ) ) { 
			m_tiro[i].activo = false;
			continue;
		}
		
		int py = (int)(m_tiro[i].y + 1.0f) / m_baldosa_alto;
		Uint16 baldosa = m_mapa.dame( 0, px, py );
		// COLISION CON OBJETOS ESPECIALES:
		objetosEspeciales( i, baldosa, px, py );
		// COLISION CON PAREDES:		
		if( m_tiro[i].tipo != BOMERANG ) {
			if( m_baldosas.izquierda( baldosa ) || m_baldosas.derecha( baldosa ) ) {
				m_sonidos.disparoFin();
				m_tiro[i].activo = false;
				return;
			}
		}

		py = (int)(m_tiro[i].y + 7.0f) / m_baldosa_alto;
		baldosa = m_mapa.dame( 0, px, py );
		// COLISION CON OBJETOS ESPECIALES:
		objetosEspeciales( i, baldosa, px, py );
		// COLISION CON PAREDES:		
		if( m_tiro[i].tipo != BOMERANG ) {
			if( m_baldosas.izquierda( baldosa ) || m_baldosas.derecha( baldosa ) ) {
				m_sonidos.disparoFin();
				m_tiro[i].activo = false;
				return;
			}
		}		
		
	}
}

void Disparos::dibuja() {
	SDL_Rect origen  = { 64, 128, 8, 8 };
	SDL_Rect destino = { 0, 0, 8, 8 };
	for( register size_t i = 0; i < m_cantidad; i++ ) {
		if( !m_tiro[i].activo ) continue;
		destino.x = (int)m_tiro[i].x - ( m_posx * m_baldosa_ancho );
		destino.y = (int)m_tiro[i].y - ( m_posy * m_baldosa_ancho );
		switch( m_tiro[i].tipo  ) {
			case FLECHA: 	origen.x = 64; origen.y = 128; break;
			case FUEGO:		origen.x = 64; origen.y = 136; break;
			case AGUA: 		origen.x = 80; origen.y = 128; break;
			case BOMERANG: 	origen.x = 80; origen.y = 136; break; 
		}
		if( m_tiro[i].direccion == IZQUIERDA ) origen.x += 8;
		m_sprites.copia( &origen, &destino );				
	}
}

void Disparos::objetosEspeciales( size_t i, Uint16 baldosa, int px, int py ) {
	if( baldosa > 763 ) {
		++m_oculto;
		if( m_oculto < 5  ) {
			m_sonidos.disparoFin();
			m_tiro[i].activo = false;
			return;
		} else m_oculto = 0;
	}
	bool objeto = false;
	bool sonido = true;
	switch( baldosa ) {
		// LADRILLO
		case 253: m_mapa.toma( 0, px, py,   0 ); objeto = true; sonido = false; break;	// NADA
		// CALAVERAS
		case 635: m_mapa.toma( 0, px, py,   0 ); objeto = true; sonido = false; break;	// NADA
		case 636: m_mapa.toma( 0, px, py, 753 ); objeto = true; break;	// PINCHOS
		case 637: m_mapa.toma( 0, px, py, 605 ); objeto = true; break;	// VIDA
		case 638: m_mapa.toma( 0, px, py, 606 ); objeto = true; break;	// POTENCIA
		case 639: m_mapa.toma( 0, px, py, 607 ); objeto = true; break;	// ARMA
		case 640: m_mapa.toma( 0, px, py, 608 ); objeto = true; break;	// VOLAR
		// COFRES:
		case 668: m_mapa.toma( 0, px, py, 753 ); objeto = true; break;	// PINCHOS
		case 669: m_mapa.toma( 0, px, py, 605 ); objeto = true; break;	// VIDA
		case 670: m_mapa.toma( 0, px, py, 606 ); objeto = true; break;	// POTENCIA
		case 671: m_mapa.toma( 0, px, py, 607 ); objeto = true; break;	// ARMA
		case 672: m_mapa.toma( 0, px, py, 608 ); objeto = true; break;	// VOLAR
		// BURBUJAS
		case 700: m_mapa.toma( 0, px, py, 753 ); objeto = true; break;	// PINCHOS
		case 701: m_mapa.toma( 0, px, py, 605 ); objeto = true; break;	// VIDA
		case 702: m_mapa.toma( 0, px, py, 606 ); objeto = true; break;	// POTENCIA
		case 703: m_mapa.toma( 0, px, py, 607 ); objeto = true; break;	// ARMA
		case 704: m_mapa.toma( 0, px, py, 608 ); objeto = true; break;	// VOLAR	
		// HIERBA
		case 732: m_mapa.toma( 0, px, py, 753 ); objeto = true; break;	// PINCHOS
		case 733: m_mapa.toma( 0, px, py, 605 ); objeto = true; break;	// VIDA
		case 734: m_mapa.toma( 0, px, py, 606 ); objeto = true; break;	// POTENCIA
		case 735: m_mapa.toma( 0, px, py, 607 ); objeto = true; break;	// ARMA
		case 736: m_mapa.toma( 0, px, py, 608 ); objeto = true; break;	// VOLAR
		// OCULTOS:
		case 764: m_mapa.toma( 0, px, py, 753 ); objeto = true; break;	// PINCHOS
		case 765: m_mapa.toma( 0, px, py, 605 ); objeto = true; break;	// VIDA
		case 766: m_mapa.toma( 0, px, py, 606 ); objeto = true; break;	// POTENCIA
		case 767: m_mapa.toma( 0, px, py, 607 ); objeto = true; break;	// ARMA
		case 768: m_mapa.toma( 0, px, py, 608 ); objeto = true; break;	// VOLAR
	}
	if( objeto ) {
		if( sonido ) m_sonidos.nuevoObjeto();
		m_sonidos.disparoFin();
		m_tiro[i].activo = false;
	}
}
