#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Este" << endl; break;
		case 2: cout << "Sur " << endl; break;
		case 3: cout << "Oeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;

	switch(ultimaAccion){
		case actFORWARD:
			switch(brujula){
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			break;
		case actTURN_R:
			brujula = (brujula + 1) % 4;
			girar_derecha = (rand()%2 == 0);
			break;
		case actTURN_L:
			brujula = (brujula + 3) % 4;
			girar_derecha = (rand()%2 == 0);
			break;
		
		
	}

	cout << "fil: " << fil << endl;
	cout << "col: " << col << endl;
	cout << "brujula: " << brujula << endl;

	if(sensores.terreno[0]=='G' and !bien_situado){
		fil = sensores.posF;
		col = sensores.posC;
		bien_situado=true;
	}

	if(bien_situado){
		mapaResultado[fil][col] = sensores.terreno[0];
		for(int i=1;i<NUM_CASILLAS_VISTA;i++){
            int anchura,profundidad;
            getAnchuraProfundidad(anchura, profundidad,i);
            cout << anchura << "  " << profundidad << endl;
			switch(brujula){
				case 0:

                    if(fil-profundidad >= MIN_CASILLAS_MAPA && col-profundidad+anchura >= MIN_CASILLAS_MAPA && col-profundidad+anchura <= MAX_CASILLAS_MAPA) {
                        if (mapaResultado[fil - profundidad][col - profundidad + anchura] == '?')
                            mapaResultado[fil - profundidad][col - profundidad + anchura]=sensores.terreno[i];
					}
					break;
				case 1:

                    if(fil-profundidad+anchura >= MIN_CASILLAS_MAPA && fil-profundidad+anchura <= MAX_CASILLAS_MAPA && col+profundidad <= MAX_CASILLAS_MAPA) {
                        if (mapaResultado[fil - profundidad + anchura][col + profundidad] == '?')
                            mapaResultado[fil - profundidad + anchura][col + profundidad]=sensores.terreno[i];
                    }
                    break;
				case 2:

                    if(col+profundidad -anchura >= MIN_CASILLAS_MAPA && fil+profundidad <= MAX_CASILLAS_MAPA && col+profundidad - anchura <= MAX_CASILLAS_MAPA) {
                        if (mapaResultado[fil + profundidad ][col + profundidad - anchura] == '?')
                            mapaResultado[fil + profundidad ][col + profundidad - anchura]=sensores.terreno[i];
                    }
                    break;
				case 3:

                    if(fil+profundidad-anchura >= MIN_CASILLAS_MAPA && fil+profundidad-anchura <= MAX_CASILLAS_MAPA && col-profundidad >= MIN_CASILLAS_MAPA) {
                        if (mapaResultado[fil + profundidad - anchura][col - profundidad] == '?')
                            mapaResultado[fil + profundidad - anchura][col - profundidad]=sensores.terreno[i];
                    }
                    break;
			}
		}
	}


	if(sensores.terreno[2] == 'X' or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'K' or sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G' and sensores.superficie[2] == '_'){
		accion = actFORWARD;
	}else if(!girar_derecha){
		accion = actTURN_L;
		cout << "Giro a la izquierda" << endl;
	}else{
		accion = actTURN_R;
		cout << "Giro a la derecha" << endl;
	}



	ultimaAccion=accion;

	// Determinar el efecto de la ultima accion enviada
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador::getAnchuraProfundidad(int &anchura, int &profundidad, int i) {
    if(i>8) {
        anchura=15-i;
        profundidad=3;
    }else if(i>3) {
        anchura = 8 - i;
        profundidad=2;
    }else {
        anchura = 3 - i;
        profundidad=1;
    }
}
