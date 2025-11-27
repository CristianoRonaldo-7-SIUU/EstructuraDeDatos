// ---------------- BUBBLE
function bubbleSort(arr, order) {
    let comparisons = 0;
    for (let i = 0; i + 1 < arr.length; ++i)
        for (let j = 0; j + 1 < arr.length - i; ++j) {
            comparisons++;
            if ((order === 'a' && arr[j] > arr[j+1]) || (order === 'd' && arr[j] < arr[j+1])) {
                [arr[j], arr[j+1]] = [arr[j+1], arr[j]];
            }
        }
    return { comparisons };
}

// ---------------- SELECTION
function selectionSort(arr, order) {
    let comparisons = 0;
    for (let i = 0; i + 1 < arr.length; ++i) {
        let idx = i;
        for (let j = i+1; j < arr.length; ++j) {
            comparisons++;
            if ((order === 'a' && arr[j] < arr[idx]) || (order === 'd' && arr[j] > arr[idx])) idx = j;
        }
        if (idx !== i) [arr[i], arr[idx]] = [arr[idx], arr[i]];
    }
    return { comparisons };
}

// ---------------- INSERTION
function insertionSort(arr, order) {
    let comparisons = 0;
    for (let i = 1; i < arr.length; ++i) {
        let key = arr[i], j = i - 1;
        while (j >= 0) {
            comparisons++;
            if ((order === 'a' && arr[j] > key) || (order === 'd' && arr[j] < key)) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = key;
    }
    return { comparisons };
}

// ---------------- QUICK (devuelve nuevo array)
function _quickSort(arr, order) {
    let comparisons = 0;
    if (arr.length <= 1) return { arr: arr.slice(), comparisons: 0 };
    
    const pivot = arr[Math.floor(Math.random() * arr.length)];
    const left = [], mid = [], right = [];
    
    for (const x of arr) {
        comparisons++; 
        if (x === pivot) mid.push(x);
        else {
            comparisons++; 
            if ((order === 'a' && x < pivot) || (order === 'd' && x > pivot)) left.push(x);
            else right.push(x);
        }
    }
    
    const leftResult = _quickSort(left, order);
    const rightResult = _quickSort(right, order);
    
    comparisons += leftResult.comparisons;
    comparisons += rightResult.comparisons;
    
    const sortedArr = leftResult.arr.concat(mid, rightResult.arr);
    return { arr: sortedArr, comparisons };
}
function quickSort(arr, order) {
    const res = _quickSort(arr, order);
    return { sortedArray: res.arr, comparisons: res.comparisons };
}


// ---------------- MERGE (devuelve nuevo array)
function _merge(a, b, order) {
    const res = [];
    let i = 0, j = 0;
    let comparisons = 0;
    
    while (i < a.length && j < b.length) {
        comparisons++;
        if ((order === 'a' && a[i] <= b[j]) || (order === 'd' && a[i] >= b[j])) res.push(a[i++]);
        else res.push(b[j++]);
    }
    while (i < a.length) res.push(a[i++]);
    while (j < b.length) res.push(b[j++]);
    
    return { arr: res, comparisons };
}
function _mergeSort(arr, order) {
    if (arr.length <= 1) return { arr: arr.slice(), comparisons: 0 };
    
    const mid = Math.floor(arr.length / 2);
    const leftResult = _mergeSort(arr.slice(0, mid), order);
    const rightResult = _mergeSort(arr.slice(mid), order);
    const mergeResult = _merge(leftResult.arr, rightResult.arr, order);
    
    return {
        arr: mergeResult.arr,
        comparisons: leftResult.comparisons + rightResult.comparisons + mergeResult.comparisons
    };
}
function mergeSort(arr, order) {
    const res = _mergeSort(arr, order);
    return { sortedArray: res.arr, comparisons: res.comparisons };
}

// ---------------- HEAP
let heapComparisons = 0;
function _heapify(arr, n, i) {
    let largest = i;
    const l = 2*i + 1, r = 2*i + 2;
    
    if (l < n) {
        heapComparisons++;
        if (arr[l] > arr[largest]) largest = l;
    }
    if (r < n) {
        heapComparisons++;
        if (arr[r] > arr[largest]) largest = r;
    }
    if (largest !== i) {
        [arr[i], arr[largest]] = [arr[largest], arr[i]];
        _heapify(arr, n, largest);
    }
}
function heapSort(arr, order) {
    heapComparisons = 0;
    const n = arr.length;
    for (let i = Math.floor(n/2) - 1; i >= 0; --i) _heapify(arr, n, i);
    for (let i = n - 1; i > 0; --i) {
        [arr[0], arr[i]] = [arr[i], arr[0]];
        _heapify(arr, i, 0);
    }
    if (order === 'd') arr.reverse();
    return { comparisons: heapComparisons };
}

// ---------------- SHELL SORT
function shellSort(arr, order) {
    let comparisons = 0;
    let n = arr.length;
    
    // Comenzamos con un gap grande y lo reducimos
    for (let gap = Math.floor(n/2); gap > 0; gap = Math.floor(gap/2)) {
        
        // Hacemos un insertion sort con saltos de tamaño 'gap'
        for (let i = gap; i < n; i += 1) {
            let temp = arr[i];
            let j;
            
            // Bucle interno de inserción
            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                let condition = (order === 'a') 
                    ? (arr[j - gap] > temp) 
                    : (arr[j - gap] < temp);
                
                if (condition) {
                    arr[j] = arr[j - gap];
                } else {
                    break; 
                }
            }
            arr[j] = temp;
        }
    }
    return { comparisons };
}

// ---------------- BUCKET
function _insertionSortForBucket(arr, order) {
    let comparisons = 0;
    for (let i = 1; i < arr.length; ++i) {
        let key = arr[i], j = i - 1;
        while (j >= 0) {
            comparisons++;
            if ((order === 'a' && arr[j] > key) || (order === 'd' && arr[j] < key)) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = key;
    }
    return { comparisons };
}
function bucketSort(arr, order) {
    let comparisons = 0;
    if (arr.length === 0) return { sortedArray: [], comparisons: 0 };
    
    const n = arr.length;
    const maxv = Math.max(...arr);
    const minv = Math.min(...arr);
    if (maxv === minv) return { sortedArray: arr.slice(), comparisons: 0 };
    
    const range = (maxv - minv + 1);
    const bucketCount = n;
    const buckets = Array.from({length: bucketCount}, () => []);
    
    for (const x of arr) {
        const idx = Math.floor(((x - minv) * (bucketCount - 1)) / (range - 1));
        buckets[idx].push(x);
    }
    
    const res = [];
    for (const b of buckets) {
        const insertionStats = _insertionSortForBucket(b, 'a');
        comparisons += insertionStats.comparisons;
        res.push(...b);
    }
    
    if (order === 'd') res.reverse();
    return { sortedArray: res, comparisons };
}

// ---------------- RADIX (LSD)
function _countingSortForRadix(arr, exp) {
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
    if (arr.length === 0) return { comparisons: 0 };
    
    let minv = Math.min(...arr);
    if (minv < 0) for (let i=0;i<arr.length;++i) arr[i] -= minv;
    
    let maxv = Math.max(...arr);
    for (let exp = 1; Math.floor(maxv/exp) > 0; exp *= 10) {
        _countingSortForRadix(arr, exp);
    }
    
    if (minv < 0) for (let i=0;i<arr.length;++i) arr[i] += minv;
    if (order === 'd') arr.reverse();
    
    return { comparisons: 0 }; // Radix no es comparativo
}

// Exportación de todos los algoritmos con metadatos
function getAlgorithms() {
    return [
        { name: "Bubble Sort",    fn: bubbleSort,    complexity: "O(n^2)",      type: "Comp." },
        { name: "Selection Sort", fn: selectionSort, complexity: "O(n^2)",      type: "Comp." },
        { name: "Insertion Sort", fn: insertionSort, complexity: "O(n^2)",      type: "Comp." },
        { name: "Quick Sort",     fn: quickSort,     complexity: "O(n log n)",  type: "Comp." },
        { name: "Merge Sort",     fn: mergeSort,     complexity: "O(n log n)",  type: "Comp." },
        { name: "Heap Sort",      fn: heapSort,      complexity: "O(n log n)",  type: "Comp." },
        { name: "Shell Sort",     fn: shellSort,     complexity: "O(n^1.5)",    type: "Comp." }, 
        { name: "Bucket Sort",    fn: bucketSort,    complexity: "O(n+k)",      type: "Dist." },
        { name: "Radix Sort",     fn: radixSort,     complexity: "O(n*d)",      type: "Dist." }
    ];
}

module.exports = { getAlgorithms };
