import os, sys, time, random

CHAR_SIZE = 25
CELL_W = 3
CELLS_R = (CHAR_SIZE - 1) // 2
CELLS_C = (CHAR_SIZE - 1) // 2

WALL = 1
PATH = 0

def pad(s):
    if len(s) >= CELL_W:
        return s[:CELL_W]
    return s + " " * (CELL_W - len(s))

WALL_STR  = pad(" M ")
PATH_STR  = pad("   ")
TRAIL_STR = pad(".")
PLAYER_STR = pad("🧙")
EXIT_STR   = pad("🔮")

if os.name == "nt":
    import msvcrt
else:
    import tty, termios, fcntl

class CellObj:
    def __init__(self,i,j):
        self.i=i; self.j=j
        self.walls=[True,True,True,True]

def index_cell(i,j):
    if 0 <= i < CELLS_R and 0 <= j < CELLS_C:
        return i*CELLS_C + j
    return -1

cells = [CellObj(i,j) for i in range(CELLS_R) for j in range(CELLS_C)]

def neighbors(i,j):
    return [(i-1,j),(i,j+1),(i+1,j),(i,j-1)]

def generate_options(cell, path):
    opts=[]
    for nb in neighbors(cell.i, cell.j):
        if index_cell(*nb) != -1 and (not path or nb != path[-1]):
            opts.append(nb)
    return opts

def remove_walls(a,b):
    di = a.i - b.i
    dj = a.j - b.j
    if di == -1:
        a.walls[2]=False; b.walls[0]=False
    elif di == 1:
        a.walls[0]=False; b.walls[2]=False
    if dj == -1:
        a.walls[1]=False; b.walls[3]=False
    elif dj == 1:
        a.walls[3]=False; b.walls[1]=False

def wilson():
    verts=[(i,j) for i in range(CELLS_R) for j in range(CELLS_C)]
    maze=[]
    first=random.choice(verts)
    maze.append(first); verts.remove(first)
    def walk():
        path=[]
        coords=random.choice(verts); path.append(coords)
        while coords not in maze:
            curr = cells[index_cell(*coords)]
            opts = generate_options(curr, path)
            if not opts:
                coords=random.choice(verts); path=[coords]; continue
            opt = random.choice(opts)
            try:
                k = path.index(opt)
            except ValueError:
                path.append(opt)
            else:
                path = path[:k+1]
            coords = opt
        maze.extend(path)
        for k in range(len(path)-1):
            a=path[k]; b=path[k+1]
            remove_walls(cells[index_cell(*a)], cells[index_cell(*b)])
            if path[k] in verts: verts.remove(path[k])
    while verts:
        walk()

def build_matrix_from_cells():
    mat = [[WALL for _ in range(CHAR_SIZE)] for __ in range(CHAR_SIZE)]
    for i in range(CELLS_R):
        for j in range(CELLS_C):
            di = 2*i+1; dj = 2*j+1
            mat[di][dj] = PATH
    for i in range(CELLS_R):
        for j in range(CELLS_C):
            cell = cells[index_cell(i,j)]
            ci, cj = 2*i+1, 2*j+1
            if not cell.walls[0]:
                mat[ci-1][cj] = PATH
            if not cell.walls[1]:
                mat[ci][cj+1] = PATH
            if not cell.walls[2]:
                mat[ci+1][cj] = PATH
            if not cell.walls[3]:
                mat[ci][cj-1] = PATH
    return mat

def render_static(mat, top=1, left=1):
    sys.stdout.write("\033[?25l")
    for r,row in enumerate(mat):
        line = "".join(WALL_STR if cell==WALL else PATH_STR for cell in row)
        sys.stdout.write(f"\033[{top + r};{left}H" + line)
    sys.stdout.flush()

def write_cell(r,c,token, top=1, left=1):
    row = top + r
    col = left + c * CELL_W + 1
    sys.stdout.write(f"\033[{row};{col}H" + token)
    sys.stdout.flush()

def can_move(mat, to_cell):
    i,j = to_cell
    if 0 <= i < CHAR_SIZE and 0 <= j < CHAR_SIZE:
        return mat[i][j] == PATH
    return False

class ExplorerAI:
    def __init__(self, start, goal, mat):
        self.start = start; self.goal = goal; self.mat = mat
        self.visited = set(); self.stack=[start]
    def step(self):
        if not self.stack: return None
        cur = self.stack[-1]; self.visited.add(cur)
        if cur == self.goal: return cur
        neighs=[]
        for d in [(-1,0),(1,0),(0,-1),(0,1)]:
            ni=cur[0]+d[0]; nj=cur[1]+d[1]
            if 0 <= ni < CHAR_SIZE and 0 <= nj < CHAR_SIZE and self.mat[ni][nj]==PATH:
                neighs.append((ni,nj))
        unvisited=[n for n in neighs if n not in self.visited]
        if unvisited:
            p=0.35
            if random.random() < p:
                unvisited.sort(key=lambda x: abs(x[0]-self.goal[0]) + abs(x[1]-self.goal[1]))
                chosen = unvisited[0]
            else:
                chosen = random.choice(unvisited)
            self.stack.append(chosen); return chosen
        else:
            self.stack.pop()
            if not self.stack: return None
            return self.stack[-1]

def main():
    random.seed()
    wilson()
    mat = build_matrix_from_cells()
    empties = [(i,j) for i in range(CHAR_SIZE) for j in range(CHAR_SIZE) if mat[i][j]==PATH]
    player = random.choice(empties)
    exitp  = random.choice(empties)
    while exitp == player:
        exitp = random.choice(empties)
    render_static(mat, top=3, left=1)
    write_cell(exitp[0], exitp[1], EXIT_STR, top=3, left=1)
    write_cell(player[0], player[1], PLAYER_STR, top=3, left=1)
    if os.name != "nt":
        fd = sys.stdin.fileno(); old_settings = termios.tcgetattr(fd); old_flags = fcntl.fcntl(fd, fcntl.F_GETFL)
        tty.setcbreak(fd); fcntl.fcntl(fd, fcntl.F_SETFL, old_flags | os.O_NONBLOCK)
    ai = None; ai_active = False; last_ai = 0
    try:
        while True:
            ch = None
            if os.name == "nt":
                if msvcrt.kbhit():
                    try: ch = msvcrt.getwch()
                    except: ch = msvcrt.getch().decode("utf-8", errors="ignore")
            else:
                try: ch = sys.stdin.read(1)
                except (BlockingIOError, IOError): ch = None
            if ch:
                c = ch.lower()
                if c == 'q': break
                if c == 'b' and not ai_active:
                    ai = ExplorerAI(player, exitp, mat); ai_active = True; last_ai = time.time()
                if not ai_active and c in ('w','a','s','d'):
                    di = dj = 0
                    if c == 'w': di = -1
                    if c == 's': di = 1
                    if c == 'a': dj = -1
                    if c == 'd': dj = 1
                    new = (player[0]+di, player[1]+dj)
                    if can_move(mat, new):
                        prev = player; player = new
                        write_cell(prev[0], prev[1], TRAIL_STR, top=3, left=1)
                        write_cell(player[0], player[1], PLAYER_STR, top=3, left=1)
                        if player == exitp:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1HHas encontrado la salida. Fin.\n"); break
            if ai_active:
                now = time.time()
                if now - last_ai >= 0.1:
                    last_ai = now
                    nextp = ai.step()
                    if nextp is None:
                        ai_active = False; ai = None
                        sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1HLa IA terminó de explorar.\n"); sys.stdout.flush()
                    else:
                        prev = player; player = nextp
                        write_cell(prev[0], prev[1], TRAIL_STR, top=3, left=1)
                        write_cell(player[0], player[1], PLAYER_STR, top=3, left=1)
                        if player == exitp:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1HLa IA encontró la salida. Fin.\n"); break
            time.sleep(0.01)
    finally:
        sys.stdout.write("\033[?25h"); sys.stdout.flush()
        if os.name != "nt":
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
            fcntl.fcntl(fd, fcntl.F_SETFL, old_flags)

if __name__ == "__main__":
    main()
