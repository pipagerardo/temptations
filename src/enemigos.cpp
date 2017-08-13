#include "enemigos.h"

Enemigos::Enemigos() {
	m_cantidad = 0;
	m_enemigos = NULL;
	m_x = 130.0f;  
	m_y = 104.0f;
	m_gravedad   = 0.0f;
	m_movimiento = 0.0f;
}

Enemigos::~Enemigos() {
	quita();
}

bool Enemigos::inicia() {
	if( m_cantidad ) quita();
	// CREAMOS LOS ENEMIGOS:
	if( m_xml.map.objectgroup.dameLongitud() == 0 ) return true; 
	PGL::Lista_Iterador< PGL::XML_objectgroup > objectgroup( m_xml.map.objectgroup );
	for( ; objectgroup.noFinal(); ++objectgroup ) {
		// Comprobación de Nombre:
		if( objectgroup->name != "Enemigos" ) continue;
		// BUSCAMOS LA PROPIEDAD MONJE Y LEEMOS LOS VALORES:
		if( objectgroup->property.dameLongitud() ) {	
			PGL::Lista_Iterador< PGL::XML_property > property( objectgroup->property );
			for( ; property.noFinal(); ++property ) {
				if( property->name == "MONJE" ) {
					property->value.sscanf(  "%f,%f,%f,%f", &m_x, &m_y, &m_gravedad, &m_movimiento );
				#ifdef PGL_DEPURANDO
					SDL_LOG( "MONJE x, y, grav, mov = %.2f, %.2f, %.2f, %.2f", m_x, m_y, m_gravedad, m_movimiento );
				#endif
					break;
				}
			}
		}
		// Comprobación de longitud:
		m_cantidad = objectgroup->object.dameLongitud();
		if( m_cantidad == 0 ) return true; 
		// Crear el array:
		m_enemigos = new Enemigos_Datos[ m_cantidad ];
		if( m_enemigos == NULL ) return false;
		#ifdef PGL_DEPURANDO
		else SDL_LOG( "m_enemigos = new Enemigos_Datos[ %u ];", (unsigned int)m_cantidad );
		#endif
		PGL::Lista_Iterador< PGL::XML_object > object( objectgroup->object );
		for( size_t i = 0; object.noFinal(); ++object, ++i ) {
			m_enemigos[ i ].x = object->x;
			m_enemigos[ i ].y = object->y;
			m_enemigos[ i ].ancho = (Uint8)object->width;
			m_enemigos[ i ].alto  = (Uint8)object->height;
			m_enemigos[ i ].y    -= m_enemigos[ i ].alto;
			if     ( object->name == "PAJARO_1"  ) m_enemigos[ i ].tipo = PAJARO_1;
			else if( object->name == "PAJARO_2"  ) m_enemigos[ i ].tipo = PAJARO_2;
			else if( object->name == "COCO_1"    ) m_enemigos[ i ].tipo = COCO_1;
			else if( object->name == "COCO_2"    ) m_enemigos[ i ].tipo = COCO_2;
			else if( object->name == "BLOQUE"    ) m_enemigos[ i ].tipo = BLOQUE;
			else if( object->name == "ESPIRITU"  ) m_enemigos[ i ].tipo = ESPIRITU;
			else if( object->name == "PIRO"      ) m_enemigos[ i ].tipo = PIRO;
			else if( object->name == "VISCOSO"   ) m_enemigos[ i ].tipo = VISCOSO;
			else if( object->name == "MUELLE"    ) m_enemigos[ i ].tipo = MUELLE;
			else if( object->name == "DEMONIO_1" ) m_enemigos[ i ].tipo = DEMONIO_1;
			else if( object->name == "DEMONIO_2" ) m_enemigos[ i ].tipo = DEMONIO_2;
			else if( object->name == "DEMONIO_3" ) m_enemigos[ i ].tipo = DEMONIO_3;
			else if( object->name == "ZIG_ZAG"   ) m_enemigos[ i ].tipo = ZIG_ZAG;
			else if( object->name == "ACUOSO"    ) m_enemigos[ i ].tipo = ACUOSO;
			else if( object->name == "BURBUJA"   ) m_enemigos[ i ].tipo = BURBUJA;
			else if( object->name == "SATANAS"   ) m_enemigos[ i ].tipo = SATANAS;
			// . . .
			else								  m_enemigos[ i ].tipo = NO_ENEMIGO;
			unsigned int s = 1, l = 1; char d = 'I';
			object->type.sscanf(  "S%u,L%u,D%c", &s, &l, &d );
			m_enemigos[ i ].pantalla = (Uint16)s;
			m_enemigos[ i ].vida = (Uint16)l;
			switch( d ) {
				case 'F': m_enemigos[ i ].direccion = FRENTE;    break;
				case 'U': m_enemigos[ i ].direccion = ARRIBA;    break;
				case 'D': m_enemigos[ i ].direccion = ABAJO;     break;
				case 'L': m_enemigos[ i ].direccion = IZQUIERDA; break;
				case 'R': m_enemigos[ i ].direccion = DERECHA;   break;
				default:  m_enemigos[ i ].direccion = IZQUIERDA; break;
			}
		}
 	}
	return true;
}

void Enemigos::quita() {
	if( m_enemigos != NULL ) {
		delete[] m_enemigos;
	#ifdef PGL_DEPURANDO
		SDL_LOG( "delete[] m_enemigos;" );
	#endif
		m_enemigos = NULL;
		m_cantidad = 0;
	}
}

bool Enemigos::dameDatos( Enemigos_Datos &datos ) {
	static size_t cantidad = 0;
	if( cantidad < m_cantidad ) {
		datos = m_enemigos[ cantidad ];
		++cantidad;
		return true;
	} 
	cantidad = 0;
	return false;
}

float Enemigos::dame_x()   const { return m_x; }
float Enemigos::dame_y()   const { return m_y; }
float Enemigos::dame_gra() const { return m_gravedad;   }
float Enemigos::dame_mov() const { return m_movimiento; }

