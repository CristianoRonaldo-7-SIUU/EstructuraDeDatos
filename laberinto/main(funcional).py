import random, os, sys, time
if os.name == "nt":
    import msvcrt
else:
    import tty, termios, fcntl

CHAR_ROWS = 25
CHAR_COLS = 25
CELLS_R = (CHAR_ROWS - 1) // 2
CELLS_C = (CHAR_COLS - 1) // 2

CHAR_CORNER = "+"
CHAR_H = "-"
CHAR_V = "|"
CHAR_PATH = " "
CHAR_PLAYER = "🧙"
CHAR_EXIT = "🔮"
CHAR_TRAIL = "."

class Cell:
    def __init__(self, i, j):
        self.i = i
        self.j = j
        self.walls = [True, True, True, True]

def index_cell(i, j):
    if 0 <= i < CELLS_R and 0 <= j < CELLS_C:
        return i * CELLS_C + j
    return -1

cells = [Cell(i, j) for i in range(CELLS_R) for j in range(CELLS_C)]

def neighbors_tuple(i, j):
    return [(i - 1, j), (i, j + 1), (i + 1, j), (i, j - 1)]

def cell_generate_options(cell, path):
    opts = []
    for nbr in neighbors_tuple(cell.i, cell.j):
        if index_cell(*nbr) != -1 and (not path or nbr != path[-1]):
            opts.append(nbr)
    return opts

def remove_walls(a, b):
    di = a.i - b.i
    dj = a.j - b.j
    if di == -1:
        a.walls[2] = False
        b.walls[0] = False
    elif di == 1:
        a.walls[0] = False
        b.walls[2] = False
    if dj == -1:
        a.walls[1] = False
        b.walls[3] = False
    elif dj == 1:
        a.walls[3] = False
        b.walls[1] = False

def wilson_generate():
    vertices = [(i, j) for i in range(CELLS_R) for j in range(CELLS_C)]
    maze = []
    first = random.choice(vertices)
    maze.append(first)
    vertices.remove(first)
    def random_walk():
        path = []
        coords = random.choice(vertices)
        path.append(coords)
        while coords not in maze:
            curr = cells[index_cell(*coords)]
            options = cell_generate_options(curr, path)
            if not options:
                coords = random.choice(vertices)
                path = [coords]
                continue
            option = random.choice(options)
            try:
                i = path.index(option)
            except ValueError:
                path.append(option)
            else:
                path = path[:i+1]
            coords = option
        maze.extend(path)
        for k in range(len(path)-1):
            a = path[k]
            b = path[k+1]
            remove_walls(cells[index_cell(*a)], cells[index_cell(*b)])
            if path[k] in vertices:
                vertices.remove(path[k])
    while vertices:
        random_walk()

def build_canvas(visited_cells, player_cell, exit_cell):
    grid = [[CHAR_PATH for _ in range(CHAR_COLS)] for __ in range(CHAR_ROWS)]
    for r in range(0, CHAR_ROWS, 2):
        for c in range(0, CHAR_COLS, 2):
            grid[r][c] = CHAR_CORNER
    for i in range(CELLS_R):
        for j in range(CELLS_C):
            ci = 2 * i + 1
            cj = 2 * j + 1
            grid[ci][cj] = CHAR_PATH
            cell = cells[index_cell(i, j)]
            if cell.walls[0]:
                grid[ci - 1][cj] = CHAR_H
            else:
                grid[ci - 1][cj] = CHAR_PATH
            if cell.walls[1]:
                grid[ci][cj + 1] = CHAR_V
            else:
                grid[ci][cj + 1] = CHAR_PATH
            if cell.walls[2]:
                grid[ci + 1][cj] = CHAR_H
            else:
                grid[ci + 1][cj] = CHAR_PATH
            if cell.walls[3]:
                grid[ci][cj - 1] = CHAR_V
            else:
                grid[ci][cj - 1] = CHAR_PATH
    for (i,j) in visited_cells:
        ci = 2 * i + 1
        cj = 2 * j + 1
        grid[ci][cj] = CHAR_TRAIL
    ei, ej = exit_cell
    grid[2*ei+1][2*ej+1] = CHAR_EXIT
    pi, pj = player_cell
    grid[2*pi+1][2*pj+1] = CHAR_PLAYER
    return grid

def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")

def render(visited, player_cell, exit_cell):
    g = build_canvas(visited, player_cell, exit_cell)
    clear_screen()
    for row in g:
        print("".join(row))
    print()
    print("W A S D mover | B iniciar IA | Q salir")

def can_move(cell, direction):
    i, j = cell
    if direction == 'w':
        if index_cell(i-1, j) == -1: return False
        return not cells[index_cell(i, j)].walls[0]
    if direction == 's':
        if index_cell(i+1, j) == -1: return False
        return not cells[index_cell(i, j)].walls[2]
    if direction == 'a':
        if index_cell(i, j-1) == -1: return False
        return not cells[index_cell(i, j)].walls[3]
    if direction == 'd':
        if index_cell(i, j+1) == -1: return False
        return not cells[index_cell(i, j)].walls[1]
    return False

def random_cell_positions():
    empties = []
    for i in range(CELLS_R):
        for j in range(CELLS_C):
            empties.append((i,j))
    return empties

class ExplorerAI:
    def __init__(self, start, goal):
        self.start = start
        self.goal = goal
        self.visited = set()
        self.stack = [start]
        self.prev = {}
    def step(self):
        if not self.stack:
            return None
        current = self.stack[-1]
        self.visited.add(current)
        if current == self.goal:
            return current
        neighs = []
        for d in [(-1,0),(1,0),(0,-1),(0,1)]:
            ni = current[0] + d[0]
            nj = current[1] + d[1]
            if 0 <= ni < CELLS_R and 0 <= nj < CELLS_C:
                if not is_wall_between(current, (ni,nj)):
                    neighs.append((ni,nj))
        unvisited = [n for n in neighs if n not in self.visited]
        if unvisited:
            p = 0.35
            if random.random() < p:
                unvisited.sort(key=lambda x: abs(x[0]-self.goal[0]) + abs(x[1]-self.goal[1]))
                chosen = unvisited[0]
            else:
                chosen = random.choice(unvisited)
            self.prev[chosen] = current
            self.stack.append(chosen)
            return chosen
        else:
            self.stack.pop()
            if not self.stack:
                return None
            return self.stack[-1]

def is_wall_between(a, b):
    ai, aj = a
    bi, bj = b
    di = ai - bi
    dj = aj - bj
    if di == 1:
        return cells[index_cell(ai,aj)].walls[0]
    if di == -1:
        return cells[index_cell(ai,aj)].walls[2]
    if dj == 1:
        return cells[index_cell(ai,aj)].walls[3]
    if dj == -1:
        return cells[index_cell(ai,aj)].walls[1]
    return True

def main():
    random.seed()
    wilson_generate()
    visited = set()
    empties = random_cell_positions()
    player_cell = random.choice(empties)
    exit_cell = random.choice(empties)
    while exit_cell == player_cell:
        exit_cell = random.choice(empties)
    visited.add(player_cell)
    ai = None
    ai_active = False
    last_ai_time = 0
    render(visited, player_cell, exit_cell)
    if os.name != "nt":
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        old_flags = fcntl.fcntl(fd, fcntl.F_GETFL)
        tty.setcbreak(fd)
        fcntl.fcntl(fd, fcntl.F_SETFL, old_flags | os.O_NONBLOCK)
    try:
        while True:
            ch = None
            if os.name == "nt":
                if msvcrt.kbhit():
                    try:
                        ch = msvcrt.getwch()
                    except:
                        ch = msvcrt.getch().decode("utf-8", errors="ignore")
            else:
                try:
                    ch = sys.stdin.read(1)
                except (BlockingIOError, IOError):
                    ch = None
            if ch:
                ch = ch.lower()
                if ch == 'q':
                    break
                if ch == 'b' and not ai_active:
                    ai = ExplorerAI(player_cell, exit_cell)
                    ai_active = True
                    last_ai_time = time.time()
                if not ai_active and ch in ('w','a','s','d'):
                    if can_move(player_cell, ch):
                        if ch == 'w': player_cell = (player_cell[0]-1, player_cell[1])
                        if ch == 's': player_cell = (player_cell[0]+1, player_cell[1])
                        if ch == 'a': player_cell = (player_cell[0], player_cell[1]-1)
                        if ch == 'd': player_cell = (player_cell[0], player_cell[1]+1)
                        visited.add(player_cell)
                        render(visited, player_cell, exit_cell)
                        if player_cell == exit_cell:
                            render(visited, player_cell, exit_cell)
                            print("🔮 ¡Has encontrado la salida! ¡Fin!")
                            break
            if ai_active:
                now = time.time()
                if now - last_ai_time >= 0.1:
                    last_ai_time = now
                    next_pos = ai.step()
                    if next_pos is None:
                        ai_active = False
                        ai = None
                        render(visited, player_cell, exit_cell)
                        print("La IA terminó de explorar.")
                    else:
                        player_cell = next_pos
                        visited.add(player_cell)
                        render(visited, player_cell, exit_cell)
                        if player_cell == exit_cell:
                            render(visited, player_cell, exit_cell)
                            print("🔮 La IA encontró la salida. ¡Fin!")
                            break
            time.sleep(0.01)
    finally:
        if os.name != "nt":
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
            fcntl.fcntl(fd, fcntl.F_SETFL, old_flags)

if __name__ == "__main__":
    main()
