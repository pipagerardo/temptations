#include "PGL_sistema.h"

#ifdef __APPLE__
	#include "CoreFoundation/CoreFoundation.h"
#endif

#ifdef PGL_ANDROID
	#include <stdlib.h>
#endif
namespace PGL {

// Frecuencias de Sonido;
// 22050 | 44100 

// Formatos de Sonido:
// AUDIO_S8 | AUDIO_U8 | AUDIO_S16SYS | AUDIO_U16SYS | AUDIO_S32SYS | AUDIO_F32SYS

// Canales de Sonido:
// 1 (mono) | 2 (stereo) | 4 (quad) | 6 (5.1)

// Buffer de Sonido:
// Debe ser potencia de dos: 512 | 1024 | 2048 | 4096 | ...

// Numero de Canales de Sonido simultáneos:
// Es la cantidad de sonidos que pueden reproducirse a la vez.

Sistema* Sistema::m_sistema = NULL;
#ifdef PGL_USAR_SDL_IMAGE
bool	 Sistema::m_imagen = false;
#endif
#ifdef PGL_USAR_SDL_MIXER
bool	 Sistema::m_sonido = false;
int      Sistema::m_frecuencia  = MIX_DEFAULT_FREQUENCY;	// 22050
Uint16   Sistema::m_formato     = MIX_DEFAULT_FORMAT;		// AUDIO_S16SYS
int      Sistema::m_canales     = MIX_DEFAULT_CHANNELS;		// 2
int	     Sistema::m_buffer      = 1024;
int      Sistema::m_num_canales = 8;
#endif
#ifdef PGL_USAR_SDL_TTF
bool     Sistema::m_ttf = false;
#endif
#ifdef PGL_USAR_SDL_NET
bool     Sistema::m_net = false;
#endif

Sistema::Sistema() { 
}

Sistema::~Sistema() { quita(); }

bool Sistema::inicia( Uint32 sistema ) {
	
	if( m_sistema != NULL ) return true;

#ifdef PGL_MAC
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
   	char path[1024];
   	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)) {
        // error!
        return false;
   	}
   	CFRelease(resourcesURL);
   	chdir(path);
#endif
	
#ifndef PGL_ANDROID	
	SDL_SetMainReady();	// En Windows no queremos SDL_main
#else
	SDL_SetHint( SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1" );
#endif
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY,  "nearest" /* "linear" */ );
	SDL_SetHint( SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "0" );
	SDL_SetHint( SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "0" );
	SDL_SetHint( SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0" );
	    
	// Iniciamos SDL:
	if( SDL_Init( sistema ) < 0 ) {	// return: ( mal < 0 )
	#ifdef PGL_DEPURANDO
		SDL_LOG( "SDL_Init: %s\n", SDL_GetError() );
	#endif
		quita();
		return false;
	}

#ifdef PGL_ANDROID	
	SDL_SetHint( SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1" );
#endif
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY,  "nearest" /* "linear" */ );
	SDL_SetHint( SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "0" );
	SDL_SetHint( SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "0" );
	SDL_SetHint( SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0" );
	
	// Desactivamos el salvapantallas:
	SDL_DisableScreenSaver();

	m_sistema = this;
	return true;
}

void Sistema::quita() {
	if( m_sistema != NULL ) {
		// Activamos el salvapantallas:
		SDL_EnableScreenSaver();
	#ifdef PGL_USAR_SDL_IMAGE
		quitaImagen();
	#endif
	#ifdef PGL_USAR_SDL_MIXER
		quitaSonido();
	#endif
	#ifdef PGL_USAR_SDL_TTF	
		quitaTTF();
	#endif
	#ifdef PGL_USAR_SDL_NET	
		quitaRed();
	#endif
		SDL_Quit();
		m_sistema = NULL;
	}
	#ifdef PGL_ANDROID
		exit( 0 );
	#endif
}

bool Sistema::iniciaSubsistema( Uint32 subsistema ) {
	if( SDL_WasInit( subsistema ) == 0 ) {
		if( SDL_InitSubSystem( subsistema ) != 0 ) {
		#ifdef PGL_DEPURANDO
			SDL_LOG( "SDL_InitSubSystem: %s\n", SDL_GetError() );
		#endif
			return false;
		}
	}
	return true;
}

void Sistema::quitaSubsistema( Uint32 subsistema ) {
	if( SDL_WasInit( subsistema ) ) SDL_QuitSubSystem( subsistema );
}

#ifdef PGL_USAR_SDL_IMAGE
bool Sistema::iniciaImagen( int imagen ) {
	if( m_imagen ) return true;
	int imagen_init = IMG_Init( imagen );
	if( ( imagen_init & imagen ) != imagen ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "IMG_Init: %s\n", IMG_GetError() );
	#endif
		return false;
	}
	m_imagen = true;
	return true;
}

void Sistema::quitaImagen() {
	if( m_imagen ) {
		IMG_Quit();
		m_imagen = false;
	}
}
#endif

#ifdef PGL_USAR_SDL_MIXER
bool Sistema::iniciaSonido( int sonido ) {
	if( m_sonido ) return true;
	// Iniciamos el sistema de Audio si no lo está.
	if( !iniciaSubsistema( SDL_INIT_AUDIO ) ) return false;
	// Iniciamos el Mixer:
	int sonido_init = Mix_Init( sonido );
	if( ( sonido_init & sonido ) != sonido ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Mix_Init: %s\n", Mix_GetError() );
	#endif
		quitaSubsistema( SDL_INIT_AUDIO );
		return false;
	}
	if( Mix_OpenAudio( m_frecuencia, m_formato , m_canales, m_buffer ) == -1 ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Mix_OpenAudio: %s\n", Mix_GetError() );
	#endif
		quitaSubsistema( SDL_INIT_AUDIO );
		return false;
	}
	Mix_AllocateChannels( m_num_canales );
	Mix_QuerySpec( &m_frecuencia, &m_formato, &m_canales );
	m_sonido = true;
	return true;
}

void Sistema::quitaSonido() {
	if( m_sonido ) {
		Mix_Quit();
		quitaSubsistema( SDL_INIT_AUDIO );
		m_sonido = false;
	}
}

// Configuración del sistema de sonido:
void Sistema::tomaFrecuencia( int hercios  ) { if( !m_sonido ) m_frecuencia  = hercios;  }
void Sistema::tomaFormato(    int formato  ) { if( !m_sonido ) m_formato     = formato;  }
void Sistema::tomaCanales(    int cantidad ) { if( !m_sonido ) m_num_canales = cantidad; }
void Sistema::tomaBuffer(     int potencia_de_dos ) { if( !m_sonido ) m_buffer = potencia_de_dos; }
void Sistema::tomaCanalesSimultaneos( int numero  ) { if( !m_sonido ) m_num_canales = numero; }

// Retornos del sistema de sonido;
int    Sistema::dameFrecuencia() 	{ return m_frecuencia; }
Uint16 Sistema::dameFormato() 		{ return m_formato; }
int    Sistema::dameCanales() 		{ return m_canales; }
int    Sistema::dameBuffer()		{ return m_buffer;  }
int    Sistema::dameCanalesSimultaneos() { return m_num_canales; }

#endif

#ifdef PGL_USAR_SDL_TTF	
bool Sistema::iniciaTTF() {
	if( m_ttf ) return true;
	if( TTF_Init() == -1 ) {
	#ifdef PGL_DEPURANDO
    	SDL_LOG( "TTF_Init: %s\n", TTF_GetError() );
    #endif
    	return false;
	}
	return true;
}
void Sistema::quitaTTF() {
	if( m_ttf ) {
		TTF_Quit();
		m_ttf = false;
	}
}
#endif

#ifdef PGL_USAR_SDL_NET	
bool Sistema::iniciaRed() {
	if( m_net ) return true;
	return false;
}
void Sistema::quitaRed() {
	if( m_net ) {
		m_net = false;
	}
}
#endif

Sistema* Sistema::dameSistema() {
	return m_sistema;
}

}


