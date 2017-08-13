#include "procesos.h"

// ----------------------------------------------------------------------------
// PROCESO:
// ----------------------------------------------------------------------------
Proceso::Proceso() {}
Proceso::~Proceso() {}
bool Proceso::colision( int px, int py ) {
	int xrel, yrel;
	switch( m_direccion ) {
		case DERECHA:
		case IZQUIERDA:
			xrel = (int)m_x - ( m_posx * m_baldosa_ancho );
			if( m_borde ) {
				if( xrel < m_pantalla_x  ) { 
					m_x = m_pantalla_x + ( m_posx * m_baldosa_ancho );
					m_direccion = DERECHA; 
					return true;
				}
				if( xrel + m_ancho > m_pantalla_w  ) { 
					m_x = m_pantalla_w - m_ancho + ( m_posx * m_baldosa_ancho );
					m_direccion = IZQUIERDA; 
					return true; 
				}
			} else {
				if( xrel < m_pantalla_x  ) px = (int)m_x + m_ancho;
				if( xrel + m_ancho < m_pantalla_x  ) m_x = m_pantalla_w + ( m_posx * m_baldosa_ancho );
				if( xrel           > m_pantalla_w  ) m_x = m_pantalla_x + ( m_posx * m_baldosa_ancho );
			}		
		break;
		case ARRIBA:
		case ABAJO:
			yrel = (int)m_y - ( m_posy * m_baldosa_alto  );
			if( m_borde ) {
				if( yrel < m_pantalla_y  ) { 
					m_y = m_pantalla_h + ( m_posy * m_baldosa_alto  ); 
					m_direccion = ABAJO;  
					return true;
				}
				if( yrel + m_alto > m_pantalla_h  ) { 
					m_y = m_pantalla_h - m_alto + ( m_posy * m_baldosa_alto  );
					m_direccion = ARRIBA;
					return true;
				}
			} else {	
				if( yrel + m_alto  < m_pantalla_y  ) m_y = m_pantalla_h + ( m_posy * m_baldosa_alto  );
				if( yrel           > m_pantalla_h  ) m_y = m_pantalla_y + ( m_posy * m_baldosa_alto  );
			}
		break;
		default:
		break;
	}
	if( m_movimiento != FRENTE ) {
		px /= m_baldosa_ancho;
		py /= m_baldosa_alto;
		Uint16 baldosa = m_mapa.dame( 0, px, py );
		switch( m_direccion ) {
			case FRENTE:
			break;
			case DERECHA:
				if( ( baldosa == 170 ) || m_baldosas.izquierda( baldosa ) ) { 
					m_direccion = IZQUIERDA; return true; 
				}
			break;
			case IZQUIERDA:    
				if( ( baldosa == 170 ) || m_baldosas.derecha( baldosa ) ) { 
					m_direccion = DERECHA; return true;
				}
			break;
			case ARRIBA:
				if( ( baldosa == 170 ) || m_baldosas.abajo( baldosa ) ) { 
					m_direccion = ABAJO; return true;
				}
			break;
			case ABAJO:
				if( ( baldosa == 170 ) || m_baldosas.arriba( baldosa ) ) { 
					m_direccion = ARRIBA; return true;
				}
			break;
			default:
			break;
		}
	}
	return false;
}
void Proceso::dibuja() {
	SDL_Rect origen = {  m_x_sprite, m_y_sprite, m_ancho, m_alto };
	SDL_Rect destino = { 0, 0, m_ancho, m_alto };
	destino.x = (int)m_x - ( m_posx * m_baldosa_ancho );
	destino.y = (int)m_y - ( m_posy * m_baldosa_ancho );
	if( m_delado && (m_direccion == DERECHA ) ) origen.x += ( m_ancho * m_num_sprites ) ;
	origen.x +=  m_animacion;
	++m_cuenta;
	if( m_cuenta > m_cuenta_num ) {
		m_cuenta = 0;
		m_animacion += m_ancho;
		if( m_animacion == ( m_ancho * m_num_sprites ) ) m_animacion = 0;
	}
	m_sprites.copia( &origen, &destino );			
}
Uint16 Proceso::dameVida() const {
	return m_vida;
}
void Proceso::restaVida( Uint16 cantidad ) {
	if( m_vida > 999 ) return; // Inmortal
	if( cantidad <= m_vida ) m_vida -= cantidad;
	else		 			 m_vida = 0;
}
// ----------------------------------------------------------------------------
// EXPLOSION:
// ----------------------------------------------------------------------------
Explosion::Explosion( float x, float y ) {
	m_movimiento  = 0.0f;		m_recuenta   = 0;
	m_x           = x;			m_y          = y;
	m_vida        = 1000; 		m_direccion  = FRENTE;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 0;			m_y_sprite   = 32;
	m_num_sprites = 8;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 2;
}
void Explosion::mueve() {
	if( m_animacion == 0 ) ++m_recuenta;
	if( m_recuenta > 4 ) m_vida = 0;
}
// ----------------------------------------------------------------------------
// BLOQUE:
// ----------------------------------------------------------------------------
Bloque::Bloque() { }
Bloque::Bloque( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 0.65f;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 0;			m_y_sprite   = 80;
	m_num_sprites = 1;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
}
void Bloque::mueve() {
	int px = 0, py = 0;
	switch( m_direccion ) {
		case FRENTE:
		break;
		case DERECHA:
			m_x += m_movimiento;
			px = (int)m_x + m_ancho;
			py = (int)m_y + (m_alto >> 1);
		break;
		case IZQUIERDA:
			m_x -= m_movimiento;
			px = (int)m_x;
			py = (int)m_y + (m_alto >> 1);
		break;
		case ARRIBA:
			m_y -= m_movimiento;
			px = (int)m_x + (m_ancho >> 1);
			py = (int)m_y;
		break;
		case ABAJO:
			m_y += m_movimiento;
			px = (int)m_x + (m_ancho >> 1);
			py = (int)m_y + m_alto;
		break;
		default:
		break;
	}
	colision( px, py );
}
// ----------------------------------------------------------------------------
// PAJARO_1:
// ----------------------------------------------------------------------------
Pajaro_1::Pajaro_1() {}
Pajaro_1::Pajaro_1( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	if( ( direccion == DERECHA ) || ( direccion == IZQUIERDA ) ) {
		m_movimiento  = 1.0f; m_inc = 0.2f; m_mul = 4.0f;
	} else {
		m_movimiento  = 0.0f; m_inc = 0.1f; m_mul = 8.0f;
	}
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = true;
	m_x_sprite    = 0;			m_y_sprite   = 48;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_y_copia     = m_y;		m_radianes    = 0.0f;
	if( direccion == ABAJO ) m_radianes = M_PI;
}
void Pajaro_1::mueve() {
	int px = 0, py = 0;
	m_y = m_y_copia + SDL_sinf( m_radianes ) * m_mul;
	m_radianes += m_inc;
	if( m_radianes > M_2PI ) m_radianes -= M_2PI;
	switch( m_direccion ) {
		case DERECHA:
			m_x += m_movimiento;
			px = (int)m_x + m_ancho;
			py = (int)m_y_copia + (m_alto >> 1);
			colision( px, py );
		break;
		case IZQUIERDA:
			m_x -= m_movimiento;
			px = (int)m_x;
			py = (int)m_y_copia + (m_alto >> 1);
			colision( px, py );
		break;
		case ARRIBA:
		case ABAJO:
			if( m_radianes > M_PI ) m_animacion = 0;
		break;
		default:
		break;
	}
}
// ----------------------------------------------------------------------------
// PAJARO_2
// ----------------------------------------------------------------------------
Pajaro_2::Pajaro_2( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	if( ( direccion == DERECHA ) || ( direccion == IZQUIERDA ) ) {
		m_movimiento  = 1.0f; m_inc = 0.2f; m_mul = 4.0f;
	} else {
		m_movimiento  = 0.0f; m_inc = 0.1f; m_mul = 8.0f;
	}
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = true;
	m_x_sprite    = 64;			m_y_sprite   = 48;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_y_copia     = m_y;		m_radianes   = 0.0f;
	if( direccion == ABAJO ) m_radianes = M_PI;
}
// ----------------------------------------------------------------------------
// COCO__1:
// ----------------------------------------------------------------------------
Coco_1::Coco_1( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 1.0f;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = false;		m_delado     = true;
	m_x_sprite    = 0;			m_y_sprite   = 64;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
}
// ----------------------------------------------------------------------------
// COCO_2:
// ----------------------------------------------------------------------------
Coco_2::Coco_2( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 0.8148733f; m_mueve      = true;
	m_x           = x;			m_y          = y;
	m_vida        = vida;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = true;
	m_x_sprite    = 64;			m_y_sprite   = 64;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_y_copia     = m_y;		m_mul = 24.0f;
	m_radianes    = 0.0f;		m_inc = 0.08f;
	if( ( direccion != DERECHA ) && ( direccion != IZQUIERDA ) ) m_direccion = IZQUIERDA;
	else m_direccion = direccion;
}
void Coco_2::mueve() {
	int px = 0, py = 0;
	m_y = m_y_copia - SDL_sinf( m_radianes ) * m_mul;
	m_radianes += m_inc;
	if( m_radianes > M_PI ) {
		m_radianes = 0;
		m_y = m_y_copia;
		m_mueve = !m_mueve;
	}
	if( !m_mueve ) {
		switch( m_direccion ) {	
			case DERECHA:
				m_x += m_movimiento;
				px = (int)m_x + m_ancho;
				py = (int)m_y + (m_alto >> 1);
			break;
			case IZQUIERDA:
				m_x -= m_movimiento;
				px = (int)m_x;
				py = (int)m_y + (m_alto >> 1);
			break;
			default:
			break;
		}
		colision( px, py );
	}
}
// ----------------------------------------------------------------------------
// ESPIRITU:
// ----------------------------------------------------------------------------
Espiritu::Espiritu( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 0.8f; // 0.65f;
	m_x           = x;			m_y          = y;
	m_vida        = vida;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 16;			m_y_sprite   = 80;
	m_num_sprites = 3;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 2;
	m_y_copia     = m_y;		m_recuenta   = 0;
	m_radianes    = 0.0f;		m_inc = 0.2f;
	m_mul         = 7.0f;
	if( ( direccion != DERECHA ) && ( direccion != IZQUIERDA ) ) m_direccion = IZQUIERDA;
	else m_direccion = direccion;
}
void Espiritu::mueve() {
	int px = 0, py = 0;
	m_y = m_y_copia + SDL_sinf( m_radianes ) * m_mul;
	m_radianes += m_inc;
	if( m_radianes > M_2PI ) m_radianes -= M_2PI;
	switch( m_direccion ) {
		case DERECHA:
			if( m_recuenta < 7 ) m_x += m_movimiento;
			else ++m_recuenta;
			px = (int)m_x + m_ancho;
		break;
		case IZQUIERDA:
			if( m_recuenta < 7 ) m_x -= m_movimiento;
			else ++m_recuenta;
			px = (int)m_x;
		break;
		default:
		break;
	}
	py = (int)m_y + (m_alto >> 1);
	if( m_recuenta > 57 ) m_recuenta = 0;
	if( colision( px, py ) ) ++m_recuenta ;
}
// ----------------------------------------------------------------------------
// PIRO:
// ----------------------------------------------------------------------------
Piro::Piro( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 0.85f;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 64;			m_y_sprite   = 80;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_direccion   = IZQUIERDA;  m_recuenta = 0;
	m_x_copia     = m_x;
}
void Piro::mueve() {
	int px = 0, py = 0;
	switch( m_direccion ) {	
		case DERECHA:
			if( m_recuenta == 0 ) {
				m_x_sprite   = 96;
				m_animacion  = 0;
				m_cuenta     = 0;
				m_cuenta_num = 8;
			}	
			++m_recuenta;
			if( m_recuenta > 64 ) {
				m_recuenta = 0;
				m_ancho    = 16;
				m_direccion = IZQUIERDA;
			} else if( m_recuenta >= 16 ) {
				m_x = m_x_copia + 8;
				m_x_sprite    = 64;
				m_animacion   = 0;
				m_cuenta      = 0;			
				m_cuenta_num  = 4;
				m_ancho       = 8;
			}
		break;
		case IZQUIERDA:
			m_x -= m_movimiento;
			px = (int)m_x;
			py = (int)m_y + (m_alto >> 1);
			colision( px, py );
		break;
		default:
		break;
	}
}
// ----------------------------------------------------------------------------
// Viscoso :
// ----------------------------------------------------------------------------
Viscoso::Viscoso( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 1.0f;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = true;
	m_x_sprite    = 0;			m_y_sprite   = 96;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 2;
}
// ----------------------------------------------------------------------------
// MUELLE:
// ----------------------------------------------------------------------------
Muelle::Muelle( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 0.9; 		m_mueve      = true;
	m_x           = x;			m_y          = y;
	m_vida        = vida;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 64;			m_y_sprite   = 96;
	m_num_sprites = 1;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_y_copia     = m_y;		m_mul = 26.0f;
	m_radianes    = 0.0f;		m_inc = 0.07f; 	 
	m_recuenta    = 0;
	if( ( direccion != DERECHA ) && ( direccion != IZQUIERDA ) ) m_direccion = IZQUIERDA;
	else m_direccion = direccion;
	// m_inc = 0.08f;
	// m_movimiento  = 1.0;
}
void Muelle::mueve() {
	int px = 0, py = 0, xrel = 0;
	m_y = m_y_copia - SDL_sinf( m_radianes ) * m_mul;
	m_radianes += m_inc;
	if( m_radianes > M_PI ) {
		m_x_sprite = 64 + 16;
		m_radianes = 0;
		m_y = m_y_copia;
		if( m_recuenta > 2 ) {
			m_mueve = !m_mueve;
			m_recuenta = 0;
		}
	}
	if( m_mueve ) {
		switch( m_direccion ) {	
			case DERECHA:
				m_x += m_movimiento;
				xrel = (int)m_x - ( m_posx * m_baldosa_ancho );
				if( xrel + m_ancho > m_pantalla_w  ) { 
					m_x = m_pantalla_w - m_ancho + ( m_posx * m_baldosa_ancho ); 
					m_direccion = IZQUIERDA; 
					return; 
				}
				px = (int)m_x + m_ancho;
				py = (int)m_y + (m_alto >> 1);
			break;
			case IZQUIERDA:
				m_x -= m_movimiento;
				xrel = (int)m_x - ( m_posx * m_baldosa_ancho );
				if( xrel < m_pantalla_x  ) { 
					m_x = m_pantalla_x + ( m_posx * m_baldosa_ancho ); 
					m_direccion = DERECHA; 
					return; 
				}
				px = (int)m_x;
				py = (int)m_y + (m_alto >> 1);
			break;
			default:
			break;
		}
		if( colision( px, py ) ) ++m_recuenta;
	} else {
		if( m_radianes == 0 ) {
			m_recuenta += 2;
		}
	}
}
void Muelle::dibuja() {
	if( m_x_sprite  > 64 ) ++m_cuenta;
	if( m_cuenta > m_cuenta_num ) {
		m_cuenta = 0;
		m_x_sprite = 64;
	}
	SDL_Rect origen = {  m_x_sprite, m_y_sprite, m_ancho, m_alto };
	SDL_Rect destino = { 0, 0, m_ancho, m_alto };
	destino.x = (int)m_x - ( m_posx * m_baldosa_ancho );
	destino.y = (int)m_y - ( m_posy * m_baldosa_ancho );
	if( !m_mueve ) {
		if( m_animacion < 4 ) 	destino.x += 1;
		else 					destino.x -= 1;
		++m_animacion; if( m_animacion > 8 ) m_animacion = 0;
	}
	m_sprites.copia( &origen, &destino );			
}
// ----------------------------------------------------------------------------
// DEMONIO_1		VERDE	Igual que pájaro 1
// ----------------------------------------------------------------------------
Demonio_1::Demonio_1( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	if( ( direccion == DERECHA ) || ( direccion == IZQUIERDA ) ) {
		m_movimiento  = 0.8f; m_inc = 0.2f; m_mul = 2.0f; m_y_sprite   = 112;
	} else {
		m_movimiento  = 0.0f; m_inc = 0.1f; m_mul = 8.0f; m_y_sprite   = 144;
	}
	m_x_sprite    = 0;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = direccion;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = true;			
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_y_copia     = m_y;		m_radianes   = 0.0f;
	if( direccion == ABAJO ) m_radianes = M_PI;
}
// ----------------------------------------------------------------------------
// DEMONIO_2		MAGENTA Se mueve en circulos.
// ----------------------------------------------------------------------------
Demonio_2::Demonio_2( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_x           = x;			m_y          = y;
	m_vida        = vida;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = true;
	m_x_sprite    = 64;			m_y_sprite   = 112;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_radianes    = 0.0f;		m_movimiento = 0.4f;
	m_inc 		  = 0.1f;		m_mul        = 8.0f;
	m_x_copia     = m_x;		m_y_copia    = m_y;
	if( ( direccion != DERECHA ) && ( direccion != IZQUIERDA ) ) m_direccion = IZQUIERDA;
	else m_direccion = direccion;
}
void Demonio_2::mueve() {
	int px = 0, py = 0;
	m_y = m_y_copia + SDL_sinf( m_radianes ) * m_mul;
	m_x = SDL_cosf( m_radianes ) * m_mul;
	m_radianes += m_inc;
	if( m_radianes > M_2PI ) m_radianes -= M_2PI;
	switch( m_direccion ) {
		case DERECHA:
			m_x_copia += m_movimiento;
			m_x += m_x_copia;
			px = (int)m_x + m_ancho;
		break;
		case IZQUIERDA:
			m_x_copia -= m_movimiento;
			m_x += m_x_copia;
			px = (int)m_x;
		break;
		default:
		break;
	}
	py = (int)m_y + (m_alto >> 1);
	colision( px, py ) ;
}
// ----------------------------------------------------------------------------
// ZIG_ZAG          EXPLOSION_MOVIL En zig-zag
// ----------------------------------------------------------------------------
Zig_Zag::Zig_Zag( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
#ifdef PGL_ANDROID
	m_movimiento  = 0.8f;		m_mov_verti  = -0.8f;
#else
	m_movimiento  = 1.0f;		m_mov_verti  = -1.0f;
#endif
	m_x           = x;			m_y          = y;
	m_vida        = vida;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 0;			m_y_sprite   = 32;
	m_num_sprites = 8;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 2;
	m_y_copia     = m_y;		m_y_tope     = m_y_copia - 32;
	if( ( direccion != DERECHA ) && ( direccion != IZQUIERDA ) ) m_direccion = IZQUIERDA;
	else m_direccion = direccion;
	// Calculamos el tope:
	int py = 0; Uint16 baldosa = 0;
	int px = ( (int)m_x + ( m_ancho >> 1 ) ) / m_baldosa_ancho;
	while( m_y_tope > 0.0f ) {
		py = (int)m_y_tope / m_baldosa_alto;
		baldosa = m_mapa.dame( 0, px, py );
		if( ( baldosa == 170 ) || m_baldosas.abajo( baldosa ) ) break;
		m_y_tope -= 1.0f;
	}
}
void Zig_Zag::mueve() {
	int px = 0, py = 0;
	m_y += m_mov_verti;
	if( m_y < m_y_tope ) {
		m_y = m_y_tope;
		m_mov_verti = -m_mov_verti;
	} else if ( m_y > m_y_copia ) {
		m_y = m_y_copia;
		m_mov_verti = -m_mov_verti;
	}
	switch( m_direccion ) {
		case DERECHA:
			m_x += m_movimiento;
			px = (int)m_x + m_ancho;
		break;
		case IZQUIERDA:
			m_x -= m_movimiento;
			px = (int)m_x;
		break;
		default:
		break;
	}
	py = (int)m_y + (m_alto >> 1);
	colision( px, py );
}
// ----------------------------------------------------------------------------
// ACUOSO			Izquierda-Derecha con pequeños saltitos
// ----------------------------------------------------------------------------
Acuoso::Acuoso( float x, float y, Uint16 vida, Enumera_Direccion direccion ) {
	m_movimiento  = 1.0f;		m_recuenta   = 0;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 0;			m_y_sprite   = 128;
	m_num_sprites = 3;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
	m_y_copia     = m_y;		m_mul = 3.0f;
	m_radianes    = 0.0f;		m_inc = 0.2f; 
	if( ( direccion != DERECHA ) && ( direccion != IZQUIERDA ) ) m_direccion = IZQUIERDA;
	else m_direccion = direccion;
}
void Acuoso::mueve() {
	int px = 0, py = 0;
	if( m_recuenta > 20 ) {
		m_y = m_y_copia - SDL_sinf( m_radianes ) * m_mul;
		m_radianes += m_inc;
		if( m_radianes > M_PI ) {
			m_radianes = 0;
			m_y = m_y_copia;
			m_recuenta = 0;
		}
	} else ++m_recuenta;
	switch( m_direccion ) {
		case DERECHA:
			m_x += m_movimiento;
			px = (int)m_x + m_ancho;
			py = (int)m_y + (m_alto >> 1);
		break;
		case IZQUIERDA:
			m_x -= m_movimiento;
			px = (int)m_x;
			py = (int)m_y + (m_alto >> 1);
		break;
		default:
		break;
	}
	colision( px, py );
}
// ----------------------------------------------------------------------------
// BURBUJA:
// ----------------------------------------------------------------------------
Burbuja::Burbuja( float x, float y, Uint16 vida ) {
	m_movimiento  = 1.0f;		m_y_copia    = y;
	m_x           = x;			m_y          = y;
	m_vida        = vida;		m_direccion  = ARRIBA;
	m_ancho       = 16;			m_alto       = 16;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 96;			m_y_sprite   = 96;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 2;
}
void Burbuja::mueve() {
	int px = 0, py = 0;
	switch( m_direccion ) {
		case ARRIBA:
			m_y -= m_movimiento;
		break;
		case ABAJO:
			m_y = m_y_copia;
			m_direccion = ARRIBA;
		break;
		default:
		break;
	}
	px = (int)m_x + (m_ancho >> 1);
	py = (int)m_y;
	colision( px, py );
}
// ----------------------------------------------------------------------------
// BOLA_FUEGO:
// ----------------------------------------------------------------------------
Bola_Fuego::Bola_Fuego( float x, float y, float xi, float yi ) {
	m_movimiento  = xi;			m_vertical   = yi;
	m_x           = x;			m_y          = y;
	m_vida        = 1000; 		m_direccion  = IZQUIERDA;
	m_ancho       = 8;			m_alto       = 8;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 48;			m_y_sprite   = 128;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 8;
	m_xmin = ( m_posx * m_baldosa_ancho );
	m_xmax = m_xmin + m_pantalla_w;
	m_ymin = ( m_posy * m_baldosa_alto );
	m_ymax = m_ymin + m_pantalla_h;
}
void Bola_Fuego::mueve() {
	m_x += m_movimiento;
	m_y += m_vertical;
	if( m_x < m_xmin  ) m_vida = 0;
	if( m_x > m_xmax  ) m_vida = 0;
	if( m_y < m_ymin  ) m_vida = 0;
	if( m_y > m_ymax  ) m_vida = 0;
}
// ----------------------------------------------------------------------------
// SATANAS:
// ----------------------------------------------------------------------------
Satanas::Satanas( float x, float y, Uint16 vida, Monje* monje, Procesos *procesos ) {
	m_movimiento  = 0.0f;		m_y_copia    = y;
	m_x           = x;			m_y          = y;
	m_vida        = vida; 		m_direccion  = ARRIBA;
	m_ancho       = 24;			m_alto       = 32;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 80;			m_y_sprite   = 144;
	m_num_sprites = 2;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 8;	
	m_monje		  = monje;		m_procesos	 = procesos;
	m_recuenta    = 0;			m_noni_y     = m_monje->dameYfloat();
}
void Satanas::mueve() {
	if( m_vida < 8 ) {
		m_procesos->muerteSatanas( m_x, m_y );
		m_vida = 0;
		return;
	}

	if( ( m_x - m_monje->dameXfloat() ) < 60.0f ) {
		m_y = ( m_y * 11.0f + m_monje->dameYfloat() - 6.0f ) / 12.0f;
	} else {
		if( m_monje->dameYfloat() - ( m_posy * m_baldosa_alto ) > 45.0f )
			m_y = ( ( (m_y * 11.0f) + m_monje->dameYfloat() - 32.0f ) / 12.0f );
		else
			m_y = ( ( (m_y * 11.0f) + m_monje->dameYfloat() + 32.0f ) / 12.0f );
	}
	if( m_y > m_y_copia ) m_y = m_y_copia;
	
	if( m_monje->estaVivo() ) {
		++m_recuenta;
	#ifdef PGL_ANDROID
		if( m_recuenta == 50 )  {
	#else
		if( m_recuenta == 40 )  {
	#endif
			m_noni_y = m_monje->dameYfloat();
	#ifdef PGL_ANDROID
		} else if ( m_recuenta > 100 ) {
	#else
		} else if ( m_recuenta > 80 ) {
	#endif
			float mod;
		#ifdef PGL_ANDROID
			if	( m_vida > 111 ) mod = 1.0f;	
			else 				 mod = 1.5f;
		#else
			if	( m_vida > 111 ) mod = 1.5f;	
			else 				 mod = 2.0f;
		#endif
			float y1 = m_y + 8.0f;
			float x2 = m_monje->dameXfloat() + 4.0f;
			float y2 = m_monje->dameYfloat() + 4.0f;
			y2 += ( m_monje->dameYfloat() - m_noni_y ) / mod;
			float angulo = PGL::Angulo2P( m_x, y1, x2, y2 );
			m_procesos->dispara( m_x, y1, SDL_cosf( angulo ) * mod, SDL_sinf( angulo ) * mod );
			m_procesos->dispara( m_x, y1, SDL_cosf( angulo + 0.2f ) * mod, SDL_sinf( angulo + 0.2f ) * mod );
			m_procesos->dispara( m_x, y1, SDL_cosf( angulo - 0.2f ) * mod, SDL_sinf( angulo - 0.2f ) * mod );
			m_recuenta = 0;
			m_noni_y = m_monje->dameYfloat();
		}
	}
}
// ----------------------------------------------------------------------------
// DERRUMBE: Sólo se mueve arriba-abajo y coloca un bloque.
// ----------------------------------------------------------------------------
Derrumbe::Derrumbe( float x ) {
	m_movimiento  = 2.0f;
	m_x           = x;			m_y          = 0;
	m_vida        = 1000; 		m_direccion  = ABAJO;
	m_ancho       = 24;			m_alto       = 8;
	m_borde       = true;		m_delado     = false;
	m_x_sprite    = 0;			m_y_sprite   = 160;
	m_num_sprites = 1;			m_animacion  = 0;
	m_cuenta      = 0;			m_cuenta_num = 4;
}
void Derrumbe::mueve() {
	bool colision = false;
	m_y += m_movimiento;

	int py = ( m_y + 8.0f ) / m_baldosa_alto;
		
	int	px = ( m_x + 4.0f ) / m_baldosa_ancho;
	Uint16 baldosa = m_mapa.dame( 0, px, py );
	if( m_baldosas.arriba( baldosa ) ) colision = true;

	px = ( m_x + 12.0f ) / m_baldosa_ancho;
	baldosa = m_mapa.dame( 0, px, py );
	if( m_baldosas.arriba( baldosa ) ) colision = true;

	px = ( m_x + 20.0f ) / m_baldosa_ancho;
	baldosa = m_mapa.dame( 0, px, py );
	if( m_baldosas.arriba( baldosa ) ) colision = true;
		
	if( colision ) {
		px = m_x / m_baldosa_ancho;
		py = ( m_y + 6.0f ) / m_baldosa_alto;
		m_vida = 0;
		if( m_y < 8 ) return;
		m_mapa.toma( 0, px, py, 238 );
		m_mapa.toma( 0, px + 1, py, 239 );
		m_mapa.toma( 0, px + 2, py, 240 );
		m_sonidos.disparoFin();
	}
}

// ----------------------------------------------------------------------------
// PROCESOS:
// ----------------------------------------------------------------------------
Procesos::Procesos() {
	m_cantidad = PROCESOS_MAX;
	m_proceso  = NULL;
}

Procesos::~Procesos() {
	quita();
}

bool Procesos::inicia() {
	if( m_proceso != NULL ) quita();
	m_proceso = new Proceso*[ m_cantidad ];
	if( m_proceso == NULL ) return false;
	#ifdef PGL_DEPURANDO
	else SDL_LOG( "m_proceso = new Proceso*[ %u ];", (unsigned int)m_cantidad );
	#endif
	for( size_t i = 0; i < m_cantidad; ++i ) {
		m_proceso[ i ] = NULL;
	}
	return true;
}

void Procesos::quita() {
	if( m_proceso != NULL ) {
		for( size_t i = 0; i < m_cantidad; ++i ) {
			if( m_proceso[i] != NULL ) {
				delete m_proceso[i];
				#ifdef PGL_DEPURANDO
				SDL_LOG( "delete m_proceso[ %u ];", (unsigned int)i );
				#endif
				m_proceso[i] = NULL;
			}
		}
		delete[] m_proceso;
		#ifdef PGL_DEPURANDO
		SDL_LOG( "delete[] m_proceso;" );
		#endif
		m_proceso = NULL;
	}
}

void Procesos::limpia() {
	// Borramos los procesos actuales:
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] != NULL ) {
			delete m_proceso[i];
			#ifdef PGL_DEPURANDO
			SDL_LOG( "delete m_proceso[ %u ];", (unsigned int)i );
			#endif
			m_proceso[i] = NULL;
		}
	}
}

// Coloca los enemigos en la pantalla correspondiente.
void Procesos::coloca( Monje* monje ) {
	
	// Borramos los procesos actuales:
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] != NULL ) {
			delete m_proceso[i];
			#ifdef PGL_DEPURANDO
			SDL_LOG( "delete m_proceso[ %u ];", (unsigned int)i );
			#endif
			m_proceso[i] = NULL;
		}
	}

	Enemigos_Datos datos;
	size_t i = 0;
	while( m_enemigos.dameDatos( datos ) ) {
		if( m_pantalla_numero == datos.pantalla ) {
			if( i >= m_cantidad ) break ;
			switch( datos.tipo ) {
				case PAJARO_1:
					m_proceso[i] = new Pajaro_1( datos.x, datos.y, datos.vida, datos.direccion ); 
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Pajaro_1()", (unsigned int)i );
					#endif
					++i;
				break;
				case PAJARO_2:
					m_proceso[i] = new Pajaro_2( datos.x, datos.y, datos.vida, datos.direccion ); 
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Pajaro_2()", (unsigned int)i );
					#endif
					++i;
				break;		
				case COCO_1:
					m_proceso[i] = new Coco_1( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Coco_1()", (unsigned int)i );
					#endif
					++i;
				break;
				case COCO_2:
					m_proceso[i] = new Coco_2( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Coco_2()", (unsigned int)i );
					#endif
					++i;
				break;
				case BLOQUE:
					m_proceso[i] = new Bloque( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Bloque()", (unsigned int)i );
					#endif
					++i;
				break;
				case ESPIRITU:
					m_proceso[i] = new Espiritu( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Espiritu()", (unsigned int)i );
					#endif
					++i;
				break;
				case PIRO:
					m_proceso[i] = new Piro( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Piro()", (unsigned int)i );
					#endif
					++i;
				break;
				case VISCOSO:
					m_proceso[i] = new Viscoso( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Viscoso()", (unsigned int)i );
					#endif
					++i;
				break;
				case MUELLE:
					m_proceso[i] = new Muelle( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Viscoso()", (unsigned int)i );
					#endif
					++i;
				break;
				case DEMONIO_1:
					m_proceso[i] = new Demonio_1( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Viscoso()", (unsigned int)i );
					#endif
					++i;
				break;
				case DEMONIO_2:
					m_proceso[i] = new Demonio_2( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Viscoso()", (unsigned int)i );
					#endif
					++i;
				break;
				case ZIG_ZAG:
					m_proceso[i] = new Zig_Zag( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Zig_Zag()", (unsigned int)i );
					#endif
					++i;
				break;
				case ACUOSO:
					m_proceso[i] = new Acuoso( datos.x, datos.y, datos.vida, datos.direccion );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Acuoso()", (unsigned int)i );
					#endif
					++i;
				break;
				case BURBUJA:
					m_proceso[i] = new Burbuja( datos.x, datos.y, datos.vida );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Burbuja()", (unsigned int)i );
					#endif
					++i;
				break;
				case SATANAS:
					m_proceso[i] = new Satanas( datos.x, datos.y, datos.vida, monje, this );
					#ifdef PGL_DEPURANDO
					SDL_LOG( "m_proceso[%u] = new Satanas()", (unsigned int)i );
					#endif
					++i;
				break;
				// ...		
				default: 
				break;
			}
		}
	}
	
}

// Mueve todos los procesos.
void Procesos::mueve() {
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] != NULL ) {
			m_proceso[i]->mueve();
		}
	}
}	

void Procesos::colisiones( Monje& monje, Disparos& disparos ) {
	SDL_Rect RectA, RectB;
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] == NULL )  continue;
		
		RectA.x = (int)m_proceso[i]->m_x;
		RectA.y = (int)m_proceso[i]->m_y;
		RectA.w = m_proceso[i]->m_ancho;
		RectA.h = m_proceso[i]->m_alto;
				
		// Colision Enemigos con Monje
		// Reducimos el área de colisión para facilitar un poco el juego.
		RectB.x = (int)monje.dameXfloat() + 2;
		RectB.y = (int)monje.dameYfloat() + 2;
		RectB.w = monje.dameAncho() - 4;
		RectB.h = monje.dameAlto() - 4;
		if( SDL_HasIntersection( &RectA, &RectB ) ) {
		#ifdef PGL_DEPURANDO
			if( m_mortal ) monje.muere();
		#else
			monje.muere();
		#endif
		}
		
		// Colision Enemigos - Disparos
		for( register size_t j = 0; j < disparos.m_cantidad; ++j ) {
			if( disparos.m_tiro[j].activo ) {
				// comprobar colisión.
				RectB.x = (int)disparos.m_tiro[j].x;
				RectB.y = (int)disparos.m_tiro[j].y;
				RectB.w = 8;
				RectB.h = 8;
				if( SDL_HasIntersection( &RectA, &RectB ) ) {
					m_proceso[i]->restaVida( (Uint16)disparos.m_tiro[j].tipo + 1 );
					if( m_proceso[i]->dameVida() == 0 ) {
						explosion( m_proceso[i]->m_x, m_proceso[i]->m_y );
						m_sonidos.muerte();
					}
					disparos.m_tiro[j].activo = false;
					m_sonidos.disparoFin();
				}
			}
		}
		
		// Si el proceso debe morir;
		if( m_proceso[i]->dameVida() == 0 ) {
			delete m_proceso[i];
			#ifdef PGL_DEPURANDO
			SDL_LOG( "delete m_proceso[ %u ];", (unsigned int)i );
			#endif
			m_proceso[i] = NULL;
		}
			
	}
}

void Procesos::explosion( float x, float y ) {
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] == NULL ) {
			m_proceso[i] = new Explosion( x, y );
			#ifdef PGL_DEPURANDO
			SDL_LOG( "m_proceso[%u] = new Explosion()", (unsigned int)i );
			#endif
			break;
		}
	}
}

void Procesos::muerteSatanas( float x, float y ) {
	limpia();
	explosion( x, y ); 		explosion( x + 16, y );
	explosion( x, y + 16 ); explosion( x + 16, y + 16 );
	explosion( x, y + 32 ); explosion( x + 16, y + 32 );
	m_sonidos.muerte();
	m_sonidos.muerte();
}

void Procesos::dispara( float x, float y, float xi, float yi ) {
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] == NULL ) {
			m_proceso[i] = new Bola_Fuego( x, y, xi, yi );
			#ifdef PGL_DEPURANDO
			SDL_LOG( "m_proceso[%u] = new Disparo()", (unsigned int)i );
			#endif
			break;
		}
	}
}

void Procesos::derrumbe( float x ) {
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] == NULL ) {
			m_proceso[i] = new Derrumbe( x );
			#ifdef PGL_DEPURANDO
			SDL_LOG( "m_proceso[%u] = new Disparo()", (unsigned int)i );
			#endif
			break;
		}
	}
}

// Dibuja todo los procesos.
void Procesos::dibuja() {
	for( register size_t i = 0; i < m_cantidad; ++i ) {
		if( m_proceso[i] != NULL ) m_proceso[i]->dibuja();
	}
}

