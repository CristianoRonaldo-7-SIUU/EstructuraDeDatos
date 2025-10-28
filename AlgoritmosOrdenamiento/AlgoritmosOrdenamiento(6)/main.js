const readline = require('readline-sync');

function printArr(a) {
    console.log(a.join(' '));
}

// ---------------- BUBBLE
function bubbleSort(arr, order) {
    for (let i = 0; i + 1 < arr.length; ++i)
        for (let j = 0; j + 1 < arr.length - i; ++j)
            if ((order === 'a' && arr[j] > arr[j+1]) || (order === 'd' && arr[j] < arr[j+1])) {
                [arr[j], arr[j+1]] = [arr[j+1], arr[j]];
            }
}

// ---------------- SELECTION
function selectionSort(arr, order) {
    for (let i = 0; i + 1 < arr.length; ++i) {
        let idx = i;
        for (let j = i+1; j < arr.length; ++j)
            if ((order === 'a' && arr[j] < arr[idx]) || (order === 'd' && arr[j] > arr[idx])) idx = j;
        if (idx !== i) [arr[i], arr[idx]] = [arr[idx], arr[i]];
    }
}

// ---------------- INSERTION
function insertionSort(arr, order) {
    for (let i = 1; i < arr.length; ++i) {
        let key = arr[i], j = i - 1;
        while (j >= 0 && ((order === 'a' && arr[j] > key) || (order === 'd' && arr[j] < key))) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// ---------------- QUICK (devuelve nuevo array)
function quickSort(arr, order) {
    if (arr.length <= 1) return arr.slice();
    const pivot = arr[Math.floor(Math.random() * arr.length)];
    const left = [], mid = [], right = [];
    for (const x of arr) {
        if (x === pivot) mid.push(x);
        else if ((order === 'a' && x < pivot) || (order === 'd' && x > pivot)) left.push(x);
        else right.push(x);
    }
    return quickSort(left, order).concat(mid, quickSort(right, order));
}

// ---------------- MERGE (devuelve nuevo array)
function merge(a, b, order) {
    const res = [];
    let i = 0, j = 0;
    while (i < a.length && j < b.length) {
        if ((order === 'a' && a[i] <= b[j]) || (order === 'd' && a[i] >= b[j])) res.push(a[i++]);
        else res.push(b[j++]);
    }
    while (i < a.length) res.push(a[i++]);
    while (j < b.length) res.push(b[j++]);
    return res;
}
function mergeSort(arr, order) {
    if (arr.length <= 1) return arr.slice();
    const mid = Math.floor(arr.length / 2);
    const left = mergeSort(arr.slice(0, mid), order);
    const right = mergeSort(arr.slice(mid), order);
    return merge(left, right, order);
}

// ---------------- HEAP
function heapify(arr, n, i) {
    let largest = i;
    const l = 2*i + 1, r = 2*i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest !== i) {
        [arr[i], arr[largest]] = [arr[largest], arr[i]];
        heapify(arr, n, largest);
    }
}
function heapSort(arr, order) {
    const n = arr.length;
    for (let i = Math.floor(n/2) - 1; i >= 0; --i) heapify(arr, n, i);
    for (let i = n - 1; i > 0; --i) {
        [arr[0], arr[i]] = [arr[i], arr[0]];
        heapify(arr, i, 0);
    }
    if (order === 'd') arr.reverse();
}

// ---------------- HASH
function hashSort(arr, order) {
    const freq = new Map();
    for (const x of arr) freq.set(x, (freq.get(x) || 0) + 1);
    const keys = Array.from(freq.keys());
    keys.sort((a,b) => order === 'a' ? a - b : b - a);
    const res = [];
    for (const k of keys) for (let i=0;i<freq.get(k);++i) res.push(k);
    return res;
}

// ---------------- BUCKET
function bucketSort(arr, order) {
    if (arr.length === 0) return arr.slice();
    const n = arr.length;
    const maxv = Math.max(...arr);
    const minv = Math.min(...arr);
    if (maxv === minv) return arr.slice();
    const range = (maxv - minv + 1);
    const bucketCount = n;
    const buckets = Array.from({length: bucketCount}, () => []);
    for (const x of arr) {
        const idx = Math.floor(((x - minv) * (bucketCount - 1)) / (range - 1));
        buckets[idx].push(x);
    }
    const res = [];
    for (const b of buckets) {
        insertionSort(b, 'a');
        res.push(...b);
    }
    if (order === 'd') res.reverse();
    return res;
}

// ---------------- RADIX (LSD)
function countingSortForRadix(arr, exp) {
    const n = arr.length;
    const output = new Array(n).fill(0);
    const count = new Array(10).fill(0);
    for (let i=0;i<n;++i) count[Math.floor(arr[i]/exp)%10]++;
    for (let i=1;i<10;++i) count[i] += count[i-1];
    for (let i=n-1;i>=0;--i) {
        const idx = Math.floor(arr[i]/exp) % 10;
        output[count[idx] - 1] = arr[i];
        count[idx]--;
    }
    for (let i=0;i<n;++i) arr[i] = output[i];
}
function radixSort(arr, order) {
    if (arr.length === 0) return arr;
    let minv = Math.min(...arr);
    if (minv < 0) for (let i=0;i<arr.length;++i) arr[i] -= minv;
    let maxv = Math.max(...arr);
    for (let exp = 1; Math.floor(maxv/exp) > 0; exp *= 10) countingSortForRadix(arr, exp);
    if (minv < 0) for (let i=0;i<arr.length;++i) arr[i] += minv;
    if (order === 'd') arr.reverse();
}

// ---------------- MAIN ----------------
function main() {
    const n = parseInt(readline.question("¿Cuántos números deseas generar?: "));
    let limit = parseInt(readline.question("Número límite: "));
    if (isNaN(limit) || limit <= 0) limit = 1;
    const arr = [];
    for (let i=0;i<n;++i) arr.push(Math.floor(Math.random() * limit) + 1);

    console.log("\nArray generado:");
    printArr(arr);

    console.log("\nAlgoritmos disponibles:");
    console.log("1-Bubble  2-Selection  3-Insertion  4-Quick  5-Merge  6-Heap");
    console.log("7-Hash  8-Bucket  9-Radix");
    const choice = parseInt(readline.question("Elige algoritmo (1-9): "));
    const order = (readline.question("Orden ascendente(a) o descendente(d): ") || "a").toLowerCase();

    const start = process.hrtime();
    let resultArr = arr;
    if (choice === 1) bubbleSort(resultArr, order);
    else if (choice === 2) selectionSort(resultArr, order);
    else if (choice === 3) insertionSort(resultArr, order);
    else if (choice === 4) resultArr = quickSort(resultArr, order);
    else if (choice === 5) resultArr = mergeSort(resultArr, order);
    else if (choice === 6) heapSort(resultArr, order);
    else if (choice === 7) resultArr = hashSort(resultArr, order);
    else if (choice === 8) resultArr = bucketSort(resultArr, order);
    else if (choice === 9) radixSort(resultArr, order);
    else console.log("Opción inválida");

    const diff = process.hrtime(start);
    const elapsed = diff[0] + diff[1] / 1e9;
    console.log(`\nTiempo de ejecución: ${elapsed.toFixed(6)} s`);
    console.log("Array ordenado:");
    printArr(resultArr);
}

main();
