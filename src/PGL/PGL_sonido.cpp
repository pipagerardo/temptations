#include "PGL_sonido.h"
#ifdef PGL_USAR_SDL_MIXER
namespace PGL {

Sonido::Sonido() {
	m_canal = 0;
	m_chunk = NULL;
}

Sonido::Sonido( const char* archivo, Uint8 volumen ) {
	m_canal = 0;
	m_chunk = NULL;
	inicia( archivo, volumen );
}

Sonido::~Sonido() {  
	quita();
}

bool Sonido::inicia( const char* archivo, Uint8 volumen ) {
	if( m_chunk != NULL ) quita();
	m_chunk = Mix_LoadWAV( archivo );
	if( m_chunk == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Mix_LoadWAV: %s\n", Mix_GetError() );
	#endif
		return false;
	}
	Mix_VolumeChunk( m_chunk, volumen ); // De 0 a 128
	return true;
}

bool Sonido::inicia( Uint8 *memoria, Uint32 longitud, Uint8 volumen ) {
	if( m_chunk != NULL ) quita();
	m_chunk = Mix_QuickLoad_RAW( memoria, longitud );
	if( m_chunk == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Mix_QuickLoad_RAW: %s\n", Mix_GetError() );
	#endif
		return false;
	}
	Mix_VolumeChunk( m_chunk, volumen ); // De 0 a 128
	return true;
}

void Sonido::quita() {
	if( m_chunk != NULL ) {
		Mix_FreeChunk( m_chunk );
		m_chunk = NULL;
		m_canal = 0;
	}
}

void Sonido::reproduce( int grupo, int veces ) {
	// Mix_HaltChannel( m_canal );
	// if( canal >= 0 ) Mix_HaltChannel( canal );
	if( m_chunk != NULL ) m_canal = Mix_PlayChannel( grupo, m_chunk, veces );
}

void Sonido::para() { 
	if( Mix_Playing( m_canal ) == 1 ) Mix_HaltChannel( m_canal );
}

void Sonido::para( int grupo ) { 
	Mix_HaltGroup( grupo );
}

void Sonido::pausa() {
	if( Mix_Playing( m_canal ) == 1 ) Mix_Pause( m_canal );
}

void Sonido::continua( ) { 
	if( Mix_Paused(  m_canal ) == 1 ) Mix_Resume( m_canal ); 
}

bool Sonido::esta_sonando() {
	return ( Mix_Playing( m_canal ) == 1 ) ? true : false;
}

void Sonido::volumen( Uint8 volumen ) { 
	Mix_VolumeChunk( m_chunk, volumen ); 
}

bool Sonido::reserva( int canales ) {
	return Mix_ReserveChannels( canales ) != canales ? false : true;
}

bool Sonido::reserva( int canal, int grupo ) {
	return Mix_GroupChannel( canal, grupo ) < 1 ? false : true;
}

bool Sonido::reserva( int del_canal, int al_canal, int grupo ) {
	return Mix_GroupChannels( del_canal, al_canal, grupo ) < 1 ? false : true;
}

}

#endif

// -----------------------------------------------------------------------------
// -- Carga un Sonido  ---------------------------------------------------------
// -----------------------------------------------------------------------------
// Mix_Chunk *Mix_LoadWAV(char *file)                     // Carga un Sonido
// void Mix_FreeChunk( Mix_Chunk *chunk)                  // Borra un Sonido
// int Mix_VolumeChunk( Mix_Chunk *chunk, int volume )    // Volumen rango de 0 a 128.
//     int loop -> Reproduce una vez loops = 0; Reproduce infinito loops = -1
//     int channel -> -1 Asigna canal automáticamente.
// int Mix_PlayChannel( int channel, Mix_Chunk *chunk, int loops )
// int Mix_PlayChannelTimed( int channel, Mix_Chunk *chunk, int loops, int ticks)
// int Mix_FadeInChannel( int channel, Mix_Chunk *chunk, int loops, int ms)
// int Mix_FadeInChannelTimed( int channel, Mix_Chunk *chunk, int loops, int ms, int ticks)
// void Mix_Pause( int channel )
// void Mix_Resume( int channel )
// int Mix_HaltChannel( int channel )
// int Mix_FadeOutChannel( int channel, int ms ) 
// int Mix_Playing( int channel )  // Si está  sonando 1, 0 si no.
// int Mix_Paused( int channel )   // Si está  pausada 0, 1 si est  sonando.
// Load a wave file of the mixer format from a memory buffer */
// Mix_Chunk* Mix_QuickLoad_WAV(Uint8 *mem);
// Load raw audio data of the mixer format from a memory buffer */
// Mix_Chunk* Mix_QuickLoad_RAW(Uint8 *mem, Uint32 len);
// int Mix_QuerySpec( int *frequency, Uint16 *format, int *channels );
// -----------------------------------------------------------------------------
