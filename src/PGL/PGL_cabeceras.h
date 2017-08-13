#ifndef _PGL_CABECERAS_
#define _PGL_CABECERAS_
#include "PGL_configuracion.h"

#include <stdlib.h>

#if ( defined( PGL_WINDOWS ) || defined( PGL_ANDROID ) )
	#include "SDL.h"
	#ifdef PGL_USAR_SDL_IMAGE
		#include "SDL_image.h"
	#endif
	#ifdef PGL_USAR_SDL_MIXER
		#include "SDL_mixer.h"
	#endif
	#ifdef PGL_USAR_SDL_TTF
		#include "SDL_ttf.h"
	#endif
	#ifdef PGL_USAR_SDL_NET
		#include "SDL_net.h"
	#endif
#elif ( defined( PGL_LINUX ) || defined( PGL_MAC ) )
	#include <SDL2/SDL.h>
	#ifdef PGL_USAR_SDL_IMAGE
		#include <SDL2/SDL_image.h>
	#endif
	#ifdef PGL_USAR_SDL_MIXER
		#include <SDL2/SDL_mixer.h>
	#endif
	#ifdef PGL_USAR_SDL_TTF
		#include <SDL2/SDL_ttf.h>
	#endif
	#ifdef PGL_USAR_SDL_NET
		#include <SDL2/SDL_net.h>
	#endif
#endif


#ifdef PGL_ANDROID
	#include <android/log.h>
	#define  LOG_TAG    	"PGL"
	#define  SDL_LOG(...)  __android_log_print( ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__ )
#else  // Para el resto
	#define  SDL_LOG(...) SDL_Log( __VA_ARGS__ )
#endif

#ifndef M_PHI
#define M_PHI		1.61803398874989484820	// ( Proporción de oro )
#endif

#ifndef M_E
#define M_E			2.71828182845904523536	// ( Número de euler )
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846	// ( Proporción de la circunferencia )
#endif

#ifndef M_PI2
#define M_PI2		1.57079632679489661923	// ( M_PI / 2 )
#endif

#ifndef M_PI4
#define M_PI4		0.78539816339744830962	// ( M_PI / 4 )
#endif

#ifndef M_PI6
#define M_PI6		0.52359877559829887308	// ( M_PI / 6 )
#endif

#ifndef M_2PI
#define M_2PI		6.28318530717958647692	// ( 2 * M_PI )
#endif

#ifndef M_RADIANES
#define M_RADIANES	0.01745329251994329577	// ( M_PI / 180.0 )
#endif

#ifndef M_GRADOS
#define M_GRADOS	57.2957795130823208768	// ( 180.0 / M_PI )
#endif

// #define M_LOG2E		1.4426950408889634074
// #define M_LOG10E		0.43429448190325182765
// #define M_LN2		0.69314718055994530942
// #define M_LN10		2.30258509299404568402

#endif

