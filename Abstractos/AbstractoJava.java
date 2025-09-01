class GOAT {
    String nombre;
    int edad;

    GOAT(String nombre, int edad) {
        this.nombre = nombre;
        this.edad = edad;
    }
}

public class AbstractoJava {
    public static void main(String[] args) {
        GOAT[] goats = {
            new GOAT("Ronaldo", 39),
            new GOAT("Mourinho", 61),
            new GOAT("Khabib", 36)
        };

        for(GOAT g : goats) {
            System.out.println(g.nombre + " " + g.edad);
        }
    }
}