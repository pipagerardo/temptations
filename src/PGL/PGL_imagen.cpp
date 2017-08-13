#include "PGL_imagen.h"
namespace PGL {
	
Imagen::Imagen() {
	m_ventana = NULL;
	m_textura = NULL;
}

Imagen::~Imagen() {
	quita();
}

bool Imagen::inicia( const Ventana& ventana, int ancho, int alto, int acceso, Color* color ) {
	m_ventana = (Ventana*)&ventana;
    bool alfa = false;
	if( color ) {
		if( color->dameAlfa() != 255 ) alfa = true;
	}
	SDL_RendererInfo info;
	SDL_GetRendererInfo( m_ventana->pantalla(), &info );
    Uint32 formato = info.texture_formats[0];
    for( Uint32 i = 0; i < info.num_texture_formats; ++i ) {
        if( !SDL_ISPIXELFORMAT_FOURCC( info.texture_formats[i] ) &&
            SDL_ISPIXELFORMAT_ALPHA( info.texture_formats[i] ) == alfa ) {
            formato = info.texture_formats[i];
            break;
        }
    }
	m_textura = SDL_CreateTexture( m_ventana->pantalla(), formato, acceso, ancho, alto );
 	if( m_textura == NULL ) return false;
	if( color ) {
		tomaColor( *color );
		if( alfa ) tomaModo( SDL_BLENDMODE_BLEND );
	}
    return true;
}

bool Imagen::inicia( const Ventana& ventana, SDL_RWops* archivo, Color* color ) {
	quita();
	m_ventana = (Ventana*)&ventana;
#ifdef PGL_USAR_SDL_IMAGE
	SDL_Surface* surface = IMG_Load_RW( archivo, 1 );
#else
	SDL_Surface* surface = SDL_LoadBMP_RW( archivo, 1 );
#endif
	if( surface == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "IMG_LoadRW: %s\n", SDL_GetError() );
	#endif
		return false;
	}
	if( color ) {
		Uint32 key = SDL_MapRGB( surface->format, color->dameRojo(), color->dameVerde(), color->dameAzul() );
		SDL_SetColorKey( surface, SDL_TRUE, key );
	}
	m_textura = SDL_CreateTextureFromSurface( ventana.pantalla(), surface );
	SDL_FreeSurface( surface );
	if( m_textura == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "SDL_CreateTextureFromSurface: %s\n", SDL_GetError() );
	#endif
		return false;
	}
	SDL_QueryTexture( m_textura, &m_formato,  &m_acceso, &m_ancho, &m_alto );
	return true;
}

bool Imagen::inicia( const Ventana& ventana, const char* archivo, Color* color ) {
	return inicia( ventana, SDL_RWFromFile( archivo, "rb" ), color );
}

void Imagen::quita() {	
	if( m_textura != NULL ) SDL_DestroyTexture( m_textura );
	m_textura = NULL;
	// m_ventana = NULL;
}

bool Imagen::tomaModo( SDL_BlendMode modo ) { 
	return ( SDL_SetTextureBlendMode( m_textura, modo ) == 0 ) ? true : false;
}

bool Imagen::tomaColor( COLOR color ) { 
	int ret = SDL_SetTextureColorMod( 
		m_textura, 
		Color::dameRojo(  color ),
		Color::dameVerde( color ),
		Color::dameAzul(  color )
	);
	if( ret != 0 ) return false;
	ret = SDL_SetTextureAlphaMod( m_textura, Color::dameAlfa( color ) );
	return ( ret == 0 ) ? true : false;
}

bool Imagen::tomaColor( Color color ) { 
	int ret = SDL_SetTextureColorMod( 
		m_textura, 
		color.dameRojo(), 
		color.dameVerde(), 
		color.dameAzul() 
	);
	if( ret != 0 ) return false;
	ret = SDL_SetTextureAlphaMod( m_textura, color.dameAlfa() );
	return ( ret == 0 ) ? true : false;
}

bool Imagen::tomaAlfa( Uint8 alfa ) { 
	return SDL_SetTextureAlphaMod( m_textura, alfa ) == 0 ? true : false;
}
	
SDL_BlendMode Imagen::dameModo() const { 
	SDL_BlendMode modo;
	SDL_GetTextureBlendMode( m_textura, &modo );
	return modo;
}

Color Imagen::dameColor() const {
	Uint8 r,g,b,a;
	SDL_GetTextureColorMod( m_textura, &r, &g, &b );
	SDL_GetTextureAlphaMod( m_textura, &a );
	Color color( r, g, b, a );
	return color;
}

int Imagen::dameAncho() const { 
	return m_ancho; 
}

int Imagen::dameAlto() const { 
	return m_alto;
}

Uint32 Imagen::dameFormato() const {
	return m_formato;
}

bool Imagen::dameAlfa() const { 
	return SDL_ISPIXELFORMAT_ALPHA( m_formato );
}

bool Imagen::activaDiana() {
	return( SDL_SetRenderTarget( m_ventana->pantalla(), m_textura ) == 0 ) ? true :false;
}
bool Imagen::desactivaDiana() {
	return( SDL_SetRenderTarget( m_ventana->pantalla(), NULL ) == 0 ) ? true :false;
}

bool Imagen::copia( const SDL_Rect* fuente, const SDL_Rect* destino ) {
	return (SDL_RenderCopy( m_ventana->pantalla(), m_textura, fuente, destino) == 0) ? true : false;
}

bool Imagen::copia( 
	const SDL_Rect* origen, 		// Rectángulo de origen.
	const SDL_Rect* destino,		// Rectángulo de destino.
	const double angulo,			// Ángulo en grados.
	const SDL_Point *centro,		// Si es NULL rota sobre ( destino.w / 2, destino.h / 2 )-
	const SDL_RendererFlip volteo	// SDL_FLIP_NONE | SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
) {
	return (SDL_RenderCopyEx( m_ventana->pantalla(), m_textura, origen, destino, angulo, centro, volteo ) == 0) ? true : false;
}

}

	/*
	quita();
	m_ventana = (Ventana*)&ventana;
	#ifdef PGL_USAR_SDL_IMAGE
	SDL_Surface* surface = IMG_Load( archivo );
	#else
	SDL_Surface* surface = SDL_LoadBMP( archivo );
	#endif
	if( surface == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "IMG_Load: %s\n", SDL_GetError() );
	#endif
		return false;	
	}
	if( color ) {
		Uint32 key = SDL_MapRGB( surface->format, color->dameRojo(), color->dameVerde(), color->dameAzul() );
		SDL_SetColorKey( surface, SDL_TRUE, key );
	}
	m_textura = SDL_CreateTextureFromSurface( ventana.pantalla(), surface );
	SDL_FreeSurface( surface );
	if( m_textura == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "SDL_CreateTextureFromSurface: %s\n", SDL_GetError() );
	#endif
		return false;
	}
	SDL_QueryTexture( m_textura, &m_formato, &m_acceso, &m_ancho, &m_alto );
	return true;
	*/

/*
int SDL_GetRendererInfo( SDL_Renderer* renderer, SDL_RendererInfo* info );
SDL_RendererInfo* info {
	const char* name;
	Uint32 flags;	// SDL_RENDERER_SOFTWARE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
	Uint32 num_texture_formats;
	Uint32[16] texture_formats;
	int max_texture_width;
 	int max_texture_height;
}
*/


/*
    SDL_bool needAlpha;     
	const SDL_PixelFormat *fmt = surface->format;
    if( fmt->Amask || SDL_GetColorKey( surface, NULL ) == 0 ) needAlpha = SDL_TRUE;
   	else needAlpha = SDL_FALSE;
*/  	


