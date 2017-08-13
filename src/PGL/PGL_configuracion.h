#ifndef _PGL_CONFIGURACION_
#define _PGL_CONFIGURACION_

// CONFIGURACIÓN DE TEMPTATIONS:
// ----------------------------
#define TACTIL
// #define TACTIL_RATON

// CONFIGURACIÓN DE PGL:
// ------------------------

// Linker (Enlazado): (Diferentes configuraciones)
// ------------------------------------------------
// -lmingw32 -mconsole -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lzlib1
// -lmingw32 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lzlib1
// -lmingw32 -lSDL2 -lSDL2_image -lSDL2_mixer -lzlib1
// -lmingw32 -lSDL2 

// Opciones Adicionales:
// ----------------------
// #define PGL_DEPURANDO				// MUESTRA MENSAJES DE ERROR, PARA EL DESARROLLO
   #define PGL_USAR_SDL_IMAGE			// USAR SDL_IMAGE	Linker: -lSDL2_image -lzlib1
   #define PGL_USAR_SDL_MIXER			// USAR SDL_MIXER	Linker: -lSDL2_mixer
// #define PGL_USAR_SDL_TTF				// USAR SDL_TTF		Linker: -lSDL2_ttf -lzlib1
// #define PGL_USAR_SDL_NET				// USAR SDL_NET		Linker: -lSDL2_net
// #define PGL_USAR_EVENTO				// USAR LA CLASE EVENTO
// #define PGL_USAR_FIGURAS				// USA LAS FIGURAS (circulo, elipse, etc.. )
// #define PGL_USAR_SUPERFICIE			// USAR IMAGENES POR SOFTWARE SDL_Surface
// #define PGL_USAR_FUENTE				// USAR FUENTES DE TEXTO MTR
// #define PGL_USAR_PARTITURA			// USAR PARTITURA PARA CREAR SONIDO Y MUSICA
   #define PGL_USAR_COMPRESOR			// USA COMPRESIÓN				Linker: -lzlib1
   #define PGL_USAR_MAPAXML				// LEE MAPAS CREADOS CON TILED 	Linker: -lzlib1
// #define PGL_USAR_EVENTOSEDICIONTEXTO	// USAR EVENTOS DE EDICION DE TEXTO
// #define PGL_USAR_EVENTOSCONTROLADOR	// USAR EVENTOS GAME CONTROLLER
// #define PGL_USAR_EVENTOSGESTOS		// USAR EVENTOS GESTURE
// #define PGL_USAR_HILO				// USAR HILOS

// Configuración de la plataforma:
// (En teoría se detecta automáticamente)
// -DPGL_WINDOWS			// ESPECIFICO PARA WINDOWS
// -DPGL_LINUX				// ESPECIFICO PARA LINUX
// -DPGL_ANDROID			// ESPECIFICO PARA ANDROID
// -DPGL_MAC				// ESPECIFICO PARA MAC
// -DPGL_WINDOWSRT			// ESPECIFICO PARA WINDOWSRT
#ifndef PGL_WINDOWS
	#if defined( WIN32 ) || defined( _WIN32 ) || defined( __CYGWIN__ ) || defined( __MINGW32__ )
		#define PGL_WINDOWS
	#endif
#endif
#ifndef PGL_LINUX
	#if ( defined( linux ) || defined( __linux ) || defined( __linux__ ) )
		#define PGL_LINUX
	#endif
#endif
#ifndef PGL_ANDROID
	#if defined(__ANDROID__)
		#define PGL_ANDROID
	#endif
#endif
#ifndef PGL_MAC
	#ifdef __APPLE__ 
		#define PGL_MAC
	#endif
#endif	

#endif

