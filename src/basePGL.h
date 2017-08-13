#ifndef _BASE_PGL_
#define _BASE_PGL_
#include "PGL/PGL.h"
#include "control.h"
#include "sonidos.h"
#include "musicas.h"

enum Enumera_Direccion { 
	FRENTE, 
	DERECHA, 
	IZQUIERDA, 
	ARRIBA, 
	ABAJO
};

class BasePGL {
protected:
	static PGL::Cadena  m_ruta;
	static PGL::Sistema	m_sistema;
	static PGL::Ventana	m_ventana;
	static PGL::Imagen	m_sprites;
#ifdef TACTIL
	static PGL::Imagen	m_tactil;
#endif
	static PGL::MapaXML	m_xml;
	static Control		m_control;
	static Sonidos      m_sonidos;
	static Musicas		m_musicas;

	// CAMARA:
	static int m_nivel;
	static int m_pantalla_numero;
	static int m_posx;			// En baldosas
	static int m_posy;			// En baldosas
	static int m_anchoLog;		// En pixeles
	static int m_altoLog;		// En pixeles
	static int m_mapa_ancho;	// En baldosas
	static int m_mapa_alto;		// En baldosas
	static int m_baldosa_ancho;	// En pixeles
	static int m_baldosa_alto;	// En pixeles
	
	static int m_pantalla_x;		// En pixeles
	static int m_pantalla_y;		// En pixeles
	static int m_pantalla_w;		// En pixeles
	static int m_pantalla_h;		// En pixeles
	static int m_pantalla_ancho;	// En baldosas
	static int m_pantalla_alto;		// En baldosas
	
	static int m_consola_x;		// En pixeles
	static int m_consola_y;		// En pixeles
	static int m_consola_ancho;	// En baldosas
	static int m_consola_alto;	// En baldosas
	static int m_consola_posy;	// Posición en baldosas del la consola en el mapa, posx = 0.

	static bool m_graficos_mejorados;
	static bool m_pausa;
	static bool m_musica;
	static bool m_ingles;
#ifdef PGL_DEPURANDO
	static bool m_mortal;
#endif
};

#endif

