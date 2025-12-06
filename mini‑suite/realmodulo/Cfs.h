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

    // *************************** PURA MAGIA
    vector<string> dividirRuta(string ruta);
    Node* buscarPorRuta(string ruta);
    Node* obtenerPunteroRecursivo(Node* nodo, string nombreBuscado); // para mv y rm
    void desconectarDelPadre(Node* nodo);
    void liberarMemoria(Node* nodo);
    // ***************************

public:
    Cfs();
    ~Cfs();

    Node* getRoot(); // Para visualización externa

    string mkdir(string rutaCompleta);
    string touch(string rutaCompleta, string contenido);
    string mv(string nombreNodo, string rutaDestino);
    string rm(string nombreNodo, bool forzar = false); // con parametro de confirmacion en caso de que sea una carpeta
    
    // Busqueda
    vector<string> find(string nombreBuscado);
    
    // Auxiliar para frontend
    bool nodoTieneHijos(string nombreNodo);
};

#endif
