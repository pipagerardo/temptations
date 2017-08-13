#include "control.h"

Control::Control() {
	m_x = 0;
	m_y = 0;
	m_ancho = 0;
	m_alto  = 0;
	m_tecla.todas = 0;
	m_ventana  = NULL;
	m_joystick = NULL;
	m_gamecontroller = NULL;
#ifdef TACTIL
	m_tactil.id	= 0;
	m_tactil.alfa = 82;
#ifdef TACTIL_RATON
	m_tactil.raton = false;
#endif
	m_tactil.zoom = 1.0f;
	float pi4 = M_PI / 4.0f;
	m_radianes[0] = M_PI / 8.0f;
	m_radianes[1] = m_radianes[0] + pi4;
	m_radianes[2] = m_radianes[1] + pi4;
	m_radianes[3] = m_radianes[2] + pi4;
	m_radianes[4] = m_radianes[3] + pi4;
	m_radianes[5] = m_radianes[4] + pi4;
	m_radianes[6] = m_radianes[5] + pi4;
	m_radianes[7] = m_radianes[6] + pi4;
#endif
}

Control::~Control() {
	quita();
}

bool Control::inicia( int ancho, int alto ) {
	m_tecla.todas = 0;
	m_ancho = ancho;
	m_alto  = alto;
#ifdef TACTIL
	actualizaTactil();
#endif
	return true;
}
bool Control::inicia( const PGL::Ventana &ventana ) {
	m_ventana = (PGL::Ventana*)&ventana;
	m_tecla.todas = 0;
	m_ancho = ventana.dameAncho();
	m_alto  = ventana.dameAlto();
#ifdef TACTIL
	actualizaTactil();
#endif
	return true;
}

void Control::quita() {
	m_ventana = NULL;
	if( m_joystick != NULL ) SDL_JoystickClose(  m_joystick );
	m_joystick = NULL;
	if( m_gamecontroller != NULL ) SDL_GameControllerClose( m_gamecontroller );
	m_gamecontroller = NULL;
	m_x = 0;
	m_y = 0;
	m_ancho = 0;
	m_alto  = 0;
}
	
void Control::leeEstado() {
	
	m_tecla.todas = 0;
	Sint16 axisr = 8000;

	SDL_Event event;
	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
			case SDL_APP_TERMINATING:
    	//	case SDL_RENDER_TARGETS_RESET:
    	//	case SDL_RENDER_DEVICE_RESET:
			case SDL_QUIT:						m_tecla.quitar = true;	break;
			case SDL_APP_WILLENTERBACKGROUND:
			case SDL_APP_DIDENTERBACKGROUND:	m_tecla.pausa = true; 	break;
			case SDL_APP_WILLENTERFOREGROUND:
			case SDL_APP_DIDENTERFOREGROUND:	m_tecla.pausa = false;	break;
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ) {
					case SDLK_ESCAPE:	m_tecla.escapar  = true;	break;
					case SDLK_AC_BACK:	m_tecla.escapar  = true;	break;
					case SDLK_F1:		m_tecla.f1       = true;	break;
					case SDLK_F2:		m_tecla.f2       = true;	break;
					case SDLK_F3:		m_tecla.f3       = true;	break;
					case SDLK_F4:		m_tecla.f4       = true;	break;
					case SDLK_F5:		m_tecla.f5       = true;	break;
					case SDLK_F11:
					case SDLK_f:
						m_tecla.pantalla = true;	
						m_ventana->cambiaPantallaCompleta();
					break;
					case SDLK_g:		m_tecla.graficos = true;	break;
					case SDLK_i: 		m_tecla.info     = true;	break;
					case SDLK_MENU:
					case SDLK_m: 		m_tecla.musica   = true;	break;
					case SDLK_PAUSE:
					case SDLK_p: 		m_tecla.pausa    = true;	break;
					case SDLK_SELECT:
					case SDLK_l: 		m_tecla.lenguaje = true;	break;
				}
			break;
			case SDL_JOYDEVICEADDED:
				if(  m_joystick == NULL )  m_joystick = SDL_JoystickOpen( event.jdevice.which );
			break;
			case SDL_JOYDEVICEREMOVED:
				if(  m_joystick == SDL_JoystickFromInstanceID( event.jdevice.which ) ) {
					SDL_JoystickClose(  m_joystick ); 
					 m_joystick = NULL;
				}
			break;
			case SDL_CONTROLLERDEVICEADDED:
				if( m_gamecontroller == NULL ) m_gamecontroller = SDL_GameControllerOpen( event.cdevice.which );
			break;
			case SDL_CONTROLLERDEVICEREMOVED:
				if( m_gamecontroller == SDL_GameControllerFromInstanceID( event.cdevice.which ) ) {
					SDL_GameControllerClose( m_gamecontroller );
					m_gamecontroller = NULL;
				}
			break;
		#ifdef TACTIL
			case SDL_FINGERMOTION:
			case SDL_FINGERDOWN:
				if( m_tactil.id == 0 ) {
					if ( SDL_GetNumTouchDevices() > 0 ) m_tactil.id = SDL_GetTouchDevice( 0 );
				}
			break;
		#endif
		#ifdef TACTIL_RATON
			case SDL_MOUSEBUTTONDOWN:
				m_tactil.raton = true;
			break;
		#endif
			case SDL_WINDOWEVENT:
				switch( event.window.event ) {
					case SDL_WINDOWEVENT_ENTER:
						m_tecla.activa = true;
					break;
					case SDL_WINDOWEVENT_LEAVE:
						m_tecla.desactiva = true;
					break;
					case SDL_WINDOWEVENT_MOVED:
						m_tecla.mueve = true;
						m_x = event.window.data1; 
						m_y = event.window.data2;
					break;
					case SDL_WINDOWEVENT_RESIZED:
						m_tecla.cambia = true;
						m_ancho = event.window.data1; 
						m_alto  = event.window.data2;
						m_ventana->cambia( m_ancho, m_alto );
					#ifdef TACTIL
						actualizaTactil();
					#endif
					break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						m_tecla.maximiza = true;
						m_ventana->maximiza();
					break;
					case SDL_WINDOWEVENT_MINIMIZED:
				    	m_tecla.minimiza = true;
						m_ventana->minimiza();  
					break;
        			case SDL_WINDOWEVENT_RESTORED:
        				m_tecla.restaura = true;
        				m_ventana->restaura();
        				m_ventana->cambia( m_ancho, m_alto );
        			break;
				}
			break;
		}
	}
				
	// ----------------------------------------------------------------
	// JOYSTICK:
	// ----------------------------------------------------------------

	if( m_gamecontroller != NULL ) {
		
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_A ) > 0 ) m_tecla.fuego_1 = true;
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_B ) > 0 ) m_tecla.fuego_2 = true;
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_X ) > 0 ) m_tecla.fuego_3 = true;
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_Y ) > 0 ) m_tecla.fuego_4 = true;
		
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_UP    ) > 0 ) m_tecla.arriba    = true;
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_DOWN  ) > 0 ) m_tecla.abajo     = true;
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_LEFT  ) > 0 ) m_tecla.izquierda = true;
		if( SDL_GameControllerGetButton( m_gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT ) > 0 ) m_tecla.derecha   = true;

		Sint16 axisvalue =  SDL_GameControllerGetAxis( m_gamecontroller, SDL_CONTROLLER_AXIS_LEFTX );
		if( axisvalue >  axisr  ) m_tecla.derecha   = true;
		if( axisvalue < -axisr  ) m_tecla.izquierda = true;

		axisvalue =  SDL_GameControllerGetAxis( m_gamecontroller, SDL_CONTROLLER_AXIS_LEFTY );
		if( axisvalue >  axisr  ) m_tecla.abajo  = true;
		if( axisvalue < -axisr  ) m_tecla.arriba = true;
		
	} else if( m_joystick != NULL ) {
		Uint8 hatvalue = SDL_JoystickGetHat( m_joystick, 0 );
		switch( hatvalue ) {
			case SDL_HAT_UP:		m_tecla.arriba    = true;		break;	
			case SDL_HAT_DOWN: 		m_tecla.abajo     = true;		break;
			case SDL_HAT_RIGHT:		m_tecla.derecha   = true;		break;
			case SDL_HAT_LEFT:		m_tecla.izquierda = true;		break;
			case SDL_HAT_RIGHTUP:	m_tecla.derecha   = true;	m_tecla.arriba = true; 	break;
			case SDL_HAT_RIGHTDOWN: m_tecla.derecha   = true;	m_tecla.abajo  = true;	break;
			case SDL_HAT_LEFTUP:	m_tecla.izquierda = true;	m_tecla.arriba = true;	break;
			case SDL_HAT_LEFTDOWN: 	m_tecla.izquierda = true;	m_tecla.abajo  = true;	break;
		}
	
		Sint16 axisvalue = SDL_JoystickGetAxis( m_joystick, 0 );
		if( axisvalue >  axisr  ) m_tecla.derecha   = true;
		if( axisvalue < -axisr  ) m_tecla.izquierda = true;

		axisvalue = SDL_JoystickGetAxis( m_joystick, 1 );
		if( axisvalue >  axisr  ) m_tecla.abajo  = true;
		if( axisvalue < -axisr  ) m_tecla.arriba = true;
		
		if( SDL_JoystickGetButton( m_joystick, 0 ) > 0 ) m_tecla.fuego_1 = true;
		if( SDL_JoystickGetButton( m_joystick, 1 ) > 0 ) m_tecla.fuego_2 = true;
		if( SDL_JoystickGetButton( m_joystick, 2 ) > 0 ) m_tecla.fuego_3 = true;
		if( SDL_JoystickGetButton( m_joystick, 3 ) > 0 ) m_tecla.fuego_4 = true;

	}

	const Uint8 *state = SDL_GetKeyboardState( NULL );
	if( state[SDL_SCANCODE_RIGHT] ) m_tecla.derecha   = true;
	if( state[SDL_SCANCODE_LEFT]  ) m_tecla.izquierda = true;
	if( state[SDL_SCANCODE_DOWN]  ) m_tecla.abajo     = true;
	if( state[SDL_SCANCODE_UP]    ) m_tecla.arriba    = true;
	if( state[SDL_SCANCODE_SPACE] ) m_tecla.fuego_1   = true;
	if( state[SDL_SCANCODE_RCTRL] ) m_tecla.fuego_1   = true;
	if( state[SDL_SCANCODE_RSHIFT]) m_tecla.fuego_2   = true;
	if( state[SDL_SCANCODE_LCTRL] ) m_tecla.fuego_3   = true;
	if( state[SDL_SCANCODE_LSHIFT]) m_tecla.fuego_4   = true;
	if( state[SDL_SCANCODE_D]  ) 	m_tecla.derecha   = true;
	if( state[SDL_SCANCODE_A]  ) 	m_tecla.izquierda = true;
	if( state[SDL_SCANCODE_S]  ) 	m_tecla.abajo     = true;
	if( state[SDL_SCANCODE_W]  ) 	m_tecla.arriba    = true;

	// ----------------------------------------------------------------
	// TÁCTIL:
	// ----------------------------------------------------------------
#ifdef TACTIL
	if( m_tactil.id != 0 ) {
		m_tactil.x = ((m_tactil.x * 5) + (m_tactil.xpad * 2)) / 7;
		m_tactil.y = ((m_tactil.y * 5) + (m_tactil.ypad * 2)) / 7;
		for( int d = 0; d < SDL_GetNumTouchDevices(); d++ ) {
			m_tactil.id = SDL_GetTouchDevice( d );
			if( SDL_GetNumTouchFingers( m_tactil.id ) == 0 ) {
				m_tactil.x = m_tactil.xpad;
				m_tactil.y = m_tactil.ypad;
			} 
			for( int i = 0; i < SDL_GetNumTouchFingers( m_tactil.id ); i++ ) {
				SDL_Finger* finger = SDL_GetTouchFinger( m_tactil.id, i );
				if( finger != NULL ) leeTactil( finger->x, finger->y );
			}
		}
	}
#endif
	// ----------------------------------------------------------------	
#ifdef TACTIL_RATON
	if( m_tactil.raton ) {
		int x, y;
		if( SDL_GetMouseState( &x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT) ) { 
			leeTactil( x / (float)m_ancho, y / (float)m_alto );
		} 
	}
#endif
	// ----------------------------------------------------------------	
}

bool Control::arriba()  	const { return m_tecla.arriba;    }
bool Control::abajo()		const { return m_tecla.abajo;     }
bool Control::derecha()		const { return m_tecla.derecha;   }
bool Control::izquierda()	const { return m_tecla.izquierda; }
bool Control::fuego_1()		const { return m_tecla.fuego_1;   }
bool Control::fuego_2()		const { return m_tecla.fuego_2;   }
bool Control::fuego_3()		const { return m_tecla.fuego_3;   }
bool Control::fuego_4()		const { return m_tecla.fuego_4;   }
bool Control::quitar()		const { return m_tecla.quitar;    }
bool Control::escapar() 	const { return m_tecla.escapar;   }
bool Control::pantalla()	const { return m_tecla.pantalla;  }
bool Control::graficos()	const { return m_tecla.graficos;  }
bool Control::info()		const { return m_tecla.info;      }
bool Control::pausa()		const { return m_tecla.pausa;     }
bool Control::lenguaje()	const { return m_tecla.lenguaje;  }
bool Control::musica()      const { return m_tecla.musica;    }
bool Control::activa()		const { return m_tecla.activa;    }
bool Control::desactiva()	const { return m_tecla.desactiva; }
bool Control::mueve()       const { return m_tecla.mueve;     }
bool Control::cambia()      const { return m_tecla.cambia;    }
bool Control::maximiza()    const { return m_tecla.maximiza;  }
bool Control::minimiza()    const { return m_tecla.minimiza;  }
bool Control::restaura()    const { return m_tecla.restaura;  }
bool Control::f1()			const { return m_tecla.f1;        }
bool Control::f2()			const { return m_tecla.f2;        }
bool Control::f3()			const { return m_tecla.f3;        }
bool Control::f4()			const { return m_tecla.f4;        }
bool Control::f5()			const { return m_tecla.f5;        }
bool Control::salir()		const { return m_tecla.quitar|| m_tecla.escapar; }
bool Control::derecha_y_arriba()	const { return m_tecla.derecha   & m_tecla.arriba;  }
bool Control::derecha_y_abajo()		const { return m_tecla.derecha   & m_tecla.abajo;   }
bool Control::izquierda_y_arriba()	const { return m_tecla.izquierda & m_tecla.arriba;  }
bool Control::izquierda_y_abajo()	const { return m_tecla.izquierda & m_tecla.abajo;   }
bool Control::dispara()		const { return m_tecla.fuego_1 || m_tecla.fuego_3;  }
bool Control::salta()		const { 
#ifdef TACTIL
	if( hayTactil() ) return m_tecla.fuego_2 || m_tecla.fuego_4;
	else			  return m_tecla.arriba  || m_tecla.fuego_2 || m_tecla.fuego_4;
#else
	return m_tecla.arriba  || m_tecla.fuego_2 || m_tecla.fuego_4;
#endif
}

int   Control::dameX()     const { return m_x;     }
int   Control::dameY()     const { return m_y;     }
int   Control::dameAncho() const { return m_ancho; }
int   Control::dameAlto()  const { return m_alto;  }
Tecla Control::dameTecla() const { return m_tecla; }

#ifdef TACTIL
Tactil Control::dameTactil() const { return m_tactil; }
bool   Control::hayTactil()  const { 
#ifdef TACTIL_RATON
	return ( m_tactil.id != 0 || m_tactil.raton ) ? true : false;
#else
	return m_tactil.id != 0 ? true : false;
#endif
}
void   Control::quitaTactil() {
	 m_tactil.id = 0;
#ifdef TACTIL_RATON
	 m_tactil.raton = false;
#endif
}
void Control::agrandaTactil() {
	m_tactil.zoom += 0.01f;
	if( m_tactil.zoom > 2.0f ) m_tactil.zoom = 2.0f;
	actualizaTactil();
}
void Control::encogeTactil() {
	m_tactil.zoom -= 0.01f;
	if( m_tactil.zoom < 0.5f ) m_tactil.zoom = 0.5f;
	actualizaTactil();
}
void Control::transparentaTactil() {
	if( m_tactil.alfa > 24 ) m_tactil.alfa -= 4;
}
void Control::opacaTactil() {
	if( m_tactil.alfa < 252 ) m_tactil.alfa += 4;
}
void Control::tomaZoom( float zoom ) {
	m_tactil.zoom = zoom;
	actualizaTactil();
}
void Control::tomaAlfa( Uint8 alfa ) {
	m_tactil.alfa = alfa;
}
// PRIVADO:
void Control::actualizaTactil() {
	float margen = 0.025f * m_ancho;
	if( m_ancho > m_alto )	m_tactil.rmax = 0.1777f * m_tactil.zoom * m_alto;
	else					m_tactil.rmax = 0.1777f * m_tactil.zoom * m_ancho;
	m_tactil.rmin = m_tactil.rmax / 7; // 2.5f; // 3.1416f; // 3.0f;
	m_tactil.rmed = m_tactil.rmax / 2.5f;
	m_tactil.rbot = m_tactil.rmax / 1.4f; // 1.618f; // 1.4f;
	m_tactil.xpad = margen + m_tactil.rmax;
	m_tactil.ypad = m_alto - m_tactil.rmax;
	m_tactil.x    = m_tactil.xpad;
	m_tactil.y    = m_tactil.ypad;				
	m_tactil.xb1  = m_ancho - margen - m_tactil.rbot;
	m_tactil.xb2  = m_tactil.xb1 - (m_tactil.rbot * 2.222f ); // 2.2f);
	if( ( m_tactil.xpad + m_tactil.rmax + margen ) > ( m_tactil.xb2 - m_tactil.rbot ) ) {
		m_tactil.zoom -= 0.01f;
		actualizaTactil();
	}
}

void Control::leeTactil( float xf, float yf ) {
	int x = xf * m_ancho;
	int y = yf * m_alto;
	float rad; int x1, y1, d1;
	// PAD
	// rad = SDL_atan2( m_tactil.xpad - x, m_tactil.ypad - y );
	// if( rad < 0 ) rad += M_2PI;
	x1 = ( x > m_tactil.xpad ) ? x - m_tactil.xpad : m_tactil.xpad - x;
	y1 = ( y > m_tactil.ypad ) ? y - m_tactil.ypad : m_tactil.ypad - y;
	d1 = (int)SDL_sqrt( (x1 * x1) + ( y1 * y1 ) );
	// if( ( d1 < m_tactil.rmax ) && ( d1 > m_tactil.rmin ) ) {
	if( d1 < m_tactil.rmax ) {
		m_tactil.x = ( (m_tactil.x * 5 ) + (x * 2) ) / 7;
		m_tactil.y = ( (m_tactil.y * 5 ) + (y * 2) ) / 7;
		rad = SDL_atan2( m_tactil.xpad - m_tactil.x, m_tactil.ypad - m_tactil.y );
		if( rad < 0 ) rad += M_2PI;
		if     ( d1 < m_tactil.rmin ) { m_tecla.derecha = false; m_tecla.izquierda = false; m_tecla.arriba = false; m_tecla.abajo = false; }
		else if( ( rad < m_radianes[1] ) &&  ( rad > m_radianes[0] ) ) { m_tecla.arriba = true;  m_tecla.izquierda = true;  }
		else if( ( rad < m_radianes[2] ) &&  ( rad > m_radianes[1] ) ) { m_tecla.izquierda = true; }
		else if( ( rad < m_radianes[3] ) &&  ( rad > m_radianes[2] ) ) { m_tecla.izquierda = true; m_tecla.abajo = true; }
		else if( ( rad < m_radianes[4] ) &&  ( rad > m_radianes[3] ) ) { m_tecla.abajo = true; }
		else if( ( rad < m_radianes[5] ) &&  ( rad > m_radianes[4] ) ) { m_tecla.abajo = true; m_tecla.derecha = true;  }
		else if( ( rad < m_radianes[6] ) &&  ( rad > m_radianes[5] ) ) { m_tecla.derecha = true; }
		else if( ( rad < m_radianes[7] ) &&  ( rad > m_radianes[6] ) ) { m_tecla.derecha = true;  m_tecla.arriba = true; }
		else														   { m_tecla.arriba = true; }
	} 
	// BOTON 1
	x1 = ( x > m_tactil.xb1 ) ? x - m_tactil.xb1 : m_tactil.xb1 - x;
	// y1 = ( y > m_tactil.ypad ) ? y - m_tactil.ypad : m_tactil.ypad - y;
	d1 = (int)SDL_sqrt( (x1 * x1) + ( y1 * y1 ) );
	if( d1 < m_tactil.rbot ) m_tecla.fuego_1 = true;
	// BOTON 2
	x1 = ( x > m_tactil.xb2 ) ? x - m_tactil.xb2 : m_tactil.xb2 - x;
	// y1 = ( y > m_tactil.ypad ) ? y - m_tactil.ypad : m_tactil.ypad - y;
	d1 = (int)SDL_sqrt( (x1 * x1) + ( y1 * y1 ) );
	if( d1 < m_tactil.rbot ) 	m_tecla.fuego_2 = true;
	// BOTONES INVISIBLES SUPERIORES;
	if( yf < 0.125f ) {
		if      ( xf < 0.333f )	m_tecla.graficos = true;
		else if ( xf < 0.666  )	m_tecla.pausa	 = true;
		else					m_tecla.musica   = true;
		SDL_Delay( 200 );
	}
}
#endif

