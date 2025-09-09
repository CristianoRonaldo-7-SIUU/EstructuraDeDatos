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
    if (filas <= 0 || columnas <= 0) { rl.close(); return; }
    const cap = filas * columnas;
    let lineal = Array.from({ length: cap }, () => Math.floor(Math.random() * 100));

    console.log("\nMatriz generada:");
    mostrar(lineal, columnas);

    rl.question("\n¿Qué elemento desea insertar a la matriz? ", nStr => {
      const n = parseInt(nStr);
      console.log(`\n¿De qué manera quiere insertar el elemento ${n}?\n1- Inserción en el índice 0.\n2- Inserción en un índice específico.\n3- Inserción al final.`);
      rl.question("Opción: ", op => {
        if (op === "1") {
          lineal.unshift(n);
          if (lineal.length > cap) lineal.pop();
          console.log("\nMatriz con el nuevo elemento:");
          mostrar(lineal, columnas);
          rl.close();
        } else if (op === "2") {
          rl.question("Índice específico: ", idxStr => {
            const idx = parseInt(idxStr);
            if (0 <= idx && idx < cap) {
              lineal.splice(idx, 0, n);
              if (lineal.length > cap) lineal.pop();
            } else {
              console.log("Índice fuera de rango, se reemplazará el último elemento.");
              lineal[cap - 1] = n;
            }
            console.log("\nMatriz con el nuevo elemento:");
            mostrar(lineal, columnas);
            rl.close();
          });
        } else if (op === "3") {
          lineal[cap - 1] = n;
          console.log("\nMatriz con el nuevo elemento:");
          mostrar(lineal, columnas);
          rl.close();
        } else {
          console.log("Opción inválida.");
          rl.close();
        }
      });
    });
  });
});