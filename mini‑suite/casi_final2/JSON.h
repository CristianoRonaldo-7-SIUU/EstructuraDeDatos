#ifndef JSON_H
#define JSON_H

#include "Node.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class JSON {
private:
    string escaparTexto(string texto);
    void serializarNodo(Node* nodo, ofstream& archivo, int nivel);

    void saltarEspacios(string& json, int& i);
    string leerString(string& json, int& i);
    int leerInt(string& json, int& i);
    void saltarHasta(string& json, int& i, char c);
    Node* deserializarNodoRecursivo(string& json, int& i);

public:
    //Importante para la papelera
    void guardarDatos(Node* root, vector<Node*>& papelera, int siguienteId);
    bool cargarDatos(Node*& root, vector<Node*>& papelera, int& siguienteId);
};

#endif
