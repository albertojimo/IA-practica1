#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = 99;
      brujula = 0;
      NUM_CASILLAS_VISTA = 16;
      MAX_CASILLAS_MAPA = 99;
      MIN_CASILLAS_MAPA = 0;
      ultimaAccion = actIDLE;
      girar_derecha = false;
      bien_situado = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  
  // Declarar aquí las variables de estado
  int fil, col, brujula, NUM_CASILLAS_VISTA, MAX_CASILLAS_MAPA, MIN_CASILLAS_MAPA;

  Action ultimaAccion;
  
  bool girar_derecha, bien_situado;

  void getAnchuraProfundidad(int & anchura, int & profundidad, int i);

};

#endif
