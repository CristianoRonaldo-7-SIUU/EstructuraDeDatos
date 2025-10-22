import os, sys, time, random

# --- GLOBALES Y CONSTANTES ---

CHAR_SIZE = 25
CELL_W = 2
CELLS_R = (CHAR_SIZE - 1) // 2
CELLS_C = (CHAR_SIZE - 1) // 2

WALL = 1
PATH = 0

def pad(s):
    if len(s) >= CELL_W:
        return s[:CELL_W]
    return s + " " * (CELL_W - len(s))

# Cadenas de Símbolos
WALL_STR     = pad(" M ")
PATH_STR     = pad(" ")
TRAIL_STR    = pad(".")
PLAYER_STR   = pad("🧙")
EXIT_STR     = pad("🔮")

# --- Nuevas constantes de características ---
TRAP_STR     = pad(" ")
HEART_STR    = "❤️"
EMPTY_HEART_STR = "🤍"
COMET_STR    = pad(" ☄ ")
SNOW_WALL_STR = pad(" ❄ ")
UNSEEN_STR   = pad("  ")
HEART_ITEM_STR = pad(" ❤ ") # (Cambio 3) Corazón para recoger

SNOW_WALL = 3 # Nuevo tipo de celda en la matriz
MAX_LIVES = 3

# Variables de estado del jugador
current_lives = MAX_LIVES
comet_count = 0

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
    # Reiniciar celdas para el nuevo nivel
    global cells
    cells = [CellObj(i,j) for i in range(CELLS_R) for j in range(CELLS_C)]
    
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

def can_move(mat, to_cell):
    i,j = to_cell
    if 0 <= i < CHAR_SIZE and 0 <= j < CHAR_SIZE:
        # El jugador solo puede moverse a celdas de CAMINO
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

def run_level(level):
    
    # --- 1. Inicialización del Nivel ---
    random.seed()
    wilson() # Regenera el laberinto
    mat = build_matrix_from_cells()
    empties = [(i,j) for i in range(CHAR_SIZE) for j in range(CHAR_SIZE) if mat[i][j]==PATH]
    
    # Reiniciar estado
    current_lives = MAX_LIVES
    comet_count = 0
    player_view_mode = True # Empezar en modo jugador (niebla de guerra)
    explored_map = [[False for _ in range(CHAR_SIZE)] for __ in range(CHAR_SIZE)]
    trail_set = set() # (Cambio 2) Almacena el rastro

    # Colocar 5 Trampas
    traps_pos = random.sample(empties, 5)
    for tp in traps_pos: empties.remove(tp)

    # Colocar 4 Cometas
    comets_pos = random.sample(empties, 4)
    for cp in comets_pos: empties.remove(cp)

    # (Cambio 3) Colocar 3 Corazones
    hearts_pos = random.sample(empties, 3)
    for hp in hearts_pos: empties.remove(hp)

    # Colocar 5 Paredes de Nieve (en posiciones de muro existentes)
    wall_positions = [(i,j) for i in range(1, CHAR_SIZE - 1) 
                        for j in range(1, CHAR_SIZE - 1) if mat[i][j]==WALL]
    snow_walls_pos = random.sample(wall_positions, 5)
    for (i,j) in snow_walls_pos:
        mat[i][j] = SNOW_WALL # Marcar en la matriz

    # Colocar Jugador y Salida
    player = random.choice(empties)
    exitp  = random.choice(empties)
    while exitp == player:
        exitp = random.choice(empties)

    # --- 2. Funciones de Ayuda (Locales al Nivel) ---

    def render_hud():
        # Dibuja el HUD en la línea 1
        life_display = (HEART_STR * current_lives) + (EMPTY_HEART_STR * (MAX_LIVES - current_lives))
        comet_display = COMET_STR * comet_count
        
        hud_line = f"VIDA: {life_display} | NIVEL: {level}/3 | ☄️: {comet_count} {comet_display}"
        controls_line = "(P: Visión, E: Romper, B: IA, Q: Salir)"
        
        # Limpiar línea y escribir
        sys.stdout.write(f"\033[1;1H" + " " * (CELL_W * CHAR_SIZE + 5)) # Limpiar línea 1
        sys.stdout.write(f"\033[2;1H" + " " * (CELL_W * CHAR_SIZE + 5)) # Limpiar línea 2
        
        sys.stdout.write(f"\033[1;1H{hud_line}")
        sys.stdout.write(f"\033[2;1H{controls_line}")
        sys.stdout.flush()

    def update_visibility(r, c):
        # (Cambio 1) La celda del jugador es visible
        if 0 <= r < CHAR_SIZE and 0 <= c < CHAR_SIZE:
             explored_map[r][c] = True
        
    def render_level_with_visibility(trail_set, top=3, left=1): # (Cambio 2) Recibe trail_set
        # Renderiza el mapa completo basado en el modo de visión
        sys.stdout.write("\033[?25l") # Ocultar cursor
        for r, row in enumerate(mat):
            line = ""
            for c, cell_val in enumerate(row):
                # Si está en modo jugador y NO explorado, mostrar niebla
                if player_view_mode and not explored_map[r][c]:
                    line += UNSEEN_STR
                else:
                    # Modo programador O celda explorada: dibujar contenido
                    pos = (r,c)
                    if pos == exitp:
                        line += EXIT_STR
                    elif pos in traps_pos:
                        line += TRAP_STR
                    elif pos in comets_pos:
                        line += COMET_STR
                    # (Cambio 3) Dibujar corazones
                    elif pos in hearts_pos:
                        line += HEART_ITEM_STR
                    elif cell_val == WALL:
                        line += WALL_STR
                    elif cell_val == SNOW_WALL:
                        line += SNOW_WALL_STR
                    else: # PATH
                        # (Modificación REVERTIDA) Mostrar rastro en AMBOS modos
                        if pos in trail_set:
                            line += TRAIL_STR
                        else:
                            line += PATH_STR
                
            sys.stdout.write(f"\033[{top + r};{left}H" + line)
        sys.stdout.flush()

    def write_cell_with_visibility(r,c,token, top=3, left=1):
        # Escribe un solo token, respetando la niebla de guerra
        if not player_view_mode or explored_map[r][c]:
            row = top + r
            col = left + c * CELL_W + 1
            sys.stdout.write(f"\033[{row};{col}H" + token)
            sys.stdout.flush()

    def handle_trap_collision(new_pos):
        nonlocal current_lives # Usar nonlocal para modificar la variable de run_level
        if new_pos in traps_pos:
            current_lives -= 1
            traps_pos.remove(new_pos) # La trampa se activa y desaparece
            render_hud()
            if current_lives <= 0:
                sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1H¡Has agotado tu vida! Fin del juego.\n")
                return True # Game Over
        return False

    def handle_collectible_collision(new_pos):
        nonlocal comet_count
        if new_pos in comets_pos:
            comet_count += 1
            comets_pos.remove(new_pos) # Cometa recogido
            render_hud()

    # (Cambio 3) Nueva función para manejar colisión con corazones
    def handle_heart_collision(new_pos):
        nonlocal current_lives
        if new_pos in hearts_pos:
            if current_lives < MAX_LIVES:
                current_lives += 1
                hearts_pos.remove(new_pos) # Corazón recogido
                render_hud()
            # Si la vida está llena, no pasa nada, el corazón no se recoge

    # --- 3. Renderizado Inicial y Configuración de Terminal ---
    
    update_visibility(player[0], player[1]) # Revelar área inicial
    render_hud()
    render_level_with_visibility(trail_set, top=3, left=1) # (Cambio 2)
    write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)

    if os.name != "nt":
        fd = sys.stdin.fileno(); old_settings = termios.tcgetattr(fd); old_flags = fcntl.fcntl(fd, fcntl.F_GETFL)
        tty.setcbreak(fd); fcntl.fcntl(fd, fcntl.F_SETFL, old_flags | os.O_NONBLOCK)
    
    ai = None; ai_active = False; last_ai = 0
    
    # --- 4. Bucle del Juego (para este nivel) ---
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
                
                # 'Q' (Salir)
                if c == 'q': return False # Salir del nivel (derrota)
                
                # 'B' (Activar IA)
                if c == 'b' and not ai_active:
                    ai = ExplorerAI(player, exitp, mat); ai_active = True; last_ai = time.time()
                
                # 'P' (Alternar Visión)
                if c == 'p':
                    player_view_mode = not player_view_mode
                    mode_msg = "Programador" if not player_view_mode else "Jugador"
                    sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1HModo de visión: {mode_msg}." + " "*20 + "\n"); sys.stdout.flush()
                    # Re-renderizar todo
                    render_level_with_visibility(trail_set, top=3, left=1) # (Cambio 2)
                    write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)

                # 'E' (Romper Pared)
                if not ai_active and c == 'e':
                    if comet_count > 0:
                        broken = False
                        for di, dj in [(-1,0), (1,0), (0,-1), (0,1)]: # Adyacentes
                            ti, tj = player[0]+di, player[1]+dj
                            if 0 <= ti < CHAR_SIZE and 0 <= tj < CHAR_SIZE and mat[ti][tj] == SNOW_WALL:
                                mat[ti][tj] = PATH # Romperla
                                snow_walls_pos.remove((ti,tj))
                                comet_count -= 1
                                render_hud()
                                write_cell_with_visibility(ti, tj, PATH_STR, top=3, left=1)
                                sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1HPared de nieve rota. 💥" + " "*20 + "\n"); sys.stdout.flush()
                                broken = True
                                break # Romper solo una
                        if not broken:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1HNo hay pared de nieve adyacente." + " "*20 + "\n"); sys.stdout.flush()
                    else:
                        sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1HNecesitas un ☄️ para romper la pared." + " "*20 + "\n"); sys.stdout.flush()

                # 'W,A,S,D' (Movimiento Manual)
                if not ai_active and c in ('w','a','s','d'):
                    di = dj = 0
                    if c == 'w': di = -1
                    if c == 's': di = 1
                    if c == 'a': dj = -1
                    if c == 'd': dj = 1
                    new = (player[0]+di, player[1]+dj)
                    
                    if can_move(mat, new):
                        prev = player; player = new
                        trail_set.add(prev) # (Cambio 2) Añadir a rastro (SOLO si se mueve)
                        update_visibility(player[0], player[1]) # (Cambio 1) Revelar nueva celda
                        
                        # Manejar colisiones
                        if handle_trap_collision(player): return False # Game Over
                        handle_collectible_collision(player)
                        handle_heart_collision(player) # (Cambio 3)

                        # Re-dibujar
                        if player_view_mode:
                            render_level_with_visibility(trail_set, top=3, left=1) # (Cambio 2)
                        else:
                            # (Cambio 2) Dibuja el rastro
                            write_cell_with_visibility(prev[0], prev[1], TRAIL_STR, top=3, left=1)
                        
                        # (Cambio 2) Dibujar jugador siempre al final
                        write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)
                        
                        # Comprobar victoria
                        if player == exitp:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1H¡Nivel {level} completado!\n"); sys.stdout.flush()
                            return True # Nivel ganado
                    
                    else:
                        # (Cambio 1) Chocar con un muro lo revela
                        # (Aquí NO se añade a trail_set, lo cual es correcto)
                        ni, nj = new
                        if 0 <= ni < CHAR_SIZE and 0 <= nj < CHAR_SIZE and mat[ni][nj] != PATH:
                            explored_map[ni][nj] = True
                            # Repintar todo para mostrar el muro revelado
                            render_level_with_visibility(trail_set, top=3, left=1) # (Cambio 2)
                            write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)
            
            # Movimiento de IA
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
                        trail_set.add(prev) # (Cambio 2)
                        update_visibility(player[0], player[1]) # (Cambio 1)
                        
                        if handle_trap_collision(player): return False # Game Over
                        handle_collectible_collision(player)
                        handle_heart_collision(player) # (Cambio 3)

                        if player_view_mode:
                            render_level_with_visibility(trail_set, top=3, left=1) # (Cambio 2)
                        else:
                            # (Cambio 2) Dibuja el rastro
                            write_cell_with_visibility(prev[0], prev[1], TRAIL_STR, top=3, left=1)
                        
                        # (Cambio 2) Dibujar jugador siempre al final
                        write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)
                        
                        if player == exitp:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1HLa IA encontró la salida. Nivel {level} completado.\n"); sys.stdout.flush()
                            return True # Nivel ganado
            
            time.sleep(0.01)
            
    finally:
        # --- 5. Limpieza de Terminal ---
        sys.stdout.write("\033[?25h"); sys.stdout.flush() # Mostrar cursor
        if os.name != "nt":
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
            fcntl.fcntl(fd, fcntl.F_SETFL, old_flags)

# --- NUEVO BUCLE PRINCIPAL (Controlador de Niveles) ---

def main():
    MAX_LEVELS = 3
    level = 1
    
    # Limpiar pantalla al inicio
    sys.stdout.write("\033[2J\033[H") 
    sys.stdout.flush()
    
    while level <= MAX_LEVELS:
        # Limpiar mensajes de nivel anterior
        sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1H" + " " * 50)
        sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1H" + " " * 50)
        sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1H" + " " * 50)
        
        sys.stdout.write(f"\033[{3+CHAR_SIZE+5};1H--- PREPARANDO NIVEL {level} ---"); sys.stdout.flush()
        time.sleep(1.5)
        
        level_success = run_level(level)
        
        if level_success:
            if level == MAX_LEVELS:
                sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1H🎉 ¡FELICIDADES! ¡Has completado los {MAX_LEVELS} niveles! 🎉\n"); sys.stdout.flush()
                break
            else:
                level += 1
                time.sleep(1) # Pausa antes de cargar el siguiente
        else:
            sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1H💀 ¡GAME OVER! 💀\n"); sys.stdout.flush()
            break

if __name__ == "__main__":
    main()