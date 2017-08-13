#ifndef _PGL_VENTANA_
#define _PGL_VENTANA_
#include "PGL_cabeceras.h"
#include "PGL_color.h"
#include "PGL_rectangulo.h"
#include "PGL_tiempo.h"
namespace PGL {

// BANDERAS DE LA VENTANA: ( SDL_WindowFlags )
//  SDL_WINDOW_FULLSCREEN 
//  SDL_WINDOW_FULLSCREEN_DESKTOP
//  SDL_WINDOW_SHOWN
//  SDL_WINDOW_HIDDEN
//  SDL_WINDOW_BORDERLESS
//  SDL_WINDOW_RESIZABLE
//  SDL_WINDOW_MINIMIZED
//  SDL_WINDOW_MAXIMIZED
//  SDL_WINDOW_INPUT_GRABBED
//  SDL_WINDOW_INPUT_FOCUS
//  SDL_WINDOW_MOUSE_FOCUS
//  SDL_WINDOW_MOUSE_CAPTURE
//  SDL_WINDOW_OPENGL
//  SDL_WINDOW_FOREIGN 
//  SDL_WINDOW_ALLOW_HIGHDPI 

// BANDERAS DE RENDERIZADO: ( SDL_RendererFlags )
//  SDL_RENDERER_SOFTWARE		the renderer is a software fallback
//  SDL_RENDERER_ACCELERATED	the renderer uses hardware acceleration
//  SDL_RENDERER_PRESENTVSYNC	present is synchronized with the refresh rate
//  SDL_RENDERER_TARGETTEXTURE	the renderer supports rendering to texture
   
// PROPIEDADES MODO DE MEZCLA: ( SDL_BlendMode )
//  SDL_BLENDMODE_NONE	Sin mezcla.
//  SDL_BLENDMODE_BLEND	Mezcla normal usando el canal Alfa.
//  SDL_BLENDMODE_ADD	Mezcla aditiva.
//  SDL_BLENDMODE_MOD	Mezcla modulada con un color.

// FRECUENCIA DE RENDERIZADO:
// Si se selecciona en banderasRenderizado la opción SDL_RENDERER_PRESENTVSYNC, la frecuencia puede
// ser otra a la dada en Hercios. Comprobarlo con la función "static Uint32 dameHz();"
// Si se quiere a toda costa una frecuencia determinada, no usar SDL_RENDERER_PRESENTVSYNC, pero 
// pueden verse parpadeos indeseados al no estar sincronizado el refresco vertical.

class Ventana {
public:
	
	// Construcción y destrucción:
	Ventana();
	~Ventana();
	bool inicia( 
		const char* titulo,			// El título de la ventana.
		int x,     int y, 			// posición_pixels | SDL_WINDOWPOS_CENTERED | SDL_WINDOWPOS_UNDEFINED 
		int ancho, int alto,		// El ancho y alto en pixels.
		Uint32 banderasVentana,		// SDL_WindowFlags:   SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
		Uint32 banderasRenderizado, // SDL_RendererFlags: SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		Uint32 hz = 50				// Frecuencia de renderizado. Ojo ver nota sobre la frecuencia de renderizado.
	);
	// bool inicia( const char* titulo, int ancho, int alto, Uint32 hz = 50 ); // SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
	void quita();

	// Funciones útiles:	
	bool medidasLogicas( int ancho_logico, int alto_logico );
	void quitaMedidasLogicas();
	bool hayMedidasLogicas() const;
	bool clip( const SDL_Rect* rect ); // NULL para restaurar.
	void titulo( const char* texto );
	bool icono(  SDL_RWops*  imagen16x16, Color* color = NULL );
	bool icono(  const char* imagen16x16, Color* color = NULL );
	void mueve(  int x,     int y    );
	void cambia( int ancho, int alto );
	void maximiza();
	void minimiza();
	void restaura();
	void muestra();
	void oculta();
	void eleva();
	bool cambiaPantallaCompleta();

	// Retornos:
	const char* 	dameTitulo() 		const;
	int 			dameX()				const; 
	int 			dameY() 			const; 
	int 			dameAncho() 		const; 
	int 			dameAlto() 			const; 
	int 			dameAnchoLog() 		const; 
	int 			dameAltoLog() 		const; 
	SDL_BlendMode	dameMezclaModo()	const;
	Color 			dameColor() 	 	const;
	Uint32      	dameID()			const;
	SDL_Window*		ventana() 			const;
	SDL_Renderer*	pantalla() 		 	const;
	bool			pantallaCompleta()  const;
				
	// Funciones para dibujar en modo textura:
	bool tomaMezclaModo( SDL_BlendMode modo );
	bool tomaColor( COLOR color );
	bool tomaColor( Color color );
	void limpia();		// Limpia la pantalla.
	void limpia( COLOR color );
	void limpia( Color color );
	// dibujar...
	void actualiza();	   		// Actualiza la pantalla con todo lo dibujado.
	static Uint32 retarda(); 	// Retarda a los hercios especificados. 
	static Uint32 dameHz(); 	// Los Hz de refresco.
	static float  dameRatio();	// 1000.0f / Hz
	
	// Puntos:
	void punto( const Punto& punto );
	void punto( int x, int y );
 	void punto(  const SDL_Point* punto );
	void puntos( const SDL_Point* puntos, int cantidad );
	
	// Líneas:
	void linea( const Linea& linea );
	void linea( int x1, int y1, int x2, int y2 );
	void linea(  const SDL_Point* punto1, const SDL_Point* punto2 );
	void lineas( const SDL_Point* puntos, int cantidad );

	// Rectángulos:	
	void rectangulo( const Rectangulo& rectangulo );
	void rectangulo( int x, int y, int ancho, int alto );
	void rectangulo(  const SDL_Rect* rectangulo );
	void rectangulos( const SDL_Rect* rectangulos, int cantidad );
	
	// Rectángulos Llenos:
	void rectanguloLleno( const Rectangulo& rectangulo );
	void rectanguloLleno( int x, int y, int ancho, int alto );
	void rectanguloLleno(   const SDL_Rect* rectangulo );
	void rectangulosLlenos( const SDL_Rect* rectangulos, int cantidad );

// ----------------------------------------------------------------------------
//                      I N I C I O   D E   F I G U R A S
// ----------------------------------------------------------------------------
#ifdef PGL_USAR_FIGURAS
	// Cajas: 
	void caja( const Rectangulo& rectangulo, int radio );
	void caja( int x, int y, int ancho, int alto, int radio );
	void cajaLlena( const Rectangulo& rectangulo, int radio );	
	void cajaLlena( int x, int y, int ancho, int alto, int radio );	
	
	// Triángulos:
	void triangulo( const Punto& punto1, const Punto& punto2, const Punto& punto3 );
	void triangulo(      int x1, int y1, int x2, int y2, int x3, int y3 );	
	void triangulo(      const SDL_Point* puntos );
	void trianguloLleno( const Punto& punto1, const Punto& punto2, const Punto& punto3 );
	void trianguloLleno( int x1, int y1, int x2, int y2, int x3, int y3 );
	void trianguloLleno( const SDL_Point* puntos );

	// Polígonos Regulares:
	void poligono( const Punto& punto, int radio, int lados );
	void poligono(      int x, int y, int radio, int lados );
	void poligonoLleno( const Punto& punto, int radio, int lados );
	void poligonoLleno( int x, int y, int radio, int lados );

	// Políogonos Irregulares: 
	void poligono(      const SDL_Point* puntos, int cantidad );
	void poligonoLleno( const SDL_Point* puntos, int cantidad ); // Ojo que esta función es bastante lenta.
		
	// Circulos:	
	void circulo( const Punto& punto, int radio );
	void circulo( int x, int y, int radio );
	void circuloLleno( const Punto& punto, int radio );
	void circuloLleno( int x, int y, int radio );
	
	// Elipses:
	void elipse( const Punto& punto, int rx, int ry );
	void elipse( int x, int y, int rx, int ry );
	void elipseLlena( const Punto& punto, int rx, int ry );
	void elipseLlena( int x, int y, int rx, int ry );
#endif
// ----------------------------------------------------------------------------
//                         F I N   D E  F I G U R A S
// ----------------------------------------------------------------------------

	// Capturas de pantalla:
	// bytesPixel 2 = RGB565, 3 = RGB888, 4 = RGBA8888;
	SDL_Surface* captura( Uint8 bytesPixel ); 
	bool         guardaBMP( const char* archivo, Uint8 bytesPixel );
#ifdef PGL_USAR_SDL_IMAGE
	bool         guardaPNG( const char* archivo, Uint8 bytesPixel );
#endif
	
private:
	int				m_x;
	int				m_y;
	int				m_ancho;					// Ancho en pixels de la pantalla.
	int				m_alto;						// Alto en pixels de la pantalla.
	int				m_ancho_log;				// Ancho lógico en pixels.
	int				m_alto_log;					// Alto lógico en pixels.
	bool			m_log;						// Hay medidas Lógicas.
	bool			m_pantallaCompleta;			//
	Uint32			m_tipoDePantallaCompleta;
	SDL_Window*		m_ventana;
	SDL_Renderer*	m_pantalla;
	static Retardo* m_retardo;
};

}
#endif

