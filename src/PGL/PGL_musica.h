#ifndef _PGL_MUSICA_
#define _PGL_MUSICA_
#include "PGL_cabeceras.h"
#ifdef PGL_USAR_SDL_MIXER
namespace PGL {

struct RepMusica {
	Uint8 *buffer; 
	int    capacidad;
	int	   posicion;
	bool   reproduce;
};
	
class Musica {
public:
    Musica();
    Musica( const char* archivo, Uint8 volumen = 64 );
    ~Musica();
    
    bool inicia( const char* archivo, Uint8 volumen = 64 );
    bool inicia( Uint8 *memoria, Uint32 longitud, Uint8 volumen = 64 );
    void quita();
        
    void reproduce( int veces = -1 );
    void para();
    void pausa();
    void continua();
    
    void inicio();
    int  esta_sonando();
    void volumen( Uint8 volumen );
    
private:
    Mix_Music *m_musica; 
    RepMusica  m_repmusica;
};

}
#endif

#endif

