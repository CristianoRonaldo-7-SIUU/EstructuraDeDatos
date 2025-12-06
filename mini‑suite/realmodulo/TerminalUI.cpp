#include "TerminalUI.h"
#include <iostream>

using namespace std;

void TerminalUI::mostrarArbol(Node* nodo, int nivel) {
    if (nodo == NULL) return;
    
    for (int i = 0; i < nivel; i++) {
        cout << "  ";
    }
    
    string icono = nodo->esCarpeta() ? "📁" : "📄";
    cout << icono << " " << nodo->nombre;
    
    if (!nodo->esCarpeta()) {
        cout << " [archivo]";
    }
    cout << endl;
    
    for (size_t i = 0; i < nodo->children.size(); i++) {
        mostrarArbol(nodo->children[i], nivel + 1);
    }
}

void TerminalUI::help() {
    cout << "--- Comandos Disponibles ---" << endl;
    cout << "- mkdir           : Crea una carpeta (Ruta completa)." << endl;
    cout << "- touch           : Crea un archivo (Ruta completa + contenido)." << endl;
    cout << "- tree            : Muestra la estructura del árbol." << endl;
    cout << "- search / find   : Busca un nodo por nombre." << endl;
    cout << "- mv              : Mueve un archivo/carpeta de ubicación." << endl;
    cout << "- rm              : Elimina un archivo/carpeta (con confirmación)." << endl;
    cout << "- /help           : Muestra esta ayuda." << endl;
    cout << "- salir           : Cierra el programa." << endl;
    cout << "----------------------------" << endl;
}

void TerminalUI::iniciar() {
    string operacion = "";

    do {
        cout << "c:> ";
        getline(cin, operacion);

        if (operacion == "mkdir") {
            string ruta = "";
            cout << "Ruta completa: ";
            getline(cin, ruta);
            cout << core.mkdir(ruta) << endl;
            cout << endl;
        }
        else if (operacion == "touch") {
            string ruta = "";
            string contenido = "";
            cout << "Ruta completa: ";
            getline(cin, ruta);
            cout << "Contenido: ";
            getline(cin, contenido);
            cout << core.touch(ruta, contenido) << endl;
            cout << endl;
        }
        // ***************************
        else if (operacion == "tree" || operacion == "export preorden") {
            cout << endl;
            mostrarArbol(core.getRoot());
            cout << endl;
        }
        else if (operacion == "search" || operacion == "find") {
            string nombre = "";
            cout << "Archivo a buscar: ";
            getline(cin, nombre);
            cout << endl;
            
            vector<string> resultados = core.find(nombre);
            if (resultados.empty()) {
                 cout << "X No se encontró: " << nombre << endl;
            } else {
                for(string r : resultados) {
                    cout << "✓ Encontrado en: " << r << endl;
                }
            }
            cout << endl;
        }
        else if (operacion == "mv") {
            string nombre = "";
            string destino = "";
            cout << "Archivo a mover: ";
            getline(cin, nombre);
            cout << "Nueva ubicacion: ";
            getline(cin, destino);
            cout << core.mv(nombre, destino) << endl;
            cout << endl;
        }
        else if (operacion == "rm") {
            string nombre = "";
            cout << "Archivo a eliminar: ";
            getline(cin, nombre);
            
            if (core.nodoTieneHijos(nombre)) {
                char confirmacion;
                cout << "⚠ La carpeta no está vacía. Se borrará todo." << endl;
                cout << "¿Estás seguro? (s/n): ";
                cin >> confirmacion;
                cin.ignore(); 

                if (confirmacion == 's' || confirmacion == 'S') {
                    cout << core.rm(nombre, true) << endl;
                } else {
                    cout << "Operación cancelada." << endl;
                }
            } else {
                cout << core.rm(nombre, false) << endl;
            }
            cout << endl;
        }
        else if (operacion == "/help") {
            cout << endl;
            help();
            cout << endl;
        }
        // ***************************

    } while(operacion != "salir");
}
