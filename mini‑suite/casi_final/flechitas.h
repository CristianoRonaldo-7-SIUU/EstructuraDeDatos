#ifndef FLECHITAS_H
#define FLECHITAS_H

#include <string>
#include <vector>

using namespace std;

class Flechitas {
private:
    // vector para no perder datos al navegar
    vector<string> historial;
    int indiceActual; 

public:
    Flechitas();
    void guardarComando(string comando);
    string obtenerAnterior(string bufferActual);
    string obtenerSiguiente(string bufferActual);
};

#endif
