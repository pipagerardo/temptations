#include "baldosas.h"

static const char *graficos_tiles[2] = { 
	"tiles.png",
	"tiles_nenefranz.png"
};

Baldosas::Baldosas() {
	m_total    = 0;
	m_cantidad = 0;
	m_baldosas    = NULL;
	m_propiedades = NULL;
	m_x = 0;
	m_y = 0;
	m_animacion  = 0;
	m_retardo    = 0;
	m_srctiles.x = 0;	
	m_srctiles.y = 0;
	m_srctiles.w = 8;
	m_srctiles.h = 8;
	m_destiles.x = 0;	
	m_destiles.y = 0;
	m_destiles.w = 8;
	m_destiles.h = 8;
	m_ancho      = 32;
}

Baldosas::~Baldosas() {
	quita();
}

bool Baldosas::inicia() {
	if( m_cantidad ) quita();
	
	// CREAMOS LAS BALDOSAS:
	m_cantidad = m_xml.map.tileset.dameLongitud();
	if( m_cantidad == 0 ) return false; 
	
	m_baldosas = new Baldosas_Datos[ m_cantidad ];
	if( m_baldosas == NULL ) return false;
#ifdef PGL_DEPURANDO
	else SDL_LOG( "m_baldosas = new Baldosas_Datos[ %u ];", (unsigned int)m_cantidad );
#endif
	
	PGL::Lista_Iterador< PGL::XML_tileset > tileset;
	tileset.alInicio( (PGL::Lista<PGL::XML_tileset> &)m_xml.map.tileset );

	for( size_t i = 0; tileset.noFinal(); ++tileset, ++i ) {
		m_baldosas[i].primera  = tileset->firstgid;
		m_baldosas[i].cantidad = tileset->tilecount;
		m_baldosas[i].ancho    = tileset->tilewidth;
		m_baldosas[i].alto     = tileset->tileheight;
		m_baldosas[i].espacio  = tileset->spacing;
		m_baldosas[i].margen   = tileset->margin;  
		m_total += tileset->tilecount;
		PGL::Color *color = NULL;
		if( tileset->image.trans.dameAlfa() != 0 ) color = &(tileset->image.trans);
		PGL::Cadena cadena;
		cadena = m_ruta;
		cadena << graficos_tiles[ m_graficos_mejorados ];
		if( !m_baldosas[i].imagen.inicia( m_ventana, cadena.dameCadena(), color ) ) {
		#ifdef PGL_DEPURANDO
			SDL_LOG( "ERROR imagen[%u].inicia = %s\n", (unsigned int)i, cadena.dameCadena() );
		#endif
		}
	}

	m_srctiles.x = 0;	
	m_srctiles.y = 0;
	m_srctiles.w = m_baldosas[0].ancho;
	m_srctiles.h = m_baldosas[0].alto;
	m_destiles.x = 0;	
	m_destiles.y = 0;
	m_destiles.w = m_baldosas[0].ancho;
	m_destiles.h = m_baldosas[0].alto;
	m_ancho = m_baldosas[0].imagen.dameAncho() / m_baldosas[0].ancho;
	
	// CREAMOS LAS PROPIEDADES DE LAS BALDOSAS:
	m_propiedades = new Baldosas_Propiedades[ m_total ];
	if( m_propiedades == NULL ) {
		quita(); 
		return false;
	}
	#ifdef PGL_DEPURANDO
	else SDL_LOG( "m_propiedades = new Baldosas_Propiedades[ %u ];", (unsigned int)m_total );
	#endif

	for( size_t i = 0; i < m_total; ++i ) {
		m_propiedades[i].todas = 0;
	}
	// VAMOS AL INICIO Y COMPROBAMOS SI HAY PROPIEDADES PARA LEER:
	tileset.alInicio();
	if( tileset->tile.dameLongitud() == 0 ) return true;
	// Recorremos todas las Baldosas:
	PGL::Lista_Iterador< PGL::XML_tile > tile( tileset->tile );
	for( ; tile.noFinal(); ++tile ) {
		// Si el identificador de la baldosa es mayor que el array que las contiene.
		if( tile->id >= m_total ) {
		#ifdef PGL_DEPURANDO
			SDL_LOG( "ERROR tile->id >= m_total (%u >= %u)\n", (unsigned int)tile->id, (unsigned int)m_total );
		#endif
			continue;
		}
		// Buscamos en las propiedades "TP":	
		PGL::Lista_Iterador< PGL::XML_property > property( tile->property );
		for( ; property.noFinal(); ++property ) {
			if( property->name == "TP" ) break;
		}
		// Procesamos el valor de la propiedad "TP":
		for( size_t i = 0; i < property->value.dameLongitud(); i++ ) {
			switch( property->value[i] ) {
				case 'T':
					m_propiedades[ tile->id ].arriba    = 1;
					m_propiedades[ tile->id ].abajo     = 1;
					m_propiedades[ tile->id ].derecha   = 1;
					m_propiedades[ tile->id ].izquierda = 1;
				break;
				case 'U': m_propiedades[ tile->id ].arriba    = 1; break;
				case 'D': m_propiedades[ tile->id ].abajo     = 1; break;
				case 'R': m_propiedades[ tile->id ].derecha   = 1; break;
				case 'L': m_propiedades[ tile->id ].izquierda = 1; break;
				case 'K': m_propiedades[ tile->id ].mata      = 1; break;
				case 'O': m_propiedades[ tile->id ].objeto    = 1; break;
				case 'A': m_propiedades[ tile->id ].animada   = 1; break;
				case 'I': m_propiedades[ tile->id ].invisible = 1; break;		
			}
		}
	}
	
	return true;
}

void Baldosas::quita() {
	if(	m_baldosas != NULL ) {
		for( size_t i = 0; i < m_cantidad; ++i ) m_baldosas[i].imagen.quita();
		delete[] m_baldosas;
	#ifdef PGL_DEPURANDO
		SDL_LOG( "delete[] m_baldosas;" );
	#endif
		m_baldosas = NULL;
		m_cantidad = 0;
	}
	if( m_propiedades != NULL ) {
		delete[] m_propiedades;
	#ifdef PGL_DEPURANDO
		SDL_LOG( "delete[] m_propiedades;" );
	#endif
		m_propiedades = NULL;
		m_total       = 0;
	}
}

bool Baldosas::cambia() {
	PGL::Cadena cadena;
	cadena = m_ruta;
	cadena << graficos_tiles[ m_graficos_mejorados ];
	m_baldosas[0].imagen.quita();
	if( !m_baldosas[0].imagen.inicia( m_ventana, cadena.dameCadena() ) ) {
	#ifdef PGL_DEPURANDO
		SDL_LOG( "ERROR imagen = %s\n", cadena.dameCadena() );
	#endif
		return false;
	}
	m_srctiles.x = 0;	
	m_srctiles.y = 0;
	m_srctiles.w = m_baldosas[0].ancho;
	m_srctiles.h = m_baldosas[0].alto;
	m_destiles.x = 0;	
	m_destiles.y = 0;
	m_destiles.w = m_baldosas[0].ancho;
	m_destiles.h = m_baldosas[0].alto;
	m_ancho = m_baldosas[0].imagen.dameAncho() / m_baldosas[0].ancho;
	return true;
}

void Baldosas::tomaPosicion( int x, int y ) {
	m_x = x;
	m_y = y;
}

void Baldosas::actualizaAnimacion() {
	++m_retardo; 
	if( m_retardo > 7 ) {
		m_retardo = 0;
		++m_animacion; 
		if( m_animacion > 7 ) m_animacion = 0;
	}
}

int Baldosas::pinta( Uint16 baldosa, int x, int y ) {
	if( baldosa == 0 ) return 0;
	if( baldosa >= m_total ) return 0;
	baldosa -= m_baldosas[0].primera;
	if( m_propiedades[ baldosa ].invisible ) return 8;
	m_destiles.x = m_x + (x << 3);
	m_destiles.y = m_y + (y << 3);
	m_srctiles.x = ( baldosa % m_ancho ) << 3;
	m_srctiles.y = ( baldosa / m_ancho ) << 3;
	if( m_propiedades[ baldosa ].animada ) m_srctiles.x += ( m_animacion << 3 );	
	m_baldosas[0].imagen.copia( &m_srctiles, &m_destiles );	
	return 8;
}

bool Baldosas::arriba(    Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].arriba    : false; }
bool Baldosas::abajo(     Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].abajo     : false; }
bool Baldosas::derecha(   Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].derecha   : false; }
bool Baldosas::izquierda( Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].izquierda : false; }
bool Baldosas::mata(      Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].mata      : false; }
bool Baldosas::invisible( Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].invisible : false; }
bool Baldosas::objeto(    Uint16 baldosa ) { return (baldosa > 0 ) ? m_propiedades[ --baldosa ].objeto    : false; }

