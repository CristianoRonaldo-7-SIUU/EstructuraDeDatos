#ifndef TERMINALUI_H
#define TERMINALUI_H

#include "Cfs.h"

class TerminalUI {
private:
    Cfs core;

    void mostrarArbol(Node* nodo = NULL, int nivel = 0);
    void help();

public:
    void iniciar();
};

#endif
