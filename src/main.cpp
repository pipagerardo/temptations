// ----------------------------------------------------------------------------
// Temptations Remake 2015 - PIPAGERARDO -  GPL v3 license 
// Versi�n: 				1.5.0.0
// Fecha: 					26-02-2016
// Dise�o Original:			LuigiL�pez 		Luis L�pez Navarro
// Gr�ficos Originales:		LuigiL�pez 		Luis L�pez Navarro
// Programaci�n Original:	LuigiL�pez 		Luis L�pez Navarro
// M�sica Original:			Gominolas		C�sar Astudillo 
// Pantalla de Carga:		Quick-fingers	Julio Mart�n Erro
// Car�tula:				Azpiri 			Alfonso Azpiri Mej�a
// Gr�ficos Mejorados:		Nene Franz		Francisco Javier Wis Gil
// M�sica Mejorada:			Snabisch		Victor Navarro
// Traducci�n:				Gravitonio		Antonio E.
// Programaci�n Remake:		PipaGerardo		Gerardo Herce Ripa
// Snabisch:				https://makeagame.bandcamp.com/
// Temptations Remake:		https://sites.google.com/site/temptationsremake/
// PipaGerardo:				https://sites.google.com/site/pipagerardo/
// E-Mail:	pipagerardo@gmail.com	pipagerardo@hotmail.es
// ----------------------------------------------------------------------------

#include "PGL/PGL.h"	// PipaGerardo Library, encapsula SDL2 y m�s...
#include "juego.h"		// Este juego propiamente dicho.

#ifndef PGL_ANDROID	
int main() {
#else
int main( int argc, char* args[] ) {
#endif

#ifdef PGL_DEPURANDO
	SDL_LOG( "-------------------------------------" );
	SDL_LOG( "VERSION DE DESARROLLO, NO DISTRIBUIR." );
	SDL_LOG( "CONTROL:\tIZQUIERDA /DERECHA" );
	SDL_LOG( "SALTAR:\tARRIBA / SHIFT" );
	SDL_LOG( "DISPARAR:\tCTRL / ESPACIO" );
	SDL_LOG( "G\t\tGRAFICOS" );
	SDL_LOG( "M\t\tMUSICA" );
	SDL_LOG( "F\t\tPANTALLA COMPLETA" );
	SDL_LOG( "P\t\tPAUSA" );
	SDL_LOG( "L\t\tIDIOMA" );	
	SDL_LOG( "F1\tSIGUIENTE NIVEL" );
	SDL_LOG( "F2\tANDA/VUELA/NADA" );
	SDL_LOG( "I\t\tINVENCIBLE" );
	SDL_LOG( "-------------------------------------" );
#endif

	Juego temptations;
	if( !temptations.inicia() ) return -1;	// Mal asunto.
	temptations.juega();
	temptations.quita();
	
	return 0;	// Todo bien.
}

