const readline = require('readline-sync');
const { COLORS, formatTime, formatNumber, RESET, WEB_COLORS } = require('./utils'); 
const info = require('./info');

const RANGES = { MIN_SIZE: 2, MAX_SIZE: 1000000, MIN_ITER: 1, MAX_ITER: 1000 };

// Almacenamiento de estado de la UI
let STATE = {
    allResults: null,
    algorithms: null,
    scenarios: [],
    currentScenario: null,
    isInverted: false,
    pythonProcess: null, // Guardará el subproceso de Python
};

function getBenchmarkConfig() {
    const size = getSize();
    const iterations = getIterations();
    const { rangeType, customRange } = getRange(size);
    
    return { size, iterations, rangeType, customRange };
}

function getSize() {
    let size;
    while (true) {
        size = parseInt(readline.question("Introduce el numero de elementos (2 - 1,000,000): "), 10);
        if (size >= RANGES.MIN_SIZE && size <= RANGES.MAX_SIZE) break;
        console.log(`Fuera de rango (${RANGES.MIN_SIZE} - ${RANGES.MAX_SIZE})`);
    }
    return size;
}

function getRange(n) {
    console.log("Se generaran escenarios (Aleatorio, Duplicados, Montaña, etc.).");
    console.log("¿Deseas añadir un escenario con Rango Específico? (s/n)");
    const choice = readline.question("Opcion: ").toLowerCase();

    if (choice === 's') {
        const min = parseInt(readline.question("Valor minimo: "), 10);
        const max = parseInt(readline.question("Valor maximo: "), 10);
        if (!isNaN(min) && !isNaN(max) && min <= max) {
            return { rangeType: 'custom', customRange: { min, max } };
        }
    }
    return { rangeType: 'default', customRange: null };
}

function getIterations() {
    let iterations;
    while (true) {
        iterations = parseInt(readline.question(`Introduce el numero de iteraciones (${RANGES.MIN_ITER} - ${RANGES.MAX_ITER}): `), 10);
        if (iterations >= RANGES.MIN_ITER && iterations <= RANGES.MAX_ITER) break;
        console.log(`Fuera de rango (${RANGES.MIN_ITER} - ${RANGES.MAX_ITER})`);
    }
    return iterations;
}


// -- Lógica de la Interfaz --

function startInterface(allResults, algorithms, pythonProcess) {
    STATE.allResults = allResults;
    STATE.algorithms = algorithms;
    STATE.scenarios = Array.from(allResults.keys());
    STATE.currentScenario = STATE.scenarios[0];
    STATE.pythonProcess = pythonProcess; // Almacena el proceso

    displayCurrentView();
    handlePostRunCommands();
}

function displayCurrentView() {
    const results = STATE.allResults.get(STATE.currentScenario);
    
    const sortedResults = [...results].sort((a, b) => a.avg.time - b.avg.time);
    const rankingMap = new Map(sortedResults.map((res, index) => [res.name, index + 1]));

    const scenarioName = STATE.currentScenario;
    const viewMode = STATE.isInverted ? "VELOCIDAD (Invertido)" : "TIEMPO (Normal)";

    console.clear();
    console.log(`--- ESCENARIO: ${scenarioName} | MODO: ${viewMode} ---`);

    const table = createTable(sortedResults, rankingMap);
    console.log(table);
    
    const chart = createChart(sortedResults);
    console.log(chart);
    
    console.log("\nComandos: [O]rdenar/Cambiar Escenario | [I]nvertir Vista | [N]fo | [Q]uit");
    
    notifyPythonGUI(sortedResults);
}

function createTable(sortedResults, rankingMap) {
    let h = "+-----+------------------+----------------+----------------+";
    let title = "| RK  | Algoritmo        | Tiempo (avg)   | Comps. (avg)   |";
    
    let body = sortedResults.map((res, index) => {
        const algo = STATE.algorithms.find(a => a.name === res.name);
        const rank = STATE.isInverted ? (sortedResults.length - index) : (index + 1);
        const color = COLORS[index % COLORS.length];
        
        const rankStr = String(rank).padStart(3);
        const nameStr = res.name.padEnd(16);
        const timeStr = formatTime(res.avg.time).padEnd(14);
        const compAvgStr = formatNumber(res.avg.comparisons).padEnd(14);
        
        return `${color}| ${rankStr} | ${nameStr} | ${timeStr} | ${compAvgStr} |${RESET}`;
    }).join("\n" + h + "\n");

    return `\n${h}\n${title}\n${h}\n${body}\n${h}`;
}

function createChart(sortedResults) {
    const MAX_WIDTH = 50;
    
    const logValues = sortedResults.map(r => {
        if (STATE.isInverted) {
            const speed = 1 / Math.max(0.001, r.avg.time);
            return Math.log(speed + 1); 
        } else {
            return Math.log(Math.max(0.001, r.avg.time) + 1);
        }
    });

    const maxLogVal = Math.max(...logValues);
    let chart = "\n--- Gráfica de Rendimiento (" + (STATE.isInverted ? "Velocidad" : "Tiempo") + ") ---\n";

    for (let i = 0; i < sortedResults.length; i++) {
        const res = sortedResults[i];
        const logVal = logValues[i];
        const color = COLORS[i % COLORS.length];
        
        let barLength = 0;
        if (maxLogVal > 0) {
            barLength = Math.max(1, Math.round((logVal / maxLogVal) * MAX_WIDTH));
        }
        
        const bar = '#'.repeat(barLength);
        const valStr = formatTime(res.avg.time);

        chart += `${color}${res.name.padEnd(16)} [${bar.padEnd(MAX_WIDTH)}]${RESET} ${valStr}\n`;
    }
    return chart;
}

function notifyPythonGUI(sortedResults) {
    if (!STATE.pythonProcess || STATE.pythonProcess.killed) {
        console.log("El proceso GUI de Python no está en ejecución.");
        return;
    }

    const labels = sortedResults.map(r => r.name);
    const data = sortedResults.map(r => {
        if (STATE.isInverted) {
            return 1 / Math.max(0.001, r.avg.time); // Velocidad
        } else {
            return r.avg.time; // Tiempo
        }
    });

    // Asigna los colores web
    const backgroundColors = sortedResults.map((res, index) => {
        return WEB_COLORS[index % WEB_COLORS.length];
    });

    const title = STATE.isInverted ? "Velocidad (1/Tiempo)" : "Tiempo (ms)";
    
    const chartData = {
        labels: labels,
        datasets: [{
            label: `${STATE.currentScenario} - ${title}`,
            data: data,
            backgroundColor: backgroundColors,
        }]
    };

    try {
        STATE.pythonProcess.stdin.write(JSON.stringify(chartData) + '\n');
    } catch (e) {
        console.error("Error al enviar datos al GUI de Python:", e.message);
    }
}


function handlePostRunCommands() {
    while (true) {
        const cmd = readline.question("\nComando (O, I, N, Q): ").toUpperCase();
        
        if (cmd === 'O') {
            handleChangeScenario();
        } else if (cmd === 'I') {
            handleInvertView();
        } else if (cmd === 'N') {
            showInfo();
        } else if (cmd === 'Q' || cmd === 'S') {
            console.log("Saliendo. Cerrando ventana GUI...");
            STATE.pythonProcess.kill(); // Cierra el proceso de Python
            process.exit(0); // Cierra Node.js
        } else {
            console.log("Comando invalido.");
        }
    }
}

function handleChangeScenario() {
    console.log("\nSelecciona un escenario para ver:");
    STATE.scenarios.forEach((s, i) => {
        console.log(`${i + 1}. ${s}`);
    });
    const choice = parseInt(readline.question("Opcion: "), 10);
    
    if (choice > 0 && choice <= STATE.scenarios.length) {
        STATE.currentScenario = STATE.scenarios[choice - 1];
        displayCurrentView();
    } else {
        console.log("Opcion invalida.");
    }
}

function handleInvertView() {
    STATE.isInverted = !STATE.isInverted;
    displayCurrentView();
}

function showInfo() {
    console.log("\nSelecciona un algoritmo para ver su info:");
    STATE.algorithms.forEach((algo, i) => {
        console.log(`${i + 1}. ${algo.name}`);
    });
    const choice = parseInt(readline.question("Opcion (1-9): "), 10);
    
    if (choice > 0 && choice <= STATE.algorithms.length) {
        const algoName = STATE.algorithms[choice - 1].name;
        const infoData = info.getInfo(algoName);
        console.log(info.formatInfo(infoData));
    } else {
        console.log("Opcion invalida.");
    }
}

module.exports = {
    getBenchmarkConfig,
    startInterface
};
