import os, sys, time, random
import json

# --- CONSTANTES ---
SAVE_FILE = "savegame.json"

CHAR_SIZE = 25
CELL_W = 2
CELLS_R = (CHAR_SIZE - 1) // 2
CELLS_C = (CHAR_SIZE - 1) // 2

WALL = 1
PATH = 0
SNOW_WALL = 3 
MAX_LIVES = 3

# --- GLOBALES DE ESTADO ---
current_lives = MAX_LIVES
comet_count = 0

def pad(s):
    if len(s) >= CELL_W:
        return s[:CELL_W]
    return s + " " * (CELL_W - len(s))

# Cadenas de Símbolos
WALL_STR     = pad(" M ")
PATH_STR     = pad(" ")
TRAIL_STR    = pad(".")
PLAYER_STR   = pad("P")
EXIT_STR     = pad("S")
TRAP_STR     = pad(" ")
HEART_STR    = "❤"
EMPTY_HEART_STR = " "
COMET_STR    = pad(" ☄ ")
SNOW_WALL_STR = pad(" ❄ ")
UNSEEN_STR   = pad("  ")
HEART_ITEM_STR = pad(" ❤ ")

if os.name == "nt":
    import msvcrt
else:
    import tty, termios, fcntl

# --- LÓGICA DE LABERINTO  ---

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
    if di == -1: a.walls[2]=False; b.walls[0]=False
    elif di == 1: a.walls[0]=False; b.walls[2]=False
    if dj == -1: a.walls[1]=False; b.walls[3]=False
    elif dj == 1: a.walls[3]=False; b.walls[1]=False

def wilson():
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
            if not cell.walls[0]: mat[ci-1][cj] = PATH
            if not cell.walls[1]: mat[ci][cj+1] = PATH
            if not cell.walls[2]: mat[ci+1][cj] = PATH
            if not cell.walls[3]: mat[ci][cj-1] = PATH
    return mat

def can_move(mat, to_cell):
    i,j = to_cell
    if 0 <= i < CHAR_SIZE and 0 <= j < CHAR_SIZE:
        return mat[i][j] == PATH
    return False

# --- IA  ---

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

# --- LÓGICA DE GUARDADO/CARGA JSON ---

def save_game(state):
    """Escribe el diccionario de estado en el archivo SAVE_FILE."""
    try:
        with open(SAVE_FILE, "w") as f:
            # Convertimos sets a listas para que JSON pueda guardarlos
            if 'trail_set' in state:
                state['trail_set'] = list(state['trail_set'])
            
            json.dump(state, f, indent=4)
        return True
    except IOError:
        return False

def load_game():
    """Lee y parsea el archivo SAVE_FILE, retornando un diccionario de estado."""
    try:
        with open(SAVE_FILE, "r") as f:
            state = json.load(f)
            
            # (Importante) Convertimos las estructuras de JSON de nuevo a
            # los tipos que el juego espera (sets y tuplas)
            if 'trail_set' in state:
                # json guarda tuplas (r,c) como listas [r,c]
                # Las convertimos de nuevo a tuplas y luego a un set
                state['trail_set'] = set(tuple(pos) for pos in state['trail_set'])
            
            # Convertir posiciones de listas [r,c] a tuplas (r,c)
            state['player'] = tuple(state['player'])
            state['exitp'] = tuple(state['exitp'])
            state['traps_pos'] = [tuple(p) for p in state['traps_pos']]
            state['comets_pos'] = [tuple(p) for p in state['comets_pos']]
            state['hearts_pos'] = [tuple(p) for p in state['hearts_pos']]
            state['snow_walls_pos'] = [tuple(p) for p in state['snow_walls_pos']]

            return state
    except (FileNotFoundError, json.JSONDecodeError, TypeError):
        return None

# --- LÓGICA DE NIVEL ---

def run_level(level, load_data=None):
    """
    Ejecuta un nivel. Retorna:
    - "WIN": Si el jugador gana el nivel.
    - "GAME_OVER": Si el jugador pierde todas las vidas.
    - "QUIT_SAVE": Si el jugador presiona 'Q' para guardar y salir.
    """
    
    global current_lives, comet_count
    
    # --- 1. Inicialización de Nivel ---
    if load_data is None:
        # Laberinto nuevo
        random.seed(time.time())
        wilson()
        mat = build_matrix_from_cells()
        empties = [(i,j) for i in range(CHAR_SIZE) for j in range(CHAR_SIZE) if mat[i][j]==PATH]
        
        # Estado nuevo (solo resetea si es Nivel 1)
        if level == 1:
            current_lives = MAX_LIVES
            comet_count = 0
            
        player_view_mode = True
        explored_map = [[False for _ in range(CHAR_SIZE)] for __ in range(CHAR_SIZE)]
        trail_set = set()

        # Colocar Objetos
        traps_pos = random.sample(empties, 5)
        for tp in traps_pos: empties.remove(tp)
        comets_pos = random.sample(empties, 4)
        for cp in comets_pos: empties.remove(cp)
        hearts_pos = random.sample(empties, 3)
        for hp in hearts_pos: empties.remove(hp)
        wall_positions = [(i,j) for i in range(1, CHAR_SIZE - 1) 
                            for j in range(1, CHAR_SIZE - 1) if mat[i][j]==WALL]
        snow_walls_pos = random.sample(wall_positions, 5)
        for (i,j) in snow_walls_pos: mat[i][j] = SNOW_WALL
            
        player = random.choice(empties)
        exitp  = random.choice(empties)
        while exitp == player:
            exitp = random.choice(empties)
    
    else:
        # --- 1b. Cargar Estado desde load_data ---
        current_lives = load_data['current_lives']
        comet_count = load_data['comet_count']
        player = load_data['player']
        exitp = load_data['exitp']
        traps_pos = load_data['traps_pos']
        comets_pos = load_data['comets_pos']
        hearts_pos = load_data['hearts_pos']
        snow_walls_pos = load_data['snow_walls_pos']
        explored_map = load_data['explored_map']
        trail_set = load_data['trail_set']
        mat = load_data['mat']
        player_view_mode = True
        
        sys.stdout.write(f"\033[{3+CHAR_SIZE+5};1H--- PARTIDA NIVEL {level} CARGADA ---"); sys.stdout.flush()
        time.sleep(1.5)

    # --- 2. Funciones de Ayuda (Locales al Nivel) ---

    def render_hud():
        life_display = (HEART_STR * current_lives) + (EMPTY_HEART_STR * (MAX_LIVES - current_lives))
        hud_line = f"VIDA: {life_display} | NIVEL: {level}/3 | ☄️: {comet_count}"
        controls_line = "(P: Visión, E: Romper, B: IA, Q: Guardar/Salir)"
        
        sys.stdout.write(f"\033[1;1H" + " " * (CELL_W * CHAR_SIZE + 5))
        sys.stdout.write(f"\033[2;1H" + " " * (CELL_W * CHAR_SIZE + 5))
        sys.stdout.write(f"\033[1;1H{hud_line}")
        sys.stdout.write(f"\033[2;1H{controls_line}")
        sys.stdout.flush()

    def update_visibility(r, c):
        if 0 <= r < CHAR_SIZE and 0 <= c < CHAR_SIZE:
             explored_map[r][c] = True
        
    def render_level_with_visibility(trail_set, top=3, left=1):
        sys.stdout.write("\033[?25l")
        for r, row in enumerate(mat):
            line = ""
            for c, cell_val in enumerate(row):
                if player_view_mode and not explored_map[r][c]:
                    line += UNSEEN_STR
                else:
                    pos = (r,c)
                    if pos == exitp: line += EXIT_STR
                    elif pos in traps_pos: line += TRAP_STR
                    elif pos in comets_pos: line += COMET_STR
                    elif pos in hearts_pos: line += HEART_ITEM_STR
                    elif cell_val == WALL: line += WALL_STR
                    elif cell_val == SNOW_WALL: line += SNOW_WALL_STR
                    else: # PATH
                        if pos in trail_set: line += TRAIL_STR
                        else: line += PATH_STR
            sys.stdout.write(f"\033[{top + r};{left}H" + line)
        sys.stdout.flush()

    def write_cell_with_visibility(r,c,token, top=3, left=1):
        if not player_view_mode or explored_map[r][c]:
            row = top + r
            col = left + c * CELL_W + 1
            sys.stdout.write(f"\033[{row};{col}H" + token)
            sys.stdout.flush()

    def handle_trap_collision(new_pos):
        global current_lives
        if new_pos in traps_pos:
            current_lives -= 1
            traps_pos.remove(new_pos)
            render_hud()
            if current_lives <= 0:
                # (Cambio 2) Retorna "GAME_OVER" en lugar de True
                return "GAME_OVER"
        return "OK" # Retorna "OK" si no hay game over

    def handle_collectible_collision(new_pos):
        global comet_count
        if new_pos in comets_pos:
            comet_count += 1
            comets_pos.remove(new_pos)
            render_hud()

    def handle_heart_collision(new_pos):
        global current_lives
        if new_pos in hearts_pos:
            if current_lives < MAX_LIVES:
                current_lives += 1
                hearts_pos.remove(new_pos)
                render_hud()

    # --- 3. Renderizado Inicial y Configuración de Terminal ---
    
    update_visibility(player[0], player[1])
    render_hud()
    render_level_with_visibility(trail_set, top=3, left=1)
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
                
                # 'Q' (Guardar y Salir)
                if c == 'q':
                    game_state = {
                        'level': level,
                        'current_lives': current_lives,
                        'comet_count': comet_count,
                        'player': player,
                        'exitp': exitp,
                        'traps_pos': traps_pos,
                        'comets_pos': comets_pos,
                        'hearts_pos': hearts_pos,
                        'snow_walls_pos': snow_walls_pos,
                        'trail_set': trail_set, # Se convertirá a lista en save_game
                        'explored_map': explored_map,
                        'mat': mat,
                    }
                    save_game(game_state)
                    
                    # Limpiar mensajes y mostrar confirmación
                    sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1H" + " " * 50)
                    sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1H" + " " * 50)
                    sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1HPartida guardada en {SAVE_FILE}\n")
                    sys.stdout.flush()
                    return "QUIT_SAVE" # Retornar código de salida
                
                # 'B' (Activar IA)
                if c == 'b' and not ai_active:
                    ai = ExplorerAI(player, exitp, mat); ai_active = True; last_ai = time.time()
                
                # 'P' (Alternar Visión)
                if c == 'p':
                    player_view_mode = not player_view_mode
                    mode_msg = "Programador" if not player_view_mode else "Jugador"
                    sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1HModo de visión: {mode_msg}." + " "*20 + "\n"); sys.stdout.flush()
                    render_level_with_visibility(trail_set, top=3, left=1)
                    write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)

                # 'E' (Romper Pared)
                if not ai_active and c == 'e':
                    if comet_count > 0:
                        broken = False
                        for di, dj in [(-1,0), (1,0), (0,-1), (0,1)]:
                            ti, tj = player[0]+di, player[1]+dj
                            if 0 <= ti < CHAR_SIZE and 0 <= tj < CHAR_SIZE and mat[ti][tj] == SNOW_WALL:
                                mat[ti][tj] = PATH
                                snow_walls_pos.remove((ti,tj))
                                comet_count -= 1
                                render_hud()
                                write_cell_with_visibility(ti, tj, PATH_STR, top=3, left=1)
                                sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1HPared de nieve rota. 💥" + " "*20 + "\n"); sys.stdout.flush()
                                broken = True
                                break
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
                        trail_set.add(prev)
                        update_visibility(player[0], player[1])
                        
                        # Comprobar código de retorno
                        collision_status = handle_trap_collision(player)
                        if collision_status == "GAME_OVER": return "GAME_OVER"
                        
                        handle_collectible_collision(player)
                        handle_heart_collision(player)

                        if player_view_mode:
                            render_level_with_visibility(trail_set, top=3, left=1)
                        else:
                            write_cell_with_visibility(prev[0], prev[1], TRAIL_STR, top=3, left=1)
                        write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)
                        
                        if player == exitp:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1H¡Nivel {level} completado!\n"); sys.stdout.flush()
                            return "WIN" # Retornar "WIN"
                        
                    else:
                        ni, nj = new
                        if 0 <= ni < CHAR_SIZE and 0 <= nj < CHAR_SIZE and mat[ni][nj] != PATH:
                            explored_map[ni][nj] = True
                            render_level_with_visibility(trail_set, top=3, left=1)
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
                        trail_set.add(prev)
                        update_visibility(player[0], player[1])
                        
                        # Comprobar código de retorno
                        collision_status = handle_trap_collision(player)
                        if collision_status == "GAME_OVER": return "GAME_OVER"
                        
                        handle_collectible_collision(player)
                        handle_heart_collision(player)

                        if player_view_mode:
                            render_level_with_visibility(trail_set, top=3, left=1)
                        else:
                            write_cell_with_visibility(prev[0], prev[1], TRAIL_STR, top=3, left=1)
                        write_cell_with_visibility(player[0], player[1], PLAYER_STR, top=3, left=1)
                        
                        if player == exitp:
                            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1HLa IA encontró la salida. Nivel {level} completado.\n"); sys.stdout.flush()
                            return "WIN" # Retornar "WIN"
            
            time.sleep(0.01)
            
    finally:
        # --- 5. Limpieza de Terminal ---
        sys.stdout.write("\033[?25h"); sys.stdout.flush()
        if os.name != "nt":
            # (Importante) Restaurar la terminal para que input() funcione
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
            fcntl.fcntl(fd, fcntl.F_SETFL, old_flags)

# --- BUCLE PRINCIPAL (Controlador de Niveles y Reinicio) ---

def main():
    global current_lives, comet_count # Acceder a las globales
    MAX_LEVELS = 3
    load_data = None
    game_running = True

    # Limpiar pantalla al inicio
    sys.stdout.write("\033[2J\033[H")  
    sys.stdout.flush()
    
    # --- PROCESO DE CARGA DE PARTIDA ---
    if os.path.exists(SAVE_FILE):
        sys.stdout.write(f"\033[1;1H¡Partida guardada encontrada! ({SAVE_FILE})\n")
        sys.stdout.write("¿Quieres Cargar (C) o Iniciar nueva (N)? (Teclea C o N y Enter): ")
        sys.stdout.flush()
        choice = sys.stdin.readline().strip().lower()
        if choice == 'c':
            load_data = load_game()
            if load_data:
                sys.stdout.write(f"Partida cargada exitosamente. Nivel {load_data['level']}.\n")
            else:
                sys.stdout.write("Error al cargar la partida. Iniciando nueva.\n")
                load_data = None
        else:
            sys.stdout.write("Iniciando nueva partida.\n")
    else:
        sys.stdout.write("Iniciando nueva partida.\n")
        
    time.sleep(1.5)
    
    # --- Bucle de Juego (permite reiniciar) ---
    while game_running:
        
        # Asignar nivel inicial
        if load_data:
            level = load_data['level']
            # Cargar vidas y cometas globales desde el archivo
            current_lives = load_data['current_lives']
            comet_count = load_data['comet_count']
        else:
            level = 1
            # Resetear vidas y cometas globales para una nueva partida
            current_lives = MAX_LIVES
            comet_count = 0

        # --- Bucle de Niveles ---
        while level <= MAX_LEVELS:
            # Limpiar mensajes de nivel anterior
            sys.stdout.write(f"\033[{3+CHAR_SIZE+1};1H" + " " * 50)
            sys.stdout.write(f"\033[{3+CHAR_SIZE+2};1H" + " " * 50)
            sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1H" + " " * 50)
            
            sys.stdout.write(f"\033[{3+CHAR_SIZE+5};1H--- PREPARANDO NIVEL {level} ---"); sys.stdout.flush()
            time.sleep(1.5)
            
            # Pasa load_data al primer nivel; será None en los siguientes
            level_result = run_level(level, load_data)
            
            # Asegurarse de que load_data solo se use una vez
            load_data = None 
            
            # --- Manejar resultado del nivel ---
            
            if level_result == "WIN":
                if level == MAX_LEVELS:
                    sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1H🎉 ¡FELICIDADES! ¡Has completado los {MAX_LEVELS} niveles! 🎉\n"); sys.stdout.flush()
                    game_running = False # Termina el juego
                    break # Salir del bucle de niveles
                else:
                    level += 1
                    time.sleep(1)
            
            elif level_result == "GAME_OVER":
                sys.stdout.write(f"\033[{3+CHAR_SIZE+3};1H💀 ¡GAME OVER! 💀\n")
                sys.stdout.write("¿Desea iniciar una nueva partida? (s/n): "); sys.stdout.flush()
                choice = sys.stdin.readline().strip().lower()
                
                if choice == 's':
                    # Borrar el archivo de guardado para no cargarlo por error
                    if os.path.exists(SAVE_FILE):
                        os.remove(SAVE_FILE)
                    break # Salir del bucle de niveles, el bucle game_running reiniciará
                else:
                    sys.stdout.write("¡ADIOS PERDEDOR!\n"); sys.stdout.flush()
                    game_running = False # Termina el juego
                    break # Salir del bucle de niveles
            
            elif level_result == "QUIT_SAVE":
                game_running = False # Termina el juego
                break # Salir del bucle de niveles

if __name__ == "__main__":
    main()
