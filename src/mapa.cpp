#include "mapa.h"

Mapa::Mapa() {
	m_cantidad = 0;
	m_mapa     = NULL;
}

Mapa::~Mapa() {
	quita();
}

bool Mapa::inicia() {

	if( m_cantidad ) quita();
	
	m_cantidad = m_xml.map.layer.dameLongitud();
	if( m_cantidad == 0 ) return false; 
	m_mapa = new Mapa_Datos[ m_cantidad ];
	if( m_mapa == NULL ) return false;
	#ifdef PGL_DEPURANDO
	else SDL_LOG( "m_mapa = new Mapa_Datos[ %u ];", (unsigned int)m_cantidad );
	#endif
	
	m_fondo = m_xml.map.backgroundcolor;
	m_fondo.tomaAlfa( 255 );
	if( ( m_nivel == 4 ) && ( !m_graficos_mejorados ) ) tomaColor( 27, 170, 35 );
	
	PGL::Lista_Iterador< PGL::XML_layer > layer;
	layer.alInicio( (PGL::Lista<PGL::XML_layer> &)m_xml.map.layer );

	for( size_t i = 0; layer.noFinal(); ++layer, ++i ) {
		m_mapa[i].x       = layer->x;
		m_mapa[i].y       = layer->y;
		m_mapa[i].ancho   = layer->width;
		m_mapa[i].alto    = layer->height;
		m_mapa[i].alfa    = ( layer->opacity * 255 );
		m_mapa[i].visible = layer->visible;
		m_mapa[i].dato.inicia( layer->data.array->dameX(), layer->data.array->dameY() );
		for( size_t y = 0; y < layer->data.array->dameY(); ++y ) {
		for( size_t x = 0; x < layer->data.array->dameX(); ++x ) {
			m_mapa[i].dato.toma( x, y, layer->data.array->dame( x, y ) );
		}
		}
	}
			
	return true;
}

void Mapa::quita() {
	if(	m_mapa != NULL ) {
		for( size_t i = 0; i < m_cantidad; ++i ) m_mapa[i].dato.quita();
		delete[] m_mapa;
		#ifdef PGL_DEPURANDO
		SDL_LOG( "delete[] m_mapa;" );
		#endif
		m_mapa     = NULL;
		m_cantidad = 0;
	}
}

size_t Mapa::dameMapas() const {
	return m_cantidad;
}

Uint16 Mapa::dame( size_t mapa, int x, int y ) const {
	if( mapa >= m_cantidad ) return 0;
	return m_mapa[mapa].dato.dame( (int)x, (int)y );
}

void   Mapa::toma( size_t mapa, int x, int y, Uint16 dato )  {
	if( mapa >= m_cantidad ) return;
	m_mapa[mapa].dato.toma( (int)x, (int)y, dato );
}

int Mapa::dameAncho( size_t mapa ) const { return m_mapa[mapa].ancho; }
int Mapa::dameAlto(  size_t mapa ) const { return m_mapa[mapa].alto;  }
PGL::Color Mapa::dameColor()       const { return m_fondo; }
void Mapa::tomaColor( Uint8 rojo, Uint8 verde, Uint8 azul, Uint8 alfa ) {
	m_fondo.inicia( rojo, verde, azul, alfa );
}
	

