#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

    /**
     * Para saber que estoy bien colocado desde el inicio
     */
    if(sensores.nivel==0) bien_situado=true;

    /**
     * Nos chocamos con un aldeano con lo que no podemos ejecutar nuestra accion asi que pasamos turno
     */
    if(sensores.colision){
        accion=ultimaAccion;
        cout << "Por favor señor aldeano, apartese que tenemos prisa y mucho que descubrir" << endl;
        return accion;
    }
    /**
     * Nos ha comido un lobete y nos reinicia todos los valores al estado inicial
     * Se intenta evitar pero hay situaciones donde no somos capaces de evitarlo
     */
    if(sensores.reset){
        cout << "Esto no es el final lobete, volvere" << endl;
        reinicio();
     }



    /**
     * Segun la accion anterior y la brujula nos actualizamos la información debido a la accion anterior
     */
	aplicar_ultima_accion();

    unsigned char casillaActual = sensores.terreno[0];
    unsigned char casillaEnFrente = sensores.terreno[2];
    unsigned  char superficieEnFrente = sensores.superficie[2];

    /**
     * Para tener en cuenta las casillas de recarga
     */
    if(casillaActual=='X' and tiempo_recarga!=0 and sensores.bateria<3000 and sensores.vida>sensores.bateria){
        tiempo_recarga--;
        cout << "La recarga se realiza sin problema" << endl;
        accion=ultimaAccion;
        return accion;
    }

    /**
     * Las casillas que nos interesa pasar al menos una vez las buscamos en nuestro rango de vista con nuestro
     * sensor por lo que si hemos pasado ni las buscamos
     */

    int casilla_zapatillas=-1;
    if(!tener_zapatillas) casilla_zapatillas = detectarZapatillas(sensores);
    int casilla_posicionamiento=-1;
    if(!bien_situado) casilla_posicionamiento = detectarPosicionamiento(sensores);
    int casilla_bikini = -1;
    if(!tener_bikini) casilla_bikini = detectarBikini(sensores);
    int casilla_recarga = detectarRecarga(sensores);
    if(tiempo_recarga>=0 and sensores.bateria<3000 and sensores.vida>sensores.bateria) casilla_recarga = detectarRecarga(sensores);


    /**
     * Si hemos llegado a una casilla de posicionamiento actualizamos la informacion del mapaResultado con los
     * datos que tenemos guardados en mapaTemporal
     */
	if(casillaActual=='G' and !bien_situado){

        if(sensores.sentido==brujula){
            actualizarMapa(sensores.posF,sensores.posC);
        }else{
            brujula=sensores.sentido;
        }
        fil=sensores.posF;
        col=sensores.posC;
		bien_situado=true;
        modo_bordear=true; //Justo al posicionarse se intenta bordear
	}
    /**
     * En caso de estar en la casilla de zapatillas o bikini se actualiza la variable de estado que tenemos
     * para identificar que tenemos ya ese objeto
     */
    if(casillaActual=='D' and !tener_zapatillas){
        tener_zapatillas=true;
    }
    if(casillaActual=='K' and !tener_bikini){
        tener_bikini=true;
    }

    /**
     * En el caso de que no hayamos pasado por alli pues nos ponemos como objetivo ir alli si tenemos el camino
     * mas simple despejado
     */
    if(casilla_zapatillas!=-1 ) crear_plan(sensores,casilla_zapatillas);

    if(casilla_posicionamiento!=-1 ) crear_plan(sensores,casilla_posicionamiento);

    if(casilla_bikini!=-1 ) crear_plan(sensores, casilla_bikini);

    if(casilla_recarga!=-1) crear_plan(sensores, casilla_recarga);

    /**
     * En el caso de que al fondo haya más territorio sin descubrir vamos a seguir recto hacia adelante
     */
    if(!hay_objetivo and bien_situado){
        anchura_objetivo=3;
        profundidad_objetivo=3;
        if(observarHorizonte(profundidad_objetivo,anchura_objetivo)){
            sondear_camino(sensores);
        }
    }

    /**
     * Pintamos en el mapaResultado o mapaTemporal lo que vayamos viendo y no este ya pintado
     */
	if(bien_situado){
        /**
         * En caso de no estar en otro modo entramos en el barrido
         */
        if(!modo_antimuro and !hay_objetivo and !modo_asomarse_abismo) modo_barrido=true;
		mapaResultado[fil][col] = casillaActual;
        /**
         * Vamos anotando por donde vamos pasando
         */
        mapaRecorrido[fil][col] ++;
		for(int i=1;i<NUM_CASILLAS_VISTA;i++){
            int anchura,profundidad;
            getAnchuraProfundidad(anchura, profundidad,i);
			switch(brujula){
				case 0:

                    if(fil-profundidad >= MIN_CASILLAS_MAPA && col+profundidad-anchura >= MIN_CASILLAS_MAPA && col+profundidad-anchura <= MAX_CASILLAS_MAPA) {
                        if (mapaResultado[fil - profundidad][col + profundidad - anchura] == '?')
                            mapaResultado[fil - profundidad][col + profundidad - anchura]=sensores.terreno[i];
					}
					break;
				case 1:

                    if(fil+profundidad-anchura >= MIN_CASILLAS_MAPA && fil+profundidad-anchura <= MAX_CASILLAS_MAPA && col+profundidad <= MAX_CASILLAS_MAPA) {
                        if (mapaResultado[fil + profundidad - anchura][col + profundidad] == '?')
                            mapaResultado[fil + profundidad - anchura][col + profundidad]=sensores.terreno[i];
                    }
                    break;
				case 2:

                    if(col - profundidad +anchura >= MIN_CASILLAS_MAPA && fil+profundidad <= MAX_CASILLAS_MAPA && col-profundidad + anchura <= MAX_CASILLAS_MAPA) {
                        if (mapaResultado[fil + profundidad ][col - profundidad + anchura] == '?')
                            mapaResultado[fil + profundidad ][col - profundidad + anchura]=sensores.terreno[i];
                    }
                    break;
				case 3:

                    if(fil-profundidad+anchura >= MIN_CASILLAS_MAPA && fil-profundidad+anchura <= MAX_CASILLAS_MAPA && col-profundidad >= MIN_CASILLAS_MAPA) {
                        if (mapaResultado[fil - profundidad + anchura][col - profundidad] == '?')
                            mapaResultado[fil - profundidad + anchura][col - profundidad]=sensores.terreno[i];
                    }
                    break;
			}
		}
	}else{
        mapaTemporal[fil][col] = casillaActual;
        for(int i=1;i<NUM_CASILLAS_VISTA;i++){
            int anchura,profundidad;
            getAnchuraProfundidad(anchura, profundidad,i);
            switch(brujula){
                case 0:


                    if (mapaTemporal[fil - profundidad][col + profundidad - anchura] == '?')
                        mapaTemporal[fil - profundidad][col + profundidad - anchura]=sensores.terreno[i];

                    break;
                case 1:

                    if (mapaTemporal[fil + profundidad - anchura][col + profundidad] == '?')
                        mapaTemporal[fil + profundidad - anchura][col + profundidad]=sensores.terreno[i];

                    break;
                case 2:

                    if (mapaTemporal[fil + profundidad ][col - profundidad + anchura] == '?')
                        mapaTemporal[fil + profundidad ][col - profundidad + anchura]=sensores.terreno[i];
                    break;
                case 3:

                    if (mapaTemporal[fil - profundidad + anchura][col - profundidad] == '?')
                        mapaTemporal[fil - profundidad + anchura][col - profundidad]=sensores.terreno[i];
                    break;
            }
        }
    }


    /**
     * Aqui ponemos que casillas podemos pasar y en caso que no sean esas casillas pues nos giramos a un lado
     */
	if(casillaEnFrente == 'X' or casillaEnFrente == 'D' or casillaEnFrente == 'K' or casillaEnFrente == 'T' or casillaEnFrente == 'S' or casillaEnFrente == 'G' and superficieEnFrente == '_'){
		accion = actFORWARD;
	}else if(!girar_derecha){
		accion = actTURN_L;
	}else{
		accion = actTURN_R;
	}
    /**
     * Permitimos que si tenemos algunos objetos poder atravesar determinadas zonas
     */
    if(casillaEnFrente == 'A' && tener_bikini) accion= actFORWARD;
    if(casillaEnFrente == 'B' && tener_zapatillas) accion = actFORWARD;

    /**
     * Si tenemos un muro delante entramos en un modo para conseguir evitarlo
     */
    if(casillaEnFrente=='M' or sensores.terreno[3]=='M') modo_antimuro=true;

    if(casillaEnFrente!='M' and sensores.terreno[3]!='M') modo_antimuro=false;


    /**
     * Forma de evitar o salir de zonas de precipicios entrando en un modo especial llamado modo_asomarse_abismo
     */
    if((casillaEnFrente=='P' or sensores.terreno[1]=='P') and bien_situado) modo_asomarse_abismo=true;

    if(casillaEnFrente!='P' and sensores.terreno[1]!='P') modo_asomarse_abismo=false;


    /**
     * En caso de salir en una zona que no deberiamos poder atravesar con nuestro objetos
     */
    if(casillaActual == 'B' and !tener_zapatillas) atascado=true;
    if(casillaActual == 'A' and  !tener_bikini) atascado= true;


    /**
     * Para evitar entrar en bucles donde se recorren todos los bordes
     */
    if(pasos_rectos>7){
        girar_derecha = !girar_derecha;
        if(girar_derecha){
            accion=actTURN_R;
        }else{
            accion=actTURN_L;
        }
        pasos_rectos=0;
    }

    /**
     * Modo que intenta bordear el mapa si se le permite
     */
    if(modo_bordear){
        if(casillaEnFrente=='P' or casillaEnFrente=='M'){
            accion=actTURN_R;
        }else{
            if(ultimaAccion==actFORWARD){
                accion=actTURN_L;
            }else {
                accion = actFORWARD;
            }
        }
        if(mapaRecorrido[fil][col]>=2){
            if(casillaEnFrente=='P' or casillaEnFrente=='M'){
                accion=actTURN_R;
            }else{
                modo_bordear=false;
                accion=actFORWARD;
                modo_barrido=true;
            }
        }
    }

    /**
     * Este modo busca una esquina y a partir de esa zona recorre como una serpiente la zona delimitada
     */

    if(modo_barrido and !hay_objetivo){
        if(barrido_largo){
            if(es_casilla_prohibida(casillaEnFrente , superficieEnFrente)){
                barrido_largo= false;
                if(giro_derecha_barrido){
                    accion=actTURN_R;
                }else{
                    accion=actTURN_L;
                }
            }else{
                accion=actFORWARD;
            }
            if(mapaRecorrido[fil][col]>=3){
                modo_barrido=false;
            }
        }else{
            if(contador_barrido>0){
                accion=actFORWARD;
                contador_barrido--;
                if(es_casilla_prohibida(casillaEnFrente , superficieEnFrente)){
                    modo_barrido= false;
                    modo_asomarse_abismo=true;
                }
                if(mapaRecorrido[fil][col]>=3){
                    modo_barrido=false;
                }
            }else{
                barrido_largo=true;
                contador_barrido=7;
                if(giro_derecha_barrido){
                    accion=actTURN_R;
                }else{
                    accion=actTURN_L;
                }
                giro_derecha_barrido=!giro_derecha_barrido;
            }
        }
    }

    /**
     * Modo para evitar salir de zona de precipicios
     */
    if(modo_asomarse_abismo){
        if(casillaEnFrente=='P'){
            accion=actTURN_R;
        }else{
            if(sensores.terreno[1]=='P'){
                accion=actFORWARD;
                if(sensores.terreno[5]!='P') crear_plan(sensores,5);
                if(sensores.terreno[11]!='P') crear_plan(sensores,11);
            }else{
                crear_plan(sensores,1);
                modo_asomarse_abismo= false;
            }
            if(mapaRecorrido[fil][col]>=2){
                modo_asomarse_abismo=false;
            }
        }
    }

    /**
     * En este modo seguimos la estela del muro hasta encontrar una salida y llegar a esa casilla
     */
    if(modo_antimuro and !modo_barrido){
        cout << "Esta el modo antimuro" << endl;
        if(casillaEnFrente=='M'){
            accion=actTURN_L;
        }
        else if(sensores.terreno[3]=='M'){
            accion=actFORWARD;
            if(sensores.terreno[7] != 'M') crear_plan(sensores,7);
            if(sensores.terreno[13] != 'M') crear_plan(sensores,13);
        }else{
            crear_plan(sensores,3);
            modo_antimuro= false;
        }
    }


    /**
     * Nos permite seguir hacia donde no esta explorado
     */
    if(bien_situado and !hay_objetivo and ultimaAccion==actFORWARD){
        if(observar_lateral_derecho()) accion=actTURN_R;
        if(observar_lateral_izquierdo()) accion=actTURN_L;
    }

    /**
     * Aqui entramos cuando tenemos una casilla a la vista a donde queremos dirigirnos
     */
    if(hay_objetivo){
        if(profundidad_objetivo>0){
            accion=actFORWARD;
            profundidad_objetivo--;
            if(anchura_objetivo==0 and profundidad_objetivo==0) hay_objetivo=false;
        }else{
            if(profundidad_objetivo==0){
                if(anchura_objetivo>0){
                    accion=actTURN_L;
                }else{
                    accion=actTURN_R;
                    anchura_objetivo=anchura_objetivo*(-1);
                }
                profundidad_objetivo--;
            }else{
                accion=actFORWARD;
                anchura_objetivo--;
                if(anchura_objetivo==0) hay_objetivo=false;
            }
        }
    }




    /**
     * Evita quedar en un bucle infinito de giros en una misma casilla
     */
    if(giros_seguidos>4) accion=actFORWARD;
    if(accion==actTURN_L or accion==actTURN_R) giros_seguidos++;

    /**
     * Busca evitar muchos pasos continuos rectos por el mismo sitio ya recorrido
     */
    if(pasos_mismo_lugar>40 and ultimaAccion==actFORWARD){
        girar_derecha = !girar_derecha;
        if(girar_derecha){
            accion=actTURN_R;
        }else{
            accion=actTURN_L;
        }
        pasos_mismo_lugar=0;
    }





    /**
     * Esto es para evitar avanzar a un lugar que no estaria permitido
     */
    if(!atascado and accion==actFORWARD and es_casilla_peligrosa(casillaEnFrente,superficieEnFrente)){
        accion=protocolo_evasion();
        if(accion==actTURN_L or accion==actTURN_R) giros_rodeado++;
        if(giros_rodeado>10 and es_casilla_peligrosa(casillaEnFrente, superficieEnFrente) and !es_casilla_prohibida(casillaEnFrente, superficieEnFrente)) accion=actFORWARD;
    }

    if(atascado and !es_casilla_prohibida(casillaEnFrente , superficieEnFrente)) accion=actFORWARD;
    /**
     * Esto es para saber cuando salir del atasco por aparecer en una casilla que no deberiamos poder recorrer
     * y esta rodeada por casillas peligrosas tambien
     */
    if(atascado and accion==actFORWARD and !es_casilla_peligrosa(casillaEnFrente,superficieEnFrente)){
        atascado= false;
    }
    /**
     * Esto es para cuando aparecen los lobos y aldeanos y nos interesa evitar el contacto con ellos que no traen nada bueno
     */
    if(sensores.nivel>2 and accion==actFORWARD and es_casilla_prohibida(casillaEnFrente , superficieEnFrente) and es_casilla_potencialmente_prohibida( sensores.superficie[1])and es_casilla_potencialmente_prohibida( sensores.superficie[3])and es_casilla_potencialmente_prohibida( sensores.superficie[6])){
        accion=protocolo_evasion();
    }

    /**
     * En caso de que sigamos recto actualizamos ciertas variables de estado que afecta esa decision
     */
    if(accion==actFORWARD){
        pasos_rectos++;
        giros_seguidos=0;
        giros_rodeado=0;
        if(bien_situado){
            if(mapaRecorrido[fil][col]>=2){
                pasos_mismo_lugar++;
            }
        }
    }

    /**
     * Final de la iteraccion
     */
    ultimaAccion=accion;
	return accion;

}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador::getAnchuraProfundidad(int &anchura, int &profundidad, int casilla) {
    if(casilla > 8) {
        anchura= 15 - casilla;
        profundidad=3;
    }else if(casilla > 3) {
        anchura = 8 - casilla;
        profundidad=2;
    }else {
        anchura = 3 - casilla;
        profundidad=1;
    }
}

void ComportamientoJugador::actualizarMapa(int fila_real, int columna_real) {
    for(int i=0;i<=MAX_CASILLAS_MAPA;i++){
        int fila_temporal= i + fil - fila_real;
        for(int j=0;j<=MAX_CASILLAS_MAPA;j++){
            int columna_temporal= j + col - columna_real;
            if(mapaTemporal[fila_temporal][columna_temporal]!='?')
                mapaResultado[i][j]=mapaTemporal[fila_temporal][columna_temporal];
        }
    }
    fil = fila_real;
    col = columna_real;
}

int ComportamientoJugador::detectarZapatillas(Sensores s) {
    int zona_zapatillas=-1;
    for(int i=1;i<NUM_CASILLAS_VISTA;i++){
        if(s.terreno[i]=='D') zona_zapatillas=i;
    }
    return zona_zapatillas;
}

int ComportamientoJugador::detectarPosicionamiento(Sensores s) {
    int zona_pos=-1;
    for(int i=1;i<NUM_CASILLAS_VISTA;i++){
        if(s.terreno[i]=='G') zona_pos=i;
    }
    return zona_pos;
}

int ComportamientoJugador::detectarBikini(Sensores s) {
    int zona_bikini=-1;
    for(int i=1;i<NUM_CASILLAS_VISTA;i++){
        if(s.terreno[i]=='K') zona_bikini=i;
    }
    return zona_bikini;
}

int ComportamientoJugador::detectarRecarga(Sensores s) {
    int zona_recarga=-1;
    for(int i=1;i<NUM_CASILLAS_VISTA;i++){
        if(s.terreno[i]=='X') zona_recarga=i;
    }
    return zona_recarga;
}

bool ComportamientoJugador::es_casilla_peligrosa(unsigned char terreno, unsigned char superficie) {
    bool es_peligrosa=false;
    es_peligrosa= es_casilla_prohibida(terreno,superficie);
    if(!tener_zapatillas and terreno == 'B')
        es_peligrosa= true;
    if(!tener_bikini and terreno == 'A')
        es_peligrosa= true;
    return es_peligrosa;
}

bool ComportamientoJugador::es_casilla_prohibida(unsigned char terreno, unsigned char superficie) {
    bool es_prohibida=false;
    es_prohibida = es_casilla_potencialmente_prohibida(superficie);
    if(terreno == 'P' or terreno == 'M')
        es_prohibida=true;
    return es_prohibida;
}

bool ComportamientoJugador::es_casilla_potencialmente_prohibida( unsigned char superficie) {
    bool es_prohibida=false;
    /*if(superficie=='a')
        es_prohibida = true;*/
    if(superficie== 'l')
        es_prohibida= true;
    return es_prohibida;
}

void ComportamientoJugador::sondear_camino(Sensores sensores) {
    bool camino_libre=true;
    if(profundidad_objetivo == 1){
        if(es_casilla_peligrosa(sensores.terreno[2],sensores.superficie[2]))
            camino_libre=false;

    }else if(profundidad_objetivo == 2){
        if(es_casilla_peligrosa(sensores.terreno[6],sensores.superficie[6]))
            camino_libre=false;
        if(anchura_objetivo>2){
            if(es_casilla_peligrosa(sensores.terreno[5],sensores.superficie[5]))
                camino_libre=false;

        }else if(anchura_objetivo<2){
            if(es_casilla_peligrosa(sensores.terreno[7],sensores.superficie[7]))
                camino_libre=false;
        }

    }else{
        if(es_casilla_peligrosa(sensores.terreno[12],sensores.superficie[12]))
            camino_libre=false;
        if(anchura_objetivo>3){
            for(int j = anchura_objetivo-1; j==3; j--){
                if(es_casilla_peligrosa(sensores.terreno[15-j],sensores.superficie[15-j]))
                    camino_libre=false;
            }
        }else if(anchura_objetivo<3){
            for(int j = 2; j==anchura_objetivo; j--){
                if(es_casilla_peligrosa(sensores.terreno[15-j],sensores.superficie[15-j]))
                    camino_libre=false;
            }
        }

    }
    anchura_objetivo=anchura_objetivo-profundidad_objetivo;
    if(camino_libre) hay_objetivo= true;
}

bool ComportamientoJugador::observarHorizonte(int profundidad,int anchura) {
    bool desconocido=false;
    switch(brujula){
        case 0:


            if (mapaResultado[fil - profundidad][col + profundidad - anchura] == '?')
                desconocido= true;

            break;
        case 1:

            if (mapaResultado[fil + profundidad - anchura][col + profundidad] == '?')
                desconocido= true;

            break;
        case 2:

            if (mapaResultado[fil + profundidad ][col - profundidad + anchura] == '?')
                desconocido= true;
            break;
        case 3:

            if (mapaResultado[fil - profundidad + anchura][col - profundidad] == '?')
                desconocido= true;
            break;
    }
    return desconocido;
}

void ComportamientoJugador::reinicio() {
    fil = col = MAX_CASILLAS_MAPA;
    brujula = 0;
    pasos_rectos =0;
    giros_seguidos = 0;
    anchura_objetivo = 0;
    profundidad_objetivo = 0;
    tiempo_recarga=MAX_CASILLAS_MAPA*2;
    contador_barrido=7;
    pasos_mismo_lugar=0;
    giros_rodeado=0;
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
    vector < vector< unsigned char> > aux;
    mapaTemporal = aux ;
    vector<unsigned char> aux3(MAX_CASILLAS_MAPA*2, '?');

    for (unsigned int i = 0; i < MAX_CASILLAS_MAPA*2; i++)
    {
        mapaTemporal.push_back(aux3);

    }

    vector<int> aux4(MAX_CASILLAS_MAPA, 0);

    for (unsigned int i = 0; i < MAX_CASILLAS_MAPA; i++)
    {
        mapaRecorrido.push_back(aux4);

    }
}

void ComportamientoJugador::crear_plan(Sensores sensores, int casilla) {
    getAnchuraProfundidad(anchura_objetivo, profundidad_objetivo,casilla);
    sondear_camino(sensores);
}

void ComportamientoJugador::aplicar_ultima_accion() {
    switch(ultimaAccion){
        case actFORWARD:
            switch (brujula) {
                case 0:
                    fil--;
                    break;
                case 1:
                    col++;
                    break;
                case 2:
                    fil++;
                    break;
                case 3:
                    col--;
                    break;
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
}

Action ComportamientoJugador::protocolo_evasion() {
    Action accion_evasion=actIDLE;
    girar_derecha=!girar_derecha;
    if(hay_objetivo) hay_objetivo=false;
    if(modo_antimuro) modo_antimuro=false;
    if(modo_barrido) modo_barrido= false;
    if(modo_asomarse_abismo) modo_asomarse_abismo=false;
    if(girar_derecha){
        accion_evasion=actTURN_R;
    }else{
        accion_evasion=actTURN_L;
    }

    return accion_evasion;
}

bool ComportamientoJugador::observar_lateral_derecho() {
    bool girar=false;
    switch (brujula) {
        case 0:
            if(col < MAX_CASILLAS_MAPA-5){
                if(mapaResultado[fil][col+4]=='?' and (mapaResultado[fil][col+1]!='P' and mapaResultado[fil][col+1]!='M')) girar=true;
            }
            break;
        case 1:
            if(fil<MAX_CASILLAS_MAPA-5){
                if(mapaResultado[fil+4][col]=='?' and (mapaResultado[fil+1][col]!='P' and mapaResultado[fil+1][col]!='M')) girar=true;
            }
            break;
        case 2:
            if(col>MIN_CASILLAS_MAPA+5){
                if(mapaResultado[fil][col-4]=='?' and (mapaResultado[fil][col-1]!='P' and mapaResultado[fil][col-1]!='M')) girar=true;
            }
            break;
        case 3:
            if(fil>MIN_CASILLAS_MAPA+5){
                if(mapaResultado[fil-4][col]=='?' and (mapaResultado[fil-1][col]!='P' and mapaResultado[fil-1][col]!='M')) girar = true;
            }
            break;
    }
    return girar;
}

bool ComportamientoJugador::observar_lateral_izquierdo()  {
    bool girar=false;
    switch (brujula) {
        case 0:
            if(col>MIN_CASILLAS_MAPA+5){
                if(mapaResultado[fil][col-4]=='?' and (mapaResultado[fil][col-1]!='P' and mapaResultado[fil][col-1]!='M')) girar=true;
            }
            break;
        case 1:
            if(fil>MIN_CASILLAS_MAPA+5){
                if(mapaResultado[fil-4][col]=='?' and (mapaResultado[fil-1][col]!='P' and mapaResultado[fil-1][col]!='M')) girar = true;
            }
            break;
        case 2:

            if(col < MAX_CASILLAS_MAPA-5){
                if(mapaResultado[fil][col+4]=='?' and (mapaResultado[fil][col+1]!='P' and mapaResultado[fil][col+1]!='M')) girar=true;
            }
            break;
        case 3:

            if(fil<MAX_CASILLAS_MAPA-5){
                if(mapaResultado[fil+4][col]=='?' and (mapaResultado[fil+1][col]!='P' and mapaResultado[fil+1][col]!='M')) girar=true;
            }
            break;
    }
    return girar;
}
