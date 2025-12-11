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
}

Cfs::~Cfs() {
    if (root) liberarMemoria(root);
    for (Node* n : papelera) {
        liberarMemoria(n);
    }
}

Node* Cfs::getRoot() {
    return root;
}

bool Cfs::existeNombreGlobal(string nombre) {
    return existeGlobalmente(root, nombre);
}

bool Cfs::existeGlobalmente(Node* nodo, string nombre) {
    if (nodo == NULL) return false;
    // Si el nodo actual se llama igual (y no es el root)
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

string Cfs::obtenerRutaAbsoluta(Node* nodo) {
    if (nodo == root) return "root";
    string ruta = nodo->nombre;
    Node* temp = nodo->padre;
    while (temp != NULL) {
        if (temp == root) {
            ruta = "root/" + ruta;
            break;
        } else {
            ruta = temp->nombre + "/" + ruta;
            temp = temp->padre;
        }
    }
    return ruta;
}

void Cfs::establecerRutaOriginalRecursivo(Node* nodo, string rutaActual) {
    nodo->rutaOriginal = rutaActual; 
    for (Node* hijo : nodo->children) {
        string rutaHijo = rutaActual + "/" + hijo->nombre;
        establecerRutaOriginalRecursivo(hijo, rutaHijo);
    }
}

void Cfs::listarPapeleraRecursivo(Node* nodo, vector<string>& lista) {
    string info = nodo->nombre + " [Origen: " + nodo->rutaOriginal + "]";
    lista.push_back(info);
    
    for (Node* hijo : nodo->children) {
        listarPapeleraRecursivo(hijo, lista);
    }
}

Node* Cfs::buscarEnPapeleraRecursivo(Node* nodo, string nombreBuscado) {
    if (nodo->nombre == nombreBuscado) return nodo;
    
    for (Node* hijo : nodo->children) {
        Node* encontrado = buscarEnPapeleraRecursivo(hijo, nombreBuscado);
        if (encontrado) return encontrado;
    }
    return NULL;
}

string Cfs::mkdir(string rutaCompleta) {
    vector<string> partes = dividirRuta(rutaCompleta);
    if (partes.empty()) return "Error: ruta vacía";
    string nombreNuevaCarpeta = partes[partes.size() - 1];
    
    // VALIDACIÓN GLOBAL
    if (existeNombreGlobal(nombreNuevaCarpeta)) {
        return "X Error: El nombre '" + nombreNuevaCarpeta + "' ya existe en el sistema.";
    }

    string rutaPadre = "root";
    size_t inicio = (partes[0] == "root") ? 1 : 0;
    for (size_t i = inicio; i < partes.size() - 1; i++) {
        rutaPadre += "/" + partes[i];
    }
    if (partes.size() == 1 || (partes.size() == 2 && partes[0] == "root")) rutaPadre = "root";
    
    Node* padre = buscarPorRuta(rutaPadre);
    if (padre == NULL) return "Error: la ruta padre no existe";
    if (!padre->esCarpeta()) return "Error: destino no es una carpeta";

    Node* nuevaCarpeta = new Node(siguienteId++, nombreNuevaCarpeta, "carpeta");
    nuevaCarpeta->padre = padre;
    padre->children.push_back(nuevaCarpeta);
    storage.guardarDatos(root, papelera, siguienteId); 
    return "Carpeta creada";
}

string Cfs::touch(string rutaCompleta, string contenido) {
    vector<string> partes = dividirRuta(rutaCompleta);
    if (partes.empty()) return "Error: ruta vacía";
    string nombreNuevoArchivo = partes[partes.size() - 1];

    // VALIDACIÓN GLOBAL
    if (existeNombreGlobal(nombreNuevoArchivo)) {
        return "X Error: El nombre '" + nombreNuevoArchivo + "' ya existe en el sistema.";
    }

    string rutaPadre = "root";
    size_t inicio = (partes[0] == "root") ? 1 : 0;
    for (size_t i = inicio; i < partes.size() - 1; i++) {
        rutaPadre += "/" + partes[i];
    }
    if (partes.size() == 1 || (partes.size() == 2 && partes[0] == "root")) rutaPadre = "root";
    Node* padre = buscarPorRuta(rutaPadre);
    if (padre == NULL) return "Error: la ruta padre no existe";
    if (!padre->esCarpeta()) return "Error: destino no es una carpeta";

    Node* nuevoArchivo = new Node(siguienteId++, nombreNuevoArchivo, "archivo", contenido);
    nuevoArchivo->padre = padre;
    padre->children.push_back(nuevoArchivo);
    storage.guardarDatos(root, papelera, siguienteId); 
    return "Archivo creado";
}

string Cfs::mv(string nombreNodo, string rutaDestino) {
    Node* nodoAMover = obtenerPunteroRecursivo(root, nombreNodo);
    if (nodoAMover == NULL) return "X Error: El archivo/carpeta no existe.";
    if (nodoAMover == root) return "X Error: No puedes mover la raíz.";
    Node* destino = buscarPorRuta(rutaDestino);
    if (destino == NULL) return "X Error: La ruta destino no existe.";
    if (!destino->esCarpeta()) return "X Error: El destino debe ser una carpeta.";
    
    for(Node* h : destino->children) {
        if(h->nombre == nodoAMover->nombre) return "X Error: Ya existe en el destino.";
    }

    desconectarDelPadre(nodoAMover);
    nodoAMover->padre = destino;
    destino->children.push_back(nodoAMover);
    storage.guardarDatos(root, papelera, siguienteId); 
    return "Se movio correctamente";
}

bool Cfs::nodoTieneHijos(string nombreNodo) {
    Node* nodo = obtenerPunteroRecursivo(root, nombreNodo);
    if (nodo != NULL && nodo->children.size() > 0) return true;
    return false;
}

string Cfs::rm(string nombreNodo, bool permanente) {
    Node* nodoAEliminar = obtenerPunteroRecursivo(root, nombreNodo);
    if (nodoAEliminar == NULL) return "X Error: El archivo/carpeta no existe.";
    if (nodoAEliminar == root) return "X Error: No puedes eliminar la raíz.";
    
    string rutaAbsoluta = obtenerRutaAbsoluta(nodoAEliminar);

    desconectarDelPadre(nodoAEliminar);

    if (permanente) {
        liberarMemoria(nodoAEliminar);
        storage.guardarDatos(root, papelera, siguienteId); 
        return "Eliminado permanentemente.";
    } else {
        establecerRutaOriginalRecursivo(nodoAEliminar, rutaAbsoluta);
        papelera.push_back(nodoAEliminar);
        storage.guardarDatos(root, papelera, siguienteId);
        return "Movido a la papelera (Ruta guardada: " + rutaAbsoluta + ")";
    }
}

vector<string> Cfs::listarPapelera() {
    vector<string> lista;
    for (Node* n : papelera) {
        listarPapeleraRecursivo(n, lista);
    }
    return lista;
}

string Cfs::trashRestore(string nombreNodo) {
    if (existeNombreGlobal(nombreNodo)) {
        return "X Error: No se puede restaurar, conflicto de nombre.";
    }

    Node* nodoARestaurar = NULL;
    Node* nodoPadreEnBasura = NULL;
    int indiceEnVector = -1;

    for (size_t i = 0; i < papelera.size(); i++) {
        Node* encontrado = buscarEnPapeleraRecursivo(papelera[i], nombreNodo);
        if (encontrado != NULL) {
            nodoARestaurar = encontrado;
            if (encontrado == papelera[i]) {
                indiceEnVector = i;
            } else {
                nodoPadreEnBasura = encontrado->padre;
            }
            break;
        }
    }

    if (nodoARestaurar == NULL) return "X Error: No se encuentra en la papelera.";

    string rutaOrigen = nodoARestaurar->rutaOriginal;
    vector<string> partes = dividirRuta(rutaOrigen);
    
    Node* padreDestino = root;
    string rutaLograda = "root";
    bool rutaCompletaExiste = true;

    size_t inicio = (partes.size() > 0 && partes[0] == "root") ? 1 : 0;
    
    for (size_t i = inicio; i < partes.size() - 1; i++) {
        string carpetaBuscada = partes[i];
        bool encontrada = false;
        
        for (Node* hijo : padreDestino->children) {
            if (hijo->nombre == carpetaBuscada) {
                padreDestino = hijo;
                rutaLograda += "/" + hijo->nombre;
                encontrada = true;
                break;
            }
        }
        
        if (!encontrada) {
            rutaCompletaExiste = false;
            break;
        }
    }

    if (indiceEnVector != -1) {
        papelera.erase(papelera.begin() + indiceEnVector);
    } else if (nodoPadreEnBasura != NULL) {
        for (size_t i = 0; i < nodoPadreEnBasura->children.size(); i++) {
            if (nodoPadreEnBasura->children[i] == nodoARestaurar) {
                nodoPadreEnBasura->children.erase(nodoPadreEnBasura->children.begin() + i);
                break;
            }
        }
    }
    
    nodoARestaurar->padre = padreDestino;
    padreDestino->children.push_back(nodoARestaurar);
    nodoARestaurar->rutaOriginal = "";

    storage.guardarDatos(root, papelera, siguienteId);

    if (rutaCompletaExiste) {
        return "Restaurado en su ubicacion original.";
    } else {
        return "Ruta original rota. Restaurado en: " + rutaLograda;
    }
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
        if (rutaParcial == "root") {
            prefijo = ""; 
        }
    }

    Node* padre = buscarPorRuta(rutaPadre);
    if (padre != NULL) {
        for (Node* hijo : padre->children) {
            if (hijo->nombre.find(prefijo) == 0) { 
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

string Cfs::renameNode(string nombreViejo, string nombreNuevo) {
    Node* nodo = obtenerPunteroRecursivo(root, nombreViejo); 
    if (nodo == NULL) return "X Error: El archivo/carpeta no existe.";
    if (nodo == root) return "X Error: No puedes renombrar la raíz.";
    
    // VALIDACIÓN GLOBAL
    if (existeNombreGlobal(nombreNuevo)) {
        return "X Error: El nombre '" + nombreNuevo + "' ya esta en uso en el sistema.";
    }

    nodo->nombre = nombreNuevo;
    storage.guardarDatos(root, papelera, siguienteId);
    return "Renombrado exitosamente";
}

string Cfs::catRead(string nombreArchivo) {
    Node* nodo = obtenerPunteroRecursivo(root, nombreArchivo);
    if (nodo == NULL) return "X Error: El archivo no existe.";
    if (nodo->esCarpeta()) return "X Error: Es una carpeta, no un archivo.";
    return nodo->contenido;
}

string Cfs::catOverwrite(string nombreArchivo, string nuevoContenido) {
    Node* nodo = obtenerPunteroRecursivo(root, nombreArchivo);
    if (nodo == NULL) return "X Error: El archivo no existe.";
    if (nodo->esCarpeta()) return "X Error: Es una carpeta, no un archivo.";
    
    nodo->contenido = nuevoContenido;
    storage.guardarDatos(root, papelera, siguienteId);
    return "Contenido reemplazado";
}

string Cfs::catAppend(string nombreArchivo, string contenidoAdicional) {
    Node* nodo = obtenerPunteroRecursivo(root, nombreArchivo);
    if (nodo == NULL) return "X Error: El archivo no existe.";
    if (nodo->esCarpeta()) return "X Error: Es una carpeta, no un archivo.";

    string procesado = contenidoAdicional;
    if (procesado.length() >= 2 && procesado.substr(0, 2) == "\\n") {
        procesado = "\n" + procesado.substr(2);
    }

    nodo->contenido += procesado;
    storage.guardarDatos(root, papelera, siguienteId);
    return "Contenido agregado";
}
