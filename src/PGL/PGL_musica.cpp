#include "PGL_musica.h"
#ifdef PGL_USAR_SDL_MIXER
namespace PGL {
	
Musica::Musica() {       
	m_musica = NULL;
}

Musica::Musica( const char* archivo, Uint8 volumen ) {
	m_musica = NULL;       
	inicia( archivo, volumen );
}

Musica::~Musica() {
	quita();
}
 
bool Musica::inicia( const char *archivo, Uint8 volumen ) {
	if( m_musica != NULL ) quita();  
	m_musica = Mix_LoadMUS( archivo );
	if( m_musica == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "Mix_LoadMUS: %s\n", Mix_GetError() );
	#endif
		return false;
	}
	Mix_VolumeMusic( volumen );
	return true;
	// SDL_Delay( 200 );
} 

void ReproductorMusica( void *dato, Uint8 *mezclador, int cantidad ) {
	if( (mezclador == NULL) || (cantidad == 0) ) return;
	RepMusica* repmusica = (RepMusica*)dato;
	if( ( repmusica == NULL ) || (repmusica->buffer == NULL) || (!repmusica->reproduce) ) {
		SDL_memset( (void *)mezclador, 0, (size_t)cantidad );
		return;
	}
	int resto = repmusica->capacidad - repmusica->posicion;
	if( resto >= cantidad ) {
		// SDL_LOG( "%d %d", resto, cantidad );
		SDL_memcpy( 
			(void *)mezclador, 
			(const void *)( repmusica->buffer + repmusica->posicion ), 
			(size_t)cantidad
		);
		repmusica->posicion += cantidad;
	} else {
		// Copiamos asta el final del buffer.
		SDL_memcpy( 
			(void *)mezclador, 
			(const void *)( repmusica->buffer + repmusica->posicion ), 
			(size_t)resto
		);
		// Copiamos desde el principio de buffer
		repmusica->posicion = cantidad - resto;
		SDL_memcpy( 
			(void *)mezclador, 
			(const void *)( repmusica->buffer ), 
			(size_t)repmusica->posicion
		);
	}
}


void ReproductorPara() {
	SDL_LOG( "PARA" );
	RepMusica* repmusica = (RepMusica*)Mix_GetMusicHookData();
	repmusica->posicion  = 0;
	repmusica->reproduce = false;
}

bool Musica::inicia( Uint8 *memoria, Uint32 longitud, Uint8 volumen ) {
	if( m_musica != NULL ) quita();
	if( m_repmusica.buffer != NULL ) {
		Mix_HookMusic( NULL, NULL );
	}
	m_repmusica.buffer    = memoria;
	m_repmusica.capacidad = longitud;
	m_repmusica.posicion  = 0;
	m_repmusica.reproduce = true;
	Mix_HookMusic( ReproductorMusica, &m_repmusica );
	Mix_HookMusicFinished( ReproductorPara );
	Mix_VolumeMusic( volumen );
	return true;
}

void Musica::quita() {
	if( m_musica != NULL ) {
		Mix_HaltMusic();
		Mix_FreeMusic( m_musica );
		m_musica = NULL;
	}
	if( m_repmusica.buffer != NULL ) {
		Mix_HookMusic( NULL, NULL );
		m_repmusica.buffer = NULL;
		Mix_HaltMusic();
	}
}
        
void Musica::reproduce( int veces ) { 
	Mix_PlayMusic( m_musica, veces ); 
}

void Musica::para() {
	Mix_HaltMusic(); 
	//SDL_Delay( 200 ); 
}

void Musica::pausa() {
	if( Mix_PlayingMusic() == 1 ) Mix_PauseMusic();  
}

void Musica::continua() { 
	if( Mix_PausedMusic()  == 1 ) Mix_ResumeMusic(); 
}

void Musica::inicio() { 
	Mix_RewindMusic();
}

int Musica::esta_sonando() { 
	return Mix_PlayingMusic();
}

void Musica::volumen( Uint8 volumen ) { 
	Mix_VolumeMusic( volumen ); 
}

}

#endif

// -----------------------------------------------------------------------------
// Carga una Música ------------------------------------------------------------
// -----------------------------------------------------------------------------
// Mix_Music *Mix_LoadMUS( const char *file )
// void Mix_FreeMusic( Mix_Music )
// int Mix_PlayMusic( Mix_Music *music, int loops )
// int Mix_FadeInMusic( Mix_Music *music, int loops, int ms )
// int Mix_VolumeMusic( int volume ) // De 0 a 128.
// void Mix_PauseMusic()
// void Mix_ResumeMusic()
// int Mix_HaltMusic()
// int Mix_FadeOutMusic(int ms)
// Int Mix_PlayingMusic()
// Int Mix_PausedMusic()
// void Mix_RewindMusic() // Su misisón es volver la reproducción de la música al principio.
// int Mix_SetMusicPosition( double position )  
//     La función nos permite situar la posición de reproducción en el lugar
//     deseado. La posición depender  directamente del formato del archivo de m£sica,
//     pudiendo ser el tiempo en milisegundo para archivos de m£sica digitalizada o la posici¢n
//     del patrón o el compás si es un archivo MOD o MIDI.
// Mix_MusicType Mix_GetMusicType(const Mix_Music *music)
//     Mix_MusicType puede tomar los siguientes valores: 
//       MUS_WAV,
//       MUS_MOD
//       MUS_MID 
//       MUS_OGG 
//       MUS_MP3  Si el valor devuelto es 
//       MUS_NONE significa que no hay ninguna m£sica en reproducci¢n. 
//       MUS_CMD  significa que la reproducci¢n la est  llevando un reproductor externo.   
// -----------------------------------------------------------------------------

