const readline = require("readline");
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

function mostrar(arr, columnas) {
  for (let i = 0; i < arr.length; i++) {
    process.stdout.write(String(arr[i]).padStart(2) + " ");
    if ((i + 1) % columnas === 0) console.log();
  }
  console.log();
}

rl.question("Número de filas: ", f => {
  rl.question("Número de columnas: ", c => {
    const filas = parseInt(f), columnas = parseInt(c);
    const cap = filas * columnas;
    let lineal = Array.from({ length: cap }, () => Math.floor(Math.random() * 100));

    console.log("\nMatriz generada:");
    mostrar(lineal, columnas);

    rl.question("\nIngrese el valor a buscar: ", objStr => {
      const objetivo = parseInt(objStr);
      let encontrado = false;
      for (let i = 0; i < lineal.length; i++) {
        if (lineal[i] === objetivo) {
          console.log(`Elemento ${objetivo} encontrado en la posición lineal ${i} (fila ${Math.floor(i/columnas)}, columna ${i%columnas})`);
          encontrado = true;
          break;
        }
      }
      if (!encontrado) console.log(`Elemento ${objetivo} no encontrado en la matriz.`);
      rl.close();
    });
  });
});