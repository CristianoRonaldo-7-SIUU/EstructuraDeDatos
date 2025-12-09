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
    if (!nodo->esCarpeta()) cout << " [archivo]";
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
    cout << "- rename          : Renombra archivo/carpeta (funciona con find)." << endl; 
    cout << "- cat             : Lee contenido de archivo." << endl; 
    cout << "- cat >           : Sobreescribe contenido." << endl; 
    cout << "- cat >>          : Agrega contenido." << endl; 
    cout << "- /help           : Muestra esta ayuda." << endl;
    cout << "- salir           : Cierra el programa." << endl;
    cout << "----------------------------" << endl;
}

void TerminalUI::iniciar() {
    string operacion = "";

    do {
        cout << "c:> ";
        operacion = tabcomandos::leerInput();

        if (operacion == "mkdir") {
            string ruta = "";
            cout << "Ruta completa: ";
            ruta = tabruta::leerRuta(core); 
            cout << core.mkdir(ruta) << endl;
            cout << endl;
        }
        else if (operacion == "touch") {
            string ruta = "";
            string contenido = "";
            cout << "Ruta completa: ";
            ruta = tabruta::leerRuta(core);

            cout << "Contenido: ";
            getline(cin, contenido);
            cout << core.touch(ruta, contenido) << endl;
            cout << endl;
        }
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
            
            cout << "Nueva ubicacion (Carpeta destino): ";
            destino = tabruta::leerRuta(core);
            
            cout << core.mv(nombre, destino) << endl;
            cout << endl;
        }
        else if (operacion == "rm") {
            string nombre = "";
            cout << "Archivo a eliminar (Nombre): ";
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
        // ***************************
        else if (operacion == "rename") {
            string nombre = "";
            string nuevoNombre = "";
            cout << "Introduce un archivo o carpeta: ";
            // USAMOS TABRUTA PARA AUTOCOMPLETAR EL ARCHIVO A RENOMBRAR
            nombre = tabruta::leerRuta(core);
            
            cout << "Nuevo nombre de " << nombre << ": ";
            getline(cin, nuevoNombre);
            cout << core.renameNode(nombre, nuevoNombre) << endl;
            cout << endl;
        }
        else if (operacion == "cat") {
            string nombre = "";
            cout << "Nombre del archivo a leer: ";
            nombre = tabruta::leerRuta(core);
            
            string contenido = core.catRead(nombre);
            if (contenido.substr(0, 2) == "X ") {
                cout << contenido << endl;
            } else {
                cout << nombre << ": " << contenido << endl;
            }
            cout << endl;
        }
        else if (operacion == "cat >") {
            string nombre = "";
            string contenido = "";
            cout << "Nombre del archivo a reemplazar: ";
            nombre = tabruta::leerRuta(core);
            
            cout << "Nuevo contenido de " << nombre << ": ";
            getline(cin, contenido);
            cout << core.catOverwrite(nombre, contenido) << endl;
            cout << endl;
        }
        else if (operacion == "cat >>") {
            string nombre = "";
            string contenido = "";
            cout << "Archivo al que quieras incluir texto: ";
            nombre = tabruta::leerRuta(core);
            
            cout << "Contenido a incluir en " << nombre << ": ";
            getline(cin, contenido);
            cout << core.catAppend(nombre, contenido) << endl;
            cout << endl;
        }
        // ***************************
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
