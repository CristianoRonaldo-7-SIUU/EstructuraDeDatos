const readline = require('readline-sync');
// Importar RESET para el fix anterior del 'undefined'
const { COLORS, formatTime, formatNumber, RESET } = require('./utils'); 
const info = require('./info');

const RANGES = { MIN_SIZE: 2, MAX_SIZE: 1000000, MIN_ITER: 1, MAX_ITER: 1000 };

function getSortConfig() {
    const size = getSize();
    const range = getRange(size);
    const semiOrdered = getSemiOrdered();
    const arrayType = semiOrdered ? "semi-ordenado" : "aleatorio";
    const iterations = getIterations();
    
    return { size, range, semiOrdered, arrayType, iterations };
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
    console.log("Elige el rango de numeros:");
    console.log("1. 0 a n-1 (0 - " + (n - 1) + ")");
    console.log("2. Rango especifico");
    const choice = readline.question("Opcion (1-2): ");

    if (choice === '2') {
        const min = parseInt(readline.question("Valor minimo: "), 10);
        const max = parseInt(readline.question("Valor maximo: "), 10);
        if (!isNaN(min) && !isNaN(max) && min <= max) {
            return { min, max };
        }
        console.log("Rango invalido, usando 0 a n-1.");
    }
    return { min: 0, max: n - 1 };
}

function getSemiOrdered() {
    const choice = readline.question("¿El arreglo debe estar semi-ordenado? (s/n): ").toLowerCase();
    return choice === 's';
}

function getIterations() {
    let iterations;
    while (true) {
        iterations = parseInt(readline.question(`Introduce el numero de iteraciones (${RANGES.MIN_ITER} - ${RANGES.MAX_ITER}): `), 10);
        
        // CORRECCIÓN: Se cambió RANGES.MAX_MAX_ITER a RANGES.MAX_ITER
        if (iterations >= RANGES.MIN_ITER && iterations <= RANGES.MAX_ITER) break; 
        
        console.log(`Fuera de rango (${RANGES.MIN_ITER} - ${RANGES.MAX_ITER})`);
    }
    return iterations;
}

function displayResults(results, algorithms) {
    // 1. Clonar y ordenar los resultados por tiempo (menor tiempo = posición 1)
    const sortedResults = [...results].sort((a, b) => a.time - b.time);

    // 2. Crear un mapa para asignar el ranking (1, 2, 3...) a cada algoritmo
    const rankingMap = new Map(sortedResults.map((res, index) => [res.name, index + 1]));

    // 3. Crear la tabla usando los resultados YA ORDENADOS
    const table = createTable(sortedResults, rankingMap, algorithms);
    console.log(table);
}

function createTable(results, rankingMap, algorithms) {
    let header = "+-----+------------------+----------------+------------------+------------------+----------+";
    let title = "| RK  | Algoritmo        | Tiempo (avg)   | Comps. (avg)     | Complejidad      | Tipo     |";
    
    let body = results.map((res, index) => {
        const algo = algorithms.find(a => a.name === res.name);
        const rank = rankingMap.get(res.name); 
        const color = COLORS[index % COLORS.length];
        
        const rankStr = String(rank).padStart(3);
        const nameStr = res.name.padEnd(16);
        const timeStr = formatTime(res.time).padEnd(14);
        const compStr = formatNumber(res.comparisons).padEnd(16);
        const complexStr = algo.complexity.padEnd(16);
        const typeStr = algo.type.padEnd(8); 

        // Usa RESET para corregir el 'undefined'
        return `${color}| ${rankStr} | ${nameStr} | ${timeStr} | ${compStr} | ${complexStr} | ${typeStr} |${RESET}`;
    }).join("\n" + header + "\n");

    return `\n${header}\n${title}\n${header}\n${body}\n${header}`;
}

function handlePostRunCommands(algorithms) {
    console.log("\nComandos:");
    console.log("  O + ENTER: Ordenar tabla por otro criterio");
    console.log("  I + ENTER: Ver informacion de un algoritmo");
    console.log("  (cualquier otra tecla para salir)");

    while (true) {
        const cmd = readline.question("\n> ").toUpperCase();
        if (cmd === 'I') {
            showInfo(algorithms);
        } else if (cmd === 'O') {
            console.log("Opcion 'Ordenar' aun no implementada.");
        } else {
            console.log("Saliendo.");
            break; 
        }
    }
}

function showInfo(algorithms) {
    console.log("\nSelecciona un algoritmo para ver su info:");
    algorithms.forEach((algo, i) => {
        console.log(`${i + 1}. ${algo.name}`);
    });
    const choice = parseInt(readline.question("Opcion (1-9): "), 10);
    
    if (choice > 0 && choice <= algorithms.length) {
        const algoName = algorithms[choice - 1].name;
        const infoData = info.getInfo(algoName);
        console.log(info.formatInfo(infoData));
    } else {
        console.log("Opcion invalida.");
    }
}

module.exports = {
    getSortConfig,
    displayResults,
    handlePostRunCommands
};
