#include "flechitas.h"

using namespace std;

Flechitas::Flechitas() {
    indiceActual = 0;
}

void Flechitas::guardarComando(string comando) {
    if (comando.empty()) return;
    
    if (!historial.empty() && historial.back() == comando) {
        indiceActual = historial.size();
        return;
    }

    historial.push_back(comando);
    indiceActual = historial.size();
}

string Flechitas::obtenerAnterior(string bufferActual) {
    if (historial.empty()) {
        return bufferActual;
    }
    
    if (indiceActual > 0) {
        indiceActual--;
        return historial[indiceActual];
    }

    return historial[0];
}

string Flechitas::obtenerSiguiente(string bufferActual) {
    if (historial.empty()) {
        return bufferActual;
    }

    if (indiceActual < historial.size()) {
        indiceActual++;
        
        if (indiceActual == historial.size()) {
            return ""; 
        }
        return historial[indiceActual];
    }

    return bufferActual;
}
