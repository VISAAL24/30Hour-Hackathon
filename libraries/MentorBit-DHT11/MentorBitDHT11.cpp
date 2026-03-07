/*


            ██████╗    ██╗    ██████╗    ██╗   ████████╗    █████╗    ██╗               
            ██╔══██╗   ██║   ██╔════╝    ██║   ╚══██╔══╝   ██╔══██╗   ██║               
            ██║  ██║   ██║   ██║  ███╗   ██║      ██║      ███████║   ██║               
            ██║  ██║   ██║   ██║   ██║   ██║      ██║      ██╔══██║   ██║               
            ██████╔╝   ██║   ╚██████╔╝   ██║      ██║      ██║  ██║   ███████╗          
            ╚═════╝    ╚═╝    ╚═════╝    ╚═╝      ╚═╝      ╚═╝  ╚═╝   ╚══════╝          
                                                                                        
     ██████╗    ██████╗    ██████╗    ███████╗   ███████╗   ██╗    ██████╗    ███╗   ██╗
    ██╔════╝   ██╔═══██╗   ██╔══██╗   ██╔════╝   ██╔════╝   ██║   ██╔════╝    ████╗  ██║
    ██║        ██║   ██║   ██║  ██║   █████╗     ███████╗   ██║   ██║  ███╗   ██╔██╗ ██║
    ██║        ██║   ██║   ██║  ██║   ██╔══╝     ╚════██║   ██║   ██║   ██║   ██║╚██╗██║
    ╚██████╗   ╚██████╔╝   ██████╔╝   ███████╗   ███████║   ██║   ╚██████╔╝   ██║ ╚████║
     ╚═════╝    ╚═════╝    ╚═════╝    ╚══════╝   ╚══════╝   ╚═╝    ╚═════╝    ╚═╝  ╚═══╝ 
        

    Autor: Digital Codesign
    Version: 1.0.0
    Fecha de creación: Septiembre de 2024
    Fecha de version: Septiembre de 2024
    Repositorio: https://github.com/DigitalCodesign/MentorBit-DHT11
    Descripcion: 
        Esta libreria esta especificamente diseñada para ser utilizada junto con 
        el modulo MentorBit DHT11
    Metodos principales:
        MentorBitDHT11 -> constructor de la clase
        obtenerTemperatura -> Obtiene y devuelve el valor de temperatura leido por el sensor DHT11 (float)
        obtenerHumedad -> Obtiene y devuelve el valor de humedad leido por el sensor DHT11 (float)

*/

#include "MentorBitDHT11.h"

MentorBitDHT11::MentorBitDHT11(uint8_t pin = 0):myDHT(pin, DHT11){
    myDHT.begin();
    _port.gpios[1] = pin;
}

float MentorBitDHT11::obtenerTemperatura(){
    float valor;
    valor = myDHT.readTemperature();
    return valor;
}

float MentorBitDHT11::obtenerHumedad(){
    float valor;
    valor = myDHT.readHumidity();
    return valor;
}

void MentorBitDHT11::configPort(const Port& port) {

    _port.type = port.type;
    _port.location = port.location;
    _port.gpios[0] = port.gpios[0];
    _port.gpios[1] = port.gpios[1];

    _reiniciarDHT();

}

void MentorBitDHT11::_reiniciarDHT()
{

    myDHT = DHT(_port.gpios[1], DHT11);
    myDHT.begin();

}