#include "tabcomandos.h"
#include "flechitas.h" 
#include <iostream>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#define TECLA_TAB 9
#define TECLA_ENTER 13
#define TECLA_BACKSPACE 8
#define TECLA_ESCAPE 27
#define FLECHA_ARRIBA 72
#define FLECHA_ABAJO 80
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#define TECLA_TAB 9
#define TECLA_ENTER 10
#define TECLA_BACKSPACE 127
#define TECLA_ESCAPE 27
#define FLECHA_ARRIBA 65
#define FLECHA_ABAJO 66

// Función auxiliar para Linux
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

static Flechitas historial; 
void limpiarLineaVisual(int longitud) {
    for (int i = 0; i < longitud; i++) {
        cout << "\b \b";
    }
}

string tabcomandos::leerInput() {
    string buffer = "";
    char ch;
    
    vector<string> comandos = {
        "mkdir", "touch", "tree", "search", "find", 
        "mv", "rm", "/help", "salir", "export preorden",
        "rename", "cat"
    };

    while (true) {
        #ifdef _WIN32
            ch = _getch();
            // Detectar flechas en Windows (devuelven -32 o 0 seguido del código)
            if (ch == -32 || ch == 0) { 
                ch = _getch(); 
                if (ch == FLECHA_ARRIBA) {
                    string nuevoBuffer = historial.obtenerAnterior(buffer);
                    if (nuevoBuffer != buffer) {
                        limpiarLineaVisual(buffer.length());
                        buffer = nuevoBuffer;
                        cout << buffer;
                    }
                    continue; 
                }
                else if (ch == FLECHA_ABAJO) {
                    string nuevoBuffer = historial.obtenerSiguiente(buffer);
                    if (nuevoBuffer != buffer) {
                        limpiarLineaVisual(buffer.length());
                        buffer = nuevoBuffer;
                        cout << buffer;
                    }
                    continue;
                }
            }
        #else
            ch = getch();
            // Detectar flechas en Linux (Secuencia Escape [ A / [ B)
            if (ch == TECLA_ESCAPE) {
                getch(); // saltar [
                int flecha = getch();
                if (flecha == FLECHA_ARRIBA) {
                    string nuevoBuffer = historial.obtenerAnterior(buffer);
                    if (nuevoBuffer != buffer) {
                        limpiarLineaVisual(buffer.length());
                        buffer = nuevoBuffer;
                        cout << buffer;
                    }
                    continue;
                }
                else if (flecha == FLECHA_ABAJO) {
                    string nuevoBuffer = historial.obtenerSiguiente(buffer);
                    if (nuevoBuffer != buffer) {
                        limpiarLineaVisual(buffer.length());
                        buffer = nuevoBuffer;
                        cout << buffer;
                    }
                    continue;
                }
            }
        #endif

        if (ch == TECLA_ENTER) {
            cout << endl;
            // Guardamos en historial si no esta vacio
            if (!buffer.empty()) {
                historial.guardarComando(buffer);
            }
            return buffer;
        }
        else if (ch == TECLA_BACKSPACE) {
            if (!buffer.empty()) {
                buffer.pop_back();
                cout << "\b \b"; 
            }
        }
        else if (ch == TECLA_TAB) {
            if (!buffer.empty()) {
                // ***************************
                if (buffer == "cat") {
                    buffer = "cat >";
                    cout << " >";
                }
                else if (buffer == "cat >") {
                    buffer = "cat >>";
                    cout << ">"; 
                }
                else if (buffer == "cat >>") {
                    limpiarLineaVisual(6); 
                    buffer = "cat";
                    cout << "cat";
                }
                // ***************************
                else if (buffer.find(' ') == string::npos) {
                    string coincidencia = "";
                    int encontrados = 0;

                    for (const string& cmd : comandos) {
                        if (cmd.find(buffer) == 0) {
                            coincidencia = cmd;
                            encontrados++;
                        }
                    }

                    if (encontrados == 1) {
                        string falta = coincidencia.substr(buffer.length());
                        buffer += falta;
                        cout << falta;
                    }
                }
            }
        }
        else if (ch >= 32 && ch <= 126) { 
            buffer += ch;
            cout << ch;
        }
    }
}
