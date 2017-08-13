#ifndef _PGL_SONIDO_
#define _PGL_SONIDO_
#include "PGL_cabeceras.h"
#ifdef PGL_USAR_SDL_MIXER
namespace PGL {
	
class Sonido {
public:
  	Sonido();
    Sonido( const char* archivo, Uint8 volumen = 64 );
    ~Sonido();

    bool inicia( const char* archivo, Uint8 volumen = 64 );
    bool inicia( Uint8 *memoria, Uint32 longitud, Uint8 volumen = 64 );
    void quita();
    
    // grupo -1 reproduce en el primer canal libre.
	// veces -1 repite infinito.
    void reproduce( int grupo, int veces );
    void para(); 			// Para el canal;
    void para( int grupo ); // Para todo el grupo.
    void pausa();
    void continua();
	bool esta_sonando();
    void volumen( Uint8 volumen );
	
	// Por defecto hay tantos canales como grupos.
	// Es buena práctica reservar canales por grupos.
	bool reserva( int canales ); 			// Reserva un numero de canales al grupo -1;
	bool reserva( int canal, int grupo );	// Un canal al grupo. Si grupo es -1 quita el canal del grupo.
	bool reserva( int del_canal, int al_canal, int grupo ); // Rango de canales al grupo. Si grupo es -1 quita los canales del grupo.
    
private:
    int m_canal;
    Mix_Chunk *m_chunk;
};

}
#endif

#endif

