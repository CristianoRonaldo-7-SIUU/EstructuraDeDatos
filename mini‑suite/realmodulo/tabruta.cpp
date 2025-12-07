#include "tabruta.h"
#include <iostream>
#include <vector>

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
// Función auxiliar para Linux
int getch_ruta() {
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

string tabruta::leerRuta(Cfs& core) {
    string buffer = "";
    char ch;

    while (true) {
        #ifdef _WIN32
            ch = _getch();
        #else
            ch = getch_ruta();
        #endif

        if (ch == TECLA_ENTER) {
            cout << endl;
            return buffer;
        }
        else if (ch == TECLA_BACKSPACE) {
            if (!buffer.empty()) {
                buffer.pop_back();
                cout << "\b \b"; 
            }
        }
        else if (ch == TECLA_TAB) {
            // ***************************
            if (!buffer.empty()) {
                vector<string> sugerencias = core.obtenerSugerencias(buffer);
                
                if (sugerencias.size() == 1) {
                    string encontrada = sugerencias[0];
                    
                    // Calcular la parte que falta escribir PURA MAGIAAA
                    if (encontrada.length() > buffer.length()) {
                        string falta = encontrada.substr(buffer.length());
                        buffer += falta;
                        cout << falta;
                    }
                }
                // Si hay más de una no se hace nada yea yea (Tal vez listar opciones al dar doble tab?)
            }
            // ***************************
        }
        else if (ch >= 32 && ch <= 126) {
            buffer += ch;
            cout << ch;
        }
    }
}
