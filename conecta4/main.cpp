#include <bits/stdc++.h>
#include <chrono>
#include <thread>
using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[H\033[2J" << flush;
#endif
}

// Mostrar tablero alineado y el historial
void mostrarTablero(const vector<vector<char>>& t, const vector<string>& historial) {
    clearScreen();
    cout << "\n    ";
    for (int c = 0; c < COLUMNAS; c++) cout << c + 1 << "   ";
    cout << "\n";

    for (int f = 0; f < FILAS; f++) {
        cout << "   ";
        for (int c = 0; c < COLUMNAS; c++) {
            cout << " " << t[f][c] << " ";
            if (c < COLUMNAS - 1) cout << "|";
        }
        cout << "\n";
        if (f < FILAS - 1) {
            cout << "   ";
            for (int c = 0; c < COLUMNAS; c++) {
                cout << "---";
                if (c < COLUMNAS - 1) cout << "+";
            }
            cout << "\n";
        }
    }

    cout << "\nHistorial de jugadas:\n";
    for (const auto &h : historial) cout << h << "\n";
    cout << "\n";
}

// Mensajes de felicitación
string mensajeVictoria(const string& nombre) {
    vector<string> mensajes = {
        "¡Felicidades " + nombre + "!",
        "¡" + nombre + " es el GOAT.",
        "Victoria épica de " + nombre,
        "¡" + nombre + " demuestra ser mejor jugador!",
        "¡Increíble! " + nombre + " conecta 4 y se lleva la partida."
    };
    return mensajes[rand() % mensajes.size()];
}

// Colocar ficha en una columna (devuelve true si se colocó y fila en filaColocada)
bool colocarFicha(vector<vector<char>>& t, int col, char ficha, int& filaColocada) {
    if (col < 0 || col >= COLUMNAS) return false;
    for (int f = FILAS - 1; f >= 0; f--) {
        if (t[f][col] == '.') {
            t[f][col] = ficha;
            filaColocada = f;
            return true;
        }
    }
    return false;
}

// Comprobar si hay ganador alrededor de la última ficha colocada
bool hayGanador(const vector<vector<char>>& t, int fila, int col, char ficha) {
    const int dirs[4][2] = {{0,1},{1,0},{1,1},{1,-1}};
    for (auto &d : dirs) {
        int cuenta = 1;
        // hacia adelante
        for (int k = 1; k < 4; k++) {
            int nf = fila + d[0]*k, nc = col + d[1]*k;
            if (nf < 0 || nf >= FILAS || nc < 0 || nc >= COLUMNAS || t[nf][nc] != ficha) break;
            cuenta++;
        }
        // hacia atrás
        for (int k = 1; k < 4; k++) {
            int nf = fila - d[0]*k, nc = col - d[1]*k;
            if (nf < 0 || nf >= FILAS || nc < 0 || nc >= COLUMNAS || t[nf][nc] != ficha) break;
            cuenta++;
        }
        if (cuenta >= 4) return true;
    }
    return false;
}

// Lista de columnas válidas
vector<int> posiblesMovimientos(const vector<vector<char>>& t) {
    vector<int> res;
    for (int c = 0; c < COLUMNAS; c++) if (t[0][c] == '.') res.push_back(c);
    return res;
}

// IA aleatoria
int jugadaAleatoria(const vector<vector<char>>& t) {
    auto opciones = posiblesMovimientos(t);
    if (opciones.empty()) return -1;
    return opciones[rand() % opciones.size()];
}

// IA "inteligente": intenta ganar, bloquea, si no elige aleatoria
int jugadaInteligente(vector<vector<char>> t, char ficha, char rival) {
    // 1) intentar ganar
    for (int c = 0; c < COLUMNAS; c++) {
        if (t[0][c] == '.') {
            int fila;
            colocarFicha(t, c, ficha, fila);
            if (hayGanador(t, fila, c, ficha)) return c;
            t[fila][c] = '.'; // revertir
        }
    }
    // 2) bloquear rival
    for (int c = 0; c < COLUMNAS; c++) {
        if (t[0][c] == '.') {
            int fila;
            colocarFicha(t, c, rival, fila);
            if (hayGanador(t, fila, c, rival)) return c;
            t[fila][c] = '.';
        }
    }
    // 3) preferir centro si está libre (ligera heurística)
    int centro = COLUMNAS / 2;
    if (t[0][centro] == '.') return centro;
    // 4) fallback aleatorio
    return jugadaAleatoria(t);
}

// Espera aleatoria para IA entre 0.5 y 2.5 s
void esperarIA() {
    int ms = 500 + (rand() % 2001); // 500..2500 ms
    this_thread::sleep_for(chrono::milliseconds(ms));
}

// Ejecutar una partida (modo: 1 PvP, 2 PvIA, 3 IA v IA)
void jugarPartida(int modo) {
    vector<vector<char>> t(FILAS, vector<char>(COLUMNAS, '.'));
    vector<string> historial;
    string nombre1, nombre2;
    char iaTipo1 = 'a', iaTipo2 = 'a'; // 'a' aleatoria, 'i' inteligente

    if (modo == 1) {
        clearScreen();
        cout << "Nombre del Jugador 1 (X): "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre1);
        cout << "Nombre del Jugador 2 (O): "; getline(cin, nombre2);
    } else if (modo == 2) {
        clearScreen();
        cout << "Nombre del Jugador (X): "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, nombre1);
        nombre2 = "IA";
        cout << "¿IA aleatoria o inteligente? (a/i): "; cin >> iaTipo2;
        iaTipo2 = (iaTipo2 == 'i' ? 'i' : 'a');
    } else { // modo 3
        nombre1 = "IA 1"; nombre2 = "IA 2";
        clearScreen();
        cout << "¿IA 1 aleatoria o inteligente? (a/i): "; cin >> iaTipo1;
        iaTipo1 = (iaTipo1 == 'i' ? 'i' : 'a');
        cout << "¿IA 2 aleatoria o inteligente? (a/i): "; cin >> iaTipo2;
        iaTipo2 = (iaTipo2 == 'i' ? 'i' : 'a');
    }

    int turnos = 0;
    bool terminado = false;

    while (turnos < FILAS * COLUMNAS && !terminado) {
        // Mostrar tablero y historial (clear dentro de mostrar)
        mostrarTablero(t, historial);

        string jugador = (turnos % 2 == 0) ? nombre1 : nombre2;
        char ficha = (turnos % 2 == 0) ? 'X' : 'O';
        bool esIA = false;
        char tipoIA = 'a';
        if (modo == 2) {
            if (turnos % 2 == 1) { esIA = true; tipoIA = iaTipo2; }
        } else if (modo == 3) {
            if (turnos % 2 == 0) { esIA = true; tipoIA = iaTipo1; }
            else { esIA = true; tipoIA = iaTipo2; }
        }

        int col = -1, filaColocada = -1;
        if (!esIA) {
            cout << "Turno de " << jugador << " (" << ficha << "). Elige columna (1-7): ";
            if (!(cin >> col)) { // manejar entrada no válida
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida. Intenta de nuevo.\n";
                continue;
            }
            col--; // ajustar índice
        } else {
            // IA: espera, luego decide movimiento según tipo
            esperarIA();
            if (tipoIA == 'i') col = jugadaInteligente(t, ficha, (ficha == 'X' ? 'O' : 'X'));
            else col = jugadaAleatoria(t);
            // mostrar jugada elegida en consola
            cout << "Turno de " << jugador << " (" << ficha << "). Elige columna (1-7): " << (col + 1) << "\n";
        }

        // Intentar colocar ficha
        if (col < 0 || col >= COLUMNAS || !colocarFicha(t, col, ficha, filaColocada)) {
            // columna inválida o llena
            if (!esIA) {
                cout << "Columna inválida o llena. Intenta de nuevo.\n";
                continue;
            } else {
                // si IA eligió columna inválida (raro), elegir random válida
                auto opts = posiblesMovimientos(t);
                if (opts.empty()) break;
                col = opts[rand() % opts.size()];
                colocarFicha(t, col, ficha, filaColocada);
            }
        }

        // Añadir al historial
        stringstream ss;
        ss << jugador << " (" << ficha << ") colocó en columna " << (col + 1);
        historial.push_back(ss.str());

        // Comprobar victoria
        if (hayGanador(t, filaColocada, col, ficha)) {
            mostrarTablero(t, historial);
            cout << mensajeVictoria(jugador) << "\n";
            terminado = true;
            break;
        }

        // comprobar empate
        if (posiblesMovimientos(t).empty()) {
            mostrarTablero(t, historial);
            cout << "¡Empate! No quedan movimientos.\n";
            terminado = true;
            break;
        }

        turnos++;
    }

    cout << "\nFin de la partida. Pulsa ENTER para volver al menú...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Menú principal
int main() {
    srand((unsigned)time(nullptr));

    while (true) {
        clearScreen();
        cout << "\n--- CONECTA 4 ---\n";
        cout << "1. Jugador vs Jugador\n";
        cout << "2. Jugador vs IA\n";
        cout << "3. IA vs IA\n";
        cout << "4. Salir\n";
        cout << "Elige una opción: ";

        int opcion;
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Intenta de nuevo.\n";
            this_thread::sleep_for(chrono::milliseconds(800));
            continue;
        }

        if (opcion == 4) break;
        if (opcion >= 1 && opcion <= 3) {
            jugarPartida(opcion);
        } else {
            cout << "Opción no válida. Intenta otra vez.\n";
            this_thread::sleep_for(chrono::milliseconds(800));
        }
    }

    cout << "Gracias por jugar. ¡Hasta pronto!\n";
    return 0;
}