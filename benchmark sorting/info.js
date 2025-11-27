const CLASSIFICATION_DATA = {
    "Bubble Sort": {
        complejidad: "O(n^2) peor/promedio, O(n) mejor",
        estable: "Sí",
        inPlace: "Sí",
        tipo: "Comparativo",
        estrategia: "Intercambio (swap)",
        determinista: "Sí",
        ventajas: "Simple de entender e implementar. Eficiente para listas pequeñas o casi ordenadas (con optimización).",
        desventajas: "Muy ineficiente para listas grandes. Demasiados intercambios.",
        idealPara: "Fines educativos, listas muy pequeñas."
    },
    "Selection Sort": {
        complejidad: "O(n^2) en todos los casos",
        estable: "No (en la implementación clásica)",
        inPlace: "Sí",
        tipo: "Comparativo",
        estrategia: "Selección",
        determinista: "Sí",
        ventajas: "Simple de entender. Minimiza el número de intercambios (swaps), útil si escribir en memoria es costoso.",
        desventajas: "Ineficiente para listas grandes. No se beneficia de arreglos casi ordenados.",
        idealPara: "Fines educativos, listas pequeñas donde los intercambios son costosos."
    },
    "Insertion Sort": {
        complejidad: "O(n^2) peor/promedio, O(n) mejor",
        estable: "Sí",
        inPlace: "Sí",
        tipo: "Comparativo",
        estrategia: "Inserción",
        determinista: "Sí",
        ventajas: "Muy eficiente para listas pequeñas y para listas casi ordenadas (adaptativo). Estable y en-sitio.",
        desventajas: "Ineficiente para listas grandes.",
        idealPara: "Listas pequeñas o como sub-algoritmo en Bucket Sort o Timsort."
    },
    "Quick Sort": {
        complejidad: "O(n log n) mejor/promedio, O(n^2) peor",
        estable: "No (en la implementación clásica)",
        inPlace: "Parcial (usa O(log n) de pila de recursión)",
        tipo: "Comparativo",
        estrategia: "Dividir y vencer",
        determinista: "No (con pivote aleatorio), Sí (con pivote fijo)",
        ventajas: "Extremadamente rápido en la práctica (promedio O(n log n)). Bajo overhead.",
        desventajas: "El peor caso O(n^2) puede ocurrir con malos pivotes (ej. arreglo ordenado). No es estable.",
        idealPara: "Listas grandes de propósito general donde la estabilidad no es necesaria."
    },
    "Merge Sort": {
        complejidad: "O(n log n) en todos los casos",
        estable: "Sí",
        inPlace: "No (requiere O(n) de espacio auxiliar)",
        tipo: "Comparativo",
        estrategia: "Dividir y vencer",
        determinista: "Sí",
        ventajas: "Rendimiento garantizado O(n log n). Estable. Excelente para datos externos (que no caben en memoria).",
        desventajas: "Requiere O(n) de memoria auxiliar, lo que puede ser un problema.",
        idealPara: "Listas grandes, ordenamiento externo, cuando se requiere estabilidad y garantía de tiempo."
    },
    "Heap Sort": {
        complejidad: "O(n log n) en todos los casos",
        estable: "No",
        inPlace: "Sí",
        tipo: "Comparativo",
        estrategia: "Heapificación (basado en selección)",
        determinista: "Sí",
        ventajas: "Rendimiento garantizado O(n log n). Es in-place (no requiere memoria extra significativa).",
        desventajas: "No es estable. Suele ser un poco más lento en la práctica que QuickSort. No es adaptativo.",
        idealPara: "Cuando se necesita garantía O(n log n) pero no se puede usar memoria extra (ej. sistemas embebidos)."
    },
    "Shell Sort": {
        complejidad: "Depende del gap. Típicamente O(n^1.5) o O(n(log n)^2)",
        estable: "No",
        inPlace: "Sí",
        tipo: "Comparativo",
        estrategia: "Inserción (con gaps decrecientes)",
        determinista: "Sí",
        ventajas: "Mucho más rápido que Insertion Sort simple. No usa pila de recursión ni memoria extra significativa.",
        desventajas: "Más lento que Quick/Merge Sort para arrays muy grandes. Complejo de analizar teóricamente.",
        idealPara: "Listas de tamaño medio, o como optimización de Insertion Sort."
    },
    "Bucket Sort": {
        complejidad: "O(n+k) promedio, O(n^2) peor",
        estable: "Sí (si el sort interno lo es)",
        inPlace: "No (crea sub-arreglos)",
        tipo: "No Comparativo (Distribución)",
        estrategia: "Distribución",
        determinista: "Sí",
        ventajas: "Extremadamente rápido (lineal) si los datos están uniformemente distribuidos.",
        desventajas: "El peor caso es O(n^2) si todos los datos caen en el mismo bucket. Usa memoria extra.",
        idealPara: "Datos que se sabe están uniformemente distribuidos (ej. flotantes entre 0 y 1)."
    },
    "Radix Sort": {
        complejidad: "O(n*d) (d=dígitos)",
        estable: "Sí (requiere un sort interno estable)",
        inPlace: "No totalmente (usa arrays auxiliares)",
        tipo: "No Comparativo (Distribución)",
        estrategia: "Distribución",
        determinista: "Sí",
        ventajas: "Muy rápido para enteros (o strings) de longitud fija. No hace comparaciones.",
        desventajas: "No es de propósito general. La 'd' (dígitos) puede ser grande. Más complejo de implementar.",
        idealPara: "Números enteros (ej. IDs, fechas), ordenamiento de strings."
    }
};

function getInfo(algorithmName) {
    return CLASSIFICATION_DATA[algorithmName] || null;
}

function formatInfo(infoData) {
    if (!infoData) return "Informacion no encontrada.";
    
    let output = "\n--- CLASIFICACION ---\n";
    output += `Complejidad: ${infoData.complejidad}\n`;
    output += `Estable:     ${infoData.estable}\n`;
    output += `In-place:    ${infoData.inPlace}\n`;
    output += `Tipo:        ${infoData.tipo}\n`;
    output += `Estrategia:  ${infoData.estrategia}\n`;
    output += `Determinista:${infoData.determinista}\n`;
    output += "\n--- ANALISIS ---\n";
    output += `Ventajas:    ${infoData.ventajas}\n`;
    output += `Desventajas: ${infoData.desventajas}\n`;
    output += `Ideal para:  ${infoData.idealPara}\n`;
    
    return output;
}

module.exports = { getInfo, formatInfo };
