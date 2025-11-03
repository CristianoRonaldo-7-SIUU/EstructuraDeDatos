// utils.js

// Códigos de color ANSI
const ANSI_COLORS = {
    reset: "\x1b[0m",
    red: "\x1b[31m",
    cyan: "\x1b[36m",
    yellow: "\x1b[33m",
    blue: "\x1b[34m",
    magenta: "\x1b[35m",
    redBright: "\x1b[91m",
    greenBright: "\x1b[92m",
    yellowBright: "\x1b[93m",
    blueBright: "\x1b[94m",
    magentaBright: "\x1b[95m",
    cyanBright: "\x1b[96m",
};

// Devuelve un color de la lista, excluyendo el verde
const TABLE_COLORS = [
    ANSI_COLORS.cyan,
    ANSI_COLORS.yellow,
    ANSI_COLORS.blue,
    ANSI_COLORS.magenta,
    ANSI_COLORS.redBright,
    ANSI_COLORS.cyanBright,
    ANSI_COLORS.yellowBright,
    ANSI_COLORS.blueBright,
    ANSI_COLORS.magentaBright,
];

function formatTime(ms) {
    if (ms < 1000) {
        return `${ms.toFixed(3)} ms`;
    }
    return `${(ms / 1000).toFixed(3)} s`;
}

function formatNumber(num) {
    if (num < 1e3) return String(num.toFixed(0));
    if (num < 1e6) return `${(num / 1e3).toFixed(1)}k`;
    if (num < 1e9) return `${(num / 1e6).toFixed(1)}M`;
    return `${(num / 1e9).toFixed(1)}G`;
}

module.exports = {
    COLORS: TABLE_COLORS, // Array de colores para las filas
    RESET: ANSI_COLORS.reset, // ¡Ahora exportamos RESET explícitamente!
    formatTime,
    formatNumber
};
