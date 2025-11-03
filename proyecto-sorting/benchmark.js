const { performance } = require('perf_hooks');

function runBenchmark(originalArray, iterations, algorithms) {
    const totals = new Map();

    for (const algo of algorithms) {
        totals.set(algo.name, { time: 0, comparisons: 0 });
    }

    for (let i = 0; i < iterations; i++) {
        for (const algo of algorithms) {
            // Crear una copia nueva para cada algoritmo en cada iteracion
            const arrayCopy = [...originalArray];
            
            const start = performance.now();
            
            const stats = algo.fn(arrayCopy, 'a'); // 'a' ascendente por defecto
            
            const end = performance.now();
            const time = end - start;
            
            const currentTotals = totals.get(algo.name);
            currentTotals.time += time;
            currentTotals.comparisons += stats.comparisons;
        }
    }

    // Calcular promedios
    const results = [];
    for (const algo of algorithms) {
        const avg = totals.get(algo.name);
        results.push({
            name: algo.name,
            time: avg.time / iterations,
            comparisons: avg.comparisons / iterations
        });
    }

    return results;
}

module.exports = { runBenchmark };