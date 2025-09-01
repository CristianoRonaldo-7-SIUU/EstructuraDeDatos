using System;

class GOAT {
    public string Nombre;
    public int Edad;

    public GOAT(string nombre, int edad) {
        Nombre = nombre;
        Edad = edad;
    }
}

class Program {
    static void Main() {
        GOAT[] goats = {
            new GOAT("Ronaldo", 39),
            new GOAT("Mourinho", 61),
            new GOAT("Khabib", 36)
        };

        foreach(GOAT g in goats) {
            Console.WriteLine($"{g.Nombre} {g.Edad}");
        }
    }
}