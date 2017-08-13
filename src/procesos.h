/*
Debe gestionar los enemigos y las colisiones.
NAME:
	EXPLOSION
	ZIG_ZAG
	PAJARO_1				VERDE
	PAJARO_2				ROJO		?? ARRIBA - ABAJO
	COCO_1					AZUL
	COCO_2					ROJO		SALTARIN
	BLOQUE
	ESPIRITU
	PIRO					SIEMPRE DL
	VISCOSO
	MUELLE
	DEMONIO_1				VERDE O ROJO
	DEMONIO_2				MAGENTA
	ACUOSO
	BURBUJA
	SATANAS
	BOLA_FUEGO
*/
#ifndef _PROCESOS_
#define _PROCESOS_
#include "PGL/PGL.h"
#include "baseTMP.h"
#include "monje.h"
#include "disparos.h"

// ----------------------------------------------------------------------------
// PROCESO:
// ----------------------------------------------------------------------------
class Proceso : public BaseTMP {
public:
	Proceso();
	virtual ~Proceso();
	virtual bool colision( int px, int py );
	virtual void mueve() = 0;
	virtual void dibuja();
	Uint16 dameVida() const;
	void   restaVida( Uint16 cantidad );
protected:
	friend class Procesos;
	float   m_x;
	float   m_y;
	float	m_movimiento;
	Uint8	m_ancho;
	Uint8	m_alto;
	Enumera_Direccion m_direccion;
	Uint16  m_vida;			// 0 = muerto, inmortal > 999
	bool	m_borde;		// cambia de dirección en los bordes de la pantalla.
	Uint16  m_x_sprite;
	Uint16  m_y_sprite;
	bool    m_delado;		// Si la animación es vista de lado.
	Uint8  	m_num_sprites;	
	Uint8   m_cuenta;
	Uint8   m_cuenta_num;
	int     m_animacion; 
};
// ----------------------------------------------------------------------------
// EXPLOSION:
// ----------------------------------------------------------------------------
class Explosion : public Proceso {
public:
	Explosion( float x, float y );
	void mueve();
private:
	Uint8 m_recuenta;
};
// ----------------------------------------------------------------------------
// BLOQUE: Sólo se mueve arriba-abajo u derecha-izquierda.
// ----------------------------------------------------------------------------
class Bloque : public Proceso {
public:
	Bloque();
	Bloque( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	virtual void mueve();
};
// ----------------------------------------------------------------------------
// PAJARO_1:
// ----------------------------------------------------------------------------
class Pajaro_1 : public Proceso {
public:
	Pajaro_1();
	Pajaro_1( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	virtual void mueve();
protected:
	float m_y_copia;
	float m_radianes;
	float m_inc;
	float m_mul;
};
// ----------------------------------------------------------------------------
// PAJARO_2:
// ----------------------------------------------------------------------------
class Pajaro_2 : public Pajaro_1  {
public:
	Pajaro_2( float x, float y, Uint16 vida, Enumera_Direccion direccion );
};
// ----------------------------------------------------------------------------
// COCO_1:
// ----------------------------------------------------------------------------
class Coco_1 : public Bloque  {
public:
	Coco_1( float x, float y, Uint16 vida, Enumera_Direccion direccion );
};
// ----------------------------------------------------------------------------
// COCO_2:
// ----------------------------------------------------------------------------
class Coco_2 : public Proceso {
public:
	Coco_2( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
private:
	float m_y_copia;
	bool  m_mueve;
	float m_radianes;
	float m_inc; 
	float m_mul;
};
// ----------------------------------------------------------------------------
// ESPIRITU:
// ----------------------------------------------------------------------------
class Espiritu : public Proceso {
public:
	Espiritu( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
protected:
	float m_y_copia;
	float m_radianes;
	float m_inc;
	float m_mul;
	Uint8 m_recuenta;
};
// ----------------------------------------------------------------------------
// PIRO:
// ----------------------------------------------------------------------------
class Piro : public Bloque {
public:
	Piro( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
protected:
	float m_x_copia;
	Uint8 m_recuenta;
};
// ----------------------------------------------------------------------------
// VISCOSO:
// ----------------------------------------------------------------------------
class Viscoso : public Bloque  {
public:
	Viscoso( float x, float y, Uint16 vida, Enumera_Direccion direccion );
};
// ----------------------------------------------------------------------------
// MUELLE:
// ----------------------------------------------------------------------------
class Muelle : public Proceso {
public:
	Muelle( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
	void dibuja();
protected:
	Uint8 m_recuenta;
	float m_y_copia;
	bool  m_mueve;
	float m_radianes;
	float m_inc; 
	float m_mul;
};
// ----------------------------------------------------------------------------
// DEMONIO_1:
// ----------------------------------------------------------------------------
class Demonio_1 : public Pajaro_1  {
public:
	Demonio_1( float x, float y, Uint16 vida, Enumera_Direccion direccion );
};
// ----------------------------------------------------------------------------
// DEMONIO_2:
// ----------------------------------------------------------------------------
class Demonio_2 : public Proceso  {
public:
	Demonio_2( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
protected:
	float m_y_copia;
	float m_x_copia;
	float m_radianes;
	float m_inc;
	float m_mul;
};
// ----------------------------------------------------------------------------
// ZIG_ZAG          EXPLOSION_MOVIL En zig-zag
// ----------------------------------------------------------------------------
class Zig_Zag : public Proceso {
public:
	Zig_Zag( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
private:
	float m_y_copia;
	float m_y_tope;
	float m_mov_verti;
};
// ----------------------------------------------------------------------------
// ACUOSO			Izquierda-Derecha con pequeños saltitos
// ----------------------------------------------------------------------------
class Acuoso : public Proceso  {
public:
	Acuoso( float x, float y, Uint16 vida, Enumera_Direccion direccion );
	void mueve();
private:
	Uint8 m_recuenta;
	float m_y_copia;		
	float m_mul;
	float m_radianes;		
	float m_inc; 
};
// ----------------------------------------------------------------------------
// BURBUJA: Siempre de abajo a arriba.
// ----------------------------------------------------------------------------
class Burbuja : public Proceso  {
public:
	Burbuja( float x, float y, Uint16 vida );
	void mueve();
private:
	float m_y_copia;
};
// ----------------------------------------------------------------------------
// BOLA_FUEGO:
// ----------------------------------------------------------------------------
class Bola_Fuego : public Proceso {
public:
	Bola_Fuego( float x, float y, float xi, float yi );
	void mueve();
private:
	float m_vertical;
	float m_xmin;
	float m_xmax;
	float m_ymin;
	float m_ymax;
};
// ----------------------------------------------------------------------------
// SATANAS:
// ----------------------------------------------------------------------------
class Procesos;
class Satanas : public Proceso {
public:
	Satanas( float x, float y, Uint16 vida,  Monje* monje, Procesos *procesos );
	void mueve();
private:
	float m_y_copia;
	Uint8 m_recuenta;
	float m_noni_y;
	Monje		*m_monje;
	Procesos	*m_procesos;
};
// ----------------------------------------------------------------------------
// DERRUMBE: Sólo se mueve arriba-abajo y coloca un bloque.
// ----------------------------------------------------------------------------
class Derrumbe : public Proceso {
public:
	Derrumbe( float x );
	void mueve();
};
// ----------------------------------------------------------------------------
// PROCESOS:
// ----------------------------------------------------------------------------
class Procesos : public BaseTMP {
public:

	Procesos();
	~Procesos();
	bool inicia();
	void quita();

	void limpia();
	void coloca( Monje *monje ); // Coloca los enemigos en la pantalla correspondiente.
	void mueve();	// Mueve todos los procesos.
	void colisiones( Monje& monje, Disparos& disparos );
	void explosion( float x, float y );
	void muerteSatanas( float x, float y );
	void dispara(   float x, float y, float xi, float yi );
	void derrumbe( float x );
	void dibuja();	// Dibuja todo los procesos.
	
private:
	size_t 	 m_cantidad;
	Proceso **m_proceso;
};

#endif
