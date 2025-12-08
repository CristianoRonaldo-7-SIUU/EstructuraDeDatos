#include "tabcomandos.h"
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#define TECLA_TAB 9
#define TECLA_ENTER 13
#define TECLA_BACKSPACE 8
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#define TECLA_TAB 9
#define TECLA_ENTER 10
#define TECLA_BACKSPACE 127
// Función auxiliar para Linux para leer sin esperar Enter
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

using namespace std;

string tabcomandos::leerInput() {
    string buffer = "";
    char ch;
    
    // Lista fija de comandos disponibles
    vector<string> comandos = {
        "mkdir", "touch", "tree", "search", "find", 
        "mv", "rm", "/help", "salir", "export preorden",
        "rename", "cat", "cat >", "cat >>" 
    };

    while (true) {
        #ifdef _WIN32
            ch = _getch();
        #else
            ch = getch();
        #endif

        if (ch == TECLA_ENTER) {
            cout << endl;
            return buffer;
        }
        else if (ch == TECLA_BACKSPACE) {
            if (!buffer.empty()) {
                buffer.pop_back();
                cout << "\b \b"; // Borrar visualmente
            }
        }
        else if (ch == TECLA_TAB) {
            if (!buffer.empty()) {
                // Permitimos espacio si empieza con cat para los subcomandos
                if (buffer.find(' ') == string::npos || buffer.substr(0, 3) == "cat") {
                    string coincidencia = "";
                    int encontrados = 0;

                    for (const string& cmd : comandos) {
                        // Si el comando empieza con lo que tenemos en el buffer
                        if (cmd.find(buffer) == 0) {
                            coincidencia = cmd;
                            encontrados++;
                        }
                    }

                    // Si hay UNA sola coincidencia completamos
                    if (encontrados == 1) {
                        string falta = coincidencia.substr(buffer.length());
                        buffer += falta;
                        cout << falta;
                    }
                }
            }
        }
        else if (ch >= 32 && ch <= 126) { // Caracteres imprimibles
            buffer += ch;
            cout << ch;
        }
    }
}
