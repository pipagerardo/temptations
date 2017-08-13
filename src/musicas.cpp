#include "musicas.h"

Musicas::Musicas() {
	m_mejorada = false;
	m_nivel = 0;
}

Musicas::~Musicas() {
	quita();
}
	
bool Musicas::inicia( bool musica_mejorada, int nivel ) {
	bool retorno = false;
	m_mejorada = musica_mejorada;
	m_nivel = nivel;
	if( m_mejorada ) {
		switch( nivel ) {
			case 0: retorno = m_musica.inicia( "musica/0-Temptations.ogg", 64 ); break;
			case 1: retorno = m_musica.inicia( "musica/1-10_Years_Ago.ogg", 64 ); break;
			case 2: retorno = m_musica.inicia( "musica/2-All_The_Same_Day.ogg", 64 ); break;
			case 3: retorno = m_musica.inicia( "musica/3-On_The_Other_Side.ogg", 64 ); break;
			case 4: retorno = m_musica.inicia( "musica/4-Aquanaut.ogg", 64 ); break;
			case 5: retorno = m_musica.inicia( "musica/5-On_The_Crest_Of_The_Wave.ogg", 64 ); break;
			case 6: retorno = m_musica.inicia( "musica/6-Meeting_Place_II.ogg", 64 ); break;
			case 7: retorno = m_musica.inicia( "musica/7-Meeting_Place.ogg", 64 ); break;
			default: m_musica.quita();  retorno = true; break;
		}
	} else {
		switch( nivel ) {
			case 0: retorno = m_musica.inicia( "musica/0-Temptations.ogg", 64 ); break;
			default: m_musica.quita();  retorno = true; break;
		}
	}

	return retorno;	
}

void Musicas::quita() {
	m_musica.quita();
}

bool Musicas::cambia() {
	if( !inicia( !m_mejorada, m_nivel ) ) return false;
	m_musica.reproduce( -1 );
	return true;
}

void Musicas::reproduce( int veces ) {
	m_musica.reproduce( veces );
}

void Musicas::para() {
	m_musica.para();
}

void Musicas::pausa() {
	m_musica.pausa();
}

void Musicas::continua() {
	m_musica.continua();
}


