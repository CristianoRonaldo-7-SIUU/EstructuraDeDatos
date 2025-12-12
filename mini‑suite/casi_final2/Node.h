#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

using namespace std;

struct Node {
    int data;
    string nombre;
    string tipo;
    string contenido;
    
    //Guarda ruta original para restuarar archivos de en la papelera
    string rutaOriginal; 

    Node *padre;
    vector<Node *> children;

    Node(int id, string n, string t, string cont = "");
    
    bool esCarpeta();
};

#endif
