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
    Node *dirActual; 
    vector<Node*> papelera;
    int siguienteId;
    JSON storage;

    vector<string> dividirRuta(string ruta);

    Node* buscarDesdeActual(string ruta); 
    
    Node* obtenerPunteroRecursivo(Node* nodo, string nombreBuscado);
    void desconectarDelPadre(Node* nodo);
    void liberarMemoria(Node* nodo);
    string obtenerRutaAbsoluta(Node* nodo);

    // ***************************
    void establecerRutaOriginalRecursivo(Node* nodo, string rutaPadre);
    void listarPapeleraRecursivo(Node* nodo, vector<string>& lista);
    Node* buscarEnPapeleraRecursivo(Node* nodo, string nombreBuscado);
    
    bool existeGlobalmente(Node* nodo, string nombre);
    // ***************************

public:
    Cfs();
    ~Cfs();

    Node* getRoot(); 
    Node* getDirActual();
    string cd(string ruta);
    string cdArriba();
    string obtenerRutaActualStr();
    // -------------------------

    bool existeNombreGlobal(string nombre);

    string mkdir(string rutaRelativa);
    string touch(string rutaRelativa, string contenido);
    string mv(string nombreNodo, string rutaDestino);
    
    // ***************************
    string rm(string nombreNodo, bool permanente);
    string trashRestore(string nombreNodo);
    vector<string> listarPapelera(); 
    // ***************************
    
    vector<string> find(string nombreBuscado);
    bool nodoTieneHijos(string nombreNodo);
    vector<string> obtenerSugerencias(string rutaParcial);

    string renameNode(string nombreViejo, string nombreNuevo);
    string catRead(string nombreArchivo);
    string catOverwrite(string nombreArchivo, string nuevoContenido);
    string catAppend(string nombreArchivo, string contenidoAdicional);
};

#endif
