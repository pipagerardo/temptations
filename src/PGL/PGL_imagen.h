#ifndef _PGL_IMAGEN_
#define _PGL_IMAGEN_
#include "PGL_cabeceras.h"
#include "PGL_color.h"
#include "PGL_ventana.h"
namespace PGL {

// int acceso:
//  SDL_TEXTUREACCESS_STATIC 	// changes rarely, not lockable
//  SDL_TEXTUREACCESS_STREAMING // changes frequently, lockable
//  SDL_TEXTUREACCESS_TARGET	// can be used as a render target

// PROPIEDADES MODO DE MEZCLA: ( SDL_BlendMode )
//  SDL_BLENDMODE_NONE	Sin mezcla.
//  SDL_BLENDMODE_BLEND	Mezcla normal usando el canal Alfa.
//  SDL_BLENDMODE_ADD	Mezcla aditiva.
//  SDL_BLENDMODE_MOD	Mezcla modulada con un color.

// VOLTEO:
// SDL_FLIP_NONE			No hay volteo
// SDL_FLIP_HORIZONTAL		Volteo Horizontal
// SDL_FLIP_VERTICAL		Volteo Vertical
// SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL

class Imagen {
public:
	Imagen();
	~Imagen();
		
	bool inicia( const Ventana& ventana, int ancho, int alto, int acceso, Color* color = NULL );
	bool inicia( const Ventana& ventana, SDL_RWops*  archivo, Color* color = NULL );	
	bool inicia( const Ventana& ventana, const char* archivo, Color* color = NULL );
	void quita();

	bool tomaModo( SDL_BlendMode modo );
	bool tomaColor( COLOR color );
	bool tomaColor( Color color );
	bool tomaAlfa( Uint8 alfa );

	SDL_BlendMode	dameModo()	const;
	Color			dameColor()	const;
	
	int		dameAncho()  	const;
	int		dameAlto() 		const;
	Uint32	dameFormato()	const;
	bool	dameAlfa() 		const;

	bool activaDiana();					// Activa las funciones de dibujo para esta textura.
	bool desactivaDiana();				// Retorna las funciones de dibujo a la ventana.

	bool copia( 
		const SDL_Rect* origen, 		// Rectángulo de origen.  (NULL toda la imagen)
		const SDL_Rect* destino			// Rectángulo de destino. (NULL toda la pantalla)
	);
	bool copia( 
		const SDL_Rect* origen, 		// Rectángulo de origen.  (NULL toda la imagen)
		const SDL_Rect* destino,		// Rectángulo de destino. (NULL toda la pantalla)
		const double angulo,			// Ángulo en grados.
		const SDL_Point *centro,		// Si es NULL rota sobre ( destino.w / 2, destino.h / 2 )-
		const SDL_RendererFlip volteo	// SDL_FLIP_NONE | SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
	);
	
private:
	Ventana*        m_ventana;
	SDL_Texture*	m_textura;
	Uint32			m_formato; 
	int				m_acceso; 
	int				m_ancho; 
	int				m_alto;
};

}
#endif

