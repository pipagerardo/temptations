#include "PGL_ventana.h"
namespace PGL {

Retardo* Ventana::m_retardo = NULL;
	
Ventana::Ventana() {
	m_ancho     = 800;
	m_alto      = 600;
    m_ancho_log = m_ancho;
    m_alto_log  = m_alto;
    m_log       = false;
	m_pantallaCompleta = false;
	m_tipoDePantallaCompleta = SDL_WINDOW_FULLSCREEN_DESKTOP;
	m_ventana       = NULL;
	m_pantalla      = NULL;
}

Ventana::~Ventana() {
	quita();
}

bool Ventana::inicia( 
	const char* titulo, 
	int x, 
	int y, 
	int ancho, 
	int alto, 
	Uint32 banderasVentana,		// SDL_WindowFlags: SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
	Uint32 banderasRenderizado, // SDL_RenderFlags: SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	Uint32 hz
) {

	// Comprobación inicial;
	if( ( m_ventana != NULL ) || ( m_pantalla != NULL ) ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "Ventana ya iniciada.\n" );
	#endif
		return true;	
	}

#ifdef PGL_ANDROID
	{
		SDL_DisplayMode modo;
		SDL_GetCurrentDisplayMode( 0, &modo );
		ancho = modo.w;
		alto = modo.h;
	}
#else
	// En sistemas windows obtenemos las medidas de la pantalla:
	if( ( ancho == 0 ) || ( alto == 0 ) ) {
		SDL_DisplayMode modo;
		SDL_GetCurrentDisplayMode( 0, &modo );
		if( ancho == 0 ) ancho = modo.w;
		if( alto  == 0 ) alto  = modo.h;
	}
#endif
 		
	m_ancho     = ancho;
	m_alto      = alto;
    m_ancho_log = m_ancho;
    m_alto_log  = m_alto;
    m_log       = false;
  	m_pantallaCompleta = false;
	m_tipoDePantallaCompleta = SDL_WINDOW_FULLSCREEN_DESKTOP;
	m_ventana   = NULL;
	m_pantalla  = NULL;
	
	// Creando la ventana
	m_ventana = SDL_CreateWindow(
		titulo,
		x,
		y,
		m_ancho,
		m_alto, 
		banderasVentana
	);
	if( m_ventana == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "SDL_CreateWindow: %s\n", SDL_GetError() );
	#endif
		return false;
	}
	SDL_SetWindowData( m_ventana, "PGL", (void *)this );

	SDL_GetWindowSize(     m_ventana, &m_ancho, &m_alto );
	SDL_GetWindowPosition( m_ventana, &m_x, &m_y );
	m_ancho_log = m_ancho; m_alto_log  = m_alto;
	
	// Creando la pantalla
	m_pantalla = SDL_CreateRenderer( m_ventana, -1, banderasRenderizado );
	if( m_pantalla == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "SDL_CreateRenderer: %s\n", SDL_GetError() );
	#endif
		SDL_DestroyWindow( m_ventana );
		return false;
	}

	// Comprobando banderas:
	banderasVentana = SDL_GetWindowFlags( m_ventana );
	if( banderasVentana & SDL_WINDOW_FULLSCREEN ) {
		m_tipoDePantallaCompleta = SDL_WINDOW_FULLSCREEN;
		m_pantallaCompleta = true;
	}
	if( banderasVentana & SDL_WINDOW_FULLSCREEN_DESKTOP ) {
		m_tipoDePantallaCompleta = SDL_WINDOW_FULLSCREEN_DESKTOP;
		m_pantallaCompleta = true;
	}
	
	// Si no se ha creado el retardo:
	bool configura_pantalla = false;
	if( m_retardo == NULL ) {
		m_retardo = new Retardo( hz );
		configura_pantalla = true;
	} else if( hz != m_retardo->dameHz() ) {
		m_retardo->inicia( hz );
		configura_pantalla = true;
	}

	// Configurando la pantalla:
	if( configura_pantalla ) {
		SDL_DisplayMode modo;
 		SDL_GetWindowDisplayMode( m_ventana, &modo );
	#ifndef PGL_ANDROID
 		modo.w = 1024;
 		modo.h = 768;
 	#endif
 		modo.refresh_rate = hz;
 		if( SDL_SetWindowDisplayMode( m_ventana, &modo ) < 0 ) {
 		#ifdef PGL_DEPURANDO
 			SDL_LOG( "SDL_SetWindowDisplayMode:\n" );
			SDL_LOG( "  w = %d, h = %d, refresh = %d\n", modo.w, modo.h, modo.refresh_rate );
		#endif
		}		
		if( banderasRenderizado & SDL_RENDERER_PRESENTVSYNC ) {
			SDL_GetWindowDisplayMode( m_ventana, &modo );
			m_retardo->inicia( modo.refresh_rate );
		}
	}

	return true;
	
}
	
void Ventana::quita() {
	if( m_retardo  != NULL ) {
		if( SDL_GetWindowID( m_ventana ) == 1 ) {
			delete m_retardo;
			m_retardo  = NULL;
		}
	}
	if( m_pantalla != NULL ) SDL_DestroyRenderer( m_pantalla );
	if( m_ventana  != NULL ) SDL_DestroyWindow(   m_ventana  );
	m_pantalla = NULL;
	m_ventana  = NULL;	
}

bool Ventana::cambiaPantallaCompleta() {
	if( m_pantallaCompleta ) {
		SDL_SetWindowFullscreen( m_ventana, 0 );
		SDL_ShowCursor( 1 );
		m_pantallaCompleta = false;
		SDL_GetWindowPosition( m_ventana, &m_x, &m_y );
	} else {
		SDL_SetWindowFullscreen( m_ventana, m_tipoDePantallaCompleta );
		SDL_ShowCursor( 0 );
		m_pantallaCompleta = true;
	}
	SDL_RenderPresent( m_pantalla );
	return m_pantallaCompleta;
}

bool Ventana::medidasLogicas( int ancho_logico, int alto_logico ) {
/*
	m_ancho_log = ancho_logico;
    m_alto_log  = alto_logico;
    m_log = true;
    SDL_RenderSetIntegerScale( m_pantalla, (SDL_bool)true );
	return SDL_RenderSetLogicalSize( m_pantalla, ancho_logico, alto_logico ) < 0 ? false : true;
*/	

    if( !ancho_logico || !alto_logico ) {
		m_ancho_log = m_ancho;
		m_alto_log  = m_alto;
		m_log = false;
        SDL_RenderSetViewport( m_pantalla, NULL );
        SDL_RenderSetScale( m_pantalla, 1.0f, 1.0f );
        return false;
    }
    
    int w = 1, h = 1;
    float want_aspect;
    float real_aspect;
    float scale;
    int   i_scale;
    float redondeo = 10.0f; // 100.0f;
    SDL_Rect viewport;
	
    if( SDL_GetRendererOutputSize( m_pantalla, &w, &h ) < 0 ) return false;
    m_ancho = w;
    m_alto  = h;
    m_ancho_log = ancho_logico;
    m_alto_log  = alto_logico;

    want_aspect = (float)ancho_logico / alto_logico;
    real_aspect = (float)w / h;

    // Clear the scale because we're setting viewport in output coordinates
    SDL_RenderSetScale( m_pantalla, 1.0f, 1.0f );
    
	// El aspecto es prácticamente el mismo:
    if( SDL_fabs( want_aspect - real_aspect ) < 0.0001 ) {
        scale = (float)w / ancho_logico;
        // Añadido:
		i_scale = (int)SDL_floor( scale * redondeo );
		if( i_scale % 2 ) --i_scale;
		scale = i_scale / redondeo;
        // Fin.
        SDL_RenderSetViewport( m_pantalla, NULL );
        
    // El ancho es correcto:
    } else if( want_aspect > real_aspect ) { // >
		scale = (float)w / ancho_logico;
		// Añadido:
		i_scale = (int)SDL_floor( scale * redondeo );
		if( i_scale % 2 ) --i_scale;
		scale = i_scale / redondeo;
		// Fin.
        viewport.x = 0;
        viewport.w = w;
        viewport.h = (int)SDL_ceil( alto_logico * scale );
		viewport.y = (h - viewport.h) / 2;
        SDL_RenderSetViewport( m_pantalla, &viewport);
        
    // El alto es correcto:
    } else {
        scale = (float)h / alto_logico;
        // Añadido:
		i_scale = (int)SDL_floor( scale * redondeo );
		if( i_scale % 2 ) --i_scale;
		scale = i_scale / redondeo;
        // Fin.
        viewport.y = 0;
        viewport.h = h;
        viewport.w = (int)SDL_ceil( ancho_logico * scale );
        viewport.x = (w - viewport.w) / 2;
        SDL_RenderSetViewport( m_pantalla, &viewport );
    }
    
    // Set the new scale
    SDL_RenderSetScale( m_pantalla, scale, scale );
    m_log = true;
    return true;

}

void Ventana::quitaMedidasLogicas() {
	m_ancho_log = m_ancho;
	m_alto_log  = m_alto;
	m_log = false;
    SDL_RenderSetViewport( m_pantalla, NULL );
    SDL_RenderSetScale( m_pantalla, 1.0f, 1.0f );
}
	
bool Ventana::hayMedidasLogicas() const { return m_log; }
	
bool Ventana::clip( const SDL_Rect* rect ) {
	return SDL_RenderSetClipRect(  m_pantalla, rect ) < 0 ? false : true;
}

bool Ventana::tomaMezclaModo( SDL_BlendMode modo ) {
	return ( SDL_SetRenderDrawBlendMode( m_pantalla, modo ) == 0 ) ? true : false;
}

bool Ventana::tomaColor( COLOR color ) {
	return ( SDL_SetRenderDrawColor( 
		m_pantalla, 
		Color::dameRojo(  color ), 
		Color::dameVerde( color ), 
		Color::dameAzul(  color ), 
		Color::dameAlfa(  color ) 
	) == 0 ) ? true : false;
}
	
bool Ventana::tomaColor( Color color ) {
	return ( SDL_SetRenderDrawColor( 
		m_pantalla, 
		color.dameRojo(), 
		color.dameVerde(), 
		color.dameAzul(), 
		color.dameAlfa() 
	) == 0 ) ? true : false;
}

void Ventana::limpia() {
	SDL_RenderClear( m_pantalla );
}

void Ventana::limpia( COLOR color ) {
	SDL_SetRenderDrawColor( 
		m_pantalla, 
		Color::dameRojo(  color ), 
		Color::dameVerde( color ), 
		Color::dameAzul(  color ), 
		Color::dameAlfa(  color ) 
	);
	SDL_RenderClear( m_pantalla );
}

void Ventana::limpia( Color color  ) {
	SDL_SetRenderDrawColor( 
		m_pantalla, 
		color.dameRojo(), 
		color.dameVerde(), 
		color.dameAzul(), 
		color.dameAlfa() 
	);
	SDL_RenderClear( m_pantalla );
}

void Ventana::actualiza() {
	SDL_RenderPresent( m_pantalla );
}

Uint32 Ventana::retarda() {
	return m_retardo->espera();
}
Uint32 Ventana::dameHz() {
	return m_retardo->dameHz();
}
float Ventana::dameRatio() {
	return m_retardo->dameRatio();
}
	
void Ventana::titulo( const char* texto ) {
	SDL_SetWindowTitle( m_ventana, texto );
}

bool Ventana::icono( SDL_RWops* imagen16x16, Color* color ) {
#ifdef PGL_USAR_SDL_IMAGE
	SDL_Surface* surface = IMG_Load_RW( imagen16x16, 1 );
#else
	SDL_Surface* surface = SDL_LoadBMP_RW( imagen16x16, 1 );
#endif
	if( surface == NULL ) return false;
	if( color ) {
		Uint32 key = SDL_MapRGB( surface->format, color->dameRojo(), color->dameVerde(), color->dameAzul() );
		SDL_SetColorKey( surface, SDL_TRUE, key );
	}
	SDL_SetWindowIcon( m_ventana, surface );
	SDL_FreeSurface( surface );
	return true;	
}
bool Ventana::icono( const char* imagen16x16, Color* color ) {
	return icono( SDL_RWFromFile( imagen16x16, "rb" ), color );
}
	
void Ventana::mueve( int x, int y ) {
	SDL_SetWindowPosition( m_ventana, x, y );
	SDL_GetWindowPosition( m_ventana, &m_x, &m_y );
}

void Ventana::cambia( int ancho, int alto ) {
	SDL_SetWindowSize(     m_ventana, ancho, alto );
	SDL_GetWindowSize(     m_ventana, &m_ancho, &m_alto );
	SDL_GetWindowPosition( m_ventana, &m_x, &m_y );
	if( m_log ) {
		medidasLogicas( m_ancho_log, m_alto_log );
	} else {
		m_ancho_log = m_ancho;
		m_alto_log  = m_alto;
	}
}

void Ventana::maximiza()  { SDL_MaximizeWindow( m_ventana ); }
void Ventana::minimiza()  { SDL_MinimizeWindow( m_ventana ); }
void Ventana::restaura()  { SDL_RestoreWindow(  m_ventana ); }
void Ventana::muestra()   { SDL_ShowWindow(     m_ventana ); }
void Ventana::oculta()    { SDL_HideWindow(     m_ventana ); }
void Ventana::eleva()     { SDL_RaiseWindow(    m_ventana ); }

const char* Ventana::dameTitulo() const {
	return SDL_GetWindowTitle( m_ventana );
}

int Ventana::dameX() 		const { return m_x; }
int Ventana::dameY() 		const { return m_y; }
int Ventana::dameAncho()	const { return m_ancho; }
int Ventana::dameAlto()		const { return m_alto; }
int Ventana::dameAnchoLog() const { return m_ancho_log; }
int Ventana::dameAltoLog()  const { return m_alto_log;  }

SDL_BlendMode Ventana::dameMezclaModo()	const {
	SDL_BlendMode modo;
	SDL_GetRenderDrawBlendMode(  m_pantalla, &modo );
	return modo;
}

Color Ventana::dameColor() const {
	Uint8 r,g,b,a;
	SDL_GetRenderDrawColor( m_pantalla, &r, &g, &b, &a );
	Color color( r, g, b, a );
	return color;
}

Uint32 Ventana::dameID() const {
	return SDL_GetWindowID( m_ventana );
}

SDL_Window*	Ventana::ventana() const { 
	return m_ventana; 
}
	
SDL_Renderer* Ventana::pantalla() const {
	return m_pantalla;
}

bool Ventana::pantallaCompleta() const {
	return m_pantallaCompleta;
} 

// Puntos:	
void Ventana::punto( const Punto& punto ) {
	SDL_RenderDrawPoint( m_pantalla, punto.dameX(), punto.dameY() );
}
void Ventana::punto( int x, int y ) {
	SDL_RenderDrawPoint( m_pantalla, x, y );
}
void Ventana::punto( const SDL_Point* punto ) {
	SDL_RenderDrawPoint( m_pantalla, punto->x, punto->y );
}
void Ventana::puntos( const SDL_Point* puntos, int cantidad ) {
	// if( puntos == NULL ) return;
	// if( cantidad < 1   ) return;
	SDL_RenderDrawPoints( m_pantalla, puntos, cantidad );
}

// Líneas:
void Ventana::linea( const Linea& linea ) {
	this->linea( linea.damePointA(), linea.damePointB() );
}
void Ventana::linea( int x1, int y1, int x2, int y2 ) { 
	SDL_RenderDrawLine( m_pantalla, x1, y1, x2, y2 );
}
void Ventana::linea( const SDL_Point* punto1, const SDL_Point* punto2 ) {
	SDL_RenderDrawLine( m_pantalla, punto1->x, punto1->y, punto2->x, punto2->y );
}
void Ventana::lineas( const SDL_Point* puntos, int cantidad ) {
	// if( puntos == NULL ) return;
	// if( cantidad < 1   ) return;
	SDL_RenderDrawLines( m_pantalla, puntos, cantidad );
}

// Rectángulos:
void Ventana::rectangulo( const Rectangulo& rectangulo ) {
	SDL_RenderDrawRect( m_pantalla, rectangulo.rectangulo() );
}
void Ventana::rectangulo( int x, int y, int ancho, int alto ) {
	SDL_Rect rect = { x, y, ancho, alto };
	SDL_RenderDrawRect( m_pantalla, &rect );
}
void Ventana::rectangulo(  const SDL_Rect* rectangulo ) {
	SDL_RenderDrawRect( m_pantalla, rectangulo );
}
void Ventana::rectangulos( const SDL_Rect* rectangulos, int cantidad ) {
	SDL_RenderDrawRects( m_pantalla, rectangulos, cantidad );
}

// Rectángulos Llenos:
void Ventana::rectanguloLleno( const Rectangulo& rectangulo ) {
	SDL_RenderFillRect( m_pantalla, rectangulo.rectangulo() );
}	
void Ventana::rectanguloLleno( int x, int y, int ancho, int alto ) {
	SDL_Rect rect = { x, y, ancho, alto };
	SDL_RenderFillRect( m_pantalla, &rect );
} 
void Ventana::rectanguloLleno( const SDL_Rect* rectangulo ) {
	SDL_RenderFillRect( m_pantalla, rectangulo );
}
void Ventana::rectangulosLlenos( const SDL_Rect* rectangulos, int cantidad ) {
	SDL_RenderFillRects( m_pantalla, rectangulos, cantidad );
}

// ----------------------------------------------------------------------------
//                      I N I C I O   D E   F I G U R A S
// ----------------------------------------------------------------------------
#ifdef PGL_USAR_FIGURAS

// Cajas: 
void Ventana::caja( const Rectangulo& rectangulo, int radio ) {
	caja( rectangulo.dameX(), rectangulo.dameY(), rectangulo.dameAncho(), rectangulo.dameAlto(), radio );
}	
void Ventana::caja( int x, int y, int ancho, int alto, int radio ) {
	if( radio < 4 ) {
		rectangulo( x, y, ancho, alto );
		return;
	}
	if( ancho > alto ) {
		if( radio > (alto  >> 1) ) radio = alto >> 1;
	} else {
		if( radio > (ancho >> 1) ) radio = ancho >> 1;
	}
	int xl = x + radio;
	int xr = x + ancho - radio;
	int yu = y + radio;
	int yd = y + alto - radio;
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	size_t cuenta = 0, num_puntos = 1 + ( radio * 5.65685f );
	SDL_Point* puntos = new SDL_Point[ num_puntos ];
	if( puntos == NULL ) return;
	SDL_RenderDrawLine( m_pantalla, xl + 1, y, xr, y ); // U
	SDL_RenderDrawLine( m_pantalla, xl, y + alto, xr - 1, y + alto ); // D
	SDL_RenderDrawLine( m_pantalla, x, y + radio, x, yd - 1 ); // L
	SDL_RenderDrawLine( m_pantalla, x + ancho, y + radio, x + ancho, yd - 1 ); // R
	while( yi > xi ) {	
		puntos[ cuenta ].x   = xl - xi; puntos[ cuenta++ ].y = yu - yi; 
		puntos[ cuenta ].x   = xl - yi; puntos[ cuenta++ ].y = yd + xi; 
		puntos[ cuenta ].x   = xr + yi; puntos[ cuenta++ ].y = yd + xi; 
		puntos[ cuenta ].x   = xr + xi; puntos[ cuenta++ ].y = yd + yi; 
		if( xi != 0 ) {
			puntos[ cuenta ].x   = xl - xi; puntos[ cuenta++ ].y = yd + yi;
			puntos[ cuenta ].x   = xl - yi; puntos[ cuenta++ ].y = xl - xi;
			puntos[ cuenta ].x   = xr + xi; puntos[ cuenta++ ].y = yu - yi;
			puntos[ cuenta ].x   = xr + yi; puntos[ cuenta++ ].y = yu - xi;
		}
		if( d < 0 ) {
			d += xi * 2 + 5;
			++xi;
		} else {
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		if( yi == xi ) {
			puntos[ cuenta ].x   = xl - xi; puntos[ cuenta++ ].y = yu - yi;
			puntos[ cuenta ].x   = xl - xi; puntos[ cuenta++ ].y = yd + yi;
			puntos[ cuenta ].x   = xr + xi; puntos[ cuenta++ ].y = yu - yi;
			puntos[ cuenta ].x   = xr + xi; puntos[ cuenta++ ].y = yd + yi;
		}
	}
	// SDL_LOG( "Cuenta %u %u %u %u", radio, cuenta, num_puntos, num_puntos - cuenta );
	if( cuenta > num_puntos ) SDL_LOG( "Error" );
	SDL_RenderDrawPoints( m_pantalla, puntos, cuenta );
	delete[] puntos;	
}

void Ventana::cajaLlena( const Rectangulo& rectangulo, int radio ) {
	cajaLlena( rectangulo.dameX(), rectangulo.dameY(), rectangulo.dameAncho(), rectangulo.dameAlto(), radio );
}	
void Ventana::cajaLlena( int x, int y, int ancho, int alto, int radio ) {
	if( radio < 4 ) {
		rectanguloLleno( x, y, ancho, alto );
		return;
	}
	if( ancho > alto ) {
		if( radio > (alto  >> 1) ) radio = alto >> 1;
	} else {
		if( radio > (ancho >> 1) ) radio = ancho >> 1;
	}
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	int anchura = ancho - (radio << 1);
	int xl = x + radio;
	int yu = y + radio;
	int yd = y + alto - radio;
	size_t cuenta = 0, num_rectangulos = 1 + ( radio << 1 );
	SDL_Rect* rectangulos = new SDL_Rect[ num_rectangulos ];
	if( rectangulos == NULL ) return;
	rectanguloLleno( x, yu + 1, ancho, alto - (radio << 1) );
	while( yi > xi ) {
		if( d < 0 ) {
			rectangulos[ cuenta ].x = xl - yi;  rectangulos[ cuenta   ].y = yu - xi;
			rectangulos[ cuenta ].w = anchura + (yi << 1); rectangulos[ cuenta++ ].h = 1;
			if( xi != 0 ) {
				rectangulos[ cuenta ].x = xl - yi;  rectangulos[ cuenta   ].y = yd + xi;
				rectangulos[ cuenta ].w = anchura + (yi << 1); rectangulos[ cuenta++ ].h = 1;
			}
			d += xi * 2 + 5;
			++xi;
		} else {
			rectangulos[ cuenta ].x = xl - xi;  rectangulos[ cuenta   ].y = yu - yi;
			rectangulos[ cuenta ].w = anchura + (xi << 1); rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = xl - yi;  rectangulos[ cuenta   ].y = yu - xi;
			rectangulos[ cuenta ].w = anchura + (yi << 1); rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = xl - xi;  rectangulos[ cuenta   ].y = yd + yi;
			rectangulos[ cuenta ].w = anchura + (xi << 1); rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = xl - yi;  rectangulos[ cuenta   ].y = yd + xi;
			rectangulos[ cuenta ].w = anchura + (yi << 1); rectangulos[ cuenta++ ].h = 1;
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		if( yi == xi ) {
			rectangulos[ cuenta ].x = xl - xi;  rectangulos[ cuenta   ].y = yu - yi;
			rectangulos[ cuenta ].w = anchura + (xi << 1); rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = xl - xi;  rectangulos[ cuenta   ].y = yd + yi;
			rectangulos[ cuenta ].w = anchura + (xi << 1); rectangulos[ cuenta++ ].h = 1;			
		}
	}
	// SDL_LOG( "Cuenta %u %u %u", radio, cuenta, num_rectangulos );
	if( cuenta > num_rectangulos ) SDL_LOG( "Error" );
	SDL_RenderFillRects( m_pantalla, rectangulos, cuenta );
	delete[] rectangulos;
}

// Triángulos:
void Ventana::triangulo( const Punto& punto1, const Punto& punto2, const Punto& punto3 ) {
	SDL_RenderDrawLine( m_pantalla, punto1.dameX(), punto1.dameY(), punto2.dameX(), punto2.dameY() );
	SDL_RenderDrawLine( m_pantalla, punto2.dameX(), punto2.dameY(), punto3.dameX(), punto3.dameY() );
	SDL_RenderDrawLine( m_pantalla, punto3.dameX(), punto3.dameY(), punto1.dameX(), punto1.dameY() );
}
void Ventana::triangulo( int x1, int y1, int x2, int y2, int x3, int y3 ) {
	SDL_RenderDrawLine( m_pantalla, x1, y1, x2, y2 );
	SDL_RenderDrawLine( m_pantalla, x2, y2, x3, y3 );
	SDL_RenderDrawLine( m_pantalla, x3, y3, x1, y1 );
}
void Ventana::triangulo( const SDL_Point* puntos ) {
	// if( puntos == NULL ) return;
	triangulo( puntos[0].x, puntos[0].y, puntos[1].x, puntos[1].y, puntos[2].x, puntos[2].y );	
}
void Ventana::trianguloLleno( const Punto& punto1, const Punto& punto2, const Punto& punto3 ) {
	trianguloLleno( 
		punto1.dameX(), punto1.dameY(), 
		punto2.dameX(), punto2.dameY(), 
		punto3.dameX(), punto3.dameY()
	);
}
void Ventana::trianguloLleno( int x1, int y1, int x2, int y2, int x3, int y3 ) {
	register int y;
	int  xa, xb;
	// Punto más alto ( x1, y1 ) y el más bajo ( x3, y3 );
	if( y1 > y2 ) {
		xa = x2; xb = y2;
		x2 = x1; y2 = y1; 
		x1 = xa; y1 = xb;
	}
	if( y1 > y3 ) {
		xa = x3; xb = y3;
		x3 = x1; y3 = y1;
		x1 = xa; y1 = xb;	
	}
	if( y2 > y3 ) {
		xa = x3; xb = y3;
		x3 = x2; y3 = y2;
		x2 = xa; y2 = xb;	
	}
	size_t cuenta = 0, num_rectangulos = y3 - y1 + 1;
	SDL_Rect* rectangulos = new SDL_Rect[ num_rectangulos ];
	if( rectangulos == NULL ) return;
	for( y = y1; y <= y3; y++ ) {
		xb = ( (float)(y - y1) / (y3 - y1)) * (x3 - x1) + x1;	
		if( y < y2 ) xa = ( (float)(y - y1) / (y2 - y1)) * (x2 - x1) + x1;
		else 		 xa = ( (float)(y - y2) / (y3 - y2)) * (x3 - x2) + x2;
		rectangulos[ cuenta   ].x = xa; 
		rectangulos[ cuenta   ].y = y; 
		rectangulos[ cuenta   ].w = xb - xa; 
		rectangulos[ cuenta++ ].h = 1;
	}
	if( cuenta > num_rectangulos ) SDL_LOG( "Error" );
	SDL_RenderFillRects( m_pantalla, rectangulos, cuenta );
	delete[] rectangulos;
}
void Ventana::trianguloLleno( const SDL_Point* puntos ) {
	// if( puntos == NULL ) return;
	trianguloLleno( puntos[0].x, puntos[0].y, puntos[1].x, puntos[1].y, puntos[2].x, puntos[2].y );
}

// Polígonos Regulares:
void Ventana::poligono( const Punto& punto, int radio, int lados ) {
	poligono( punto.dameX(), punto.dameY(), radio, lados );
}
void Ventana::poligono( int x, int y, int radio, int lados ) {
	if( lados < 3 ) return;
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	if( lados > radio ) {
		circulo( x, y, radio );
		return;
	}
	size_t num_puntos = lados + 1;
	SDL_Point* puntos = new SDL_Point[ num_puntos ];
	if( puntos == NULL ) return;
	double angulo = 0.0;
	double delta_angulo = M_2PI / (double)lados;
	double radio_double = radio;	
	if( (lados % 4 ) == 0 ) angulo = delta_angulo / 2.0;
	else if( lados % 2 ) 	angulo = M_PI + M_PI2;
	else            		angulo = 0;
	for( size_t i = 0; i < (size_t)lados; i++ ) {
		puntos[ i ].x = x + (int)( radio_double * SDL_cos( angulo ) );
		puntos[ i ].y = y + (int)( radio_double * SDL_sin( angulo ) );
		angulo += delta_angulo;
	}
	puntos[ lados ].x = puntos[0].x;
	puntos[ lados ].y = puntos[0].y;
	SDL_RenderDrawLines( m_pantalla, puntos, num_puntos );
	delete[] puntos;
}
void Ventana::poligonoLleno( const Punto& punto, int radio, int lados ) {
	poligonoLleno( punto.dameX(), punto.dameY(), radio, lados );
}
void Ventana::poligonoLleno( int x, int y, int radio, int lados ) {
	if( lados < 3 ) return;
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	if( lados > radio ) {
		circuloLleno( x, y, radio );
		return;
	}
	double angulo = 0.0;
	double delta_angulo = M_2PI / (double)lados;
	double radio_double = radio;	
	if( (lados % 4 ) == 0 ) angulo = delta_angulo / 2.0;
	else if( lados % 2 ) 	angulo = M_PI + M_PI2;
	else            		angulo = 0;
	int x2 = x + (int)( radio_double * SDL_cos( angulo ) );
	int y2 = y + (int)( radio_double * SDL_sin( angulo ) );
	int x3, y3;
	for( size_t i = 0; i < (size_t)(lados); ++i ) {
		angulo += delta_angulo;
		x3 = x + (int)( radio_double * SDL_cos( angulo ) );
		y3 = y + (int)( radio_double * SDL_sin( angulo ) );
		trianguloLleno( x, y, x2, y2, x3, y3 );
		x2 = x3; y2 = y3;
	}
}

// Polígonos Irregulares:
void Ventana::poligono( const SDL_Point* puntos, int cantidad ) {
	SDL_RenderDrawLines( m_pantalla, puntos, cantidad );
}
int ComparaPuntos( const void *a, const void *b ) {
	return (*(const int *) a) - (*(const int *) b);
}
void Ventana::poligonoLleno( const SDL_Point* puntos, int cantidad ) {
	if( puntos == NULL ) return;
	if( cantidad < 3 ) return;
	register int i, y, miny, maxy;
	int xa, xb;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	// Reserva memoria:
	int *poligonoPuntos = NULL;
	poligonoPuntos = (int*)SDL_malloc( sizeof(int) * cantidad );
	if( poligonoPuntos == NULL ) return;
	// Busca el máximo y el mínimo de y:
	miny = puntos[0].y; maxy = puntos[0].y;
	for( i = 1; ( i < cantidad ); i++ ) {
		if      ( puntos[i].y < miny ) miny = puntos[i].y;
		else if ( puntos[i].y > maxy ) maxy = puntos[i].y;
	}
	// Bucle desde miny a maxy:
	for( y = miny; ( y <= maxy ); y++ ) {
		ints = 0;
		for( i = 0; i < cantidad; i++ ) {
			if( !i ) {
				ind1 = cantidad - 1;
				ind2 = 0;
			} else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = puntos[ind1].y;
			y2 = puntos[ind2].y;
			if( y1 < y2 ) {
				x1 = puntos[ind1].x;
				x2 = puntos[ind2].x;
			} else if( y1 > y2 ) {
				y2 = puntos[ind1].y;
				y1 = puntos[ind2].y;
				x2 = puntos[ind1].x;
				x1 = puntos[ind2].x;
			} else {
				continue;
			}
			if( ((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)) ) {
			// Esto evita usar aritmética en punto flotante:
				poligonoPuntos[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			// Artitmética en punto flotante:
				// poligonoPuntos[ints++] = ( (float)(y - y1) / (y2 - y1)) * (x2 - x1) + x1;
			}
		}
		SDL_qsort( poligonoPuntos, ints, sizeof(int), ComparaPuntos );
		for( i = 0; (i < ints); i += 2 ) {
		// Esto evita usar aritmética en punto flotante:
			xa = poligonoPuntos[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = poligonoPuntos[i+1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			SDL_RenderDrawLine( m_pantalla, xa, y, xb, y );
		// Artitmética en punto flotante:
			// SDL_RenderDrawLine( m_pantalla, poligonoPuntos[i], y, poligonoPuntos[i+1], y );
		}
	}
	SDL_free( poligonoPuntos );
}

// Circulos:
void Ventana::circulo( const Punto& punto, int radio ) {
	circulo( punto.dameX(), punto.dameY(), radio );
}
void Ventana::circulo( int x, int y, int radio ) {
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	size_t cuenta = 0, num_puntos = 1 + ( radio * 5.65685f );
	SDL_Point* puntos = new SDL_Point[ num_puntos ];
	if( puntos == NULL ) return;
	while( yi > xi ) {	
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
		puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y + xi;	
		puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y + xi;
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
		if( xi != 0 ) {
			puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
			puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y - xi;
			puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
			puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y - xi;		
		}
		if( d < 0 ) {
			d += xi * 2 + 5;
			++xi;
		} else {
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		if( yi == xi ) {
			puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
			puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
			puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
			puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
		}	
	}
	// SDL_LOG( "Cuenta %u %u %u %u", radio, cuenta, num_puntos, num_puntos - cuenta );
	if( cuenta > num_puntos ) SDL_LOG( "Error" );
	SDL_RenderDrawPoints( m_pantalla, puntos, cuenta );
	delete[] puntos;
}

void Ventana::circuloLleno( const Punto& punto, int radio ) {
	circuloLleno( punto.dameX(), punto.dameY(), radio );
} 
void Ventana::circuloLleno( int x, int y, int radio ) {
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	size_t cuenta = 0, num_rectangulos = 1 + ( radio << 1 );
	SDL_Rect* rectangulos = new SDL_Rect[ num_rectangulos ];
	if( rectangulos == NULL ) return;
	while( yi > xi ) {
		if( d < 0 ) {
			rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y - xi;
			rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1;
			if( xi != 0 ) {
				rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y + xi;
				rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1;
			}
			d += xi * 2 + 5;
			++xi;
		} else {
			rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y - yi;
			rectangulos[ cuenta ].w = xi << 1; rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y - xi;
			rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y + yi;
			rectangulos[ cuenta ].w = xi << 1; rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y + xi;
			rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1;
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		if( yi == xi ) {
			rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y - yi;
			rectangulos[ cuenta ].w = xi << 1; rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y + yi;
			rectangulos[ cuenta ].w = xi << 1; rectangulos[ cuenta++ ].h = 1;			
		}
	}
	// SDL_LOG( "Cuenta %u %u %u", radio, cuenta, num_rectangulos );
	if( cuenta > num_rectangulos ) SDL_LOG( "Error" );
	SDL_RenderFillRects( m_pantalla, rectangulos, cuenta );
	delete[] rectangulos;
}

void Ventana::elipse( const Punto& punto, int rx, int ry ) {
	elipse( punto.dameX(), punto.dameY(), rx, ry );
} 
void Ventana::elipse( int x, int y, int rx, int ry ) {
	if( rx < 1 ) rx = 1;
	if( ry < 1 ) ry = 1;
	size_t cuenta = 0;
	// size_t num_puntos = 4 + SDL_sqrt( ( rx * rx ) + ( ry * ry ) ) * 4;
	size_t num_puntos = 2 + SDL_sqrt( ( rx * rx ) + ( ry * ry ) ) * 4;
	SDL_Point* puntos = new SDL_Point[ num_puntos ];
	if( puntos == NULL ) return;
	float xi, yi, rx2, ry2, p1, p2;
	puntos[ cuenta ].x = x; puntos[ cuenta++ ].y = y - ry;
	puntos[ cuenta ].x = x; puntos[ cuenta++ ].y = y + ry;
	xi = 0;
	yi = ry;
	rx2 = SDL_pow( rx, 2 );
	ry2 = SDL_pow( ry, 2 );
	p1 = ry2 - ( rx2 * ry ) + ( 0.25f * rx2 );
	while( ( ry2 * xi) < ( rx2 * yi ) ) {
		if( p1 < 0 ) { 
			xi++;
			p1 = p1 + ( 2 * ry2 * xi ) + ry2;
		} else {
           xi++; yi--;
           p1 = p1 + ( 2 * ry2 * xi ) - ( 2 * rx2 * yi ) + ry2;
		}
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
		if( yi != 0 ) {
			puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
			puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
		}
	}
	p2 = (ry2) * SDL_pow( (xi + 0.5), 2 ) + (rx2) * SDL_pow( (yi-1), 2 ) - (rx2 * ry2);
	while( yi > 0 ) {
		if( p2 > 0 ) {
			yi--;
			p2 = p2 - ( 2 * rx2 * yi ) + rx2;
        } else {
			xi++; yi--;
			p2 = p2 + ( 2 * ry2 * xi ) - ( 2 * rx2 * yi ) + rx2;
        }
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
		if( yi != 0 ) {
			puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
			puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
		}
	}
	// SDL_LOG( "Cuenta %u %u %d", cuenta, num_puntos, num_puntos - cuenta );
	if( cuenta > num_puntos ) SDL_LOG( "Error" );
	SDL_RenderDrawPoints( m_pantalla, puntos, cuenta );
	delete[] puntos;
}

void Ventana::elipseLlena( const Punto& punto, int rx, int ry ) {
	elipseLlena( punto.dameX(), punto.dameY(), rx, ry );
} 
void Ventana::elipseLlena( int x, int y, int rx, int ry ) {
	if( rx < 1 ) rx = 1;
	if( ry < 1 ) ry = 1;
	size_t cuenta = 0;
	size_t num_rectangulos = 1 + ( ry << 1 );
	SDL_Rect* rectangulos = new SDL_Rect[ num_rectangulos ];
	if( rectangulos == NULL ) return;
	float xi, yi, rx2, ry2, p1, p2;
	rectangulos[ cuenta ].x = x - rx;  rectangulos[ cuenta   ].y = y;
	rectangulos[ cuenta ].w = rx << 1; rectangulos[ cuenta++ ].h = 1;	
	xi = 0;
	yi = ry;
	rx2 = SDL_pow( rx, 2 );
	ry2 = SDL_pow( ry, 2 );
	p1 = ry2 - ( rx2 * ry ) + ( 0.25f * rx2 );
	while( ( ry2 * xi) < ( rx2 * yi ) ) {
		if( p1 < 0 ) { 
			xi++;
			p1 = p1 + ( 2 * ry2 * xi ) + ry2;
		} else {
			rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y + yi;
			rectangulos[ cuenta ].w = xi + xi; rectangulos[ cuenta++ ].h = 1;
			rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y - yi;
			rectangulos[ cuenta ].w = xi + xi; rectangulos[ cuenta++ ].h = 1;
			xi++; yi--;
			p1 = p1 + ( 2 * ry2 * xi ) - ( 2 * rx2 * yi ) + ry2;
		}		
	}
	p2 = (ry2) * SDL_pow( (xi + 0.5), 2 ) + (rx2) * SDL_pow( (yi-1), 2 ) - (rx2 * ry2);
	while( yi > 0 ) {
		rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y + yi;
		rectangulos[ cuenta ].w = xi + xi; rectangulos[ cuenta++ ].h = 1;
		rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y - yi;
		rectangulos[ cuenta ].w = xi + xi; rectangulos[ cuenta++ ].h = 1;
		if( p2 > 0 ) {
			yi--;
			p2 = p2 - ( 2 * rx2 * yi ) + rx2;
        } else {
			xi++; yi--;
			p2 = p2 + ( 2 * ry2 * xi ) - ( 2 * rx2 * yi ) + rx2;
        }
	}
	// SDL_LOG( "Cuenta %u %u", cuenta, num_rectangulos );
	if( cuenta > num_rectangulos ) SDL_LOG( "Error" );
	SDL_RenderFillRects( m_pantalla, rectangulos, cuenta );
	delete[] rectangulos;
}
#endif
// ----------------------------------------------------------------------------
//                         F I N   D E  F I G U R A S
// ----------------------------------------------------------------------------

SDL_Surface* Ventana::captura( Uint8 bytesPixel ) {
	Uint32 formato;
	switch( bytesPixel ) {
		case 2:
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			formato = SDL_PIXELFORMAT_BGR565;
		#else
			formato = SDL_PIXELFORMAT_RGB565;
		#endif
		break;
		case 3:
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			formato = SDL_PIXELFORMAT_BGR888;
		#else
			formato = SDL_PIXELFORMAT_RGB888;
		#endif
		break;
		case 4:
		default:
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			formato = SDL_PIXELFORMAT_ABGR8888; 
		#else
			formato = SDL_PIXELFORMAT_RGBA8888; 
		#endif
		break;
	}
	// int depth = 8;	
	int bpp;
	Uint32 Rmask, Gmask, Bmask, Amask;
	if( !SDL_PixelFormatEnumToMasks( formato, &bpp, &Rmask, &Gmask, &Bmask, &Amask ) ) {
	#ifdef PGL_DEPURANDO		
		SDL_Log( "SDL_PixelFormatEnumToMasks: %s\n", SDL_GetError() );
	#endif
		return NULL;
	}
	// int bytes = bpp / depth;
	SDL_Rect rect; rect.x = 0; rect.y = 0;
	SDL_GetWindowSize( m_ventana, &rect.w, &rect.h ); // SDL_RenderGetViewport( m_pantalla, &rect );
	SDL_Surface *surface = SDL_CreateRGBSurface( 0, rect.w, rect.h, bpp, Rmask, Gmask, Bmask, Amask );
	if( surface == NULL ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "SDL_CreateRGBSurface: %s\n", SDL_GetError() );
	#endif
		return NULL;
	}
	if( SDL_RenderReadPixels( 
		m_pantalla, &rect, formato, surface->pixels, surface->pitch
	) < 0 ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "SDL_RenderReadPixels: %s\n", SDL_GetError() );
	#endif
		SDL_FreeSurface( surface );
		return NULL;
	}
	return surface;
}

bool Ventana::guardaBMP( const char* archivo, Uint8 bytesPixel ) {
	SDL_Surface* surface = captura( bytesPixel );
	if( surface == NULL ) return false;
	bool retorno = true;
	if( SDL_SaveBMP( surface, archivo ) < 0 ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "SDL_SaveBMP: %s\n", SDL_GetError() );
	#endif
		retorno = false;
	}
	SDL_FreeSurface( surface );
	return retorno;
}

#ifdef PGL_USAR_SDL_IMAGE
bool Ventana::guardaPNG( const char* archivo, Uint8 bytesPixel ) {
	SDL_Surface* surface = captura( bytesPixel );
	if( surface == NULL ) return false;
	bool retorno = true;
	if( IMG_SavePNG( surface, archivo ) < 0 ) {
	#ifdef PGL_DEPURANDO
		SDL_Log( "IMG_SavePNG: %s\n", SDL_GetError() );
	#endif
		retorno = false;
	}
	SDL_FreeSurface( surface );
	return retorno;
}
#endif

}


/*
void Ventana::circulo( int x, int y, int radio ) {
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	size_t cuenta = 0, num_puntos = 13 + ( radio * 5.65685f );
	SDL_Point* puntos = new SDL_Point[ num_puntos ];
	if( puntos == NULL ) return;
	puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
	puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y - xi;
	puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y + xi;				
	puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
	puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
	puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y + xi;
	puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y - xi;	
	puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
	while( yi > xi ) {	
		if( d < 0 ) {
			d += xi * 2 + 5;
			++xi;
		} else {
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
		puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y - xi;
		puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y + xi;				
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
		puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y + xi;
		puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y - xi;	
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;	
	}
	// SDL_LOG( "Cuenta %u %u %u %u", radio, cuenta, num_puntos, num_puntos - cuenta );
	if( cuenta > num_puntos ) SDL_LOG( "Error" );
	SDL_RenderDrawPoints( m_pantalla, puntos, cuenta );
	delete[] puntos;
}
*/

/*
void Ventana::circuloLleno( int x, int y, int radio ) {
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	size_t cuenta = 0, num_puntos = 13 + ( radio * 5.65685424949238f );
	SDL_Point* puntos = new SDL_Point[ num_puntos ];
	if( puntos == NULL ) return;
	puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
	puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
	puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
	puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
	puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y + xi;
	puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y + xi;
	puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y - xi;			
	puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y - xi;
	while( yi > xi ) {
		if( d < 0 ) {
			d += xi * 2 + 5;
			++xi;
		} else {
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y - yi;
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y - yi;
		puntos[ cuenta ].x = x + xi; puntos[ cuenta++ ].y = y + yi;
		puntos[ cuenta ].x = x - xi; puntos[ cuenta++ ].y = y + yi;
		puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y + xi;
		puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y + xi;
		puntos[ cuenta ].x = x + yi; puntos[ cuenta++ ].y = y - xi;			
		puntos[ cuenta ].x = x - yi; puntos[ cuenta++ ].y = y - xi;
	}
	if( cuenta > num_puntos ) SDL_LOG( "Error" );
	SDL_RenderDrawLines( m_pantalla, puntos, cuenta );
	delete[] puntos;
}
*/

/*
void Ventana::circuloLleno( int x, int y, int radio ) {
	if( radio < 2 ) {
		SDL_RenderDrawPoint( m_pantalla, x, y );
		return;
	}
	int xi = 0, yi = radio;
	int d = 1 - radio; // float d = 5.0 / 4 - radio;
	// size_t cuenta = 0, num_rectangulos = 3 + ( radio * 1.414213562373095f );
	size_t cuenta = 0, num_rectangulos = 4 + ( radio * 2.82842712474619f );
	SDL_Rect* rectangulos = new SDL_Rect[ num_rectangulos ];
	if( rectangulos == NULL ) return;
	rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y - xi;
	rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1; // xi << 1;
	while( yi > xi ) {
		if( d < 0 ) {
			d += xi * 2 + 5;
			++xi;
		} else {
			d += (xi - yi) * 2 + 5;
			++xi;
			--yi;
		}
		SDL_LOG( "%d %d", xi, yi );
		rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y - yi;
		rectangulos[ cuenta ].w = xi << 1; rectangulos[ cuenta++ ].h = 1; // yi << 1;
	
		rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y - xi;
		rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1; // xi << 1;
		
		rectangulos[ cuenta ].x = x - xi;  rectangulos[ cuenta   ].y = y + yi;
		rectangulos[ cuenta ].w = xi << 1; rectangulos[ cuenta++ ].h = 1; // yi << 1;
		
		rectangulos[ cuenta ].x = x - yi;  rectangulos[ cuenta   ].y = y + xi;
		rectangulos[ cuenta ].w = yi << 1; rectangulos[ cuenta++ ].h = 1; // xi << 1;	
	
	}
	if( cuenta > num_rectangulos ) SDL_LOG( "Error" );
	SDL_RenderDrawRects( m_pantalla, rectangulos, cuenta );
	delete[] rectangulos;
}
*/

