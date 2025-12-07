#include "Cfs.h"
#include <sstream>

using namespace std;

Cfs::Cfs() {
    root = NULL;
    siguienteId = 1;
    bool cargado = storage.cargarDatos(root, siguienteId);
    
    if (!cargado || root == NULL) {
        root = new Node(0, "root", "carpeta");
        siguienteId = 1;
    }
}

Cfs::~Cfs() {
    if (root) liberarMemoria(root); 
}

Node* Cfs::getRoot() {
    return root;
}

// ***************************
vector<string> Cfs::dividirRuta(string ruta) {
    vector<string> partes;
    stringstream ss(ruta);
    string parte;

    while (getline(ss, parte, '/')) {
        if (!parte.empty()) {
            partes.push_back(parte);
        }
    }
    return partes;
}

Node* Cfs::buscarPorRuta(string ruta) {
    if (ruta == "root" || ruta == "/") {
        return root;
    }
    
    vector<string> partes = dividirRuta(ruta);
    Node *actual = root;

    size_t inicio = (partes.size() > 0 && partes[0] == "root") ? 1 : 0;

    for (size_t i = inicio; i < partes.size(); i++) {
        string nombreBuscado = partes[i];
        bool encontrado = false;
        
        for (size_t j = 0; j < actual->children.size(); j++) {
            if (actual->children[j]->nombre == nombreBuscado) {
                actual = actual->children[j];
                encontrado = true;
                break;
            }
        }
        if (!encontrado) return NULL;
    }
    return actual;
}

Node* Cfs::obtenerPunteroRecursivo(Node* nodo, string nombreBuscado) {
    if (nodo == NULL) return NULL;
    if (nodo->nombre == nombreBuscado) return nodo;

    for (size_t i = 0; i < nodo->children.size(); i++) {
        Node* encontrado = obtenerPunteroRecursivo(nodo->children[i], nombreBuscado);
        if (encontrado != NULL) return encontrado;
    }
    return NULL;
}

void Cfs::desconectarDelPadre(Node* nodo) {
    if (nodo->padre == NULL) return;
    
    Node* padre = nodo->padre;
    for (size_t i = 0; i < padre->children.size(); i++) {
        if (padre->children[i] == nodo) {
            padre->children.erase(padre->children.begin() + i);
            break;
        }
    }
    nodo->padre = NULL;
}

void Cfs::liberarMemoria(Node* nodo) {
    for (size_t i = 0; i < nodo->children.size(); i++) {
        liberarMemoria(nodo->children[i]);
    }
    delete nodo;
}
// ***************************

string Cfs::mkdir(string rutaCompleta) {
    vector<string> partes = dividirRuta(rutaCompleta);
    if (partes.empty()) return "Error: ruta vacía";
    string nombreNuevaCarpeta = partes[partes.size() - 1];
    string rutaPadre = "root";
    size_t inicio = (partes[0] == "root") ? 1 : 0;
    for (size_t i = inicio; i < partes.size() - 1; i++) {
        rutaPadre += "/" + partes[i];
    }
    if (partes.size() == 1 || (partes.size() == 2 && partes[0] == "root")) rutaPadre = "root";
    Node* padre = buscarPorRuta(rutaPadre);
    if (padre == NULL) return "Error: la ruta padre no existe";
    if (!padre->esCarpeta()) return "Error: destino no es una carpeta";
    for (size_t i = 0; i < padre->children.size(); i++) {
        if (padre->children[i]->nombre == nombreNuevaCarpeta) return "Error: la carpeta ya existe";
    }
    Node* nuevaCarpeta = new Node(siguienteId++, nombreNuevaCarpeta, "carpeta");
    nuevaCarpeta->padre = padre;
    padre->children.push_back(nuevaCarpeta);
    storage.guardarDatos(root, siguienteId); 
    return "✓ Carpeta creada";
}

string Cfs::touch(string rutaCompleta, string contenido) {
    vector<string> partes = dividirRuta(rutaCompleta);
    if (partes.empty()) return "Error: ruta vacía";
    string nombreNuevoArchivo = partes[partes.size() - 1];
    string rutaPadre = "root";
    size_t inicio = (partes[0] == "root") ? 1 : 0;
    for (size_t i = inicio; i < partes.size() - 1; i++) {
        rutaPadre += "/" + partes[i];
    }
    if (partes.size() == 1 || (partes.size() == 2 && partes[0] == "root")) rutaPadre = "root";
    Node* padre = buscarPorRuta(rutaPadre);
    if (padre == NULL) return "Error: la ruta padre no existe";
    if (!padre->esCarpeta()) return "Error: destino no es una carpeta";
    for (size_t i = 0; i < padre->children.size(); i++) {
        if (padre->children[i]->nombre == nombreNuevoArchivo) return "Error: el archivo ya existe";
    }
    Node* nuevoArchivo = new Node(siguienteId++, nombreNuevoArchivo, "archivo", contenido);
    nuevoArchivo->padre = padre;
    padre->children.push_back(nuevoArchivo);
    storage.guardarDatos(root, siguienteId); 
    return "✓ Archivo creado";
}

string Cfs::mv(string nombreNodo, string rutaDestino) {
    Node* nodoAMover = obtenerPunteroRecursivo(root, nombreNodo);
    if (nodoAMover == NULL) return "X Error: El archivo/carpeta no existe.";
    if (nodoAMover == root) return "X Error: No puedes mover la raíz.";
    Node* destino = buscarPorRuta(rutaDestino);
    if (destino == NULL) return "X Error: La ruta destino no existe.";
    if (!destino->esCarpeta()) return "X Error: El destino debe ser una carpeta.";
    desconectarDelPadre(nodoAMover);
    nodoAMover->padre = destino;
    destino->children.push_back(nodoAMover);
    storage.guardarDatos(root, siguienteId); 
    return "✓ Se movió correctamente";
}

bool Cfs::nodoTieneHijos(string nombreNodo) {
    Node* nodo = obtenerPunteroRecursivo(root, nombreNodo);
    if (nodo != NULL && nodo->children.size() > 0) return true;
    return false;
}

string Cfs::rm(string nombreNodo, bool forzar) {
    Node* nodoAEliminar = obtenerPunteroRecursivo(root, nombreNodo);
    if (nodoAEliminar == NULL) return "X Error: El archivo/carpeta no existe.";
    if (nodoAEliminar == root) return "X Error: No puedes eliminar la raíz.";
    if (nodoAEliminar->children.size() > 0 && !forzar) {
        return "REQUIERE_CONFIRMACION";
    }
    desconectarDelPadre(nodoAEliminar);
    liberarMemoria(nodoAEliminar);
    storage.guardarDatos(root, siguienteId); 
    return "✓ Eliminado correctamente";
}

vector<string> Cfs::find(string nombreBuscado) {
    vector<string> resultados;
    auto recurse = [&](auto&& self, Node* nodo, string rutaActual) -> void {
        if (nodo == NULL) return;
        if (nodo->nombre == nombreBuscado) {
            resultados.push_back(rutaActual);
        }
        for (size_t i = 0; i < nodo->children.size(); i++) {
            string nuevaRuta = rutaActual + "/" + nodo->children[i]->nombre;
            self(self, nodo->children[i], nuevaRuta);
        }
    };
    recurse(recurse, root, "root");
    return resultados;
}

// ***************************
vector<string> Cfs::obtenerSugerencias(string rutaParcial) {
    vector<string> coincidencias;
    
    if (string("root").find(rutaParcial) == 0 && rutaParcial.length() < 4) {
        coincidencias.push_back("root");
        return coincidencias;
    }

    string rutaPadre = "root";
    string prefijo = rutaParcial;

    size_t ultimoSlash = rutaParcial.rfind('/');
    if (ultimoSlash != string::npos) {
        rutaPadre = rutaParcial.substr(0, ultimoSlash);
        prefijo = rutaParcial.substr(ultimoSlash + 1);
        if (rutaPadre == "") rutaPadre = "root";
    } else {
        rutaPadre = "root";
        prefijo = rutaParcial;
        
        // Si lo que escribio ya es root entonces sugerimos hijos de root
        if (rutaParcial == "root") {
            prefijo = ""; // sugerir todo lo que hay en root
        }
    }

    Node* padre = buscarPorRuta(rutaPadre);
    if (padre != NULL) {
        for (Node* hijo : padre->children) {
            if (hijo->nombre.find(prefijo) == 0) { // Si empieza con el prefijo
                string sugerencia = rutaPadre + "/" + hijo->nombre;
                if (rutaPadre == "root" && rutaParcial.find('/') == string::npos && rutaParcial != "root") {
                   sugerencia = "root/" + hijo->nombre;
                }
                coincidencias.push_back(sugerencia);
            }
        }
    }
    return coincidencias;
}
// ***************************
