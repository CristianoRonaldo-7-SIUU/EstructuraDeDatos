#include <iostream>
using namespace std;

struct GOAT {
    string nombre;
    int edad;
};

int main() {
    GOAT goats[3] = {
        {"Ronaldo", 39},
        {"Mourinho", 61},
        {"Khabib", 36}
    };

    for(int i = 0; i < 3; i++) {
        cout << goats[i].nombre << " " << goats[i].edad << endl;
    }
    return 0;
}