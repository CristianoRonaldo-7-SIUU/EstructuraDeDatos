const readline = require("readline");

function bubbleSort(arr, order) {
  for (let i = 0; i < arr.length - 1; i++) {
    for (let j = 0; j < arr.length - 1 - i; j++) {
      if ((order === "a" && arr[j] > arr[j+1]) || (order === "d" && arr[j] < arr[j+1])) {
        [arr[j], arr[j+1]] = [arr[j+1], arr[j]];
      }
    }
  }
}

function selectionSort(arr, order) {
  for (let i = 0; i < arr.length - 1; i++) {
    let idx = i;
    for (let j = i+1; j < arr.length; j++) {
      if ((order === "a" && arr[j] < arr[idx]) || (order === "d" && arr[j] > arr[idx])) {
        idx = j;
      }
    }
    if (idx !== i) [arr[i], arr[idx]] = [arr[idx], arr[i]];
  }
}

function insertionSort(arr, order) {
  for (let i = 1; i < arr.length; i++) {
    let key = arr[i];
    let j = i-1;
    while (j >= 0 && ((order === "a" && arr[j] > key) || (order === "d" && arr[j] < key))) {
      arr[j+1] = arr[j];
      j--;
    }
    arr[j+1] = key;
  }
}

function quickSort(arr, order) {
  if (arr.length <= 1) return arr;
  const pivot = arr[Math.floor(arr.length/2)];
  const left = [], middle = [], right = [];
  for (let x of arr) {
    if (x === pivot) middle.push(x);
    else if ((order === "a" && x < pivot) || (order === "d" && x > pivot)) left.push(x);
    else right.push(x);
  }
  return [...quickSort(left, order), ...middle, ...quickSort(right, order)];
}

const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

rl.question("¿Cuántos números deseas generar?: ", nStr => {
  rl.question("Número límite: ", limitStr => {
    let n = parseInt(nStr), limit = parseInt(limitStr);
    let arr = Array.from({length: n}, () => Math.floor(Math.random()*limit)+1);

    console.log("Array generado:");
    console.log(arr);

    rl.question("Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick: ", choiceStr => {
      rl.question("Orden ascendente(a) o descendente(d): ", order => {
        let choice = parseInt(choiceStr);
        const start = Date.now();

        if (choice===1) bubbleSort(arr, order);
        else if (choice===2) selectionSort(arr, order);
        else if (choice===3) insertionSort(arr, order);
        else if (choice===4) arr = quickSort(arr, order);
        else console.log("Opción inválida");

        const end = Date.now();
        console.log(`Tiempo de ejecución: ${(end-start)/1000} s`);
        console.log("Array ordenado:");
        console.log(arr);
        rl.close();
      });
    });
  });
});
