#include "TerminalUI.h"
#include <iostream>
#include <algorithm> 
#include <vector>

using namespace std;

// Función auxiliar para ordenar: carpetas arriba, archivos abajo, alfabéticamente
bool compararNodos(Node* a, Node* b) {
    if (a->esCarpeta() && !b->esCarpeta()) return true;
    if (!a->esCarpeta() && b->esCarpeta()) return false;
    return a->nombre < b->nombre;
}

void TerminalUI::mostrarArbolRecursivo(Node* nodo, string prefix, bool esUltimo) {
    if (nodo == NULL) return;

    cout << prefix;
    
    if (nodo == core.getRoot() || (prefix == "" && nodo == core.getDirActual())) { 
        cout << nodo->nombre << endl;
    } else {
        if (esUltimo) cout << "└── ";
        else          cout << "├── ";
        
        string icono = nodo->esCarpeta() ? "📁" : "📄";
        cout << icono << " " << nodo->nombre;
        if (!nodo->esCarpeta()) cout << " [archivo]";
        cout << endl;
    }

    // Calcular nuevo prefijo para los hijos
    string nuevoPrefix = prefix;
    // Solo agregamos indentación si NO es el nodo base inicial sin prefijo
    if (!(prefix == "" && (nodo == core.getRoot() || nodo == core.getDirActual()))) {
        nuevoPrefix += (esUltimo ? "    " : "│   ");
    }

    vector<Node*> hijosOrdenados = nodo->children;
    sort(hijosOrdenados.begin(), hijosOrdenados.end(), compararNodos);

    for (size_t i = 0; i < hijosOrdenados.size(); i++) {
        mostrarArbolRecursivo(hijosOrdenados[i], nuevoPrefix, i == hijosOrdenados.size() - 1);
    }
}

void TerminalUI::help() {
    cout << "--- Comandos ---" << endl;
    cout << "- cd <ruta>       : Navegar (cd .. para subir)." << endl;
    cout << "- ls              : Listar directorio actual." << endl;
    cout << "- mkdir <ruta>    : Crear carpeta." << endl;
    cout << "- touch <ruta>    : Crear archivo." << endl;
    cout << "- rm <ruta>       : Mover a papelera." << endl;
    cout << "- rm -f <ruta>    : Eliminar permanente." << endl;
    cout << "- mv <org> <dest> : Mover archivo/carpeta." << endl;
    cout << "- rename          : Renombrar." << endl;
    cout << "- cat <ruta>      : Leer archivo." << endl;
    cout << "- cat > <ruta>    : Sobrescribir archivo." << endl;
    cout << "- tree            : Ver todo el arbol." << endl;
    cout << "- search          : Buscar por nombre global." << endl;
    cout << "----------------" << endl;
}

void TerminalUI::iniciar() {
    string operacion = "";

    do {
        cout << core.obtenerRutaActualStr() << ":> ";
        operacion = tabcomandos::leerInput();

        if (operacion == "cd") {
            string ruta = "";
            cout << "Ruta destino: ";
            ruta = tabruta::leerRuta(core); 
            string res = core.cd(ruta);
            if(res != "") cout << res << endl;
            cout << endl;
        }
        else if (operacion == "cd ..") {
            string res = core.cdArriba();
            if(res != "") cout << res << endl;
            cout << endl;
        }
        else if (operacion == "ls") {
            cout << endl;
            // Muestra solo desde la carpeta actual hacia abajo
            mostrarArbolRecursivo(core.getDirActual(), "", true);
            cout << endl;
        }
        else if (operacion == "mkdir") {
            string ruta = "";
            cout << "Nombre/Ruta: ";
            ruta = tabruta::leerRuta(core); 
            cout << core.mkdir(ruta) << endl;
            cout << endl;
        }
        else if (operacion == "touch") {
            string ruta = "";
            string contenido = "";
            cout << "Nombre/Ruta: ";
            ruta = tabruta::leerRuta(core);

            cout << "Contenido: ";
            getline(cin, contenido);
            cout << core.touch(ruta, contenido) << endl;
            cout << endl;
        }
        else if (operacion == "mv") {
            string origen = "";
            string destino = "";
            cout << "Origen (Ruta): ";
            origen = tabruta::leerRuta(core); 
            
            cout << "Destino (Carpeta): ";
            destino = tabruta::leerRuta(core);
            
            cout << core.mv(origen, destino) << endl;
            cout << endl;
        }
        else if (operacion == "rm") {
            string ruta = "";
            cout << "Archivo/Carpeta a borrar: ";
            ruta = tabruta::leerRuta(core);
            cout << core.rm(ruta, false) << endl;
            cout << endl;
        }
        else if (operacion == "rm -f") {
            string ruta = "";
            cout << "Archivo/Carpeta a borrar: ";
            ruta = tabruta::leerRuta(core);
            cout << core.rm(ruta, true) << endl;
            cout << endl;
        }
        else if (operacion == "rename") {
            string ruta = "";
            string nuevoNombre = "";
            cout << "Archivo/Carpeta a renombrar: ";
            ruta = tabruta::leerRuta(core);
            
            cout << "Nuevo nombre: ";
            getline(cin, nuevoNombre);
            cout << core.renameNode(ruta, nuevoNombre) << endl;
            cout << endl;
        }
        else if (operacion == "cat") {
            string ruta = "";
            cout << "Archivo a leer: ";
            ruta = tabruta::leerRuta(core);
            
            string contenido = core.catRead(ruta);
            if (contenido.substr(0, 2) == "X ") {
                cout << contenido << endl;
            } else {
                cout << "--- Contenido ---" << endl;
                cout << contenido << endl;
                cout << "-----------------" << endl;
            }
            cout << endl;
        }
        else if (operacion == "cat >") {
            string ruta = "";
            string contenido = "";
            cout << "Archivo a sobrescribir: ";
            ruta = tabruta::leerRuta(core);
            
            cout << "Nuevo contenido: ";
            getline(cin, contenido);
            cout << core.catOverwrite(ruta, contenido) << endl;
            cout << endl;
        }
        else if (operacion == "cat >>") {
            string ruta = "";
            string contenido = "";
            cout << "Archivo a adjuntar: ";
            ruta = tabruta::leerRuta(core);
            
            cout << "Contenido extra: ";
            getline(cin, contenido);
            cout << core.catAppend(ruta, contenido) << endl;
            cout << endl;
        }
        else if (operacion == "tree" || operacion == "export preorden") {
            cout << endl;
            mostrarArbolRecursivo(core.getRoot(), "", true);
            cout << endl;
        }
        else if (operacion == "search" || operacion == "find") {
            string nombre = "";
            cout << "Nombre a buscar (Global): ";
            getline(cin, nombre); 
            cout << endl;
            
            vector<string> resultados = core.find(nombre);
            if (resultados.empty()) {
                 cout << "X No se encontro: " << nombre << endl;
            } else {
                for(string r : resultados) {
                    cout << "✓ Encontrado en: " << r << endl;
                }
            }
            cout << endl;
        }
        else if (operacion == "trash-restore") {
            vector<string> lista = core.listarPapelera();
            cout << "--- Papelera de Reciclaje ---" << endl;
            if (lista.empty()) {
                cout << "(Vacia)" << endl;
            } else {
                for (string s : lista) cout << " - " << s << endl;
                
                string nombre = "";
                cout << endl << "Nombre del archivo a restaurar: ";
                getline(cin, nombre);
                cout << core.trashRestore(nombre) << endl;
            }
            cout << endl;
        }
        else if (operacion == "/help") {
            cout << endl;
            help();
            cout << endl;
        }
        else if (operacion != "salir" && operacion != "") {
            cout << "Comando no reconocido." << endl;
        }

    } while(operacion != "salir");
}
