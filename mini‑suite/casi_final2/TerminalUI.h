#ifndef TERMINALUI_H
#define TERMINALUI_H

#include "Cfs.h"
#include "tabcomandos.h"
#include "tabruta.h"
#include <string>

using namespace std;

class TerminalUI {
private:
    Cfs core;

    void mostrarArbolRecursivo(Node* nodo, string prefix, bool esUltimo);

    void help();

public:
    void iniciar();
};

#endif
