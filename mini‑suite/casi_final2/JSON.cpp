#include "JSON.h"
#include <iostream>

using namespace std;

string JSON::escaparTexto(string texto) { 
    return texto; 
}

void JSON::serializarNodo(Node* nodo, ofstream& archivo, int nivel) { 
    string indent(nivel * 2, ' ');
    archivo << indent << "{\n";
    archivo << indent << "  \"id\": " << nodo->data << ",\n";
    archivo << indent << "  \"nombre\": \"" << escaparTexto(nodo->nombre) << "\",\n";
    archivo << indent << "  \"tipo\": \"" << nodo->tipo << "\",\n";
    archivo << indent << "  \"contenido\": \"" << escaparTexto(nodo->contenido) << "\",\n";
    
    // Guardar ruta original si existe
    if (!nodo->rutaOriginal.empty()) {
        archivo << indent << "  \"rutaOriginal\": \"" << escaparTexto(nodo->rutaOriginal) << "\",\n";
    }

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

void JSON::saltarEspacios(string& json, int& i) { 
    while (i < json.length() && (json[i] == ' ' || json[i] == '\n' || json[i] == '\r' || json[i] == '\t')) i++;
}

string JSON::leerString(string& json, int& i) {
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

int JSON::leerInt(string& json, int& i) {
    saltarEspacios(json, i);
    string numStr = "";
    while (i < json.length() && (isdigit(json[i]))) {
        numStr += json[i];
        i++;
    }
    if (numStr == "") return 0;
    return stoi(numStr);
}

void JSON::saltarHasta(string& json, int& i, char c) { 
    while (i < json.length() && json[i] != c) i++;
    if (i < json.length()) i++;
}

Node* JSON::deserializarNodoRecursivo(string& json, int& i) {
    saltarEspacios(json, i);
    if (json[i] != '{') return NULL;
    i++; 

    int id = 0;
    string nombre = "", tipo = "", contenido = "", rutaOriginal = "";
    vector<Node*> hijos;

    while (i < json.length()) {
        saltarEspacios(json, i);
        if (json[i] == '}') { i++; break; }

        string key = leerString(json, i);
        saltarHasta(json, i, ':');

        if (key == "id") id = leerInt(json, i);
        else if (key == "nombre") nombre = leerString(json, i);
        else if (key == "tipo") tipo = leerString(json, i);
        else if (key == "contenido") contenido = leerString(json, i);
        else if (key == "rutaOriginal") rutaOriginal = leerString(json, i);
        else if (key == "children") {
            saltarEspacios(json, i);
            if (json[i] == '[') {
                i++; 
                while (i < json.length()) {
                    saltarEspacios(json, i);
                    if (json[i] == ']') { i++; break; }
                    if (json[i] == '{') {
                        Node* hijo = deserializarNodoRecursivo(json, i);
                        if (hijo) hijos.push_back(hijo);
                    } else i++;
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
    nodo->rutaOriginal = rutaOriginal;

    for (Node* hijo : hijos) {
        hijo->padre = nodo;
        nodo->children.push_back(hijo);
    }
    return nodo;
}

void JSON::guardarDatos(Node* root, vector<Node*>& papelera, int siguienteId) {
    ofstream archivo("filesystem.json");
    if (archivo.is_open()) {
        archivo << "{\n";
        archivo << "  \"siguienteId\": " << siguienteId << ",\n";
        
        // Guardar Arbol
        archivo << "  \"arbol\": ";
        serializarNodo(root, archivo, 2);
        
        // Guardar Papelera
        archivo << ",\n  \"papelera\": [\n";
        for (size_t i = 0; i < papelera.size(); i++) {
            serializarNodo(papelera[i], archivo, 4);
            if (i < papelera.size() - 1) archivo << ",";
            archivo << "\n";
        }
        archivo << "  ]\n";

        archivo << "}";
        archivo.close();
    }
}

bool JSON::cargarDatos(Node*& root, vector<Node*>& papelera, int& siguienteId) { 
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
                } else if (key == "papelera") {
                    // Limpiar papelera actual si existe
                    for(Node* n : papelera) delete n;
                    papelera.clear();

                    saltarEspacios(json, i);
                    if (json[i] == '[') {
                        i++;
                        while (i < json.length()) {
                            saltarEspacios(json, i);
                            if (json[i] == ']') { i++; break; }
                            if (json[i] == '{') {
                                Node* basura = deserializarNodoRecursivo(json, i);
                                if (basura) papelera.push_back(basura);
                            } else i++;
                            
                            saltarEspacios(json, i);
                            if (json[i] == ',') i++;
                        }
                    }
                }
                
                saltarEspacios(json, i);
                if (json[i] == ',') i++;
            }
        }
        return true;
    }
    return false;
}
