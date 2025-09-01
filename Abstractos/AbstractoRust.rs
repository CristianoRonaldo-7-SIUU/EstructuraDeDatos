struct GOAT {
    nombre: String,
    edad: u32,
}

fn main() {
    let goats = [
        GOAT { nombre: "Ronaldo".to_string(), edad: 39 },
        GOAT { nombre: "Mourinho".to_string(), edad: 61 },
        GOAT { nombre: "Khabib".to_string(), edad: 36 },
    ];

    for g in goats.iter() {
        println!("{} {}", g.nombre, g.edad);
    }
}