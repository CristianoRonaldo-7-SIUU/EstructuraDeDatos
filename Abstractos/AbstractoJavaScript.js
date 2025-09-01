class GOAT {
    constructor(nombre, edad) {
        this.nombre = nombre;
        this.edad = edad;
    }
}

let goats = [
    new GOAT("Ronaldo", 39),
    new GOAT("Mourinho", 61),
    new GOAT("Khabib", 36)
];

for (let g of goats) {
    console.log(g.nombre, g.edad);
}