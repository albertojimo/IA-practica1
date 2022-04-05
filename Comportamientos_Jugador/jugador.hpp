#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = size;
      brujula = 0;
      MAX_CASILLAS_MAPA = size;
      pasos_rectos =0;
      giros_seguidos = 0;
      anchura_objetivo = 0;
      profundidad_objetivo = 0;
      tiempo_recarga=size*2;
      contador_barrido=7;
      pasos_mismo_lugar=0;
      giros_rodeado=0;
      ultimaAccion = actIDLE;
      girar_derecha = false;
      bien_situado = false;
      tener_bikini  = false;
      tener_zapatillas = false;
      hay_objetivo = false;
      modo_antimuro = false;
      modo_barrido= false;
      giro_derecha_barrido= false;
      barrido_largo= false;
      atascado= false;
      modo_asomarse_abismo= false;
      modo_bordear= false;
      vector<unsigned char> aux3(size*2, '?');

      for (unsigned int i = 0; i < size*2; i++)
      {
          mapaTemporal.push_back(aux3);

      }
      vector<int> aux4(size, 0);

      for (unsigned int i = 0; i < size; i++)
      {
          mapaRecorrido.push_back(aux4);

      }
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  
  // Declarar aquí las variables de estado

  /**
   * @brief Indica el numero de la fila. Puede corresponderse con la fila del mapaResultado o del mapaTemporal
   */
  int fil;

  /**
   * @brief Indica el numero de la columna. Puede corresponderse con la columna del mapaResultado o del mapaTemporal
   */
  int col;

  /**
   * @brief Indica la orientación que tiene el jugador. Siendo 0-Norte, 1-Este, 2-Sur y 3-Oeste
   */
  int brujula;

  /**
   * @brief Contabiliza cuantos pasos seguidos ha dado el jugador de forma recta
   */
  int pasos_rectos;

  /**
   * @brief Contabiliza cuantos giros seguidos ha dado el jugador sin avanzar
   */
  int giros_seguidos;

  /**
   * @brief Representa la anchura para localizar la casilla que tenemos como objetivo
   */
  int anchura_objetivo;

  /**
   * @brief Representa la profundidad para localizar la casilla que tenemos como objetivo
   */
  int profundidad_objetivo;

  /**
   * @brief Es un contador que usamos en el modo_barrido para contar los pasos antes de entrar de nuevo en
   * el barrido largo
   */
  int contador_barrido;

  /**
   * @brief Es el tiempo que podemos seguir recargando como máximo
   */
  int tiempo_recarga;

  /**
   * @brief Contabiliza cuantos pasos seguidos ha dado el jugador de forma recta por casillas ya recorridas
   */
  int pasos_mismo_lugar;

  /**
   * @brief Contabiliza los giros que se da cuando esta rodeado por casillas que no se pueden en principio acceder
   */
  int giros_rodeado;

  /**
   * @brief Numero de casillas que tiene el sensor a la vista
   */
  static const int NUM_CASILLAS_VISTA=16;

  /**
   * @brief Maximo de casillas en el mapa
   */
  int MAX_CASILLAS_MAPA;

  /**
   * @brief Minimo de casillas en el mapa
   */
  static const int MIN_CASILLAS_MAPA=0;

  /**
   * @brief Representa la acción realizada en el turno anterior
   */
  Action ultimaAccion;

  /**
   * @brief Representa si se gira a la derecha o no
   */
  bool girar_derecha;

  /**
   * @brief Representa si las filas y las columnas son las correspondientes a mapaResultado
   */
  bool bien_situado;

  /**
   * @brief Representa si se posee el objeto zapatillas
   */
  bool tener_zapatillas;

  /**
   * @brief Representa si se posee el objeto bikini
   */
  bool tener_bikini;

  /**
   * @brief Representa si hay una casilla a la vista planificada como objetivo
   */
  bool hay_objetivo;

  /**
   * @brief Representa si se gira a la derecha en el modo_barrido para asi conseguir un movimiento de serpiente
   */
  bool giro_derecha_barrido;

  /**
   * @brief Representa si dentro del modo_barrido se esta recorrriendo un tramo largo indefinido
   */
  bool barrido_largo;

  /**
   * @brief Representa si se está dentro del modo_antimuro
   */
  bool modo_antimuro;

  /**
   * @brief Representa si está dentro del modo_barrido
   */
  bool modo_barrido;

  /**
   * @brief Representa si está atascado debido a que está en una casilla que no debería estar y rodeado por otras similares
   */
  bool atascado;

  /**
   * @brief Representa si está en el modo_asomarse_abismo
   */
  bool modo_asomarse_abismo;

  /**
   * @brief Representa si está en el modo_bordear
   */
  bool modo_bordear;

  /**
   * @brief Esta matriz acumula la información antes de estar bien posicionados para luego trasladarla al mapaResultado
   */
  vector< vector< unsigned char> > mapaTemporal;

  /**
   * @brief En esta matriz cuando ya estamos bien posicionados empezamos a guardar por donde pasamos para ayudarnos
   * en algunos métodos para no repetir mucho las mismas zonas
   */
  vector< vector< int> > mapaRecorrido;

  /**
   * @brief Se obtiene la anchura y profundidad en el rango de vista segun el sensor
   * @param anchura Tiene un rango desde el 0 hasta el 6 en mayor caso
   * @param profundidad  Tiene un rango desde 0 hasta 3
   * @param casilla Esta es el numero de casilla como lo ve el sensor desde el 0 al 15
   */
  void getAnchuraProfundidad(int & anchura, int & profundidad, int casilla);

  /**
   * @brief Busca trasladar la información almacenada en el mapaTemporal hacia el mapaResultado
   * @param fila_real Valor de fila en el mapaResultado en este momento
   * @param columna_real  Valor de columna en el mapaResultado en este momento
   */
  void actualizarMapa(int fila_real, int columna_real);
  /**
   * @brief Busca dentro del sensor la casilla donde hay zapatillas
   * @param s Sensores
   * @return Se devuelve el numero de la casilla desde el 0 al 15
   */
  int detectarZapatillas(Sensores s);
  /**
   * @brief Busca dentro del sensor la casilla donde hay posicionamiento
   * @param s Sensores
   * @return Se devuelve el numero de la casilla desde el 0 al 15
   */
  int detectarPosicionamiento(Sensores s);
  /**
   * @brief Busca dentro del sensor la casilla donde hay bikini
   * @param s Sensores
   * @return Se devuelve el numero de la casilla desde el 0 al 15
   */
  int detectarBikini(Sensores s);
    /**
     * @brief Busca dentro del sensor la casilla donde hay recarga
     * @param s Sensores
     * @return Se devuelve el numero de la casilla desde el 0 al 15
     */
  int detectarRecarga(Sensores s);
  /**
   * @brief Detecta si hay algo en el terreno o en la superficie que es peligroso
   * @param terreno Caracter que nos dice que tipo de casilla es
   * @param superficie Caracter que nos dice que elemento hay en esa casilla
   * @return Booleano que nos afirma o niega si una casilla es peligrosa
   */
  bool es_casilla_peligrosa(unsigned char terreno, unsigned char superficie);
  /**
   * @brief Detecta si hay algo en el terreno o en la superficie que esta prohibida. Tan solo se usa en niveles con lobos y aldeanos
   * @param terreno Caracter que nos dice que tipo de casilla es
   * @param superficie Caracter que nos dice que elemento hay en esa casilla
   * @return Booleano que nos afirma o niega si una casilla esta prohibida
   */
  bool es_casilla_prohibida(unsigned char terreno, unsigned char superficie);

  /**
   * @brief Detecta si hay algo en la superficie que esta prohibido en casillas conexas a la que vamos a avanzar
   * @param superficie Caracter que nos dice que elemento hay en esa casilla
   * @return Booleano que nos afirma o niega si una casilla es potencialmente prohibida
   */
  bool es_casilla_potencialmente_prohibida(unsigned char superficie);
  /**
   * @brief Verifica que el camino que sea ir recto y girar y avanzar hacia la casilla destino este libre
   * y en caso afirmativo nos consideramos esas casilla como objetivo
   * @param sensores Sensores
   */
  void sondear_camino(Sensores sensores);
  /**
   * @brief Buscamos en la casilla mas lejana en linea recta y si esta desconocida devolvemos true
   * @param profundidad Profundidad de la casilla que buscamos
   * @param anchura  Anchura de la casilla que buscamos
   * @return  Booleano con la respuesta a si es desconocida o no esa casilla
   */
  bool observarHorizonte(int profundidad, int anchura);
  /**
   * @brief Metodo para reiniciar a los valores iniciales las variables de estado
   */
  void reinicio();
  /**
   * @brief Se busca poner una casilla como objetivo si podemos llegar facilmente a ella
   * @param sensores  Sensores
   * @param casilla Casilla destino que queremos ir
   */
  void crear_plan(Sensores sensores, int casilla);

  /**
   * @brief Se actualiza la informacion del jugador segun la ultimaAccion
   */
  void aplicar_ultima_accion();

  /**
   * @brief Sentencias a ejecutar si debemos abortar el movimiento planeado por la necesidad de evadir una casilla
   * @return Accion resultante que debemos tomar para evadir el peligro
   */
  Action protocolo_evasion();

  /**
   * @brief Miramos al lateral derecho para ver si la casilla cercana que estaría en su horizonte esta sin explorar
   * @details Debido a la posibilidad de mirar el mapa vamos a no girar para mirar sino solo girar cuando sea necesario
   * @return Devuelve un booleano con la respuesta a la operación
   */
  bool observar_lateral_derecho();

    /**
     * @brief Miramos al lateral izquierdo para ver si la casilla cercana que estaría en su horizonte esta sin explorar
     * @details Debido a la posibilidad de mirar el mapa vamos a no girar para mirar sino solo girar cuando sea necesario
     * @return Devuelve un booleano con la respuesta a la operación
     */
  bool observar_lateral_izquierdo();

};

#endif
