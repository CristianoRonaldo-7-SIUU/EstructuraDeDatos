#ifndef CFS_H
#define CFS_H

#include "Node.h"
#include "JSON.h"
#include <vector>
#include <string>

using namespace std;

class Cfs {
private:
    Node *root;
    int siguienteId;
    JSON storage;

    // ***************************
    vector<string> dividirRuta(string ruta);
    Node* buscarPorRuta(string ruta);
    Node* obtenerPunteroRecursivo(Node* nodo, string nombreBuscado);
    void desconectarDelPadre(Node* nodo);
    void liberarMemoria(Node* nodo);
    // ***************************

public:
    Cfs();
    ~Cfs();

    Node* getRoot(); 

    // Logica de todos los comandos
    // NOTA CLAVE: Acuérdate de agregar aquí siempre al agregar un comando.
    string mkdir(string rutaCompleta);
    string touch(string rutaCompleta, string contenido);
    string mv(string nombreNodo, string rutaDestino);
    string rm(string nombreNodo, bool forzar = false);
    
    // Busqueda
    vector<string> find(string nombreBuscado);
    
    // Auxiliar para frontend
    bool nodoTieneHijos(string nombreNodo);
    
    // ***************************
    vector<string> obtenerSugerencias(string rutaParcial);
    // ***************************
};

#endif
