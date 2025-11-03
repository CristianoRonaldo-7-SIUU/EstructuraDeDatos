function generateArray(size, range, semiOrdered) {
    const arr = [];
    const min = range.min;
    const max = range.max;
    const rangeSize = max - min + 1;

    for (let i = 0; i < size; i++) {
        arr.push(Math.floor(Math.random() * rangeSize) + min);
    }

    if (semiOrdered) {
        // "Semi-ordenado": Ordenamos y luego introducimos un 10% de "ruido"
        arr.sort((a, b) => a - b);
        const swaps = Math.floor(size * 0.1);
        for (let i = 0; i < swaps; i++) {
            const idx1 = Math.floor(Math.random() * size);
            const idx2 = Math.floor(Math.random() * size);
            [arr[idx1], arr[idx2]] = [arr[idx2], arr[idx1]];
        }
    }

    return arr;
}

module.exports = { generateArray };