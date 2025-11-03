const ui = require('./ui');
const arrayGenerator = require('./arrayGenerator');
const benchmark = require('./benchmark');

// --- MAIN ---
function main() {
    try {
        const config = ui.getSortConfig();
        const algorithms = require('./algorithms').getAlgorithms();

        console.log("\nGenerando arreglo...");
        const originalArray = arrayGenerator.generateArray(
            config.size,
            config.range,
            config.semiOrdered
        );
        
        console.log(`Arreglo generado (${config.arrayType}). Iniciando benchmark...`);

        const results = benchmark.runBenchmark(
            originalArray,
            config.iterations,
            algorithms
        );

        ui.displayResults(results, algorithms);

        ui.handlePostRunCommands(algorithms);

    } catch (error) {
        console.error("Se ha producido un error:", error.message);
    }
}

main();