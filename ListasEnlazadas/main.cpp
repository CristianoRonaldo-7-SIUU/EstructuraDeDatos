#include <iostream>
#include <stdlib.h>
#include "singly.h"
#include "doubly.h"
#include "circular.h"
#include "cdoubly.h"

using namespace std;

int main() {
    int option = 0;
    while(option != 5) {
        cout << "\n\n********* MASTER MENU *********\n";
        cout << "1. Lista Simple (Singly)\n";
        cout << "2. Lista Doble (Doubly)\n";
        cout << "3. Lista Circular Simple\n";
        cout << "4. Lista Circular Doble\n";
        cout << "5. Salir\n";
        cout << "\nOpcion: ";
        cin >> option;

        switch(option) {
            case 1: Singly::runMenu(); break;
            case 2: Doubly::runMenu(); break;
            case 3: Circular::runMenu(); break;
            case 4: CDoubly::runMenu(); break;
            case 5: exit(0); break;
            default: cout << "Opcion invalida.\n";
        }
    }
    return 0;
}
