const { performance } = require('perf_hooks');

function runBenchmark(allScenarios, iterations, algorithms) {
    const allResults = new Map();
    
    // 1. Define los dos modos de ordenamiento que queremos probar
    const sortOrders = [
        { key: 'a', name: 'Ascendente' },
        { key: 'd', name: 'Descendente' }
    ];

    // 2. Itera sobre cada TIPO de arreglo (Aleatorio, Montaña, etc.)
    for (const [scenarioName, originalArray] of allScenarios.entries()) {
        
        // 3. Para CADA escenario, ejecuta AMBOS benchmarks (Asc y Desc)
        for (const order of sortOrders) {
            
            // 4. Crea un nombre de escenario único para los resultados
            const fullScenarioName = `${scenarioName} [${order.name}]`;
            
            const totals = new Map();
            for (const algo of algorithms) {
                totals.set(algo.name, { time: 0, comparisons: 0 });
            }

            // Ejecuta N iteraciones para ESE escenario Y ESE orden
            for (let i = 0; i < iterations; i++) {
                for (const algo of algorithms) {
                    const arrayCopy = [...originalArray];
                    
                    const start = performance.now();
                    // 5. Pasa la clave de orden correcta ('a' o 'd') al algoritmo
                    const stats = algo.fn(arrayCopy, order.key); 
                    const end = performance.now();
                    
                    const time = end - start;
                    
                    const currentTotals = totals.get(algo.name);
                    currentTotals.time += time;
                    currentTotals.comparisons += stats.comparisons;
                }
            }

            // Calcula promedios para ESE escenario Y ESE orden
            const scenarioResults = [];
            for (const algo of algorithms) {
                const avg = totals.get(algo.name);
                scenarioResults.push({
                    name: algo.name,
                    avg: {
                        time: avg.time / iterations,
                        comparisons: avg.comparisons / iterations
                    }
                });
            }
            
            // 6. Almacena los resultados con el nombre completo
            allResults.set(fullScenarioName, scenarioResults);
        }
    }

    return allResults;
}

module.exports = { runBenchmark };
