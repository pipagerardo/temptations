// PGL - PipaGerardo Library //
// 2016 - PIPAGERARDO        //
// pipagerardo@gmail.com     //
// pipagerardo@hotmail.es    //
// GPL v3 license            //
// Última actualización:     //
//         13-06-2017        //
// ------------------------- //

#ifndef _PGL_
#define _PGL_

#include "PGL_configuracion.h"	// Muy importante, la configuración de PGL

#ifndef PGL_ANDROID
	#define SDL_MAIN_HANDLED
#endif

#include "PGL_cabeceras.h"
#include "PGL_funciones.h"
#include "PGL_contenedor.h"
#include "PGL_tabla.h"
#include "PGL_matriz.h"
#include "PGL_lista.h"
#include "PGL_color.h"
#include "PGL_rectangulo.h"
#include "PGL_cadena.h"
#include "PGL_archivo.h"
#include "PGL_tiempo.h"
#include "PGL_sistema.h"
#include "PGL_ventana.h"
#include "PGL_imagen.h"		// Imágenes por hardware SDL_Texture;

#ifdef PGL_USAR_COMPRESOR
#include "PGL_compresor.h"
#endif

#ifdef PGL_USAR_EVENTO
#include "PGL_evento.h"
#endif

#ifdef PGL_USAR_FUENTE
#include "PGL_fuente.h"		// Texto usando una imagen y una métrica
#endif

#ifdef PGL_USAR_HILO
#include "PGL_hilo.h"		// Multihilo, hacer varias cosas a la vez (yo no puedo)
#endif

#ifdef PGL_USAR_MAPAXML
#include "PGL_mapaxml.h"	// Lee mapas XML creados con TILED
#endif

#ifdef PGL_USAR_SUPERFICIE
#include "PGL_superficie.h" // Imágenes por software SDL_Surface;
#endif

#ifdef PGL_USAR_PARTITURA
#include "PGL_partitura.h"	// Crea sonido apartir de una partitura.
#endif

#ifdef PGL_USAR_SDL_MIXER	// SDL2_mixer;
#include "PGL_musica.h"		
#include "PGL_sonido.h" 
#endif

#ifdef PGL_USAR_SDL_TTF		// SDL2_ttf;
#include "PGL_fuenteTTF.h"	
#endif

#ifdef PGL_USAR_SDL_NET		// SDL2_net;
#include "PGL_red.h"		
#endif

#endif

