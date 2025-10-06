// battleship.js
// Juego Battleship completo pero no tanto (Node.js, consola), primera parte revisada el 26 de Septiembre del 2025


const readline = require('readline');
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });
const question = (q) => new Promise(resolve => rl.question(q, ans => resolve(ans.trim())));

const ROWS = 10;
const COLS = 10;
const LETTERS = 'ABCDEFGHIJ';

const SHIPS_MASTER = [
  { id: 1, name: 'Portaaviones', len: 5 },
  { id: 2, name: 'Acorazado',    len: 4 },
  { id: 3, name: 'Crucero',      len: 3 },
  { id: 4, name: 'Submarino',    len: 3 },
  { id: 5, name: 'Destructor',   len: 2 },
];

const makeEmptyBoard = () => Array.from({ length: ROWS }, () => Array.from({ length: COLS }, () => '.'));
const cloneBoard = (b) => b.map(row => row.slice());

function clearScreen() {
  process.stdout.write('\x1B[2J\x1B[0f');
}

function printBoard(board, previewCells = []) {
  const display = cloneBoard(board);
  for (const [r,c] of previewCells) if (r>=0 && r<ROWS && c>=0 && c<COLS) display[r][c]='O';

  process.stdout.write('\n    ');
  for (let c=0;c<COLS;c++) process.stdout.write((c+1).toString().padStart(2,' ') + '  ');
  process.stdout.write('\n');

  for (let r=0;r<ROWS;r++) {
    process.stdout.write(String.fromCharCode(65+r) + '  ');
    for (let c=0;c<COLS;c++) {
      process.stdout.write(' ' + display[r][c] + ' ');
      if (c < COLS-1) process.stdout.write('|');
    }
    process.stdout.write('\n');
    if (r < ROWS-1) {
      process.stdout.write('   ');
      for (let c=0;c<COLS;c++) {
        process.stdout.write('---');
        if (c < COLS-1) process.stdout.write('+');
      }
      process.stdout.write('\n');
    }
  }
  process.stdout.write('\n');
}

function printTwoBoardsSideBySide(ownBoard, attackBoard) {
  const leftHeader = '    ' + Array.from({length:COLS},(_,i)=> (i+1).toString().padStart(2,' ') + '  ').join('');
  const rightHeader = leftHeader;
  clearScreen();
  console.log(leftHeader + '     ' + rightHeader);
  for (let r=0;r<ROWS;r++) {
    // left row
    let left = String.fromCharCode(65+r) + '  ';
    for (let c=0;c<COLS;c++) {
      left += ' ' + ownBoard[r][c] + ' ';
      if (c < COLS-1) left += '|';
    }
    // right row
    let right = String.fromCharCode(65+r) + '  ';
    for (let c=0;c<COLS;c++) {
      right += ' ' + attackBoard[r][c] + ' ';
      if (c < COLS-1) right += '|';
    }
    console.log(left + '     ' + right);
    if (r < ROWS-1) {
      const sep = '   ' + Array.from({length:COLS},()=> '---').join('+');
      console.log(sep + '     ' + sep);
    }
  }
  console.log('\n');
}

function cellsForShip(anchorR, anchorC, len, horizontal) {
  const cells = [];
  for (let i=0;i<len;i++) cells.push(horizontal ? [anchorR, anchorC+i] : [anchorR+i, anchorC]);
  return cells;
}
function withinBounds(cells) { return cells.every(([r,c])=>r>=0 && r<ROWS && c>=0 && c<COLS); }
function overlapsExisting(board, cells) { return cells.some(([r,c])=>board[r][c] !== '.'); }
function commitShipToBoard(board, shipId, cells) { for (const [r,c] of cells) board[r][c] = shipId.toString(); }
function parseCoordinate(input) {
  input = input.replace(',', ' ').replace(/\s+/g,' ').trim().toUpperCase();
  const parts = input.split(' ');
  if (parts.length === 2) {
    const [p1,p2] = parts;
    if (/^[A-J]$/.test(p1) && (/^[1-9]$|^10$/.test(p2))) return [p1.charCodeAt(0)-65, parseInt(p2,10)-1];
    if ((/^[1-9]$|^10$/.test(p1)) && /^[A-J]$/.test(p2)) return [p2.charCodeAt(0)-65, parseInt(p1,10)-1];
  } else {
    const token = parts[0];
    const m1 = token.match(/^([A-J])([1-9]|10)$/);
    if (m1) return [m1[1].charCodeAt(0)-65, parseInt(m1[2],10)-1];
    const m2 = token.match(/^([1-9]|10)([A-J])$/);
    if (m2) return [m2[2].charCodeAt(0)-65, parseInt(m2[1],10)-1];
  }
  return null;
}

function printShipsOptions(shipsList) {
  console.log('Barcos por colocar:');
  shipsList.forEach((s, idx) => {
    console.log(`${idx+1}. ${s.name} (${s.len} casillas)`);
  });
  console.log('');
}

async function placeShipsForPlayer(playerName) {
  clearScreen();
  console.log(`Colocación de barcos para: ${playerName}\n`);
  let board = makeEmptyBoard();
  let shipsList = SHIPS_MASTER.map(s=>({...s}));

  const placedShips = [];

  while (shipsList.length > 0) {
    printBoardWithHeader(board, playerName);
    printShipsOptions(shipsList);
    let optRaw = await question(`Elige opción de barco (1-${shipsList.length}): `);
    let opt = parseInt(optRaw,10);
    if (!Number.isInteger(opt) || opt < 1 || opt > shipsList.length) {
      console.log('Opción inválida. Intenta de nuevo.');
      continue;
    }
    const chosenShip = shipsList.splice(opt-1,1)[0];

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
        if (withinBounds(cellsForShip(r,c,chosenShip.len,!horizontal))) horizontal = !horizontal;
        else { console.log('No cabe en esa posición.'); continue; }
      }

      let anchorR = r, anchorC = c;
      let placed = false;
      while (!placed) {
        const previewCells = cellsForShip(anchorR, anchorC, chosenShip.len, horizontal);
        clearScreen();
        console.log(`Jugador: ${playerName}  —  Colocando: ${chosenShip.name} (${chosenShip.len} casillas)`);
        printBoard(board, previewCells);
        console.log('Controles: w=arriba, s=abajo, a=izquierda, d=derecha, r=rotar, p=colocar, c=cancelar');
        const ctrl = (await question('Ingresa control: ')).toLowerCase();
        if (ctrl === 'w' || ctrl === 's' || ctrl === 'a' || ctrl === 'd') {
          let nr = anchorR + (ctrl === 's' ? 1 : ctrl === 'w' ? -1 : 0);
          let nc = anchorC + (ctrl === 'd' ? 1 : ctrl === 'a' ? -1 : 0);
          const newCells = cellsForShip(nr, nc, chosenShip.len, horizontal);
          if (!withinBounds(newCells)) { console.log('Te saliste del tablero.'); await question(''); }
          else { anchorR = nr; anchorC = nc; }
        } else if (ctrl === 'r') {
          const newHoriz = !horizontal;
          const newCells = cellsForShip(anchorR, anchorC, chosenShip.len, newHoriz);
          if (!withinBounds(newCells)) { console.log('No cabe rotando aquí.'); await question(''); }
          else horizontal = newHoriz;
        } else if (ctrl === 'p') {
          const finalCells = cellsForShip(anchorR, anchorC, chosenShip.len, horizontal);
          if (!withinBounds(finalCells)) { console.log('Fuera de tablero.'); await question(''); continue; }
          if (overlapsExisting(board, finalCells)) { console.log('Choca con otro barco.'); await question(''); continue; }
          commitShipToBoard(board, chosenShip.id, finalCells);
          placedShips.push({ id: chosenShip.id, name: chosenShip.name, len: chosenShip.len, cells: finalCells.slice(), hits: 0 });
          console.log(`${chosenShip.name} colocado en: ${finalCells.map(([rr,cc]) => String.fromCharCode(65+rr)+(cc+1)).join(', ')}`);
          await question('ENTER para seguir...');
          placed = true;
        } else if (ctrl === 'c') {
          console.log('Cancelado, elige otra coordenada.');
          await question('ENTER...');
          break;
        } else {
          console.log('Control inválido.'); await question('');
        }
      }
      if (placed) break;
    }
  }

  clearScreen();
  console.log(`Tablero final de ${playerName}:`);
  printBoard(board);
  await question('ENTER para continuar...');
  return { board, ships: placedShips };
}

function printBoardWithHeader(board, playerName) {
  clearScreen();
  console.log(`Jugador: ${playerName}`);
  printBoard(board);
}

function findShipById(ships, id) { return ships.find(s=>s.id === id); }

function applyHit(opponent, attackBoard, r, c) {
  const cell = opponent.board[r][c];
  if (cell === '.' || cell === 'x' || cell === 'X') return { result: 'miss' };
  const id = parseInt(cell,10);
  opponent.board[r][c] = 'x';
  attackBoard[r][c] = 'x';
  const ship = findShipById(opponent.ships, id);
  if (!ship) return { result: 'hit' };
  ship.hits++;
  if (ship.hits >= ship.len) {
    for (const [sr,sc] of ship.cells) {
      opponent.board[sr][sc] = 'X';
      attackBoard[sr][sc] = 'X';
    }
    return { result: 'sunk', shipName: ship.name };
  }
  return { result: 'hit' };
}

function applyMiss(attackBoard, r, c) {
  attackBoard[r][c] = 'o';
}

function allSunk(ships) {
  return ships.every(s => s.hits >= s.len);
}

function iaChooseRandom(attackBoard) {
  const options = [];
  for (let r=0;r<ROWS;r++) for (let c=0;c<COLS;c++) if (attackBoard[r][c] === '.') options.push([r,c]);
  if (options.length === 0) return null;
  return options[Math.floor(Math.random()*options.length)];
}

function coordToRC(input) {
  return parseCoordinate(input);
}

async function playGame(mode, p1Obj, p2Obj, name1, name2) {
  const attack1 = makeEmptyBoard();
  const attack2 = makeEmptyBoard();

  let turn = 1;
  let currentName, currentBoard, currentShips, currentAttack, opponentName, opponentObj, opponentAttack;
  let iaDelayMs = 500;

  while (true) {
    if (turn === 1) {
      currentName = name1; currentBoard = p1Obj.board; currentShips = p1Obj.ships; currentAttack = attack1;
      opponentName = name2; opponentObj = p2Obj; opponentAttack = attack2;
    } else {
      currentName = name2; currentBoard = p2Obj.board; currentShips = p2Obj.ships; currentAttack = attack2;
      opponentName = name1; opponentObj = p1Obj; opponentAttack = attack1;
    }

    printTwoBoardsSideBySide(currentBoard, currentAttack);
    console.log(`Turno de ${currentName} (${turn===1? 'X':'O'})`);
    let target = null;
    let isAI = (mode === 2 && ((turn===2))) || (mode === 3);
    if (mode === 3) {
      isAI = true;
    }

    if (isAI) {
      // delay para IA
      await new Promise(res => setTimeout(res, iaDelayMs));
      const choice = iaChooseRandom(currentAttack);
      if (!choice) { console.log('No quedan casillas para atacar'); break; }
      target = choice;
      console.log(`${currentName} ataca ${String.fromCharCode(65+target[0])}${target[1]+1}`);
    } else {
      // humano
      while (true) {
        const coordRaw = await question('Elige coordenada a atacar (ej. A5): ');
        const parsed = coordToRC(coordRaw);
        if (!parsed) { console.log('Coordenada inválida.'); continue; }
        const [r,c] = parsed;
        if (currentAttack[r][c] !== '.') { console.log('Ya atacaste esa casilla.'); continue; }
        target = [r,c];
        break;
      }
    }

    const [tr,tc] = target;
    // ataque sin hit
    const occupant = opponentObj.board[tr][tc];
    if (occupant === '.' ) {
      applyMiss(currentAttack, tr, tc);
      printTwoBoardsSideBySide(currentBoard, currentAttack);
      console.log('Agua.');
      // cambiar turno
      turn = (turn === 1 ? 2 : 1);
    } else if (occupant === 'x' || occupant === 'X') {
      // no repetir casilla
      console.log('Error: casilla ya dañada.');
      turn = (turn === 1 ? 2 : 1);
    } else {
      // hit
      const res = applyHit(opponentObj, currentAttack, tr, tc);
      if (res.result === 'hit') {
        printTwoBoardsSideBySide(currentBoard, currentAttack);
        console.log('Tocado!');
        // mismo jugador continua
      } else if (res.result === 'sunk') {
        printTwoBoardsSideBySide(currentBoard, currentAttack);
        console.log(`Hundido: ${res.shipName}!`);
        // verifica ganador
        if (allSunk(opponentObj.ships)) {
          console.log(`\n¡${currentName} ha ganado la partida!`);
          break;
        }
      }
    }


    if (isAI && mode !== 3) await new Promise(res => setTimeout(res, 300));
  }

  console.log('Partida finalizada. Pulsa ENTER para salir al menú.');
  await question('');
}

(async function main(){
  try {
    while (true) {
      clearScreen();
      console.log('=== BATTLESHIP (Juego completo) ===');
      console.log('1. Jugador vs Jugador');
      console.log('2. Jugador vs IA');
      console.log('3. IA vs IA');
      console.log('4. Salir');
      const optRaw = await question('Elige modo: ');
      const opt = parseInt(optRaw,10);
      if (!Number.isInteger(opt) || opt <1 || opt>4) { console.log('Opción inválida'); await question('ENTER'); continue; }
      if (opt === 4) break;

      // Player names
      let name1 = 'Jugador 1', name2 = 'Jugador 2';
      if (opt === 1) {
        name1 = await question('Nombre Jugador 1: ');
        name2 = await question('Nombre Jugador 2: ');
      } else if (opt === 2) {
        name1 = await question('Nombre Jugador (humano): ');
        name2 = 'IA';
      } else {
        name1 = 'IA 1'; name2 = 'IA 2';
      }

      console.log(`\n${name1}, coloca tus barcos. Pulsa ENTER cuando listo.`);
      await question('');
      const p1 = await placeShipsForPlayer(name1);

      console.log(`\n${name2}, coloca tus barcos. Pulsa ENTER cuando listo.`);
      await question('');
      const p2 = await placeShipsForPlayer(name2);

      console.log('\nAmbos jugadores han colocado sus barcos. Pulsa ENTER para comenzar la partida.');
      await question('');

      await playGame(opt, p1, p2, name1, name2);
    }
  } finally {
    rl.close();
  }
})();
