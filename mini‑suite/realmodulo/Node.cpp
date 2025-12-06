#include "Node.h"

Node::Node(int id, string n, string t, string cont) {
    data = id;
    nombre = n;
    tipo = t;
    contenido = cont;
    padre = NULL;
}

bool Node::esCarpeta() {
    return tipo == "carpeta";
}
