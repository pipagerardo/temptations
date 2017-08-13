#ifndef _PGL_RECTANGULO_
#define _PGL_RECTANGULO_
#include "PGL_cabeceras.h"

namespace PGL {

// ----------------------------------------------------------------------------
//                                P U N T O
// ----------------------------------------------------------------------------
class Punto {
public:
	Punto();
	Punto( const Punto&     punto );
	Punto( const SDL_Point& punto );
	Punto( int   x, int   y );
	Punto( float x, float y );
	~Punto();

	void inicia();
	void inicia( const Punto&     punto );
	void inicia( const SDL_Point& punto );
	void inicia( int   x, int   y );
	void inicia( float x, float y );
	void quita();
	
	int   dameX()  const;
	int   dameY()  const;
	float dameXF() const;
	float dameYF() const;
	
	void tomaX( int   x );
	void tomaY( int   y );
	void tomaX( float x );
	void tomaY( float y );
	void invierteX();
	void invierteY();
	
	void posicion( int   x, int   y );
	void posicion( float x, float y );
	
	void suma( Punto&     punto );
	void suma( SDL_Point& punto );
	void suma( int   x, int   y );
	void suma( float x, float y );
	
	int   distancia(  Punto& punto );
	float distanciaF( Punto& punto );
	float anguloRadianes( Punto& punto );
	float anguloGrados(   Punto& punto );
	
	Punto& operator = ( const Punto& punto );
	bool  operator == ( Punto& punto ); 
	bool  operator != ( Punto& punto );

	SDL_Point* operator &();
	SDL_Point* punto() const;
	
private:
	friend class Linea;
	// friend class Rectangulo;
	SDL_Point m_punto;
	float m_x;
	float m_y;
};

// ----------------------------------------------------------------------------
//                                 L I N E A
// ----------------------------------------------------------------------------
class Linea {
public:
	Linea();
	Linea( const Linea& linea );
	Linea( const Punto& puntoA, const Punto& puntoB );
	Linea( const SDL_Point& puntoA, const SDL_Point& puntoB );
	Linea( int   x1, int   y1, int   x2, int   y2 );
	Linea( float x1, float y1, float x2, float y2 );
	~Linea();

	void inicia();
	void inicia( const Linea& linea );
	void inicia( const Punto& puntoA, const Punto& puntoB );
	void inicia( const SDL_Point& puntoA, const SDL_Point& puntoB );
	void inicia( int   x1, int   y1, int   x2, int   y2 );
	void inicia( float x1, float y1, float x2, float y2 );
	void quita();	

	Punto&     damePuntoA() const;
	SDL_Point* damePointA() const;
	void damePuntoA( Punto&     puntoA ) const;
	void damePuntoA( SDL_Point& puntoA ) const;
	void damePuntoA( int&   x1, int&   y1  ) const;
	void damePuntoA( float& x1, float& y1  ) const;
		
	Punto&     damePuntoB() const;
	SDL_Point* damePointB() const;
	void damePuntoB( Punto&     puntoB ) const;
	void damePuntoB( SDL_Point& puntoB ) const;
	void damePuntoB( int&   x2, int&   y2  ) const;
	void damePuntoB( float& x1, float& y1  ) const;
			
	void tomaPuntoA( Punto& puntoA );
	void tomaPuntoA( SDL_Point& puntoA );
	void tomaPuntoA( int   x1, int   y1 );
	void tomaPuntoA( float x1, float y1 );
		
	void tomaPuntoB( Punto& puntoB );
	void tomaPuntoB( SDL_Point& puntoB );
	void tomaPuntoB( int   x2, int   y2 );
	void tomaPuntoB( float x1, float y1 );

	int   longitud();
	float longitudF();

	bool estaDentro( Punto& punto );
	bool hayInterseccion( Linea& linea );
			
	Linea& operator  = ( const Linea& linea );
	bool   operator == ( Linea& linea );
	bool   operator != ( Linea& linea );
	
private:
	// friend class Punto;
	Punto m_puntoA;
	Punto m_puntoB;
};

// ----------------------------------------------------------------------------
//                             R E C T A N G U L O
// ----------------------------------------------------------------------------
class Rectangulo {
public:
	Rectangulo();
	Rectangulo( const Rectangulo& rect );
	Rectangulo( const SDL_Rect&   rect );
	Rectangulo( int x, int y, int ancho, int alto );
	Rectangulo( float x, float y, int ancho, int alto );
	~Rectangulo();

	void inicia();
	void inicia( const Rectangulo& rect );
	void inicia( const SDL_Rect&   rect );
	void inicia( int x, int y, int ancho, int alto );
 	void inicia( float x, float y, int ancho, int alto );
	void quita();
	
	int dameX() const;
	int dameY() const;
	float dameXF() const;
	float dameYF() const;	
	int dameAncho() const;
	int dameAlto() const;
	
	void tomaX( int x );
	void tomaY( int y );
 	void tomaX( float x );
 	void tomaY( float y );
	
	void tomaAncho( int ancho );
	void tomaAlto(  int alto );

 	void posicion( Punto& punto );
	void posicion( SDL_Point& punto );	
	void posicion( int x, int y );
	void posicion( float x, float y );
 
 	void suma( Punto& punto );
	void suma( SDL_Point& punto );
	void suma( int x, int y );
	void suma( float x, float y );
	
	bool estaDentro( Punto& punto );
	bool hayInterseccion( Linea& linea );
 	bool hayInterseccion( Rectangulo &rectangulo );
 	
	Rectangulo& operator  = ( const Rectangulo& rect );
	bool        operator == ( Rectangulo& rect );
	bool        operator != ( Rectangulo& rect );
	
	SDL_Rect* operator &();
	SDL_Rect* rectangulo() const;
	
private:
	// friend class punto;
	SDL_Rect m_rectangulo;
	float m_x;
	float m_y;
};

}
#endif

/*
struct SDL_Point {
    int x;
    int y;
} SDL_Point;

struct SDL_Rect {
    int x, y;
    int w, h;
} SDL_Rect;

// Returns true if the rectangle has no area.
SDL_bool SDL_RectEmpty( const SDL_Rect* r );

// Calculate the intersection of two rectangles.
// return SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
SDL_bool SDL_IntersectRect( const SDL_Rect* A, const SDL_Rect* B, SDL_Rect* result );

// Calculate the union of two rectangles.
void SDL_UnionRect( const SDL_Rect* A, const SDL_Rect* B, SDL_Rect* result );

// Calculate a minimal rectangle enclosing a set of points
// return SDL_TRUE if any points were within the clipping rect
SDL_bool SDL_EnclosePoints( const SDL_Point* points, int count, const SDL_Rect* clip, SDL_Rect* result );

// Calculate the intersection of a rectangle and line segment.
// SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
SDL_bool SDL_IntersectRectAndLine( const SDL_Rect* rect, int *X1, int *Y1, int *X2, int *Y2 );
*/


