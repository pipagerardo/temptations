#include "juego.h"

static const char *graficos_sprites[2] = { "graficos/sprites.png", "graficos/sprites_nenefranz.png" };
static const char *graficos_carga[2]   = { "graficos/carga.png",   "graficos/carga_mejorada.png"    };
static const char *texto_acabose[2]    = { "ACABOSE", "THE VERY ENDING" };
static const char *texto_pausa[2]      = { "PAUSA",    "PAUSE" };

Juego::Juego() {
	m_rutaConfiguracion = NULL;
	m_nivel = 0;
	m_posx  = 0;
	m_posy  = 0;
	m_unavez = true;
	m_cuenta_revive = 0;
	m_parpadeo = 0;
}

Juego::~Juego() {
	quita();
}

bool Juego::inicia() {
		
	if( !m_sistema.inicia( 
			SDL_INIT_VIDEO  | 
			SDL_INIT_EVENTS | 
			SDL_INIT_TIMER  | 
			SDL_INIT_JOYSTICK |
			SDL_INIT_GAMECONTROLLER
		) 
	) return false;
	
	if( !m_sistema.iniciaImagen( IMG_INIT_PNG ) ) return false;
	if( !m_sistema.iniciaSonido( MIX_INIT_OGG ) ) return false;
	
#ifdef PGL_ANDROID
	if( !m_ventana.inicia( 
		"Temptations Remake",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		0, 0,
		SDL_WINDOW_FULLSCREEN,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
		60 ) 
	) {
#else
	if( !m_ventana.inicia( 
		"Temptations Remake",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		768, 576,
		SDL_WINDOW_RESIZABLE,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
		60 )
	) {
#endif
		quita();
		return false;			
	}
#ifndef PGL_ANDROID
	m_ventana.icono( "graficos/icono16x16.png" );
#endif

	m_ventana.medidasLogicas( m_anchoLog, m_altoLog );
	m_control.inicia( m_ventana );
	m_ruta = "datos/";
	
#ifdef TACTIL
	if( !m_tactil.inicia( m_ventana, "graficos/touchpad.png" ) ) return false;
	m_tactil.tomaAlfa( 82 );
#endif
	
	// Leemos configuracion;
	rutaConfiguracion();
	leeConfiguracion();
	
	// Pantalla de Carga:
#ifndef PGL_DEPURANDO
	if( !pantallaCarga() ) return false;
#endif
	// Cargamos los Sonidos:
	if( !m_sonidos.inicia( m_graficos_mejorados ) ) return false;
	// Cargamos los Sprites:
	if( !m_sprites.inicia( m_ventana, graficos_sprites[ m_graficos_mejorados ] ) ) return false;
	// Iniciamos los disparos:
	if( !m_disparos.inicia() ) return false;
	// Iniciamos los procesos:
	if( !m_procesos.inicia() ) return false;
	// Pantalla de Inicio:
	if( !pantallaInicio() ) return false;
	// Comienza la partida:
	if( !iniciaNivel( 1 ) ) return false;

	return true;
}

void Juego::quita() {
	// Guardamos la configuración;
	guardaConfiguracion();
	if( m_rutaConfiguracion != NULL ) {
		delete[] m_rutaConfiguracion;
		m_rutaConfiguracion = NULL;
	}
	// LOCAL:	
	m_nivel = 0;
	m_posx  = 0;
	m_posy  = 0;
	// TMP:
	m_noni.quita();
	m_dibuja.quita(); // Quita Baldosas, Mapa y Enemigos.
	m_control.quita();
	m_sonidos.quita();
	m_musicas.quita();
	m_disparos.quita();
	m_procesos.quita();
	// PGL:
	m_ruta.quita();
	m_xml.quita();
#ifdef TACTIL
	m_tactil.quita();
#endif
	m_sprites.quita();
	m_ventana.quita();
	m_sistema.quita();
}

void Juego::rutaConfiguracion() {
	if( m_rutaConfiguracion != NULL ) return;
#ifdef _PORT_ANDROID_
	const char* ruta = SDL_AndroidGetExternalStoragePath();
	size_t longitud = SDL_strlen( ruta ) + 12;
	m_rutaConfiguracion = new char[ longitud ];
	if( m_rutaConfiguracion == NULL ) return;
	SDL_strlcpy( m_rutaConfiguracion, ruta, longitud );
	SDL_strlcat( m_rutaConfiguracion, "/config.dat", longitud );
#else
	m_rutaConfiguracion = new char[ 11 ];
	SDL_strlcpy( m_rutaConfiguracion, "config.dat", 11 );
#endif

#ifdef PGL_DEPURANDO
	SDL_LOG( "m_rutaConfiguracion=%s\n", m_rutaConfiguracion );
#endif
}

void Juego::leeConfiguracion() {
#ifdef PGL_ANDROID
	if( ( SDL_AndroidGetExternalStorageState() & SDL_ANDROID_EXTERNAL_STORAGE_READ  ) == 0 ) return;
#endif
	PGL::Archivo archivo;
	if( !archivo.inicia( m_rutaConfiguracion, "rb" ) ) {
		m_ventana.cambiaPantallaCompleta();
		return;
	}
	Configuracion configuracion;
	archivo.lee( (void *)&configuracion, sizeof( configuracion ), 1 );
	m_graficos_mejorados = configuracion.graficos_mejorados;
#ifndef PGL_ANDROID
	if( configuracion.pantalla_completa ) m_ventana.cambiaPantallaCompleta();
	if( !configuracion.pantalla_completa ) {
		m_ventana.cambia( configuracion.pantalla_ancho, configuracion.pantalla_alto );
		m_control.inicia( m_ventana.dameAncho(), m_ventana.dameAlto() );
	}
#endif
	m_musica = configuracion.musica;
	m_ingles = configuracion.ingles;
#ifdef TACTIL
	m_control.tomaZoom( configuracion.zoom );
	m_control.tomaAlfa( configuracion.alfa );
#endif
	archivo.quita();
	return;
	
}
void Juego::guardaConfiguracion() {
#ifdef PGL_ANDROID
	if( ( SDL_AndroidGetExternalStorageState() & SDL_ANDROID_EXTERNAL_STORAGE_WRITE ) == 0 ) return;
#endif
	PGL::Archivo archivo;
	if( !archivo.inicia( m_rutaConfiguracion, "wb" ) ) return;
	Configuracion configuracion;
	configuracion.graficos_mejorados = m_graficos_mejorados;
#ifndef PGL_ANDROID
	configuracion.pantalla_completa  = m_ventana.pantallaCompleta();
	if( configuracion.pantalla_completa ) {
		configuracion.pantalla_ancho = 256 * 3;
		configuracion.pantalla_alto  = 192 * 3;
	} else {
		configuracion.pantalla_ancho = m_ventana.dameAncho(); 
		configuracion.pantalla_alto  = m_ventana.dameAlto(); 	
	}
#endif
	configuracion.musica = m_musica;
	configuracion.ingles = m_ingles;
#ifdef TACTIL
	Tactil tactil = m_control.dameTactil();
	configuracion.zoom = tactil.zoom;
	configuracion.alfa = tactil.alfa;
#endif
	archivo.escribe( (void*)&configuracion, sizeof( configuracion ), 1 );
	archivo.quita();
	return;	
}
	
bool Juego::iniciaNivel( int numero ) {
	m_unavez = true;
	m_cuenta_revive = 0;
	m_nivel = numero;
	m_pantalla_numero = 1;
	if( m_nivel > ULTIMO_NIVEL ) m_nivel = ULTIMO_NIVEL;
	if( m_nivel < 0 ) m_nivel = 0;
	m_posx  = 0;
	m_posy  = 0;
	if( !m_dibuja.inicia( m_nivel ) ) {
		quita();
		return false;
	}
	// Cargamos las música.
	if( m_musica ) {
		if( !m_musicas.inicia( m_graficos_mejorados, m_nivel ) ) return false;
	}
	// Si pantalla de Inicio u Pantalla final reproduce música y sale:
	if( ( m_nivel == 0 ) || ( m_nivel == ULTIMO_NIVEL ) ) {
		m_musicas.reproduce();
		return true;
	}
	// Casos especiales:
	if( m_nivel == 1 ) m_noni.inicia();
	if( m_nivel == 4 ) m_noni.nada();
	else			   m_noni.anda();
	// Colocamos a noni:
	m_noni.toma(
		m_enemigos.dame_x(),
		m_enemigos.dame_y(),
		m_enemigos.dame_gra(),
		m_enemigos.dame_mov()
	);
	m_noni.guardaPosicionInicial();
	// Limpiamos los disparos:
	m_disparos.limpia();
	// Colocamos los procesos:
	m_procesos.coloca( &m_noni );
	PGL::Cadena nivel( 16 );
	nivel = ( m_ingles ) ? "LEVEL " : "NIVEL ";
	nivel << ( m_nivel + 48 );
	if( !escribe( nivel.dameCadena() ) ) return false;
	m_musicas.reproduce();
	return true;
}

bool Juego::leeControl() {
	m_control.leeEstado();
	
	if( m_control.quitar() ) return false; 
	if( m_control.salir()  ) {
		m_pausa = false;
		m_musicas.quita();
		m_sonidos.andaPara();
		if( !pantallaInicio() ) return false;
		if( !iniciaNivel( 1 ) ) return false;
	}
	if( m_control.maximiza() ) {
		m_pausa = false;
		m_sonidos.continua();
		m_musicas.continua();
	}
	if( m_control.minimiza() ) { 
		m_pausa = true;
		m_sonidos.pausa();
		m_musicas.pausa();
	}
	if( m_control.restaura() ) {
		m_pausa = false;
		m_sonidos.continua();
		m_musicas.continua();
	}
	if( m_control.graficos() ) {
		if( !m_dibuja.cambiaGraficos() ) return false;
		if( !m_sonidos.cambia() ) return false;
		if( m_musica ) {
			if( !m_musicas.cambia() ) return false;
		}
		if( m_pausa ) {
			m_sonidos.pausa();
			m_musicas.pausa();
		}
		SDL_Delay( 200 );
	}
	if( m_control.musica() && !m_pausa ) {
		if( m_musica ) {
			m_musica = false;
			m_musicas.quita();
		} else {
			m_musica = true;
			if( !m_musicas.inicia( m_graficos_mejorados, m_nivel ) ) return false;
			if( !m_pausa ) m_musicas.reproduce();
		}
		SDL_Delay( 200 );
	}
	if( m_control.lenguaje() ) { m_ingles = !m_ingles; SDL_Delay( 200 ); }
	if( m_control.pausa() )  {
		m_pausa = !m_pausa;
		if( m_pausa ) {
			m_sonidos.pausa();
			m_musicas.pausa();
		} else {
			m_sonidos.continua();
			m_musicas.continua();
		}
		SDL_Delay( 200 );
	}
	if( m_pausa ) {
		SDL_Delay( 40 );
		return true;
	}

	/* BORRAR LUEGO */
#ifdef PGL_DEPURANDO
	if( m_control.info() ) {
		m_mortal = !m_mortal;
		m_sonidos.cogerObjeto();
	}
	if( m_control.f1() ) {
	
		m_posx = m_mapa_ancho;
		m_noni.tomaX( ( m_mapa_ancho * 8 ) + 256 );

		/*
		m_sonidos.andaPara();
		if( !iniciaNivel( ++m_nivel ) ) return false;
		if( m_nivel == ULTIMO_NIVEL ) {
			if( !pantallaFinal()  ) return false;
			if( !pantallaInicio() ) return false;
			if( !iniciaNivel( 1 ) ) return false;
		}
		*/
	}
	if( m_control.f2() ) {
		static int tipo = 1;
		if     ( tipo == 0 ) { m_noni.anda();  tipo = 1; }
		else if( tipo == 1 ) { m_noni.vuela(); tipo = 2; }
		else                 { m_noni.nada();  tipo = 0; }
		SDL_Delay( 200 );
	}
#endif
	/* FIN DE BORRAR LUEGO */

	return true;
}

bool Juego::mueve() {
	if( m_pausa ) return true;
	/* MOVEMOS A NONI */
	m_noni.mueve();
	/* SI TOCA LOS BORDES DEL AREA */
	if( m_noni.dameXrel() > m_pantalla_w - m_noni.dameAncho() ) {
		// CAMBIAMOS DE PANTALLA:
		if( m_posx < m_mapa_ancho - m_pantalla_ancho  ) {
			m_sonidos.andaPara();
			m_sonidos.saltaPara();
			for( int i = 0; i < m_pantalla_ancho; ++ i ) {
				++m_posx; 
				m_dibuja.pantalla( m_posx, m_posy );
				m_dibuja.consola( m_noni, m_nivel, m_pantalla_numero  );
			#ifdef TACTIL
				dibujaTactil();
			#endif
				m_ventana.actualiza();
				m_ventana.retarda();
			}
			++m_pantalla_numero;
			m_disparos.limpia();
			m_procesos.coloca( &m_noni );
			m_noni.frena(); // <--- Frenamos un poco a noni.
		} else {
			// SIGUIENTE NIVEL...
			m_sonidos.andaPara();
			m_sonidos.saltaPara();
			m_musicas.quita();
			if( !iniciaNivel( ++m_nivel ) ) return false;
			// SI HEMOS LLEGADO AL ÚLTIMO NIVEL:
			if( m_nivel == ULTIMO_NIVEL ) {
				if( !pantallaFinal() ) 	return false;
				if( !pantallaInicio() ) return false;
				if( !iniciaNivel( 1 ) ) return false;
			}
		}
		m_noni.guardaPosicionInicial();
	}
	if( m_noni.dameXrel() < m_pantalla_x ) m_noni.tomaXrel( m_pantalla_x );
	if( m_noni.dameYrel() > m_pantalla_h - m_noni.dameAlto() ) {
		// CAMBIAMOS DE PANTALLA:
		m_sonidos.andaPara();
		m_sonidos.saltaPara();
		if( m_posy < m_mapa_alto - m_pantalla_alto ) {
			for( int i = 0; i < m_pantalla_alto; ++ i ) {
				++m_posy; 
				m_dibuja.pantalla( m_posx, m_posy );
				m_dibuja.consola( m_noni, m_nivel, m_pantalla_numero );
			#ifdef TACTIL
				dibujaTactil();
			#endif
				m_ventana.actualiza();
				m_ventana.retarda();
			}
			++m_pantalla_numero;
			m_disparos.limpia();
			m_procesos.coloca( &m_noni );
		} else m_noni.tomaYrel( m_pantalla_h );
		m_noni.guardaPosicionInicial();
	}
	if( m_noni.dameYrel() < m_pantalla_y ) m_noni.tomaYrel( m_pantalla_y );
	// DISPAROS:
	if( m_control.dispara() && m_noni.estaVivo() ) m_disparos.dispara( m_noni );
	m_disparos.mueve();
	// ENEMIGOS:
	m_procesos.mueve();
	// COLISIONES:
	m_procesos.colisiones( m_noni, m_disparos );
	/* MUERTE Y FIN DEL JUEGO */
	if( m_noni.estaMuerto() ) {
		if( m_unavez ) {
			m_procesos.explosion( m_noni.dameXfloat(), m_noni.dameYfloat() );
			m_sonidos.andaPara();
			m_sonidos.saltaPara();
			m_sonidos.muerte();
			m_unavez = false;
		}
		if( m_cuenta_revive > 50 ) {
			if( m_control.dispara() ) {
				m_cuenta_revive = 0;
				m_unavez = true;
				if( m_noni.dameVidas() == 0 ) {
					m_musicas.quita();
					m_sonidos.acabose();
					if( !escribe( texto_acabose[ m_ingles ] ) ) return false;
					// INICIO:
					if( !pantallaInicio() ) return false;
					if( !iniciaNivel( 1 ) ) return false;
				} else {
					m_noni.resucita();
					m_sonidos.nuevoObjeto();
				}
			} 
		} else ++m_cuenta_revive;
	}
	return true;
}

#ifdef TACTIL
void Juego::dibujaTactil() {
	if( !m_control.hayTactil() ) return;
	Tactil tactil = m_control.dameTactil();
	m_tactil.tomaAlfa( tactil.alfa );

	SDL_RenderSetViewport( m_ventana.pantalla(), NULL );
	SDL_RenderSetScale( m_ventana.pantalla(), 1.0f, 1.0f );
	
	// PAD
	SDL_Rect origen  = { 192, 0, 96, 96 };
	SDL_Rect destino = { tactil.xpad - tactil.rmax, tactil.ypad - tactil.rmax, tactil.rmax * 2, tactil.rmax * 2 };
	m_tactil.copia( &origen, &destino );
	// Dibuja la posición del pad:
	origen.x = 288; origen.y = 28;
	origen.w = 40;  origen.h = 40;
	destino.x = tactil.x - tactil.rmed;
	destino.y = tactil.y - tactil.rmed;
	destino.w = destino.h = tactil.rmed * 2;
	m_tactil.copia( &origen, &destino );
	
	// BOTÓN A:
	origen.x = 192; origen.y = 96; 
	origen.w = 72; origen.h = 72;
	destino.x = tactil.xb1 - tactil.rbot;
	destino.y = tactil.ypad - tactil.rbot;
	destino.w = destino.h = tactil.rbot * 2;
	m_tactil.copia( &origen, &destino );
	
	// BOTÓN B:
	origen.x = 264;
	destino.x = tactil.xb2 - tactil.rbot;
	m_tactil.copia( &origen, &destino );
	
	// BOTONES L, C y R:
	if( m_pausa ) {
		origen.x = 0; origen.y = 0;
		origen.w = 192; origen.h = 56;
		destino.x = 0; destino.y = 0; 
		destino.w = m_ventana.dameAncho() / 3;
		destino.h = m_ventana.dameAlto() / 8;
		m_tactil.copia( &origen, &destino );
		origen.y = 56; destino.x += destino.w; 
		m_tactil.copia( &origen, &destino );
		origen.y = 112; destino.x += destino.w; 
		m_tactil.copia( &origen, &destino );
	}
		
	m_ventana.medidasLogicas( m_anchoLog, m_altoLog );   
}
#endif

void Juego::pausa() {
#ifdef TACTIL
	if( m_control.arriba()    ) m_control.agrandaTactil();
	if( m_control.abajo()     ) m_control.encogeTactil();
	if( m_control.derecha()   ) m_control.transparentaTactil();
	if( m_control.izquierda() ) m_control.opacaTactil();
	if( m_control.musica()    ) {
		m_control.quitaTactil();
		m_pausa = false;
		m_sonidos.continua();
		m_musicas.continua();
	}
	if( m_control.fuego_2() ) { 
		m_ingles = !m_ingles; 
		SDL_Delay( 200 ); 
	};
#endif
	if( m_control.dispara() ) {
		m_pausa = false;
		m_sonidos.continua();
		m_musicas.continua();
	}
	m_ventana.rectanguloLleno( 80, 88, 90, 24 );
	if( m_parpadeo < 10 ) {
		m_dibuja.escribe( texto_pausa[ m_ingles ] );
	}
	++m_parpadeo; if( m_parpadeo > 20 ) m_parpadeo = 0;
}

void Juego::juega() {
	while( leeControl() ) {
		if( !mueve() ) break;
		m_dibuja.pantalla( m_posx, m_posy );
		m_dibuja.consola(  m_noni, m_nivel, m_pantalla_numero );
		m_noni.dibuja();
		m_disparos.dibuja();
		m_procesos.dibuja();
		if( m_pausa ) pausa();
	#ifdef TACTIL
		dibujaTactil();
	#endif
		m_ventana.actualiza();
		m_ventana.retarda();
	}
}

bool Juego::escribe( const char* texto ) {
	m_ventana.tomaColor( PGL::NEGRO );
	for( register int i = 0; i < 120; ++i ) {
		m_control.leeEstado();
		if( m_control.quitar() ) return false; 
		if( m_control.salir()  ) return false;
		if( m_control.maximiza() ) m_pausa = false;
		if( m_control.minimiza() ) m_pausa = true;
		if( m_control.restaura() ) m_pausa = false;
		if( m_control.pausa()  ) { m_pausa = !m_pausa; SDL_Delay( 200 ); break; }
		m_ventana.limpia();
		m_dibuja.escribe( texto );
	#ifdef TACTIL
		dibujaTactil();
	#endif
		m_ventana.actualiza();
		m_ventana.retarda();
	}
	return true;
}

bool Juego::pantallaCarga() {
	SDL_Rect desintro = {16,16,224,160};
	SDL_Rect rect = {0,0,256,23};
	PGL::COLOR  color;
	color = PGL::NEGRO;
	PGL::Imagen imagen;
	if( !imagen.inicia( m_ventana, graficos_carga[ m_graficos_mejorados ] ) ) return false;
	for( int i = 0; i < 160; ++i ) {
		m_control.leeEstado();
		if( m_control.quitar() ) return false; 
		if( m_control.salir()  ) return false;
		if( m_control.maximiza() ) m_pausa = false;
		if( m_control.minimiza() ) { m_pausa = true; break; }
		if( m_control.restaura() ) m_pausa = false;
		if( m_control.pausa()  ) { m_pausa = !m_pausa; break; }
		if( m_control.graficos() ) {
			m_graficos_mejorados = !m_graficos_mejorados;
			if( !imagen.inicia( m_ventana, graficos_carga[ m_graficos_mejorados ] ) ) return false;
			SDL_Delay( 200 );
		}
		m_ventana.tomaColor( PGL::NEGRO );
		m_ventana.limpia();
		for( rect.y -= 192; rect.y < 192; rect.y += 23 ) {
			m_ventana.tomaColor( color );
			m_ventana.rectanguloLleno( rect.x, rect.y, rect.w, rect.h );
			color = PGL::COLOR( color + 1 );
			if( color > 15 ) color = PGL::NEGRO;
		}
		imagen.copia( NULL, &desintro );
	#ifdef TACTIL
		dibujaTactil();
	#endif
		m_ventana.actualiza();
		m_ventana.retarda();
	}
	imagen.quita();
	return true;
}
	
bool Juego::pantallaInicio() {
	if( !iniciaNivel( 0 ) ) return false;
	m_cuenta = 0;
	while( true ) {
		m_control.leeEstado();
		if( m_control.quitar() || m_control.salir() ) {
			m_musicas.quita();
	 		return false;
		}
		if( m_control.maximiza() ) {
			m_pausa = false;
			m_musicas.continua();
		}
		if( m_control.minimiza() ) { 
			m_pausa = true; 
			m_musicas.pausa();
		}
		if( m_control.restaura() ) {
			m_pausa = false; 
			m_musicas.continua();
		}
		if( m_control.graficos() ) {
			if( !m_dibuja.cambiaGraficos() ) return false;
			if( !m_sonidos.cambia() ) return false;
			if( m_musica ) {
				if( !m_musicas.cambia() ) return false;
			}
			if( m_pausa ) {
				m_sonidos.pausa();
				m_musicas.pausa();
			}
			SDL_Delay( 200 );
		}
		if( m_control.lenguaje() ) {
			m_ingles = !m_ingles;
			SDL_Delay( 200 );
		}
		if( m_control.musica() && !m_pausa ) {
			if( m_musica ) {
				m_musica = false;
				m_musicas.quita();
			} else {
				m_musica = true;
				if( !m_musicas.inicia( m_graficos_mejorados, m_nivel ) ) return false;
				if( !m_pausa ) m_musicas.reproduce();
			}
			SDL_Delay( 200 );
		}
		if( m_control.pausa() )  {
			m_pausa = !m_pausa;
			if( m_pausa ) m_musicas.pausa();
			else		  m_musicas.continua();
			SDL_Delay( 200 );
		}
	#ifdef TACTIL
		if( m_control.arriba()    ) m_control.agrandaTactil();
		if( m_control.abajo()     ) m_control.encogeTactil();
		if( m_control.derecha()   ) m_control.transparentaTactil();
		if( m_control.izquierda() ) m_control.opacaTactil();
		if( m_control.fuego_2()   ) { m_ingles = !m_ingles; SDL_Delay( 200 ); }
	#endif
		if( m_cuenta > 50 ) {
			if( m_control.dispara() ) break;
		} else ++m_cuenta;
		m_dibuja.inicio();
		if( m_pausa ) {
		#ifdef TACTIL
			if( m_control.musica() ) {
				m_control.quitaTactil(); 
				m_pausa = false;
				if( m_musica ) m_musicas.continua();	  
			}
		#endif
			SDL_Delay( 40 );
			m_ventana.rectanguloLleno( 80, 88, 90, 24 );
			if( m_parpadeo < 10 ) {
				m_dibuja.escribe( texto_pausa[ m_ingles ] );
			}
			++m_parpadeo; if( m_parpadeo > 20 ) m_parpadeo = 0;
		}
	#ifdef TACTIL
		dibujaTactil();
	#endif
		m_ventana.actualiza();
		m_ventana.retarda();
	}
	m_musicas.quita();
	m_pausa = false;
	return true;
}

bool Juego::muerteNoni() {
	PGL::Semilla();
	int y    = 0;
	m_posx   = 0;
	m_posy   = 0;
	m_cuenta = 0;
	bool ingles;
	m_noni.vuela();
	m_noni.tomaXrel( 30 * 8 );
	m_noni.tomaYrel( 13 * 8 );
	while( true ) {
		m_control.leeEstado();
		if( m_control.quitar() ) return false; 
		if( m_control.salir()  ) return false;
		if( m_control.graficos() ) {
			m_nivel = ULTIMO_NIVEL;
			if( !m_dibuja.cambiaGraficos() ) return false;
			m_nivel = ULTIMO_NIVEL - 1;
			if( !m_sonidos.cambia() ) return false;
			if( m_musica ) {
				if( !m_musicas.cambia() ) return false;
			}
			SDL_Delay( 200 );
		}
		if( m_control.lenguaje() ) {
			m_ingles = !m_ingles;
			SDL_Delay( 200 );
		}
		m_procesos.mueve();
		m_procesos.colisiones( m_noni, m_disparos );
		ingles = m_ingles; m_ingles = false;
		m_dibuja.pantalla( m_posx, m_posy );
		m_ingles = ingles;
		m_noni.dibuja();
		m_procesos.dibuja();
		m_dibuja.consola( m_noni, ULTIMO_NIVEL - 1, 7 );
	#ifdef TACTIL
		dibujaTactil();
	#endif
		m_ventana.actualiza();
		m_ventana.retarda();
		++m_cuenta;
		
		if( m_cuenta == 40 )  {
			m_noni.tomaXrel( 29 * 8 );
			m_mapa.toma( 0, 31, 13, 753 );
			m_mapa.toma( 0, 31, 14, 753 );
		}
		if( m_cuenta == 50 )  {
			m_noni.tomaXrel( 28 * 8 );
			m_mapa.toma( 0, 31, 13, 238 );
			m_mapa.toma( 0, 31, 14, 238 );
			m_mapa.toma( 0, 30, 13, 753 );
			m_mapa.toma( 0, 30, 14, 753 );
		}
		if( m_cuenta == 60 )  {
			m_noni.tomaXrel( 27 * 8 );
			m_mapa.toma( 0, 31, 13, 239 );
			m_mapa.toma( 0, 31, 14, 239 );
			m_mapa.toma( 0, 30, 13, 238 );
			m_mapa.toma( 0, 30, 14, 238 );
			m_mapa.toma( 0, 29, 13, 753 );
			m_mapa.toma( 0, 29, 14, 753 );
		}
		if( m_cuenta == 70 )  {
			m_mapa.toma( 0, 31, 13, 239 );
			m_mapa.toma( 0, 31, 14, 239 );
			m_mapa.toma( 0, 30, 13, 239 );
			m_mapa.toma( 0, 30, 14, 239 );
			m_mapa.toma( 0, 29, 13, 238 );
			m_mapa.toma( 0, 29, 14, 238 );
			m_mapa.toma( 0, 28, 13, 753 );
			m_mapa.toma( 0, 28, 14, 753 );
		}
		if( m_cuenta == 80 ) {
			m_noni.muere();
			m_sonidos.muerte();
			m_procesos.explosion( m_noni.dameXfloat(), m_noni.dameYfloat() );
		}
		if( m_cuenta == 130 ) {
			m_procesos.limpia();
		}
		if( (m_cuenta > 130) && (m_cuenta < 500) ) {
			if( m_cuenta % 7 ) {
				y = PGL::Aleatorio( 0, 55 ) >> 1;
				m_procesos.derrumbe( y * 8 );
			}
		}
		if( m_cuenta == 500 ) {
			m_posx = 32;
			if( m_musica ) m_musicas.reproduce( 0 );
		}
		if( m_cuenta > 1000 ) {
			if( m_musica ) { 
				if( !Mix_PlayingMusic() ) break;
			} else break;
		}
	}
	m_posx = 0;
	m_noni.quita();
	return true;
}

bool Juego::pantallaFinal() {
	m_enemigos.quita();
	m_procesos.limpia();
	if( m_musica ) m_musicas.para();
	if( !muerteNoni() ) return false;
	if( m_musica ) m_musicas.inicia( m_graficos_mejorados, 7 );
	if( m_musica ) m_musicas.reproduce();

	m_posy = 16;
	m_cuenta = 0;
	Uint32 cuenta    = 0;
	int    animacion = 0; 
	float  radianes  = 0;
	SDL_Rect origen  = { 96, 128, 16, 16 };
	SDL_Rect destino = { 64, 64, 16, 16 };
	
	while( true ) {
		m_control.leeEstado();
		if( m_control.quitar() ) return false; 
		if( m_control.salir()  ) return false;
		if( m_control.graficos() ) {
			m_nivel = ULTIMO_NIVEL;
			if( !m_dibuja.cambiaGraficos() ) return false;
			m_nivel = ULTIMO_NIVEL - 1;
			if( !m_sonidos.cambia() ) return false;
			if( m_musica ) {
				if( !m_musicas.cambia() ) return false;
			}
			SDL_Delay( 200 );
		}
		if( m_control.lenguaje() ) {
			m_ingles = !m_ingles;
			SDL_Delay( 200 );
		}
		if( m_cuenta > 50 )	{
			if( m_control.dispara() ) break;	
		} else ++m_cuenta;
		m_dibuja.final();
		m_dibuja.consola( m_noni, ULTIMO_NIVEL - 1, 7 );
		// Movimiento;
		origen.x  = 96 + animacion; origen.y = 128;
		destino.x = 24; destino.y = 32;
		destino.x += (int)(SDL_sinf( radianes ) * 16.0f);
		destino.y += (int)(SDL_cosf( radianes ) * 16.0f);
		radianes += 0.08f;
		if( radianes > M_2PI ) radianes = 0.0f;
		++cuenta;
		if( cuenta > 4 ) {
			cuenta = 0;
			if( animacion == 0 ) animacion = 16;
			else			     animacion = 0;
		}
		m_sprites.copia( &origen, &destino );
		destino.x += 192;
		m_sprites.copia( &origen, &destino );
		destino.x -= 160; destino.y += 64; 
		m_sprites.copia( &origen, &destino );
		destino.x += 120;
		m_sprites.copia( &origen, &destino );
	#ifdef TACTIL
		dibujaTactil();
	#endif
		m_ventana.actualiza();
		m_ventana.retarda();
	}
	
	m_dibuja.quita();
	return true;
}


