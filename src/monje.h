#ifndef _MONJE_
#define _MONJE_
#include "PGL/PGL.h"
#include "baseTMP.h"

class Monje : public BaseTMP {
public:
	bool inicia();
	void quita();

	void anda();
	void vuela();
	void nada();

	Uint8 dameVidas() const;
	
	// Posición relativa a la ventana:
	int  dameXrel();
	int  dameYrel();
	void tomaXrel( int x );
	void tomaYrel( int y );
	
	// Posición absoluta en el mapa:
	int   dameX() const;
	int   dameY() const;	
	void  tomaX( int x );
	void  tomaY( int y );
	void  toma( float x,  float y, float gravedad, float movimiento );
	float dameXfloat() const;
	float dameYfloat() const;
	Uint8 dameAncho()  const;
	Uint8 dameAlto()   const;
	
	Enumera_Direccion    dameDireccion()       const;
	Enumera_Tipo_Disparo dameTipoDisparo()     const;
	Uint8				 damePotenciaDisparo() const;
	
	void guardaPosicionInicial();
	void restauraPosicionInicial();
	void frena();

	void muere();
	bool estaMuerto() const;
	bool estaVivo()   const;
	bool resucita();

	void mueve();
	void dibuja();

private:
	
	bool colisionAbajo(     float rango );
	bool colisionArriba(    float rango );
	bool colisionDerecha(   float rango );
	bool colisionIzquierda( float rango );
	void objetos( int px, int py, Uint16 baldosa );

	bool  m_vive;
	Uint8 m_vidas;
	Enumera_Tipo_Movimiento m_tipo_movimiento;
	
	Enumera_Direccion  m_direccion_inicial;
	float m_x_inicial;
	float m_y_inicial;
	float m_gravedad_inicial;
	float m_movimiento_inicial;

	Enumera_Direccion  m_direccion;	
	float m_x;			// En pixels absoluto al mapa.
	float m_y;
	float m_gravedad;	// Gravedad, movimiento vertical automático. ( caer > 0 ) (suelo == 0 ) ( saltar < 0 ) 
	float m_movimiento;	// Movimiento horizontal automático.   ( derecha > 0 ) ( parado == 0 ) (izquierda < 0 )

	float m_inc_gravedad;
	float m_salto;
	float m_inc_movimiento;
	float m_inercia;	// Inercia
	
	Uint8 m_ancho;
	Uint8 m_alto;
	Uint8 m_ancho2;
	Uint8 m_alto2;
	
	Enumera_Tipo_Disparo m_tipo_disparo;
	Uint8  m_potencia_disparo;
	Uint32 m_cuenta;
	int    m_animacion; 
};

#endif
