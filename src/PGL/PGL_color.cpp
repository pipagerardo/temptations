#include "PGL_color.h"
namespace PGL {
	
Uint8 Color::m_paleta[16][4] = {
		{   0,   0,   0,   0 }, //  0 Transparente
		{   0,   0,   0, 255 }, //  1 Negro
		{  33, 200,  66, 255 }, //  2 Verde Medio
		{  94, 220, 120, 255 }, //  3 Verde Claro
		{  84,  85, 237, 255 }, //  4 Azul Oscuro
		{ 125, 118, 252, 255 }, //  5 Azul Claro
		{ 212,  82,  77, 255 }, //  6 Rojo Oscuro
		{  66, 235, 245, 255 }, //  7 Cianógeno
		{ 252,  85,  84, 255 }, //  8 Rojo Medio
		{ 255, 121, 120, 255 }, //  9 Rojo Claro
		{ 212, 193,  84, 255 }, // 10 Amarillo Oscuro
		{ 230, 206, 128, 255 }, // 11 Amarillo Claro
		{  33, 176,  59, 255 }, // 12 Verde Oscuro
		{ 201,  91, 186, 255 }, // 13 Magenta
		{ 204, 204, 204, 255 }, // 14 Gris
		{ 255, 255, 255, 255 }  // 15 Blanco
};

Color::Color() {
	quita();
}

Color::Color( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa ) {
	m_rgba[0] = rojo;
	m_rgba[1] = verde;
	m_rgba[2] = azul;
	m_rgba[3] = alfa;
}

Color::Color( COLOR color ) {
	m_rgba[0] = m_paleta[color][0];
	m_rgba[1] = m_paleta[color][1];
	m_rgba[2] = m_paleta[color][2];
	m_rgba[3] = m_paleta[color][3];
}

Color::Color( const Color& color ) {
	m_rgba[0] = color.m_rgba[0];
	m_rgba[1] = color.m_rgba[1];
	m_rgba[2] = color.m_rgba[2];
	m_rgba[3] = color.m_rgba[3];
}

Color::Color( const char*  html  ) {
	quita();
	inicia( html );
}
	
Color::~Color() {
	quita();
}

void Color::inicia( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa ) {
	m_rgba[0] = rojo;
	m_rgba[1] = verde;
	m_rgba[2] = azul;
	m_rgba[3] = alfa;
}
void Color::inicia( COLOR color ) {
	m_rgba[0] = m_paleta[color][0];
	m_rgba[1] = m_paleta[color][1];
	m_rgba[2] = m_paleta[color][2];
	m_rgba[3] = m_paleta[color][3];
}
void Color::inicia( const Color& color ) {
	m_rgba[0] = color.m_rgba[0];
	m_rgba[1] = color.m_rgba[1];
	m_rgba[2] = color.m_rgba[2];
	m_rgba[3] = color.m_rgba[3];
}

Color* Color::operator &() {
	return this;
}

Color& Color::operator = ( const Color& color ) {
	m_rgba[0] = color.m_rgba[0];
	m_rgba[1] = color.m_rgba[1];
	m_rgba[2] = color.m_rgba[2];
	m_rgba[3] = color.m_rgba[3];
	return *this;
}
	
void Color::inicia( const char*  html  ) {
	size_t longitud = SDL_strlen( html );
	size_t inicio   = 0;
	char *p; char str[3] = { '\0' , '\0' , '\0' };
	if( html[0] == '#' ) ++inicio;
	if(( longitud - inicio ) != 6 ) return;
	for( size_t i = 0; i < 3; ++i ) {
		str[0] = html[ inicio++ ];
		str[1] = html[ inicio++ ];
		m_rgba[i] = (Uint8)SDL_strtoul( str, &p, 16 );
	}
	m_rgba[3] = 255;
}
	
void Color::quita() {
	m_rgba[0] = 0;
	m_rgba[1] = 0;
	m_rgba[2] = 0;
	m_rgba[3] = 255;
}

void Color::tomaRojo(  Uint8 rojo  ) { m_rgba[0] = rojo;  }
void Color::tomaVerde( Uint8 verde ) { m_rgba[1] = verde; }
void Color::tomaAzul(  Uint8 azul  ) { m_rgba[2] = azul;  }
void Color::tomaAlfa(  Uint8 alfa  ) { m_rgba[3] = alfa;  }


Uint8 Color::dameRojo()  const { return m_rgba[0]; }
Uint8 Color::dameVerde() const { return m_rgba[1]; }
Uint8 Color::dameAzul()  const { return m_rgba[2]; }
Uint8 Color::dameAlfa()  const { return m_rgba[3]; }
	
Uint8 Color::dameRojo(  COLOR color ) { return m_paleta[color][0]; }
Uint8 Color::dameVerde( COLOR color ) { return m_paleta[color][1]; }
Uint8 Color::dameAzul(  COLOR color ) { return m_paleta[color][2]; }
Uint8 Color::dameAlfa(  COLOR color ) { return m_paleta[color][3]; }

}


