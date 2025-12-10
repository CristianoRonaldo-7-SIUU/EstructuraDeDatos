#ifdef _WIN32
    #include <windows.h>
#endif

#include "TerminalUI.h"

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif
    
    TerminalUI ui;
    ui.iniciar();
    return 0;
}