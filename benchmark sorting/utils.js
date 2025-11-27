// Códigos de color ANSI (Terminal)
const ANSI_COLORS = {
    reset: "\x1b[0m",
    cyan: "\x1b[36m",
    yellow: "\x1b[33m",
    blue: "\x1b[34m",
    magenta: "\x1b[35m",
    redBright: "\x1b[91m",
    cyanBright: "\x1b[96m",
    yellowBright: "\x1b[93m",
    blueBright: "\x1b[94m",
    magentaBright: "\x1b[95m",
};

// Array de colores de la terminal
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

const WEB_COLORS = [
    '#00FFFF', // cyan
    '#FFFF00', // yellow
    '#0000FF', // blue
    '#FF00FF', // magenta
    '#FF0000', // redBright
    '#00FFFF', // cyanBright
    '#FFFF00', // yellowBright
    '#0000FF', // blueBright
    '#FF00FF', // magentaBright
];

function formatTime(ms) {
    if (ms < 1000) {
        return `${ms.toFixed(3)} ms`;
    }
    return `${(ms / 1000).toFixed(3)} s`;
}

function formatNumber(num) {
    if (num < 1e3) return String(Math.round(num));
    if (num < 1e6) return `${(num / 1e3).toFixed(1)}k`;
    if (num < 1e9) return `${(num / 1e6).toFixed(1)}M`;
    return `${(num / 1e9).toFixed(1)}G`;
}

module.exports = {
    COLORS: TABLE_COLORS,
    RESET: ANSI_COLORS.reset,
    WEB_COLORS,
    formatTime,
    formatNumber
};
