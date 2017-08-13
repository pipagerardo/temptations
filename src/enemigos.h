/*
Esta clase debe importar de mapa TMX los enemigos.

NAME:
	EXPLOSION
	
	PAJARO_1		VERDE
	VAJARO_2		ROJO		?? ARRIBA - ABAJO
	COCO_1			AZUL
	COCO_2			ROJO		SALTARIN
	BLOQUE
	ESPIRITU
	PIRO			SIEMPRE DL
	VISCOSO			Igual que COCO_1
	MUELLE			Parecido a COCO_2
	DEMONIO_1		VERDE	Igual que pájaro 1
	DEMONIO_2		ROJO	Igual que pájaro 2
	DEMONIO_3		MAGENTA Se mueve en circulos.
	ZIG_ZAG         EXPLOSION_MOVIL En zig-zag
	ACUOSO			Izquierda-Derecha con pequeños saltitos
	BURBUJA			Bloque.
	SATANAS

TYPE:
	S001	El número de la pantalla en la que aparece.
	L001	La cantidad de vida del enemigo, 999 infinita.
	DL		Dirección U D L R por defecto DL.
	
*/

#ifndef _ENEMIGOS_
#define _ENEMIGOS_
#include "PGL/PGL.h"
#include "basePGL.h"

enum Enumera_Enemigo { 
	NO_ENEMIGO, 
	PAJARO_1, 
	PAJARO_2, 
	COCO_1, 
	COCO_2, 
	BLOQUE,
	ESPIRITU,
	PIRO,
	VISCOSO,
	MUELLE,
	DEMONIO_1,
	DEMONIO_2,
	DEMONIO_3,
	ZIG_ZAG,
	ACUOSO,
	BURBUJA,
	SATANAS,
	BOLA_FUEGO
};

struct Enemigos_Datos {
	float   x;
	float   y;
	Uint8	ancho;
	Uint8	alto;
	Enumera_Enemigo	tipo;
	Uint16	pantalla;
	Uint16  vida;
	Enumera_Direccion direccion;
};

class Enemigos : public BasePGL {
public:
	
	Enemigos();
	~Enemigos();
	
	bool inicia(); // Lee los enemigos del mapa TMX y los guarda.
	void quita();

	bool dameDatos( Enemigos_Datos &datos );
	
	float dame_x()   const;
	float dame_y()   const;
	float dame_gra() const;
	float dame_mov() const;

private:
	size_t 			m_cantidad;
	Enemigos_Datos *m_enemigos;
	// Posición y movimiento de Noni:
	float m_x;  
	float m_y;
	float m_gravedad;
	float m_movimiento;
};

#endif
