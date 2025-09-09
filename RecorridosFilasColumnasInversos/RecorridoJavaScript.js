const readline = require("readline");

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.question("Número de filas: ", (f) => {
  rl.question("Número de columnas: ", (c) => {
    let filas = parseInt(f), columnas = parseInt(c);

    let matriz = Array.from({ length: filas }, () =>
      Array.from({ length: columnas }, () => Math.floor(Math.random() * 100))
    );

    console.log("\nMatriz generada:");
    matriz.forEach(fila => console.log(fila.map(x => x.toString().padStart(2)).join(" ")));

    rl.question("\nRecorrer por...\n1- Filas\n2- Columnas\nOpción: ", (op) => {
      rl.question("¿Desea recorrer la matriz en orden inverso (s/n)? ", (inv) => {
        console.log("\nRecorrido:");
        if (op === "1") { // Filas
          if (inv.toLowerCase() === "s") {
            for (let i = filas - 1; i >= 0; i--)
              for (let j = columnas - 1; j >= 0; j--)
                process.stdout.write(matriz[i][j] + " ");
          } else {
            for (let i = 0; i < filas; i++)
              for (let j = 0; j < columnas; j++)
                process.stdout.write(matriz[i][j] + " ");
          }
        } else { // Columnas
          if (inv.toLowerCase() === "s") {
            for (let j = columnas - 1; j >= 0; j--)
              for (let i = filas - 1; i >= 0; i--)
                process.stdout.write(matriz[i][j] + " ");
          } else {
            for (let j = 0; j < columnas; j++)
              for (let i = 0; i < filas; i++)
                process.stdout.write(matriz[i][j] + " ");
          }
        }
        console.log();
        rl.close();
      });
    });
  });
});