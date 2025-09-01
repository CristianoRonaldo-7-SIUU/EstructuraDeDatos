#include <stdio.h>

struct GOAT {
    char nombre[20];
    int edad;
};

int main() {
    struct GOAT goats[3] = {
        {"Ronaldo", 39},
        {"Mourinho", 61},
        {"Khabib", 36}
    };

    for(int i = 0; i < 3; i++) {
        printf("%s %d\n", goats[i].nombre, goats[i].edad);
    }
    return 0;
}