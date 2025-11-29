// battleship_place.js
// Script de Node para colocar barcos en el juego Battleship
// Revisado (primera parte: colocación de barcos): 26 de Septiembre del 2025

const readline = require('readline');

const rl = readline.createInterface({ input: process.stdin, output: process.stdout });
const question = (q) => new Promise(resolve => rl.question(q, ans => resolve(ans.trim())));

const ROWS = 10; 
const COLS = 10; 

// lista de barcos con su tamaño
const SHIPS_MASTER = [
  { id: 1, name: 'Portaaviones', len: 5 },
  { id: 2, name: 'Acorazado',    len: 4 },
  { id: 3, name: 'Crucero',      len: 3 },
  { id: 4, name: 'Submarino',    len: 3 },
  { id: 5, name: 'Destructor',   len: 2 },
];

// tablero vacío con puntos
const makeEmptyBoard = () => Array.from({ length: ROWS }, () => Array.from({ length: COLS }, () => '.'));

// clonar para no modificar el original sin querer
const cloneBoard = (b) => b.map(row => row.slice());

// imprime tablero con coordenadas
function printBoard(board, previewCells = []) {
  const display = cloneBoard(board);
  for (const [r, c] of previewCells) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
      display[r][c] = 'O';
    }
  }

  process.stdout.write('\n    ');
  for (let c = 0; c < COLS; c++) {
    process.stdout.write((c+1).toString().padStart(2,' ') + '  ');
  }
  process.stdout.write('\n');

  for (let r = 0; r < ROWS; r++) {
    process.stdout.write(String.fromCharCode(65 + r) + '  ');
    for (let c = 0; c < COLS; c++) {
      process.stdout.write(' ' + display[r][c] + ' ');
      if (c < COLS - 1) process.stdout.write('|');
    }
    process.stdout.write('\n');
    if (r < ROWS - 1) {
      process.stdout.write('   ');
      for (let c = 0; c < COLS; c++) {
        process.stdout.write('---');
        if (c < COLS - 1) process.stdout.write('+');
      }
      process.stdout.write('\n');
    }
  }
  process.stdout.write('\n');
}

// devuelve las casillas que ocuparía un barco según ancla y orientación
function cellsForShip(anchorR, anchorC, len, horizontal) {
  const cells = [];
  for (let i = 0; i < len; i++) {
    cells.push(horizontal ? [anchorR, anchorC + i] : [anchorR + i, anchorC]);
  }
  return cells;
}

// ver si todas las casillas caen dentro del tablero
function withinBounds(cells) {
  return cells.every(([r,c]) => r >= 0 && r < ROWS && c >= 0 && c < COLS);
}

// ver si alguna casilla ya está ocupada
function overlapsExisting(board, cells) {
  return cells.some(([r,c]) => board[r][c] !== '.');
}

// coloca un barco definitivo en el tablero
function commitShipToBoard(board, shipId, cells) {
  for (const [r,c] of cells) board[r][c] = shipId.toString();
}

// limpiar pantalla
function clearScreen() {
  process.stdout.write('\x1B[2J\x1B[0f');
}

function parseCoordinate(input) {
  input = input.replace(',', ' ').replace(/\s+/g,' ').trim().toUpperCase();
  const parts = input.split(' ');
  if (parts.length === 2) {
    const [p1,p2] = parts;
    if (/^[A-J]$/.test(p1) && /^[1-9]$|^10$/.test(p2)) return [p1.charCodeAt(0)-65, parseInt(p2,10)-1];
    if (/^[1-9]$|^10$/.test(p1) && /^[A-J]$/.test(p2)) return [p2.charCodeAt(0)-65, parseInt(p1,10)-1];
  } else {
    const token = parts[0];
    const m1 = token.match(/^([A-J])([1-9]|10)$/);
    if (m1) return [m1[1].charCodeAt(0)-65, parseInt(m1[2],10)-1];
    const m2 = token.match(/^([1-9]|10)([A-J])$/);
    if (m2) return [m2[2].charCodeAt(0)-65, parseInt(m2[1],10)-1];
  }
  return null;
}

// muestra los barcos que quedan por colocar
function printShipsOptions(shipsList) {
  console.log('Barcos por colocar:');
  shipsList.forEach((s, idx) => {
    console.log(`${idx+1}. ${s.name} (${s.len} casillas)`);
  });
  console.log('');
}

// loop para que un jugador coloque todos sus barcos
async function placeShipsForPlayer(playerName) {
  clearScreen();
  console.log(`Colocación de barcos para: ${playerName}\n`);
  let board = makeEmptyBoard();
  let ships = SHIPS_MASTER.map(s => ({...s}));

  while (ships.length > 0) {
    printBoardWithHeader(board, playerName);
    printShipsOptions(ships);
    let optRaw = await question(`Elige opción de barco (1-${ships.length}): `);
    let opt = parseInt(optRaw,10);
    if (!Number.isInteger(opt) || opt < 1 || opt > ships.length) {
      console.log('Opción inválida. Intenta de nuevo.');
      continue;
    }
    const chosenShip = ships.splice(opt-1,1)[0];

    while (true) {
      printBoardWithHeader(board, playerName);
      console.log(`Colocando: ${chosenShip.name} (${chosenShip.len} casillas).`);
      const coordRaw = await question('Escribe coordenada inicial (ej. A1 o 1A): ');
      const parsed = parseCoordinate(coordRaw);
      if (!parsed) { console.log('Coordenada inválida. Intenta de nuevo.'); continue; }
      let [r,c] = parsed;
      let horizontal = true;
      let previewCells = cellsForShip(r,c,chosenShip.len,horizontal);
      if (!withinBounds(previewCells)) {
        if (withinBounds(cellsForShip(r,c,chosenShip.len,!horizontal))) {
          horizontal = !horizontal;
        } else {
          console.log('No cabe en esa posición.');
          continue;
        }
      }

      let anchorR = r, anchorC = c;
      let placed = false;
      while (!placed) {
        previewCells = cellsForShip(anchorR, anchorC, chosenShip.len, horizontal);
        clearScreen();
        console.log(`Jugador: ${playerName}  —  Colocando: ${chosenShip.name} (${chosenShip.len} casillas)`);
        printBoard(board, previewCells);
        console.log('Controles: w=arriba, s=abajo, a=izquierda, d=derecha, r=rotar, p=colocar, c=cancelar');
        const ctrl = (await question('Ingresa control: ')).toLowerCase();
        if (ctrl === 'w' || ctrl === 's' || ctrl === 'a' || ctrl === 'd') {
          let nr = anchorR + (ctrl === 's' ? 1 : ctrl === 'w' ? -1 : 0);
          let nc = anchorC + (ctrl === 'd' ? 1 : ctrl === 'a' ? -1 : 0);
          const newCells = cellsForShip(nr, nc, chosenShip.len, horizontal);
          if (!withinBounds(newCells)) {
            console.log('Fuera del tablero.');
            await question('');
          } else {
            anchorR = nr; anchorC = nc;
          }
        } else if (ctrl === 'r') {
          const newHoriz = !horizontal;
          const newCells = cellsForShip(anchorR, anchorC, chosenShip.len, newHoriz);
          if (!withinBounds(newCells)) {
            console.log('Imposible rotar.');
            await question('');
          } else {
            horizontal = newHoriz;
          }
        } else if (ctrl === 'p') {
          const finalCells = cellsForShip(anchorR, anchorC, chosenShip.len, horizontal);
          if (!withinBounds(finalCells)) {
            console.log('Fuera de tablero.');
            await question('');
            continue;
          }
          if (overlapsExisting(board, finalCells)) {
            console.log('No puedes poner un barco encima de otro.');
            await question('');
            continue;
          }
          commitShipToBoard(board, chosenShip.id, finalCells);
          console.log(`${chosenShip.name} colocado en: ${finalCells.map(([rr,cc]) => String.fromCharCode(65+rr)+(cc+1)).join(', ')}`);
          await question('ENTER para seguir...');
          placed = true;
        } else if (ctrl === 'c') {
          console.log('Cancelado, elige otra coordenada.');
          await question('ENTER...');
          break;
        } else {
          console.log('Control inválido.');
          await question('');
        }
      }
      if (placed) break;
    }
  }

  clearScreen();
  console.log(`Tablero final de ${playerName}:`);
  printBoard(board);
  await question('ENTER para continuar...');
  return board;
}

function printBoardWithHeader(board, playerName) {
  clearScreen();
  console.log(`Jugador: ${playerName}`);
  printBoard(board);
}

(async function main(){
  try {
    console.log('=== Battleship: fase de colocación ===\n');
    const p1 = await question('Nombre Jugador 1: ');
    const p2 = await question('Nombre Jugador 2: ');

    console.log(`\n${p1}, coloca tus barcos.`);
    await question('ENTER para empezar...');
    const board1 = await placeShipsForPlayer(p1);

    console.log(`\n${p2}, coloca tus barcos.`);
    await question('ENTER para empezar...');
    const board2 = await placeShipsForPlayer(p2);

    clearScreen();
    console.log('Colocación terminada. Resumen:\n');
    console.log(`${p1}:\n`); printBoard(board1);
    console.log(`${p2}:\n`); printBoard(board2);

    console.log('Listo, barcos colocados.\n');
  } finally {
    rl.close();
  }

})();

