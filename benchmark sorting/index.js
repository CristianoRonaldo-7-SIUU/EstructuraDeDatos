const ui = require('./ui');
const arrayGenerator = require('./arrayGenerator');
const benchmark = require('./benchmark');
const { spawn } = require('child_process');

function main() {
    let pythonProcess;

    try {
        // --- 1. Inicia el script de Python GUI ---
        pythonProcess = spawn('python3', [__dirname + '/graph_gui.py']);

        pythonProcess.stderr.on('data', (data) => {
            console.error(`[Python-stderr]: ${data}`);
        });

        pythonProcess.stdout.on('data', (data) => {
            console.log(`[Python-stdout]: ${data}`);
        });

        pythonProcess.on('close', (code) => {
            if (code !== 0) {
                console.log(`[Servidor] El script de Python GUI se cerró con código ${code}`);
            }
            // Si la ventana se cierra, salimos de la app de terminal.
            console.log("Cerrando aplicación de terminal.");
            process.exit(0);
        });

        // --- 2. Ejecuta el Benchmark ---
        const config = ui.getBenchmarkConfig();
        const algorithms = require('./algorithms').getAlgorithms();

        console.log("Generando todos los escenarios de prueba...");
        const allScenarios = arrayGenerator.generateAllScenarios(config);
        
        console.log(`Ejecutando benchmarks en ${config.iterations} iteraciones...`);
        const allResults = benchmark.runBenchmark(
            allScenarios,
            config.iterations,
            algorithms
        );
        
        console.log("¡Benchmarks completados!");
        
        // 3. Inicia la UI de la terminal, pasándole el subproceso de Python
        ui.startInterface(allResults, algorithms, pythonProcess);

    } catch (error) {
        console.error("Se ha producido un error:", error.message);
        if (pythonProcess) {
            pythonProcess.kill();
        }
    }
}

main();
