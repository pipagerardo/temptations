#ifndef _PGL_SISTEMA_
#define _PGL_SISTEMA_
#include "PGL_cabeceras.h"
// ----------------------------------------------------------------------	
// flags_sdl:				flags_img:			flags_mix:
//	SDL_INIT_VIDEO			 IMG_INIT_JPG		 MIX_INIT_FLAC
// 	SDL_INIT_EVENTS			 IMG_INIT_PNG		 MIX_INIT_MOD
// 	SDL_INIT_TIMER			 IMG_INIT_TIF		 MIX_INIT_MODPLUG
// 	SDL_INIT_JOYSTICK		 IMG_INIT_WEBP		 MIX_INIT_MP3
// 	SDL_INIT_GAMECONTROLLER						 MIX_INIT_OGG
// 	SDL_INIT_HAPTIC								 MIX_INIT_FLUIDSYNTH
// ----------------------------------------------------------------------
namespace PGL {
	
class Sistema {
public:
	
	// Construcción y destrucción:
	Sistema();
	~Sistema();
	
	bool inicia( Uint32 sistema = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER );
	void quita();								// Quita absolutamente todo.
	
	bool iniciaSubsistema( Uint32 subsistema );
	void quitaSubsistema(  Uint32 subsistema );	// Quita un subsistema.

#ifdef PGL_USAR_SDL_IMAGE	
	bool iniciaImagen( int imagen = 0 );
	void quitaImagen();	// Quita sólo el subsistema de Imagen. ( SDL_image )
#endif

#ifdef PGL_USAR_SDL_MIXER	
	bool iniciaSonido( int sonido = 0 );
	void quitaSonido();	// Quita sólo el subsistema de Sonido. ( SDL_mixer )
#endif

#ifdef PGL_USAR_SDL_TTF	
	bool iniciaTTF();
	void quitaTTF();	// Quita sólo el subsistema de Sonido. ( SDL_ttf )
#endif

#ifdef PGL_USAR_SDL_NET	
	bool iniciaRed();
	void quitaRed();	// Quita sólo el subsistema de Sonido. ( SDL_net )
#endif

	static Sistema* dameSistema();

#ifdef PGL_USAR_SDL_MIXER		
	// Configuración del sistema de sonido: Usar antes de iniciaSonido();
	// Por defecto 22050 Hz, AUDIO_S16SYS, stereo, 1024 bytes, 8 canales.
	static void tomaFrecuencia( int hercios ); // [22050] | 44100 
	static void tomaFormato( int formato );    // AUDIO_S8 | AUDIO_U8 | [AUDIO_S16SYS] | AUDIO_U16SYS | AUDIO_S32SYS | AUDIO_F32SYS
	static void tomaCanales( int cantidad );   // 1 (mono) | [2 (stereo)] | 4 (quad) | 6 (5.1)
	static void tomaBuffer(  int potencia_de_dos ); // 512 | [1024] | 2048 | 4096 | ...
	static void tomaCanalesSimultaneos( int numero ); // Entre [8] y 16.
	
	// Retornos del sistema de sonido: Mejor después de iniciaSonido();
	static int    dameFrecuencia();
	static Uint16 dameFormato();
	static int    dameCanales();
	static int    dameBuffer();
	static int    dameCanalesSimultaneos();
#endif

private:
	static Sistema *m_sistema;
#ifdef PGL_USAR_SDL_IMAGE
	static bool     m_imagen;
#endif
#ifdef PGL_USAR_SDL_MIXER	
	static bool     m_sonido;
	static int      m_frecuencia;
	static Uint16   m_formato;
	static int      m_canales;
	static int	    m_buffer;
	static int      m_num_canales;
#endif
#ifdef PGL_USAR_SDL_TTF
	static bool     m_ttf;
#endif
#ifdef PGL_USAR_SDL_NET
	static bool     m_net;
#endif
};

}
#endif

