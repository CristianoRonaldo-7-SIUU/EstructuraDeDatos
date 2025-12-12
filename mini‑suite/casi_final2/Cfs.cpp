#include "Cfs.h"
#include <sstream>

using namespace std;

Cfs::Cfs() {
    root = NULL;
    siguienteId = 1;
    bool cargado = storage.cargarDatos(root, papelera, siguienteId);
    
    if (!cargado || root == NULL) {
        root = new Node(0, "root", "carpeta");
        siguienteId = 1;
    }
    dirActual = root;
}

Cfs::~Cfs() {
    if (root) liberarMemoria(root);
    for (Node* n : papelera) {
        liberarMemoria(n);
    }
}

Node* Cfs::getRoot() { return root; }
Node* Cfs::getDirActual() { return dirActual; }

bool Cfs::existeNombreGlobal(string nombre) {
    return existeGlobalmente(root, nombre);
}

bool Cfs::existeGlobalmente(Node* nodo, string nombre) {
    if (nodo == NULL) return false;
    if (nodo->nombre == nombre && nodo != root) return true;
    for (Node* hijo : nodo->children) {
        if (existeGlobalmente(hijo, nombre)) return true;
    }
    return false;
}
// ---------------------------------

vector<string> Cfs::dividirRuta(string ruta) {
    vector<string> partes;
    stringstream ss(ruta);
    string parte;
    while (getline(ss, parte, '/')) {
        if (!parte.empty()) partes.push_back(parte);
    }
    return partes;
}

string Cfs::obtenerRutaActualStr() {
    return obtenerRutaAbsoluta(dirActual);
}

string Cfs::cd(string ruta) {
    Node* destino = buscarDesdeActual(ruta);
    if (destino == NULL) return "X Ruta no encontrada.";
    if (!destino->esCarpeta()) return "X No es una carpeta.";
    
    dirActual = destino;
    return ""; 
}

string Cfs::cdArriba() {
    if (dirActual == root) return "Ya estas en la raiz.";
    if (dirActual->padre != NULL) {
        dirActual = dirActual->padre;
        return "";
    }
    return "Error desconocido.";
}

Node* Cfs::buscarDesdeActual(string ruta) {
    if (ruta == "root" || ruta == "/") return root;
    if (ruta == "." || ruta == "") return dirActual;
    if (ruta == "..") return dirActual->padre;

    Node* inicio = dirActual;

    if (ruta.rfind("root", 0) == 0) {
        inicio = root;
    }

    vector<string> partes = dividirRuta(ruta);
    Node *actual = inicio;

    size_t i = 0;
    if (partes.size() > 0 && partes[0] == "root") i = 1;

    for (; i < partes.size(); i++) {
        string nombreBuscado = partes[i];
        
        if (nombreBuscado == "..") {
            if (actual->padre) actual = actual->padre;
            continue;
        }
        
        bool encontrado = false;
        for (Node* hijo : actual->children) {
            if (hijo->nombre == nombreBuscado) {
                actual = hijo;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) return NULL;
    }
    return actual;
}
// ----------------------------------------

Node* Cfs::obtenerPunteroRecursivo(Node* nodo, string nombreBuscado) {
    if (nodo == NULL) return NULL;
    if (nodo->nombre == nombreBuscado) return nodo;
    for (Node* hijo : nodo->children) {
        Node* res = obtenerPunteroRecursivo(hijo, nombreBuscado);
        if (res) return res;
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
    for (Node* h : nodo->children) liberarMemoria(h);
    delete nodo;
}

string Cfs::obtenerRutaAbsoluta(Node* nodo) {
    if (nodo == root) return "root";
    string ruta = nodo->nombre;
    Node* temp = nodo->padre;
    while (temp != NULL) {
        ruta = temp->nombre + "/" + ruta;
        temp = temp->padre;
    }
    return ruta;
}

void Cfs::establecerRutaOriginalRecursivo(Node* nodo, string rutaActual) {
    nodo->rutaOriginal = rutaActual; 
    for (Node* hijo : nodo->children) {
        establecerRutaOriginalRecursivo(hijo, rutaActual + "/" + hijo->nombre);
    }
}

void Cfs::listarPapeleraRecursivo(Node* nodo, vector<string>& lista) {
    lista.push_back(nodo->nombre + " [Origen: " + nodo->rutaOriginal + "]");
    for (Node* hijo : nodo->children) listarPapeleraRecursivo(hijo, lista);
}

Node* Cfs::buscarEnPapeleraRecursivo(Node* nodo, string nombreBuscado) {
    if (nodo->nombre == nombreBuscado) return nodo;
    for (Node* h : nodo->children) {
        Node* res = buscarEnPapeleraRecursivo(h, nombreBuscado);
        if (res) return res;
    }
    return NULL;
}

// Los comandos del fokin diablo

string Cfs::mkdir(string rutaRelativa) {
    size_t lastSlash = rutaRelativa.find_last_of('/');
    string nombreNuevo, rutaPadre;
    
    if (lastSlash == string::npos) {
        nombreNuevo = rutaRelativa;
        rutaPadre = ".";
    } else {
        nombreNuevo = rutaRelativa.substr(lastSlash + 1);
        rutaPadre = rutaRelativa.substr(0, lastSlash);
    }

    if (existeNombreGlobal(nombreNuevo)) return "X Error: Nombre duplicado globalmente.";

    Node* padre = buscarDesdeActual(rutaPadre);
    if (!padre) return "X Error: Ruta padre no existe.";
    if (!padre->esCarpeta()) return "X Error: Destino no es carpeta.";

    Node* nuevo = new Node(siguienteId++, nombreNuevo, "carpeta");
    nuevo->padre = padre;
    padre->children.push_back(nuevo);
    storage.guardarDatos(root, papelera, siguienteId); 
    return "Carpeta creada en " + obtenerRutaAbsoluta(padre);
}

string Cfs::touch(string rutaRelativa, string contenido) {
    size_t lastSlash = rutaRelativa.find_last_of('/');
    string nombreNuevo, rutaPadre;
    
    if (lastSlash == string::npos) {
        nombreNuevo = rutaRelativa;
        rutaPadre = ".";
    } else {
        nombreNuevo = rutaRelativa.substr(lastSlash + 1);
        rutaPadre = rutaRelativa.substr(0, lastSlash);
    }

    if (existeNombreGlobal(nombreNuevo)) return "X Error: Nombre duplicado globalmente.";

    Node* padre = buscarDesdeActual(rutaPadre);
    if (!padre) return "X Error: Ruta padre no existe.";
    if (!padre->esCarpeta()) return "X Error: Destino no es carpeta.";

    Node* nuevo = new Node(siguienteId++, nombreNuevo, "archivo", contenido);
    nuevo->padre = padre;
    padre->children.push_back(nuevo);
    storage.guardarDatos(root, papelera, siguienteId); 
    return "Archivo creado en " + obtenerRutaAbsoluta(padre);
}

string Cfs::mv(string nombreNodo, string rutaDestino) {
    Node* nodoAMover = buscarDesdeActual(nombreNodo);
    if (!nodoAMover) nodoAMover = obtenerPunteroRecursivo(root, nombreNodo);

    if (!nodoAMover) return "X Error: Nodo no encontrado.";
    if (nodoAMover == root) return "X No mover root.";

    Node* destino = buscarDesdeActual(rutaDestino);
    if (!destino || !destino->esCarpeta()) return "X Destino invalido.";

    for(Node* h : destino->children) {
        if(h->nombre == nodoAMover->nombre) return "X Ya existe en destino.";
    }

    desconectarDelPadre(nodoAMover);
    nodoAMover->padre = destino;
    destino->children.push_back(nodoAMover);
    storage.guardarDatos(root, papelera, siguienteId); 
    return "Movido a " + obtenerRutaAbsoluta(destino);
}

string Cfs::rm(string nombreNodo, bool permanente) {
    Node* nodo = buscarDesdeActual(nombreNodo);
    if (!nodo && nombreNodo.find('/') == string::npos) 
        nodo = obtenerPunteroRecursivo(root, nombreNodo);
    
    if (!nodo) return "X No encontrado.";
    if (nodo == root) return "X No borrar root.";
    if (nodo == dirActual) return "X No borrar directorio actual.";

    string ruta = obtenerRutaAbsoluta(nodo);
    desconectarDelPadre(nodo);

    if (permanente) {
        liberarMemoria(nodo);
        storage.guardarDatos(root, papelera, siguienteId); 
        return "Eliminado permanentemente.";
    } else {
        establecerRutaOriginalRecursivo(nodo, ruta);
        papelera.push_back(nodo);
        storage.guardarDatos(root, papelera, siguienteId);
        return "A papelera (" + ruta + ")";
    }
}

string Cfs::trashRestore(string nombreNodo) {
    if (existeNombreGlobal(nombreNodo)) return "X Nombre en uso actualmente.";

    Node* nodo = NULL;
    Node* padrePapelera = NULL;
    int idx = -1;

    for (size_t i = 0; i < papelera.size(); i++) {
        Node* res = buscarEnPapeleraRecursivo(papelera[i], nombreNodo);
        if (res) {
            nodo = res;
            if (res == papelera[i]) idx = i;
            else padrePapelera = res->padre;
            break;
        }
    }
    if (!nodo) return "X No esta en papelera.";

    string rutaOrigen = nodo->rutaOriginal; 
    vector<string> partes = dividirRuta(rutaOrigen);
    
    Node* destino = root; 
    string rutaLograda = "root";
    bool caminoRoto = false;

    for (size_t i = 1; i < partes.size() - 1; i++) {
        string carpetaBuscada = partes[i];
        bool encontrada = false;

        for (Node* h : destino->children) {
            if (h->nombre == carpetaBuscada && h->esCarpeta()) {
                destino = h;
                rutaLograda += "/" + h->nombre;
                encontrada = true;
                break;
            }
        }
        if (!encontrada) {
            caminoRoto = true;
            break;
        }
    }

    if (idx != -1) papelera.erase(papelera.begin() + idx);
    else if (padrePapelera) {
        for(size_t i=0; i<padrePapelera->children.size(); i++) {
            if(padrePapelera->children[i] == nodo) {
                padrePapelera->children.erase(padrePapelera->children.begin()+i);
                break;
            }
        }
    }

    nodo->padre = destino;
    destino->children.push_back(nodo);
    nodo->rutaOriginal = "";
    storage.guardarDatos(root, papelera, siguienteId);

    if (caminoRoto) {
        return "⚠ Ruta original rota. Restaurado en: " + rutaLograda;
    } else {
        return "Restaurado en ubicacion original.";
    }
}
// ------------------------------

vector<string> Cfs::listarPapelera() {
    vector<string> l;
    for(Node* n : papelera) listarPapeleraRecursivo(n, l);
    return l;
}

vector<string> Cfs::find(string nombreBuscado) {
    vector<string> resultados;
    auto recurse = [&](auto&& self, Node* nodo, string rutaActual) -> void {
        if (!nodo) return;
        if (nodo->nombre == nombreBuscado) resultados.push_back(rutaActual);
        for (Node* h : nodo->children) {
            self(self, h, rutaActual + "/" + h->nombre);
        }
    };
    recurse(recurse, root, "root");
    return resultados;
}

vector<string> Cfs::obtenerSugerencias(string rutaParcial) {
    vector<string> res;
    size_t lastSlash = rutaParcial.find_last_of('/');
    string base = "";
    string prefijo = rutaParcial;
    Node* contexto = dirActual;

    if (lastSlash != string::npos) {
        base = rutaParcial.substr(0, lastSlash);
        prefijo = rutaParcial.substr(lastSlash + 1);
        contexto = buscarDesdeActual(base);
    }
    
    if (contexto) {
        for(Node* h : contexto->children) {
            if(h->nombre.find(prefijo) == 0) {
                string sug = (base == "") ? h->nombre : base + "/" + h->nombre;
                res.push_back(sug);
            }
        }
    }
    return res;
}

string Cfs::renameNode(string nombreViejo, string nombreNuevo) {
    Node* nodo = buscarDesdeActual(nombreViejo);
    if (!nodo) nodo = obtenerPunteroRecursivo(root, nombreViejo);

    if (!nodo) return "X No existe.";
    if (nodo == root) return "X No renombrar root.";
    if (existeNombreGlobal(nombreNuevo)) return "X Nombre ocupado.";

    nodo->nombre = nombreNuevo;
    storage.guardarDatos(root, papelera, siguienteId);
    return "Renombrado.";
}

string Cfs::catRead(string nombre) {
    Node* n = buscarDesdeActual(nombre);
    if (!n) n = obtenerPunteroRecursivo(root, nombre);
    if (!n) return "X No existe.";
    if (n->esCarpeta()) return "X Es carpeta.";
    return n->contenido;
}

string Cfs::catOverwrite(string nombre, string cont) {
    Node* n = buscarDesdeActual(nombre);
    if (!n) n = obtenerPunteroRecursivo(root, nombre);
    if (!n) return "X No existe.";
    if (n->esCarpeta()) return "X Es carpeta.";
    n->contenido = cont;
    storage.guardarDatos(root, papelera, siguienteId);
    return "Sobrescrito.";
}

string Cfs::catAppend(string nombre, string cont) {
    Node* n = buscarDesdeActual(nombre);
    if (!n) n = obtenerPunteroRecursivo(root, nombre);
    if (!n) return "X No existe.";
    if (n->esCarpeta()) return "X Es carpeta.";
    
    if (cont.length() >= 2 && cont.substr(0,2) == "\\n") 
        cont = "\n" + cont.substr(2);
    
    n->contenido += cont;
    storage.guardarDatos(root, papelera, siguienteId);
    return "Agregado.";
}
