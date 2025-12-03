#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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

public:
    FileSystem()
    {
        root = new Node(0, "root", "carpeta");
        siguienteId = 1;
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
    
    ~FileSystem() {
        liberarMemoria(root);
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
        else if (operacion == "tree") {
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

    } while(operacion != "salir");

    return 0;

}
