#include "basePGL.h"

PGL::Cadena		BasePGL::m_ruta;
PGL::Sistema	BasePGL::m_sistema;
PGL::Ventana	BasePGL::m_ventana;
PGL::Imagen		BasePGL::m_sprites;
#ifdef TACTIL
PGL::Imagen		BasePGL::m_tactil;
#endif
PGL::MapaXML	BasePGL::m_xml;
Control			BasePGL::m_control;
Sonidos  		BasePGL::m_sonidos;
Musicas			BasePGL::m_musicas;

int BasePGL::m_nivel    = 1;
int BasePGL::m_pantalla_numero = 1;

int BasePGL::m_posx     = 0;
int BasePGL::m_posy     = 0;

int BasePGL::m_anchoLog = 256;
int BasePGL::m_altoLog  = 192;

int BasePGL::m_mapa_ancho = 0;
int BasePGL::m_mapa_alto  = 0;

int BasePGL::m_baldosa_ancho = 8;
int BasePGL::m_baldosa_alto  = 8;

int BasePGL::m_pantalla_x;		// En pixeles
int BasePGL::m_pantalla_y;		// En pixeles
int BasePGL::m_pantalla_w;		// En pixeles
int BasePGL::m_pantalla_h;		// En pixeles
int BasePGL::m_pantalla_ancho;	// En baldosas
int BasePGL::m_pantalla_alto;	// En baldosas
	
int BasePGL::m_consola_x;		// En pixeles
int BasePGL::m_consola_y;		// En pixeles
int BasePGL::m_consola_ancho;	// En baldosas
int BasePGL::m_consola_alto;	// En baldosas
int BasePGL::m_consola_posy;	// Posición en baldosas del la consola en el mapa, posx = 0.

bool BasePGL::m_graficos_mejorados = true;
bool BasePGL::m_pausa = false;
bool BasePGL::m_musica = true;
bool BasePGL::m_ingles = false;
#ifdef PGL_DEPURANDO
bool BasePGL::m_mortal = true;
#endif

