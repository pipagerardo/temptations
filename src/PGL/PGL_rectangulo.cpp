#include "PGL_rectangulo.h"
#include "PGL_funciones.h"
namespace PGL {

// ----------------------------------------------------------------------------
//                                P U N T O
// ----------------------------------------------------------------------------
Punto::Punto() { quita(); }
Punto::Punto( const Punto& punto ) { inicia( punto ); }
Punto::Punto( const SDL_Point& punto ) { inicia( punto ); }
Punto::Punto( int x, int y ) { inicia( x, y ); }
Punto::Punto( float x, float y ) { inicia( x, y ); }
Punto::~Punto() { quita(); }

void Punto::inicia() { quita(); }
void Punto::inicia( const Punto& punto ) {
	m_punto.x = punto.m_punto.x; 
	m_punto.y = punto.m_punto.y;
	m_x = (float)punto.m_punto.x; 
	m_y = (float)punto.m_punto.y;
}
void Punto::inicia( const SDL_Point& punto ) {
	m_punto.x = punto.x; 
	m_punto.y = punto.y;
	m_x = (float)punto.x;
	m_y = (float)punto.y;
}
void Punto::inicia( int x, int y ) {
	m_punto.x = x;
	m_punto.y = y;
	m_x = (float)x;
	m_y = (float)y;
}
void Punto::inicia( float x, float y ) {
	m_punto.x = (int)x;
	m_punto.y = (int)y;
	m_x = x;
	m_y = y;
}
void Punto::quita() {
	m_punto.x = 0;
	m_punto.y = 0;
	m_x = 0.0f;
	m_y = 0.0f; 
}
	
int   Punto::dameX()  const { return m_punto.x; }
int   Punto::dameY()  const { return m_punto.y; }
float Punto::dameXF() const { return m_x; }
float Punto::dameYF() const { return m_y; }
	
void Punto::tomaX( int x ) { m_punto.x = x; m_x = (float)x; }
void Punto::tomaY( int y ) { m_punto.y = y; m_y = (float)y; }
void Punto::tomaX( float x ) { m_x = x; m_punto.x = (int)x; }
void Punto::tomaY( float y ) { m_y = y; m_punto.y = (int)y; }
void Punto::invierteX() { m_punto.x = -m_punto.x; m_x = -m_x; }
void Punto::invierteY() { m_punto.y = -m_punto.y; m_y = -m_y; }

void Punto::posicion( int x, int y ) { 
	m_punto.x = x; 
	m_punto.y = y; 
	m_x = (float)x; 
	m_y = (float)y;
}
void Punto::posicion( float x, float y ) { 
	m_x = x;
	m_y = y; 
	m_punto.x = (int)x;
	m_punto.y = (int)y;
}

void Punto::suma( Punto& punto ) {
	m_x += punto.m_x;
	m_y += punto.m_y;
	m_punto.x = (int)m_x;
	m_punto.y = (int)m_y;
}
void Punto::suma( SDL_Point& punto ) {
	m_punto.x += punto.x;
	m_punto.y += punto.y;
	m_x = (float)m_punto.x;
	m_y = (float)m_punto.y;	
}
void Punto::suma( int x, int y ) {
	 m_punto.x += x;
	 m_punto.y += y;
	 m_x = (float)m_punto.x;
	 m_y = (float)m_punto.y;
}
void Punto::suma( float x, float y ) {
	 m_x += x;
	 m_y += y;
	 m_punto.x = (int)m_x;
	 m_punto.y = (int)m_y;
}

int Punto::distancia( Punto& punto ) {
	int a = punto.m_punto.y - m_punto.y;
	int b = punto.m_punto.x - m_punto.x;
	return (int)SDL_sqrt( (a * a) + (b * b) );
}
float Punto::distanciaF( Punto& punto ) {
	float a = punto.m_punto.y - m_punto.y;
	float b = punto.m_punto.x - m_punto.x;
	return (float)SDL_sqrt( (a * a) + (b * b) );
}

float Punto::anguloRadianes( Punto& punto ) {
	return (float)SDL_atan2( punto.m_y - m_y, punto.m_x - m_x );
}
float Punto::anguloGrados( Punto& punto ) {
	return Grados( SDL_atan2( punto.m_y - m_y, punto.m_x - m_x ) );
}

Punto& Punto::operator = ( const Punto& punto ) { 
	inicia( punto ); 
	return *this;
}

bool Punto::operator == ( Punto& punto ) { 
	if( punto.m_punto.y != m_punto.y ) return false;
	if( punto.m_punto.x != m_punto.x ) return false;
   	return true; 
}

bool Punto::operator != ( Punto& punto ) { 
	if( ( punto.m_punto.y != m_punto.y ) && ( punto.m_punto.x != m_punto.x ) ) return true;
   	return false; 
}

SDL_Point* Punto::operator &() { return &m_punto; }
SDL_Point* Punto::punto() const { return (SDL_Point*)&m_punto; }

// ----------------------------------------------------------------------------
//                                 L I N E A
// ----------------------------------------------------------------------------
Linea::Linea() { quita(); }
Linea::Linea( const Linea& linea ) { inicia( linea ); }
Linea::Linea( const Punto&     puntoA, const Punto&     puntoB ) { inicia( puntoA, puntoB ); }
Linea::Linea( const SDL_Point& puntoA, const SDL_Point& puntoB ) { inicia( puntoA, puntoB ); }
Linea::Linea( int   x1, int   y1, int   x2, int   y2 ) { inicia( x1, y1, x2, y2 ); }
Linea::Linea( float x1, float y1, float x2, float y2 ) { inicia( x1, y1, x2, y2 ); }
Linea::~Linea() { quita(); }

void Linea::inicia() { quita(); }
void Linea::inicia(	const Linea& linea ) { m_puntoA.inicia( linea.m_puntoA ); m_puntoB.inicia( linea.m_puntoB ); }
void Linea::inicia( const Punto&     puntoA, const Punto&     puntoB ) { m_puntoA.inicia( puntoA ); m_puntoB.inicia( puntoB ); }
void Linea::inicia( const SDL_Point& puntoA, const SDL_Point& puntoB ) { m_puntoA.inicia( puntoA.x, puntoA.y ); m_puntoB.inicia( puntoB.x, puntoB.y ); }
void Linea::inicia( int   x1, int   y1, int   x2, int   y2 ) { m_puntoA.inicia( x1, y1 ); m_puntoB.inicia( x2, y2 ); }
void Linea::inicia( float x1, float y1, float x2, float y2 ) { m_puntoA.inicia( x1, y1 ); m_puntoB.inicia( x2, y2 ); }
void Linea::quita() { m_puntoA.quita(); m_puntoB.quita(); }

Punto&     Linea::damePuntoA() const { return (Punto&)this->m_puntoA; }
SDL_Point* Linea::damePointA() const { return (SDL_Point*)&m_puntoA; }
void Linea::damePuntoA( Punto&     puntoA ) const { puntoA.inicia( m_puntoA ); }
void Linea::damePuntoA( SDL_Point& puntoA ) const { puntoA.x = m_puntoA.dameX(); puntoA.y = m_puntoA.dameY(); }
void Linea::damePuntoA( int&   x1, int&   y1  ) const { x1 = m_puntoA.dameX(); y1 = m_puntoA.dameY(); }
void Linea::damePuntoA( float& x1, float& y1  ) const { x1 = m_puntoA.dameXF(); y1 = m_puntoA.dameYF(); }

Punto&     Linea::damePuntoB() const { return (Punto&)this->m_puntoB; }
SDL_Point* Linea::damePointB() const { return (SDL_Point*)&m_puntoB; }
void Linea::damePuntoB( Punto&     puntoB )    const { puntoB.inicia( m_puntoB ); }
void Linea::damePuntoB( SDL_Point& puntoB )    const { puntoB.x = m_puntoB.dameX(); puntoB.y = m_puntoB.dameY(); }
void Linea::damePuntoB( int&   x2, int&   y2 ) const { x2 = m_puntoB.dameX();  y2 = m_puntoB.dameY();  }
void Linea::damePuntoB( float& x1, float& y1 ) const { x1 = m_puntoB.dameXF(); y1 = m_puntoB.dameYF(); }

void Linea::tomaPuntoA( Punto&     puntoA  ) { m_puntoA.inicia( puntoA ); }
void Linea::tomaPuntoA( SDL_Point& puntoA  ) { m_puntoA.inicia( puntoA ); }
void Linea::tomaPuntoA( int   x1, int   y1 ) { m_puntoA.inicia( x1, y1 ); }
void Linea::tomaPuntoA( float x1, float y1 ) { m_puntoA.inicia( x1, y1 ); }

void Linea::tomaPuntoB( Punto&     puntoB  ) { m_puntoB.inicia( puntoB ); }
void Linea::tomaPuntoB( SDL_Point& puntoB  ) { m_puntoB.inicia( puntoB ); }
void Linea::tomaPuntoB( int   x2, int   y2 ) { m_puntoB.inicia( x2, y2 ); }
void Linea::tomaPuntoB( float x2, float y2 ) { m_puntoB.inicia( x2, y2 ); }

int   Linea::longitud()  { return m_puntoA.distancia(  m_puntoB ); }
float Linea::longitudF() { return m_puntoA.distanciaF( m_puntoB ); }

bool Linea::estaDentro( Punto& punto ) {
	if( m_puntoA == m_puntoB ) return false; 
    if( m_puntoB.m_x > m_puntoA.m_x ) {
		if( punto.m_x < m_puntoA.m_x ) return false;
		if( punto.m_x > m_puntoB.m_x ) return false;
	} else {
		if( punto.m_x < m_puntoB.m_x ) return false;
		if( punto.m_x > m_puntoA.m_x ) return false;
	}
    if( m_puntoB.m_y > m_puntoA.m_y ) {
		if( punto.m_y < m_puntoA.m_y ) return false;
		if( punto.m_y > m_puntoB.m_y ) return false;
	} else {
		if( punto.m_y < m_puntoB.m_y ) return false;
		if( punto.m_y > m_puntoA.m_y ) return false;
	}
    // Calculamos las ecuación de la recta:
	float m = 0.0f;
	if( m_puntoA.m_x != m_puntoB.m_x ) m = ( m_puntoA.m_y - m_puntoB.m_y ) / ( m_puntoA.m_x - m_puntoB.m_x );    
    float b = m_puntoB.m_y - ( m * m_puntoB.m_x ); 
	float d = ( ( m * punto.m_x ) - punto.m_y + b ) / SDL_sqrt( m * m + 1.0f ); 
	if( d == 0.0f ) return true;
	return false;
}
bool Linea::hayInterseccion( Linea& linea ) {
	if( linea == *this ) return true;
	return false;
}


Linea& Linea::operator = ( const Linea& linea ) {
	inicia( linea );
	return *this;
}

bool Linea::operator == ( Linea& linea ) { 
	if( linea.m_puntoA != m_puntoA ) return false;
	if( linea.m_puntoB != m_puntoB ) return false;
   	return true; 
}

bool Linea::operator != ( Linea& linea ) { 
	if( ( linea.m_puntoA != m_puntoA ) && ( linea.m_puntoB != m_puntoB ) ) return true;
   	return false; 
}

// ----------------------------------------------------------------------------
//                             R E C T A N G U L O
// ----------------------------------------------------------------------------
Rectangulo::Rectangulo() { quita(); }
Rectangulo::Rectangulo( const Rectangulo& rect ) { inicia( rect ); }
Rectangulo::Rectangulo( const SDL_Rect&   rect ) { inicia( rect ); }	
Rectangulo::Rectangulo( int x, int y, int ancho, int alto ) { inicia( x, y, ancho, alto ); }
Rectangulo::Rectangulo( float x, float y, int ancho, int alto ) { inicia( x, y, ancho, alto ); }
Rectangulo::~Rectangulo() {	quita(); }

void Rectangulo::inicia() {
	quita();
}
void Rectangulo::inicia( const Rectangulo& rect ) {
	m_rectangulo.x = rect.m_rectangulo.x; m_rectangulo.y = rect.m_rectangulo.y;
	m_rectangulo.w = rect.m_rectangulo.w; m_rectangulo.h = rect.m_rectangulo.h;
	m_x = (float)m_rectangulo.x; m_y = (float)m_rectangulo.y;
}
void Rectangulo::inicia( const SDL_Rect&   rect ) {
	m_rectangulo.x = rect.x; m_rectangulo.y = rect.y;
	m_rectangulo.w = rect.w; m_rectangulo.h = rect.h;
	m_x = (float)m_rectangulo.x; m_y = (float)m_rectangulo.y;
}
void Rectangulo::inicia( int x, int y, int ancho, int alto ) {
	m_rectangulo.x = x; m_rectangulo.y = y;
	m_rectangulo.w = ancho; m_rectangulo.h = alto;
	m_x = (float)x; m_y = (float)y;
}
void Rectangulo::inicia( float x, float y, int ancho, int alto ) {
	m_rectangulo.x = (int)x; m_rectangulo.y = (int)y;
	m_rectangulo.w = ancho; m_rectangulo.h = alto;
	m_x = x; m_y = y;
}
void Rectangulo::quita() {
	m_rectangulo.x = 0; m_rectangulo.y = 0;
	m_rectangulo.w = 0; m_rectangulo.h = 0;
	m_x = 0.0f; m_y = 0.0f;
}

int Rectangulo::dameX() const { return m_rectangulo.x; }
int Rectangulo::dameY() const { return m_rectangulo.y; }
float Rectangulo::dameXF() const { return m_x; }
float Rectangulo::dameYF() const { return m_y; }
int Rectangulo::dameAncho() const { return m_rectangulo.w; }
int Rectangulo::dameAlto()  const { return m_rectangulo.h; }
	
void Rectangulo::tomaX( int x ) { m_rectangulo.x = x; m_x = (float)x; }
void Rectangulo::tomaY( int y ) { m_rectangulo.y = y; m_y = (float)y; }
void Rectangulo::tomaX( float x ) { m_rectangulo.x = (int)x; m_x = x; }
void Rectangulo::tomaY( float y ) { m_rectangulo.y = (int)y; m_y = y; }

void Rectangulo::tomaAncho( int ancho ) { m_rectangulo.w = ancho; }
void Rectangulo::tomaAlto(  int alto  ) { m_rectangulo.h = alto; }

void Rectangulo::posicion( Punto& punto ) {
	m_rectangulo.x = punto.dameX(); m_rectangulo.y = punto.dameY();
	m_x = punto.dameXF(); m_y = punto.dameXF(); 
}
void Rectangulo::posicion( SDL_Point& punto ) {
	m_rectangulo.x = punto.x; m_rectangulo.y = punto.y;
	m_x = (float)m_rectangulo.x; m_y = (float)m_rectangulo.y;
}
void Rectangulo::posicion( int x, int y ) { 
	m_rectangulo.x = x; m_rectangulo.y = y;
	m_x = (float)x; m_y = (float)y;
}
void Rectangulo::posicion( float x, float y ) { 
	m_rectangulo.x = (int)x; m_rectangulo.y = (int)y;
	m_x = x; m_y = y;
}

void Rectangulo::suma( Punto& punto ) {
	m_x += punto.dameXF(); 
	m_y += punto.dameYF(); 
	m_rectangulo.x = (int)m_x;
	m_rectangulo.y = (int)m_y;
}
void Rectangulo::suma( SDL_Point& punto ) {
	m_rectangulo.x += punto.x;
	m_rectangulo.y += punto.y;
	m_x = (float)m_rectangulo.x;
	m_y = (float)m_rectangulo.y;
}
void Rectangulo::suma( int x, int y ) {
	m_rectangulo.x += x; 
	m_rectangulo.y += y;
	m_x = (float)m_rectangulo.x;
	m_y = (float)m_rectangulo.y;
}
void Rectangulo::suma( float x, float y ) {
	m_x += x;
	m_y += y;
	m_rectangulo.x = (int)x;
	m_rectangulo.y = (int)y;
}

bool Rectangulo::estaDentro( Punto& punto ) {
	return (bool)SDL_PointInRect( &punto, &m_rectangulo );
}

bool Rectangulo::hayInterseccion( Linea& linea ) {
	int X1, Y1, X2, Y2;
	linea.damePuntoA( X1, Y1 ); linea.damePuntoB( X2, Y2 );
	return (bool)SDL_IntersectRectAndLine( &m_rectangulo, &X1, &Y1, &X2, &Y2 );
}

bool Rectangulo::hayInterseccion( Rectangulo &rectangulo ) {
	return (bool)SDL_HasIntersection( &m_rectangulo, &rectangulo );	
}




Rectangulo& Rectangulo::operator = ( const Rectangulo& rect ) { inicia( rect ); return *this; }

bool Rectangulo::operator == ( Rectangulo& rect ) {
	return (bool)SDL_RectEquals( &m_rectangulo, &rect.m_rectangulo );
}
bool Rectangulo::operator != ( Rectangulo& rect ) {
	return (bool)!SDL_RectEquals( &m_rectangulo, &rect.m_rectangulo );
}

SDL_Rect* Rectangulo::operator &() { return &m_rectangulo; }
SDL_Rect* Rectangulo::rectangulo() const { return (SDL_Rect*)&m_rectangulo; }
	
}


