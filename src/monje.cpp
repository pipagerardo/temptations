#include "monje.h"

bool Monje::inicia() {
	
	m_vive = true;
	m_vidas = 9;
	m_tipo_movimiento = ANDA;
	
	m_direccion_inicial = FRENTE;
	m_x_inicial = 0.0f;
	m_y_inicial = 0.0f;
	m_gravedad_inicial   =  0.0f;
	m_movimiento_inicial =  0.0f;

	m_direccion  = DERECHA;	
	m_x = 0.0f;
	m_y = 0.0f;	
	m_gravedad =  0.0f;
	m_movimiento =  0.0f;

	m_inc_gravedad 	 =  0.16f;
	m_salto  		 =  -2.9f;	
	m_inc_movimiento =  1.0f;
	m_inercia  		 =  0.12f; // 0.1f;
	
	m_ancho  = 16;
	m_alto   = 16;
	m_ancho2 = m_ancho >> 1;
	m_alto2  = m_alto >> 1; 
	
	m_tipo_disparo	   = FLECHA;
	m_potencia_disparo = 1;
	m_cuenta    = 0;
	m_animacion = 0; 
	return true;	
}

void Monje::quita() {

}

void Monje::anda() {
	m_tipo_movimiento = ANDA;
	m_direccion  = FRENTE;
}
void Monje::vuela() {
	m_tipo_movimiento = VUELA;
	if( m_direccion == FRENTE ) m_direccion = DERECHA;
	m_sonidos.andaPara();
}

void Monje::nada() {
	m_tipo_movimiento = NADA;
	m_direccion = DERECHA;
	m_sonidos.andaPara();
}

Uint8 Monje::dameVidas() const { return m_vidas; }

// Posición relativa a la ventana:
int  Monje::dameXrel() { return (int)m_x - ( m_posx * m_baldosa_ancho ); }
int  Monje::dameYrel() { return (int)m_y - ( m_posy * m_baldosa_alto  ); }
void Monje::tomaXrel( int x ) { m_x = ( m_posx * m_baldosa_ancho ) + x; }
void Monje::tomaYrel( int y ) { m_y = ( m_posy * m_baldosa_alto  ) + y; }

// Posición absoluta en el mapa:
int  Monje::dameX() const { return (int)m_x; }
int  Monje::dameY() const { return (int)m_y;  }
void Monje::tomaX( int x ) { m_x = (float)x; }
void Monje::tomaY( int y ) { m_y = (float)y; }
void Monje::toma( float x,  float y, float gravedad, float movimiento ) {
	if( movimiento > 0.0f ) m_direccion = DERECHA;
	m_x = x;
	m_y = y;
	m_gravedad = gravedad;
	m_movimiento = movimiento;
	m_tipo_disparo = FLECHA;
	m_potencia_disparo = 1;
}

float Monje::dameXfloat() const { return m_x;  }
float Monje::dameYfloat() const { return m_y;  }
Uint8 Monje::dameAncho()  const { return m_ancho; }
Uint8 Monje::dameAlto()   const { return m_alto;  }


Enumera_Direccion    Monje::dameDireccion()       const { return m_direccion; }
Enumera_Tipo_Disparo Monje::dameTipoDisparo()     const { return m_tipo_disparo; }
Uint8				 Monje::damePotenciaDisparo() const { return m_potencia_disparo; }
	
void Monje::guardaPosicionInicial() {
	m_x_inicial = m_x;
	m_y_inicial = m_y;
	m_gravedad_inicial =  m_gravedad;
	m_movimiento_inicial = m_movimiento;
	m_direccion_inicial  = m_direccion;
}

void Monje::restauraPosicionInicial() {
	m_x = m_x_inicial;
	m_y = m_y_inicial;
	m_gravedad = m_gravedad_inicial;
	m_movimiento = m_movimiento_inicial;
	m_direccion  = m_direccion_inicial;
}

void Monje::frena() {
	m_movimiento /= 2.0f;
	if( m_gravedad > 0 ) m_gravedad *= 2.0f;
	else				 m_gravedad /= 2.0f;
	
}

void Monje::muere() {
	m_vive = false;
}

bool Monje::estaMuerto() const {
	return !m_vive;
}
bool Monje::estaVivo() const {
	return m_vive;
}
	
bool Monje::resucita() {
	if( m_vive ) return true;
	if( m_vidas == 0 ) return false;
	--m_vidas;
	restauraPosicionInicial();
	m_vive = true;
	return true;
}

void Monje::mueve() {
	if( !m_vive ) return;
	switch( m_tipo_movimiento ) {
		case ANDA:

			// Movimiento Vertical, Gravedad y Salto:
			m_gravedad += m_inc_gravedad;
			if( m_gravedad > -m_salto ) m_gravedad = -m_salto;
			if( m_gravedad > 0 ) { 
				if( colisionAbajo( m_gravedad ) ) {
					m_gravedad = 0;
				 	m_sonidos.saltaPara();
				}
			} else if ( m_gravedad < 0 ) {
				if( colisionArriba( -m_gravedad ) ) m_gravedad = m_inc_gravedad; 
				// if( m_gravedad == 0 ) m_y += m_gravedad;
			}           
	
			// Movimiento Horizontal, Inercia:
			if( m_movimiento > 0 ) {
				if( m_gravedad == 0 ) {
					m_movimiento -= m_inercia;
					if( m_movimiento < 0 ) m_movimiento = 0;
				}
				colisionDerecha( m_inc_movimiento );
			} else	if( m_movimiento < 0 ) {
				if( m_gravedad == 0 ) {
					m_movimiento += m_inercia;
					if( m_movimiento > 0 ) {
						m_movimiento = 0;
					} 
				}
				colisionIzquierda( m_inc_movimiento );
			} 
							
			// MOVIMIENTO MANUAL:
			// El salto activa el movimiento automático:
			if ( m_gravedad == 0 ) {
				if( m_control.salta() ) {	// Saltar:
					if( m_movimiento > 0 || m_control.derecha()   ) { m_direccion = DERECHA;   m_movimiento =  m_inc_movimiento; }
					if( m_movimiento < 0 || m_control.izquierda() ) { m_direccion = IZQUIERDA; m_movimiento = -m_inc_movimiento; }
					if( (m_direccion == DERECHA) && (m_direccion == IZQUIERDA) ) m_direccion = FRENTE;
					if( (m_direccion != DERECHA) && (m_direccion != IZQUIERDA) ) m_direccion = FRENTE;
					m_gravedad = m_salto;
					m_sonidos.salta( dameYrel() );
				}
			} else if( m_gravedad > 0 ) { // Al caerse:
				if( m_direccion == IZQUIERDA ) m_movimiento = -m_inc_movimiento;
				if( m_direccion == DERECHA   ) m_movimiento =  m_inc_movimiento;
				m_sonidos.salta( dameYrel() + 16 );
			}

			// Si estamos en el suelo y no hay inercia:
			if( ( m_gravedad == 0 ) && ( m_movimiento == 0 ) ) {
				if( m_control.derecha()  ) {
					m_direccion = DERECHA;
					colisionDerecha( m_inc_movimiento );
				}
				if( m_control.izquierda() ) { 
					m_direccion = IZQUIERDA;
					colisionIzquierda( m_inc_movimiento );
				} 
				if(  m_control.derecha() &&  m_control.izquierda() ) m_direccion = FRENTE;
				if( !m_control.derecha() && !m_control.izquierda() ) m_direccion = FRENTE;
			}
		
		break;
		case VUELA:
		case NADA:
			if( m_control.izquierda() )	{
				m_direccion = IZQUIERDA;
				colisionIzquierda( m_inc_movimiento );
			}
			if( m_control.derecha() ) {
				m_direccion = DERECHA;
				colisionDerecha( m_inc_movimiento );
			}
			if( m_control.arriba() ) { 
				colisionArriba( m_inc_movimiento );
			}
			if( m_control.abajo() )	{
				colisionAbajo( m_inc_movimiento );
			}
		break;
	}

}
	
void Monje::dibuja() {
	if( !m_vive ) {
		m_sonidos.andaPara();
		m_sonidos.saltaPara();
		return;
	}
	SDL_Rect origen  = { 32, 0, 16, 16 };
	SDL_Rect destino = {  0, 0, 16, 16 };
	destino.x = m_x - ( m_posx * m_baldosa_ancho );
	destino.y = m_y - ( m_posy * m_baldosa_alto  );
	switch( m_tipo_movimiento ) {
		case ANDA:
			origen.y = 0;
			if( m_gravedad == 0  ) {
				if     ( m_direccion == DERECHA   ) { origen.x =  0 + m_animacion; m_sonidos.anda(); }
				else if( m_direccion == IZQUIERDA ) { origen.x = 32 + m_animacion; m_sonidos.anda(); }
				else						        { origen.x = 64;               m_sonidos.andaPara(); }
			} else {
				m_sonidos.andaPara();
				if     ( m_direccion == DERECHA   ) { origen.x = 80;  }
				else if( m_direccion == IZQUIERDA ) { origen.x = 96;  }
				else 					            { origen.x = 112; }
			}
		break;
		case VUELA:
			origen.y = 16;
			if( m_direccion == DERECHA ) origen.x = 0  + m_animacion;
			else 						 origen.x = 32 + m_animacion;
		break;		                 
		case NADA:
			origen.y = 16;
			if( m_direccion == DERECHA ) origen.x = 64 + m_animacion;
			else 		                 origen.x = 96 + m_animacion;
		break;
	}	
	
	++m_cuenta;
	if( m_cuenta > 4 ) {
		m_cuenta = 0;
		if( m_animacion == 0 ) m_animacion = 16;
		else			       m_animacion = 0;
	}
	m_sprites.copia( &origen, &destino );
	
}

bool Monje::colisionAbajo( float rango ) {
	int x[3], y[3];
	x[0] = (int)m_x + m_ancho - 4;
	y[0] = (int)m_y + m_alto;
	x[1] = (int)m_x + m_ancho2;
	y[1] = (int)m_y + m_alto;
	x[2] = (int)m_x + 2;
	y[2] = (int)m_y + m_alto;
	int rangoi = (int)rango + 1;
	int px, py, tope = 3;
	Uint16 baldosa;
	for( register int i = 0; i < rangoi; i++ ) {
		for( register int j = 0; j < tope; j++ ) { 
			px = x[j] / m_baldosa_ancho;
			py = ( y[j] + i ) / m_baldosa_alto;
			baldosa = m_mapa.dame( 0, px, py );
			if( m_baldosas.mata( baldosa ) ) {
				muere();
				return true;
			}
			if( m_baldosas.arriba( baldosa ) ) {
				py = ( y[j] + i - 1 ) / m_baldosa_alto;
				baldosa = m_mapa.dame( 0, px, py );
				if( m_baldosas.arriba( baldosa ) ) continue;
				m_y += i;
				return true;
			}
			objetos( px, py, baldosa );
		}
	}
	m_y += rango;
	return false;
}

bool Monje::colisionArriba( float rango ) {	
	int x[3], y[3];
	x[0] = (int)m_x + m_ancho - 4;
	y[0] = (int)m_y;
	x[1] = (int)m_x + m_ancho2;
	y[1] = (int)m_y;
	x[2] = (int)m_x + 2;
	y[2] = (int)m_y;
	int rangoi = (int)rango + 1;
	int px, py, tope = 3;
	Uint16 baldosa;
	for( register int i = 0; i < rangoi; i++ ) {
		for( register int j = 0; j < tope; j++ ) { 
			px = x[j] / m_baldosa_ancho;
			py = ( y[j] - i ) / m_baldosa_alto;
			baldosa = m_mapa.dame( 0, px, py );
			if( m_baldosas.mata( baldosa ) ) {
				muere();
				return true;
			}
			if( m_baldosas.abajo( baldosa ) ) {
				m_y -= i;
				return true;
			}
			objetos( px, py, baldosa );
		}
	}
	m_y -= rango;
	return false;
}

bool Monje::colisionDerecha( float rango ) {
	int x[3], y[3];
	x[0] = (int)m_x + m_ancho - 2;
	y[0] = (int)m_y + m_alto  - 1;
	x[1] = (int)m_x + m_ancho - 2;
	y[1] = (int)m_y + m_alto2;
	x[2] = (int)m_x + m_ancho - 2;
	y[2] = (int)m_y + 1;
	int rangoi = (int)rango + 1;
	int px, py, tope = 3;
	Uint16 baldosa;
	for( register int i = 0; i < rangoi; i++ ) {
		for( register int j = 0; j < tope; j++ ) {
			px = ( x[j] + i ) / m_baldosa_ancho;
			py = y[j] / m_baldosa_alto;
			baldosa = m_mapa.dame( 0, px, py );
			if( m_baldosas.mata( baldosa ) ) {
				muere();
				return true;
			}
			if( m_baldosas.izquierda( baldosa ) ) {
				m_x+= i;
				return true;
			}
			objetos( px, py, baldosa );
		}
	}
	m_x += rango;
	return false;
}

bool Monje::colisionIzquierda( float rango ) {
	int x[3], y[3];
	x[0] = (int)m_x;
	y[0] = (int)m_y + m_alto - 1;
	x[1] = (int)m_x;
	y[1] = (int)m_y + m_alto2;
	x[2] = (int)m_x;
	y[2] = (int)m_y + 1;
	int rangoi = (int)rango + 1;
	int px, py, tope = 3;
	Uint16 baldosa;
	for( register int i = 0; i < rangoi; i++ ) {
		for( register int j = 0; j < tope; j++ ) {
			px = ( x[j] - i ) / m_baldosa_ancho;
			py = y[j] / m_baldosa_alto;
			baldosa = m_mapa.dame( 0, px, py );
			if( m_baldosas.mata( baldosa ) ) {
				muere();
				return true;
			}
			if( m_baldosas.derecha( baldosa ) ) {
				m_x -= i;
				return true;
			}
			objetos( px, py, baldosa );
		}
	}
	m_x -= rango;
	return false;	
}

void Monje::objetos( int px, int py, Uint16 baldosa ) {
	if( !m_baldosas.objeto( baldosa ) ) return;
	switch( baldosa ) {
		case 605: 	// 172: /* Vida */
			m_mapa.toma( 0, px, py, 0 );
			m_sonidos.cogerObjeto();
			++m_vidas; 
		break;
		case 606: 	// 173: /* incrementa el número de disparos */ 
			m_mapa.toma( 0, px, py, 0 );
			m_sonidos.cogerObjeto();
			if( m_potencia_disparo < DISPAROS_MAX ) ++m_potencia_disparo; 
		break; 
		case 607:	// 174: /* potencia el arma */ 
			m_mapa.toma( 0, px, py, 0 );
			m_sonidos.cogerObjeto();
			if( m_tipo_disparo != BOMERANG ) m_tipo_disparo = (Enumera_Tipo_Disparo)(m_tipo_disparo + 1); 
		break;
		case 608: 	// 171: /* Volare oh oh */
			m_mapa.toma( 0, px, py, 0 );
			m_sonidos.cogerObjeto();
			vuela();
		break;
	}
}

