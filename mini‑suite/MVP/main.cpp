#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream> // JSON

using namespace std;

struct Node
{
    int data;
    string nombre;
    string tipo;
    string contenido;
    Node *padre;
    vector<Node *> children;

    Node(int id, string n, string t, string cont = "")
    {
        data = id;
        nombre = n;
        tipo = t;
        contenido = cont;
        padre = NULL;
    }
    
    bool esCarpeta() {
        return tipo == "carpeta";
    }
};

class FileSystem
{
private:
    Node *root;
    int siguienteId;

    vector<string> dividirRuta(string ruta)
    {
        vector<string> partes;
        stringstream ss(ruta);
        string parte;

        while (getline(ss, parte, '/'))
        {
            if (!parte.empty()) {
                partes.push_back(parte);
            }
        }

        return partes;
    }

    void buscarRecursivo(Node* nodo, string nombreBuscado, string rutaActual, bool &encontrado) {
        if (nodo == NULL) return;

        if (nodo->nombre == nombreBuscado) {
            cout << "✓ Encontrado en: " << rutaActual << endl;
            encontrado = true;
        }

        for (size_t i = 0; i < nodo->children.size(); i++) {
            string nuevaRuta = rutaActual + "/" + nodo->children[i]->nombre;
            buscarRecursivo(nodo->children[i], nombreBuscado, nuevaRuta, encontrado);
        }
    }

    // ***************************
    // Función auxiliar para obtener el puntero del nodo buscando por nombre (para mv y rm)
    Node* obtenerPunteroRecursivo(Node* nodo, string nombreBuscado) {
        if (nodo == NULL) return NULL;
        if (nodo->nombre == nombreBuscado) return nodo;

        for (size_t i = 0; i < nodo->children.size(); i++) {
            Node* encontrado = obtenerPunteroRecursivo(nodo->children[i], nombreBuscado);
            if (encontrado != NULL) return encontrado;
        }
        return NULL;
    }

    // Función auxiliar para desconectar un nodo de su padre (sin borrar memoria)
    void desconectarDelPadre(Node* nodo) {
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
    // ***************************

    // JSON: Métodos privados para serialización (guardar)
    string escaparTexto(string texto) { // JSON
        // Simple escape para evitar romper el JSON si el nombre tiene comillas
        // En un proyecto real se usaría una librería, aquí asumimos nombres simples
        return texto; 
    }

    void serializarNodo(Node* nodo, ofstream& archivo, int nivel) { // JSON
        string indent(nivel * 2, ' ');
        archivo << indent << "{\n";
        archivo << indent << "  \"id\": " << nodo->data << ",\n";
        archivo << indent << "  \"nombre\": \"" << escaparTexto(nodo->nombre) << "\",\n";
        archivo << indent << "  \"tipo\": \"" << nodo->tipo << "\",\n";
        archivo << indent << "  \"contenido\": \"" << escaparTexto(nodo->contenido) << "\",\n";
        archivo << indent << "  \"children\": [";
        
        if (!nodo->children.empty()) {
            archivo << "\n";
            for (size_t i = 0; i < nodo->children.size(); i++) {
                serializarNodo(nodo->children[i], archivo, nivel + 2);
                if (i < nodo->children.size() - 1) archivo << ",";
                archivo << "\n";
            }
            archivo << indent << "  ]\n";
        } else {
            archivo << "]\n";
        }
        archivo << indent << "}";
    }

    // Métodos privados para cargar el json
    void saltarEspacios(string& json, int& i) { // JSON
        while (i < json.length() && (json[i] == ' ' || json[i] == '\n' || json[i] == '\r' || json[i] == '\t')) i++;
    }

    string leerString(string& json, int& i) {
        saltarEspacios(json, i);
        if (json[i] == '"') i++;
        string valor = "";
        while (i < json.length() && json[i] != '"') {
            valor += json[i];
            i++;
        }
        if (i < json.length()) i++;
        return valor;
    }

    int leerInt(string& json, int& i) {
        saltarEspacios(json, i);
        string numStr = "";
        while (i < json.length() && (isdigit(json[i]))) {
            numStr += json[i];
            i++;
        }
        if (numStr == "") return 0;
        return stoi(numStr);
    }

    void saltarHasta(string& json, int& i, char c) { 
        while (i < json.length() && json[i] != c) i++;
        if (i < json.length()) i++;
    }

    Node* deserializarNodoRecursivo(string& json, int& i) {
        saltarEspacios(json, i);
        if (json[i] != '{') return NULL;
        i++; 

        int id = 0;
        string nombre = "", tipo = "", contenido = "";
        vector<Node*> hijos;

        while (i < json.length()) {
            saltarEspacios(json, i);
            if (json[i] == '}') { i++; break; }

            string key = leerString(json, i);
            saltarHasta(json, i, ':');

            if (key == "id") {
                id = leerInt(json, i);
            } else if (key == "nombre") {
                nombre = leerString(json, i);
            } else if (key == "tipo") {
                tipo = leerString(json, i);
            } else if (key == "contenido") {
                contenido = leerString(json, i);
            } else if (key == "children") {
                saltarEspacios(json, i);
                if (json[i] == '[') {
                    i++; 
                    while (i < json.length()) {
                        saltarEspacios(json, i);
                        if (json[i] == ']') { i++; break; }
                        if (json[i] == '{') {
                            Node* hijo = deserializarNodoRecursivo(json, i);
                            if (hijo) hijos.push_back(hijo);
                        } else {
                            i++; 
                        }
                    }
                }
            } else {
                saltarEspacios(json, i);
                if (json[i] == '"') leerString(json, i);
                else leerInt(json, i);
            }
            saltarEspacios(json, i);
            if (json[i] == ',') i++;
        }

        Node* nodo = new Node(id, nombre, tipo, contenido);
        for (Node* hijo : hijos) {
            hijo->padre = nodo;
            nodo->children.push_back(hijo);
        }
        return nodo;
    }

    void guardarDatos() {
        ofstream archivo("filesystem.json");
        if (archivo.is_open()) {
            archivo << "{\n";
            archivo << "  \"siguienteId\": " << siguienteId << ",\n";
            archivo << "  \"arbol\": ";
            serializarNodo(root, archivo, 2);
            archivo << "\n}";
            archivo.close();
        } else {
            cout << "Error al guardar el archivo JSON." << endl;
        }
    }

    void cargarDatos() { 
        ifstream archivo("filesystem.json");
        if (archivo.is_open()) {
            stringstream buffer;
            buffer << archivo.rdbuf();
            string json = buffer.str();
            archivo.close();

            int i = 0;
            saltarEspacios(json, i);
            if (json[i] == '{') {
                i++;
                while (i < json.length()) {
                    saltarEspacios(json, i);
                    if (json[i] == '}') break;
                    
                    string key = leerString(json, i);
                    saltarHasta(json, i, ':');

                    if (key == "siguienteId") {
                        siguienteId = leerInt(json, i);
                    } else if (key == "arbol") {
                        if (root) delete root;
                        root = deserializarNodoRecursivo(json, i);
                    }
                    
                    saltarEspacios(json, i);
                    if (json[i] == ',') i++;
                }
            }
            cout << "Sistema de archivos cargado correctamente." << endl;
        } else {
            root = new Node(0, "root", "carpeta");
            siguienteId = 1;
            cout << "Iniciando sistema nuevo." << endl;
        }
    }

public:
    FileSystem()
    {
        root = NULL;
        siguienteId = 1;
        cargarDatos();
        if (root == NULL) {
            root = new Node(0, "root", "carpeta");
            siguienteId = 1;
        }
    }

    void find(string nombreBuscado) {
        bool encontrado = false;
        buscarRecursivo(root, nombreBuscado, "root", encontrado);

        if (!encontrado) {
            cout << "X No se encontró ningún archivo o carpeta con el nombre: " << nombreBuscado << endl;
        }
    }

    Node *buscarPorRuta(string ruta)
    {
        if (ruta == "root" || ruta == "/") {
            return root;
        }
        
        vector<string> partes = dividirRuta(ruta);
        Node *actual = root;

        size_t inicio = (partes.size() > 0 && partes[0] == "root") ? 1 : 0;

        for (size_t i = inicio; i < partes.size(); i++)
        {
            string nombreBuscado = partes[i];
            bool encontrado = false;
            
            for (size_t j = 0; j < actual->children.size(); j++)
            {
                if (actual->children[j]->nombre == nombreBuscado)
                {
                    actual = actual->children[j];
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado)
            {
                return NULL;
            }
        }

        return actual;
    }

    void mkdir(string rutaCompleta) {
        vector<string> partes = dividirRuta(rutaCompleta);
        
        if (partes.empty()) {
            cout << "Error: ruta vacía" << endl;
            return;
        }
        
        string nombreNuevaCarpeta = partes[partes.size() - 1];
        
        string rutaPadre = "root";
        size_t inicio = (partes[0] == "root") ? 1 : 0;
        
        for (size_t i = inicio; i < partes.size() - 1; i++) {
            rutaPadre += "/" + partes[i];
        }
        
        if (partes.size() == 1 || (partes.size() == 2 && partes[0] == "root")) {
            rutaPadre = "root";
        }
        
        Node* padre = buscarPorRuta(rutaPadre);
        
        if (padre == NULL) {
            cout << "Error: la ruta '" << rutaPadre << "' no existe" << endl;
            return;
        }
        
        if (!padre->esCarpeta()) {
            cout << "Error: '" << rutaPadre << "' no es una carpeta" << endl;
            return;
        }
        
        for (size_t i = 0; i < padre->children.size(); i++) {
            if (padre->children[i]->nombre == nombreNuevaCarpeta) {
                cout << "Error: la carpeta '" << nombreNuevaCarpeta << "' ya existe" << endl;
                return;
            }
        }
        
        Node* nuevaCarpeta = new Node(siguienteId++, nombreNuevaCarpeta, "carpeta");
        nuevaCarpeta->padre = padre;
        padre->children.push_back(nuevaCarpeta);
        
        guardarDatos(); // guardar al modificar
        cout << "✓ Carpeta '" << nombreNuevaCarpeta << "' creada" << endl;
    }
    
    void touch(string rutaCompleta, string contenido = "") {
        vector<string> partes = dividirRuta(rutaCompleta);
        
        if (partes.empty()) {
            cout << "Error: ruta vacía" << endl;
            return;
        }
        
        string nombreNuevoArchivo = partes[partes.size() - 1];
        
        string rutaPadre = "root";
        size_t inicio = (partes[0] == "root") ? 1 : 0;
        
        for (size_t i = inicio; i < partes.size() - 1; i++) {
            rutaPadre += "/" + partes[i];
        }
        
        if (partes.size() == 1 || (partes.size() == 2 && partes[0] == "root")) {
            rutaPadre = "root";
        }
        
        Node* padre = buscarPorRuta(rutaPadre);
        
        if (padre == NULL) {
            cout << "Error: la ruta '" << rutaPadre << "' no existe" << endl;
            return;
        }
        
        if (!padre->esCarpeta()) {
            cout << "Error: '" << rutaPadre << "' no es una carpeta" << endl;
            return;
        }
        
        for (size_t i = 0; i < padre->children.size(); i++) {
            if (padre->children[i]->nombre == nombreNuevoArchivo) {
                cout << "Error: el archivo '" << nombreNuevoArchivo << "' ya existe" << endl;
                return;
            }
        }
        
        Node* nuevoArchivo = new Node(siguienteId++, nombreNuevoArchivo, "archivo", contenido);
        nuevoArchivo->padre = padre;
        padre->children.push_back(nuevoArchivo);
        
        guardarDatos(); // guardar al modificar
        cout << "✓ Archivo '" << nombreNuevoArchivo << "' creado" << endl;
    }
    
    void mostrarArbol(Node* nodo = NULL, int nivel = 0) {
        if (nodo == NULL) {
            nodo = root;
        }
        
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

    void moverNodo(string nombreNodo, string rutaDestino) {
        Node* nodoAMover = obtenerPunteroRecursivo(root, nombreNodo);
        
        if (nodoAMover == NULL) {
            cout << "X Error: El archivo/carpeta '" << nombreNodo << "' no existe." << endl;
            return;
        }
        
        if (nodoAMover == root) {
            cout << "X Error: No puedes mover la raíz." << endl;
            return;
        }

        Node* destino = buscarPorRuta(rutaDestino);
        
        if (destino == NULL) {
            cout << "X Error: La ruta destino '" << rutaDestino << "' no existe." << endl;
            return;
        }

        if (!destino->esCarpeta()) {
            cout << "X Error: El destino debe ser una carpeta." << endl;
            return;
        }

        desconectarDelPadre(nodoAMover);
        
        nodoAMover->padre = destino;
        destino->children.push_back(nodoAMover);
        
        guardarDatos(); // guardar al modificar
        cout << "✓ Se movió '" << nombreNodo << "' a '" << rutaDestino << "'" << endl;
    }

    void eliminarNodo(string nombreNodo) {
        Node* nodoAEliminar = obtenerPunteroRecursivo(root, nombreNodo);

        if (nodoAEliminar == NULL) {
            cout << "X Error: El archivo/carpeta '" << nombreNodo << "' no existe." << endl;
            return;
        }

        if (nodoAEliminar == root) {
            cout << "X Error: No puedes eliminar la raíz." << endl;
            return;
        }

        if (nodoAEliminar->children.size() > 0) {
            char confirmacion;
            cout << "⚠ La carpeta '" << nombreNodo << "' no está vacía. Se borrará todo su contenido." << endl;
            cout << "¿Estás seguro? (s/n): ";
            cin >> confirmacion;
            cin.ignore(); 

            if (confirmacion != 's' && confirmacion != 'S') {
                cout << "Operación cancelada." << endl;
                return;
            }
        }

        desconectarDelPadre(nodoAEliminar);
        liberarMemoria(nodoAEliminar);
        guardarDatos(); // guardar al modificar
        cout << "✓ '" << nombreNodo << "' ha sido eliminado." << endl;
    }

    void help() {
        cout << "--- Comandos Disponibles ---" << endl;
        cout << "- mkdir           : Crea una carpeta (Ruta completa)." << endl;
        cout << "- touch           : Crea un archivo (Ruta completa + contenido)." << endl;
        cout << "- tree            : Muestra la estructura del árbol." << endl;
        cout << "- export preorden : Alias de 'tree'." << endl;
        cout << "- search / find   : Busca un nodo por nombre." << endl;
        cout << "- mv              : Mueve un archivo/carpeta de ubicación." << endl;
        cout << "- rm              : Elimina un archivo/carpeta (con confirmación)." << endl;
        cout << "- /help           : Muestra esta ayuda." << endl;
        cout << "- salir           : Cierra el programa." << endl;
        cout << "----------------------------" << endl;
    }
    // ***************************
    
    ~FileSystem() {
        if (root) liberarMemoria(root); // checar nulidad del json
    }
    
    void liberarMemoria(Node* nodo) {
        for (size_t i = 0; i < nodo->children.size(); i++) {
            liberarMemoria(nodo->children[i]);
        }
        delete nodo;
    }
};

int main(){
    FileSystem fs;
    string operacion = "";

    do {
        cout << "c:> ";
        getline(cin, operacion);

        if (operacion == "mkdir") {
            string ruta = "";
            cout << "Ruta completa: ";
            getline(cin, ruta);
            fs.mkdir(ruta);
            cout << endl;
        }
        else if (operacion == "touch") {
            string ruta = "";
            string contenido = "";
            cout << "Ruta completa: ";
            getline(cin, ruta);
            cout << "Contenido: ";
            getline(cin, contenido);
            fs.touch(ruta, contenido);
            cout << endl;
        }
        // ***************************
        else if (operacion == "tree" || operacion == "export preorden") {
            cout << endl;
            fs.mostrarArbol();
            cout << endl;
        }
        else if (operacion == "search" || operacion == "find") {
            string nombre = "";
            cout << "Archivo a buscar: ";
            getline(cin, nombre);
            cout << endl;
            fs.find(nombre);
            cout << endl;
        }
        else if (operacion == "mv") {
            string nombre = "";
            string destino = "";
            cout << "Archivo a mover: ";
            getline(cin, nombre);
            cout << "Nueva ubicacion de " << nombre << ": ";
            getline(cin, destino);
            fs.moverNodo(nombre, destino);
            cout << endl;
        }
        else if (operacion == "rm") {
            string nombre = "";
            cout << "Archivo a eliminar: ";
            getline(cin, nombre);
            fs.eliminarNodo(nombre);
            cout << endl;
        }
        else if (operacion == "/help") {
            cout << endl;
            fs.help();
            cout << endl;
        }
        // ***************************

    } while(operacion != "salir");

    return 0;
}
