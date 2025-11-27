function generateAllScenarios(config) {
    const { size, rangeType, customRange } = config;
    const scenarios = new Map();

    // 1. Aleatorio (Rango 0 a n-1)
    scenarios.set('Aleatorio (0 a n-1)', createRandomArray(size, 0, size - 1));

    // 2. Aleatorio (Rango Estrecho / Duplicados)
    // Usamos un rango de 0 a 20, o 1/100 de N, lo que sea mayor, para forzar duplicados.
    const duplicateRangeMax = Math.max(20, Math.floor(size / 100));
    scenarios.set('Duplicados (Rango 0-' + duplicateRangeMax + ')', createRandomArray(size, 0, duplicateRangeMax));

    // 3. Aleatorio (Rango Específico)
    if (rangeType === 'custom') {
        scenarios.set('Rango Específico', createRandomArray(size, customRange.min, customRange.max));
    }
    
    // 4. Semi-Ordenado
    scenarios.set('Semi-Ordenado', createSemiOrderedArray(size));

    // 5. Montaña (1...N/2...1)
    scenarios.set('Montaña (Asc-Desc)', createMountainArray(size));
    
    // 6. Valle (N/2...1...N/2)
    scenarios.set('Valle (Desc-Asc)', createValleyArray(size));

    return scenarios;
}

function createRandomArray(size, min, max) {
    const arr = [];
    const rangeSize = max - min + 1;
    for (let i = 0; i < size; i++) {
        arr.push(Math.floor(Math.random() * rangeSize) + min);
    }
    return arr;
}

function createSemiOrderedArray(size) {
    const arr = createRandomArray(size, 0, size - 1);
    arr.sort((a, b) => a - b);
    const swaps = Math.floor(size * 0.1); // 10% de "ruido"
    for (let i = 0; i < swaps; i++) {
        const idx1 = Math.floor(Math.random() * size);
        const idx2 = Math.floor(Math.random() * size);
        [arr[idx1], arr[idx2]] = [arr[idx2], arr[idx1]];
    }
    return arr;
}

function createMountainArray(size) {
    const arr = [];
    const mid = Math.ceil(size / 2);
    for (let i = 0; i < mid; i++) {
        arr.push(i);
    }
    let val = mid - (size % 2 === 0 ? 2 : 1);
    for (let i = mid; i < size; i++) {
        arr.push(val--);
    }
    return arr;
}

function createValleyArray(size) {
    const arr = [];
    const mid = Math.ceil(size / 2);
    let val = mid;
    for (let i = 0; i < mid; i++) {
        arr.push(val--);
    }
    val = 1;
    for (let i = mid; i < size; i++) {
        arr.push(val++);
    }
    return arr;
}

module.exports = { generateAllScenarios };
