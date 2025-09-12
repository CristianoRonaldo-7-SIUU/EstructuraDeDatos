import java.util.*;

public class Main {
    static Scanner sc = new Scanner(System.in);
    static Random rand = new Random();
    static char[][] tablero = new char[3][3];
    static List<String> historial = new ArrayList<>();

    public static void main(String[] args) {
        String jugador1, jugador2;
        boolean jugar = true;

        System.out.print("Nombre del jugador 1 (X): ");
        jugador1 = sc.nextLine();
        System.out.print("Nombre del jugador 2 (O): ");
        jugador2 = sc.nextLine();

        while (jugar) {
            inicializarTablero();
            historial.clear();
            jugarPartida(jugador1, jugador2);

            System.out.print("\n¿Quieren jugar otra partida? (s/n): ");
            String resp = sc.nextLine().trim().toLowerCase();
            if (resp.equals("s")) {
                System.out.print("¿Quieren cambiar de nombre? (s/n): ");
                String cambiar = sc.nextLine().trim().toLowerCase();
                if (cambiar.equals("s")) {
                    System.out.print("Nombre del jugador 1 (X): ");
                    jugador1 = sc.nextLine();
                    System.out.print("Nombre del jugador 2 (O): ");
                    jugador2 = sc.nextLine();
                }
            } else {
                jugar = false;
                System.out.println("\nGracias por jugar. ¡Hasta la próxima!");
            }
        }
    }

    static void inicializarTablero() {
        for (int i = 0; i < 3; i++) {
            Arrays.fill(tablero[i], ' ');
        }
    }

    static void jugarPartida(String j1, String j2) {
        int turnos = 0;
        boolean victoria = false;
        String jugadorActual = j1;
        char figura = 'X';

        while (turnos < 9 && !victoria) {
            limpiarPantalla();
            mostrarTurno(jugadorActual, figura);
            mostrarTableroConInstrucciones();

            int col = pedirColumna();
            int fila = pedirFila();

            if (tablero[fila][col] != ' ') {
                System.out.println("\nLa posición " + (char)('A' + fila) + (col + 1) + " está ocupada. Pulsa ENTER para continuar...");
                sc.nextLine();
                continue;
            }

            tablero[fila][col] = figura;
            String jugada = jugadorActual + " (" + figura + ") colocó en " + (char)('A' + fila) + (col + 1);
            historial.add(0, jugada);
            turnos++;

            if (turnos >= 5 && hayVictoria(figura)) {
                limpiarPantalla();
                mostrarTurno(jugadorActual, figura);
                mostrarTableroConInstrucciones();
                mostrarFelicitacion(jugadorActual);
                victoria = true;
                break;
            }

            // Cambiar de jugador
            if (jugadorActual.equals(j1)) {
                jugadorActual = j2;
                figura = 'O';
            } else {
                jugadorActual = j1;
                figura = 'X';
            }
        }

        if (!victoria && turnos == 9) {
            limpiarPantalla();
            mostrarTurno(jugadorActual, figura);
            mostrarTableroConInstrucciones();
            System.out.println("\n¡Es un empate!");
        }
    }

    static void mostrarTurno(String jugador, char figura) {
        System.out.println("Turno de " + jugador + " (" + figura + ")");
    }

    static void mostrarTableroConInstrucciones() {
        System.out.println("    1   2   3");
        for (int i = 0; i < 3; i++) {
            System.out.print((char)('A' + i) + "   ");
            for (int j = 0; j < 3; j++) {
                System.out.print(tablero[i][j]);
                if (j < 2) System.out.print(" | ");
            }
            System.out.println();
            if (i < 2) System.out.println("   ---+---+---");
        }

        System.out.println("\nElige columna y fila para colocar tu figura.");

        if (!historial.isEmpty()) {
            System.out.println("\nHistorial de jugadas:");
            for (String jugada : historial) {
                System.out.println(jugada);
            }
        }
    }

    static int pedirColumna() {
        int col = -1;
        while (col < 0 || col > 2) {
            System.out.print("Columna (1-3): ");
            String input = sc.nextLine().trim();
            if (input.matches("[1-3]")) {
                col = Integer.parseInt(input) - 1;
            }
        }
        return col;
    }

    static int pedirFila() {
        int fila = -1;
        while (fila < 0 || fila > 2) {
            System.out.print("Fila (A-C): ");
            String input = sc.nextLine().trim().toUpperCase();
            if (input.matches("[ABC]")) {
                fila = input.charAt(0) - 'A';
            }
        }
        return fila;
    }

    static boolean hayVictoria(char f) {
        for (int i = 0; i < 3; i++) {
            if (tablero[i][0] == f && tablero[i][1] == f && tablero[i][2] == f) return true;
        }
        for (int j = 0; j < 3; j++) {
            if (tablero[0][j] == f && tablero[1][j] == f && tablero[2][j] == f) return true;
        }
        if (tablero[0][0] == f && tablero[1][1] == f && tablero[2][2] == f) return true;
        if (tablero[0][2] == f && tablero[1][1] == f && tablero[2][0] == f) return true;
        return false;
    }

    static void mostrarFelicitacion(String ganador) {
        String[] mensajes = {
            "¡Maravillosa jugada!",
            "Pónganle un rival de verdad",
            "Casi tan bueno como Cristiano Ronaldo",
            "¡Qué talento!",
            "Ganó en el gato al gato"
        };
        System.out.println("\n" + ganador + " gana. " + mensajes[rand.nextInt(mensajes.length)]);
    }

    static void limpiarPantalla() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }
}