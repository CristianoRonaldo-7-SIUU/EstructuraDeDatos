const readline = require("readline");

// ----- BUBBLE SORT -----
function bubbleSort(arr, order) {
  for (let i = 0; i < arr.length - 1; i++) {
    for (let j = 0; j < arr.length - 1 - i; j++) {
      if ((order === "a" && arr[j] > arr[j + 1]) || (order === "d" && arr[j] < arr[j + 1])) {
        [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]];
      }
    }
  }
}

// ----- SELECTION SORT -----
function selectionSort(arr, order) {
  for (let i = 0; i < arr.length - 1; i++) {
    let idx = i;
    for (let j = i + 1; j < arr.length; j++) {
      if ((order === "a" && arr[j] < arr[idx]) || (order === "d" && arr[j] > arr[idx])) {
        idx = j;
      }
    }
    if (idx !== i) [arr[i], arr[idx]] = [arr[idx], arr[i]];
  }
}

// ----- INSERTION SORT -----
function insertionSort(arr, order) {
  for (let i = 1; i < arr.length; i++) {
    let key = arr[i];
    let j = i - 1;
    while (j >= 0 && ((order === "a" && arr[j] > key) || (order === "d" && arr[j] < key))) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

// ----- QUICK SORT -----
function quickSort(arr, order) {
  if (arr.length <= 1) return arr;
  const pivot = arr[Math.floor(arr.length / 2)];
  const left = [], middle = [], right = [];
  for (let x of arr) {
    if (x === pivot) middle.push(x);
    else if ((order === "a" && x < pivot) || (order === "d" && x > pivot)) left.push(x);
    else right.push(x);
  }
  return [...quickSort(left, order), ...middle, ...quickSort(right, order)];
}

// ----- MERGE SORT -----
function merge(left, right, order) {
  const result = [];
  while (left.length && right.length) {
    if ((order === "a" && left[0] <= right[0]) || (order === "d" && left[0] >= right[0])) {
      result.push(left.shift());
    } else {
      result.push(right.shift());
    }
  }
  return result.concat(left, right);
}

function mergeSort(arr, order) {
  if (arr.length <= 1) return arr;
  const mid = Math.floor(arr.length / 2);
  const left = mergeSort(arr.slice(0, mid), order);
  const right = mergeSort(arr.slice(mid), order);
  return merge(left, right, order);
}

// ----- HEAP SORT -----
function heapify(arr, n, i, order) {
  let extreme = i; // máximo o mínimo según el orden
  let left = 2 * i + 1;
  let right = 2 * i + 2;

  if (order === "a") {
    if (left < n && arr[left] > arr[extreme]) extreme = left;
    if (right < n && arr[right] > arr[extreme]) extreme = right;
  } else {
    if (left < n && arr[left] < arr[extreme]) extreme = left;
    if (right < n && arr[right] < arr[extreme]) extreme = right;
  }

  if (extreme !== i) {
    [arr[i], arr[extreme]] = [arr[extreme], arr[i]];
    heapify(arr, n, extreme, order);
  }
}

function heapSort(arr, order) {
  const n = arr.length;
  for (let i = Math.floor(n / 2) - 1; i >= 0; i--) heapify(arr, n, i, order);
  for (let i = n - 1; i > 0; i--) {
    [arr[0], arr[i]] = [arr[i], arr[0]];
    heapify(arr, i, 0, order);
  }
  if (order === "d") arr.reverse(); // ajusta orden descendente
}

// ----- MAIN -----
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

rl.question("¿Cuántos números deseas generar?: ", nStr => {
  rl.question("Número límite: ", limitStr => {
    const n = parseInt(nStr);
    const limit = parseInt(limitStr);
    let arr = Array.from({ length: n }, () => Math.floor(Math.random() * limit) + 1);

    console.log("Array generado:");
    console.log(arr);

    rl.question("Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick 5-Merge 6-Heap: ", choiceStr => {
      rl.question("Orden ascendente(a) o descendente(d): ", order => {
        const choice = parseInt(choiceStr);
        const start = Date.now();

        if (choice === 1) bubbleSort(arr, order);
        else if (choice === 2) selectionSort(arr, order);
        else if (choice === 3) insertionSort(arr, order);
        else if (choice === 4) arr = quickSort(arr, order);
        else if (choice === 5) arr = mergeSort(arr, order);
        else if (choice === 6) heapSort(arr, order);
        else console.log("Opción inválida");

        const end = Date.now();
        console.log(`\nTiempo de ejecución: ${(end - start) / 1000} s`);
        console.log("Array ordenado:");
        console.log(arr);
        rl.close();
      });
    });
  });
});
