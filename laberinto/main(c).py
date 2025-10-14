# laberinto_con_elementos.py
import random
import time
import sys
from collections import deque

# ---------------- CONFIG ----------------
N = 25
STEP_DELAY = 0.1
DEBUG_MODE = False        # True para GDB Online / entornos que no limpian pantalla
FALSE_POINTS_COUNT = 10   # número de puntos falsos que se generarán
CLEAR_SEQ = "\033[H\033[J"

# ---------------- MAPA DE CAPAS (PDF) ----------------
layer_map = [
[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
[1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1],
[1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1],
[1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,1,1],
[1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,6,6,7,6,6,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,5,5,5,5,5,5,5,5,5,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,2,2,1,1],
[1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,1,1],
[1,1,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,1,1],
[1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1],
[1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1],
[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
]

# ---------------- estructuras ----------------
walls = [[1 for _ in range(N)] for _ in range(N)]      # 1 = muro, 0 = camino
objects = [[None for _ in range(N)] for _ in range(N)]  # overlay para J, TP, llaves, etc.

OBJ_SYMBOLS = {
    "player": "J",
    "tp": "🪞",
    "special_key": "️ ",
    "key": " ",
    "potion": " ",
    "trap": " ",
    "door": " ",
    "chest": " ",
    "false": " "
}

# ---------------- utilidades ----------------
def clear_screen():
    if DEBUG_MODE:
        return
    sys.stdout.write(CLEAR_SEQ)
    sys.stdout.flush()

def sleep_step():
    if DEBUG_MODE:
        return
    time.sleep(STEP_DELAY)

def print_map():
    clear_screen()
    for y in range(N):
        row = []
        for x in range(N):
            o = objects[y][x]
            if o:
                row.append(OBJ_SYMBOLS.get(o, "?"))
            else:
                row.append("." if walls[y][x] == 0 else "#")
        print(" ".join(row))
    if DEBUG_MODE:
        print()
    sys.stdout.flush()

def neighbors(x, y):
    for dx, dy in ((1,0),(-1,0),(0,1),(0,-1)):
        nx, ny = x+dx, y+dy
        if 0 <= nx < N and 0 <= ny < N:
            yield nx, ny

def coords_in_layer(layer_num):
    pts = []
    for y in range(N):
        for x in range(N):
            if layer_map[y][x] == layer_num:
                pts.append((x,y))
    return pts

def not_adjacent_8(pos, other):
    return max(abs(pos[0]-other[0]), abs(pos[1]-other[1])) > 1

def current_open_set():
    s = set()
    for y in range(N):
        for x in range(N):
            if walls[y][x] == 0:
                s.add((x,y))
    return s

def contains_2x2(open_set):
    for y in range(N-1):
        for x in range(N-1):
            if (x,y) in open_set and (x+1,y) in open_set and (x,y+1) in open_set and (x+1,y+1) in open_set:
                return True
    return False

# ---------------- PATHFINDERS ----------------
def bfs_shortest(start, goal):
    if start == goal:
        return [start]
    q = deque([start])
    parent = {start: None}
    while q:
        cur = q.popleft()
        if cur == goal:
            break
        cx, cy = cur
        for nx, ny in neighbors(cx, cy):
            if (nx, ny) not in parent:
                parent[(nx, ny)] = cur
                q.append((nx, ny))
    if goal not in parent:
        return []
    path = []
    cur = goal
    while cur is not None:
        path.append(cur)
        cur = parent[cur]
    path.reverse()
    return path

# BFS que evita crear 2x2 comparando la unión con abiertos actuales
def bfs_no_2x2(start, goal):
    if start == goal:
        return [start]
    open_now = current_open_set()
    q = deque([start])
    parent = {start: None}
    visited = {start}
    while q:
        cur = q.popleft()
        if cur == goal:
            break
        cx, cy = cur
        for nx, ny in neighbors(cx, cy):
            if (nx, ny) in visited:
                continue
            # construir path tentativa
            path_to_cur = []
            p = cur
            while p is not None:
                path_to_cur.append(p)
                p = parent[p]
            path_to_cur.reverse()
            tentative = path_to_cur + [(nx, ny)]
            union = set(open_now) | set(tentative)
            if contains_2x2(union):
                continue
            parent[(nx, ny)] = cur
            visited.add((nx, ny))
            q.append((nx, ny))
    if goal not in parent:
        return []
    path = []
    cur = goal
    while cur is not None:
        path.append(cur)
        cur = parent[cur]
    path.reverse()
    return path

# ---------------- TALLADO SEGURO ----------------
def carve_cell(x, y):
    if walls[y][x] == 0:
        print_map()
        sleep_step()
        return True
    union = current_open_set() | {(x,y)}
    if contains_2x2(union):
        return False
    walls[y][x] = 0
    print_map()
    sleep_step()
    return True

# ---------------- CONEXIONES DENTRO DE CAPA (alinear, pivotes y detour) ----------------
def connect_strict_same_layer(a, b, layer_num):
    ax, ay = a; bx, by = b
    def attempt(order):
        path = []
        cx, cy = ax, ay
        if order == "hv":
            step = 1 if bx > cx else -1
            for x in range(cx + step, bx + step, step):
                if layer_map[ay][x] != layer_num:
                    return None
                path.append((x, ay))
            stepv = 1 if by > ay else -1
            for y in range(ay + stepv, by + stepv, stepv):
                if layer_map[y][bx] != layer_num:
                    return None
                path.append((bx, y))
        else:
            stepv = 1 if by > cy else -1
            for y in range(cy + stepv, by + stepv, stepv):
                if layer_map[y][ax] != layer_num:
                    return None
                path.append((ax, y))
            step = 1 if bx > ax else -1
            for x in range(ax + step, bx + step, step):
                if layer_map[by][x] != layer_num:
                    return None
                path.append((x, by))
        return [(ax, ay)] + path

    p1 = attempt("hv")
    if p1 is not None and not contains_2x2(current_open_set() | set(p1)):
        return p1
    p2 = attempt("vh")
    if p2 is not None and not contains_2x2(current_open_set() | set(p2)):
        return p2
    return None

# intenta conectar dentro de la misma capa con pivote(s)
def connect_with_pivots(a, b, layer_num, attempts=30):
    # si la conexión estricta falla, pruebo pivotes aleatorios dentro de la capa:
    candidates = coords_in_layer(layer_num)
    if not candidates:
        return None
    for _ in range(attempts):
        pivot = random.choice(candidates)
        if pivot == a or pivot == b:
            continue
        p1 = connect_strict_same_layer(a, pivot, layer_num)
        if not p1:
            continue
        p2 = connect_strict_same_layer(pivot, b, layer_num)
        if not p2:
            continue
        union = set(p1) | set(p2)
        if contains_2x2(current_open_set() | union):
            continue
        # retorno concatenado (evitar repetir pivot doble)
        return p1 + p2[1:]
    return None

# ---------------- VALIDACIÓN DE ESPACIO 3x3 ----------------
def area_3x3_free(center):
    cx, cy = center
    for y in range(cy-1, cy+2):
        for x in range(cx-1, cx+2):
            if not (0 <= x < N and 0 <= y < N):
                return False
            if objects[y][x] is not None:
                return False
    return True

# ---------------- GENERACIÓN DE PUNTOS POR CAPA ----------------
def generate_points_per_layer(player_pos, tp_pos):
    extras_probs = [0.5, 1/3, 1/6, 1/12, 1/24]
    capas = {i: coords_in_layer(i)[:] for i in range(1,8)}
    usados = set([player_pos, tp_pos])
    puntos_por_capa = {}
    for capa, candidatos in capas.items():
        candidatos = [c for c in candidatos if c not in usados]
        if not candidatos:
            continue
        elegido = random.choice(candidatos)
        puntos = [elegido]
        usados.add(elegido)
        for p in extras_probs:
            if len(puntos) >= 5:
                break
            if random.random() < p:
                disponibles = [c for c in candidatos if c not in usados]
                if not disponibles:
                    break
                e = random.choice(disponibles)
                puntos.append(e)
                usados.add(e)
            else:
                break
        puntos_por_capa[capa] = puntos
    return puntos_por_capa

# ---------------- FUNCIONES PARA COLOCAR ELEMENTOS ----------------
def place_object_if_area_free(pos, name):
    if area_3x3_free(pos):
        x,y = pos
        objects[y][x] = name
        return True
    return False

# coloca TP obligando a 3-4 capas de distancia si es posible
def place_tp(player_pos):
    px, py = player_pos
    player_layer = layer_map[py][px]
    candidatos = []
    for y in range(N):
        for x in range(N):
            if objects[y][x] is None and not_adjacent_8((x,y), player_pos):
                layer_diff = abs(layer_map[y][x] - player_layer)
                if layer_diff in (3,4):
                    candidatos.append((x,y))
    if not candidatos:
        for y in range(N):
            for x in range(N):
                if objects[y][x] is None and not_adjacent_8((x,y), player_pos):
                    if abs(layer_map[y][x] - player_layer) >= 3:
                        candidatos.append((x,y))
    if not candidatos:
        for y in range(N):
            for x in range(N):
                if objects[y][x] is None and not_adjacent_8((x,y), player_pos):
                    candidatos.append((x,y))
    tp = random.choice(candidatos)
    placed = place_object_if_area_free(tp, "tp")
    if not placed:
        # buscar alternativa no adyacente que tenga area libre
        alts = [c for c in candidatos if area_3x3_free(c)]
        if alts:
            tp = random.choice(alts)
            place_object_if_area_free(tp, "tp")
        else:
            # forzar colocar (poco probable)
            x,y = tp
            objects[y][x] = "tp"
    print_map()
    print(f"TP colocado en {tp} (capa {layer_map[tp[1]][tp[0]]})")
    return tp

def place_player():
    while True:
        jx, jy = random.randint(0,N-1), random.randint(0,N-1)
        if objects[jy][jx] is None:
            if place_object_if_area_free((jx,jy), "player"):
                walls[jy][jx] = 0
                print_map()
                print(f"Jugador en {(jx,jy)} (capa {layer_map[jy][jx]})")
                return (jx,jy)

# generar puntos falsos: para cada uno creamos un punto central y trazamos un camino similar al TP (pero no conectado a la secuencia)
def generate_false_points(count, player_pos):
    falsos = []
    candidatos = [(x,y) for y in range(N) for x in range(N) if objects[y][x] is None and not_adjacent_8((x,y), player_pos)]
    random.shuffle(candidatos)
    for c in candidatos:
        if len(falsos) >= count:
            break
        if not area_3x3_free(c):
            continue
        # elegimos un destino aleatorio (en otra capa preferiblemente)
        dest_candidates = [(x2,y2) for y2 in range(N) for x2 in range(N) if (x2,y2) != c]
        if not dest_candidates:
            continue
        dest = random.choice(dest_candidates)
        # intentar trazar un camino seguro entre c y dest
        path = bfs_no_2x2(c, dest)
        if not path:
            path = bfs_shortest(c, dest)
        # tallar el camino
        carved_any = False
        for (x,y) in path:
            if carve_cell(x,y):
                carved_any = True
        # marcar la cabecera del falso si area libre
        if place_object_if_area_free(c, "false"):
            falsos.append(c)
            print(f"Punto falso creado en {c} (path len {len(path)})")
    return falsos

# place special key same logic as TP (and mark)
def place_special_key_like_tp(player_pos):
    candidatos = [(x,y) for y in range(N) for x in range(N) if objects[y][x] is None and not_adjacent_8((x,y), player_pos)]
    random.shuffle(candidatos)
    for c in candidatos:
        if area_3x3_free(c):
            objects[c[1]][c[0]] = "special_key"
            print_map()
            print(f"Llave especial colocada en {c}")
            return c
    # fallback
    c = candidatos[0]
    objects[c[1]][c[0]] = "special_key"
    print_map()
    print(f"Llave especial forzada en {c}")
    return c

# generar llaves y pociones (5 cada una) según la regla: 50% colocar en camino, 50% crear camino de 5 celdas
def place_keys_and_potions(n_keys=5, n_potions=5):
    placed_keys = []
    placed_potions = []
    # helper: find path cell free of object
    def find_path_cell_free():
        path_cells = [(x,y) for y in range(N) for x in range(N) if walls[y][x] == 0 and objects[y][x] is None]
        return random.choice(path_cells) if path_cells else None

    def create_small_path_and_place(layer_choice=None):
        # elegimos un punto de inicio en la capa indicada (o cualquiera)
        layers = list(range(1,8)) if layer_choice is None else [layer_choice]
        random.shuffle(layers)
        for layer in layers:
            candidates = coords_in_layer(layer)
            random.shuffle(candidates)
            for start in candidates:
                # intentar trazar segmento recto de longitud 5 (hv o vh) sin salir de capa (intentos con pivote si no)
                ax, ay = start
                # probar 4 direcciones (horizontal + vertical combos)
                directions = [(1,0),(-1,0),(0,1),(0,-1)]
                for dx,dy in directions:
                    path = []
                    cx, cy = ax, ay
                    ok = True
                    for step in range(1,6):  # 5 celdas
                        nx, ny = ax + dx*step, ay + dy*step
                        if not (0 <= nx < N and 0 <= ny < N):
                            ok = False; break
                        if layer_map[ny][nx] != layer:
                            ok = False; break
                        path.append((nx, ny))
                    if not ok:
                        continue
                    # verificar 2x2 al unir
                    if contains_2x2(current_open_set() | set(path)):
                        continue
                    # tallar y devolver posición final
                    for (x,y) in path:
                        carve_cell(x,y)
                    end = path[-1] if path else start
                    return end
        return None

    # colocar keys
    for _ in range(n_keys):
        if random.random() < 0.5:
            pos = find_path_cell_free()
            if pos and place_object_if_area_free(pos, "key"):
                placed_keys.append(pos)
                continue
        # crear camino propio de 5 celdas
        end = create_small_path_and_place()
        if end and place_object_if_area_free(end, "key"):
            placed_keys.append(end)
            # posibilidad de extender al siguiente/previo layer ~200/3% (~66.6%)
            if random.random() < (200.0/3.0)/100.0:
                # escoger direction ±1 de capa
                layer_dir = random.choice([-1,1])
                new_layer = layer_map[end[1]][end[0]] + layer_dir
                if 1 <= new_layer <= 7:
                    # intentar otro camino de 5 en la nueva capa empezando desde end projection into that layer
                    ext = create_small_path_and_place(layer_choice=new_layer)
                    if ext:
                        # colocar llave en el extremo ext
                        if place_object_if_area_free(ext, "key"):
                            placed_keys.append(ext)
        else:
            # fallback: intentar colocar en cualquier camino libre
            pos = find_path_cell_free()
            if pos and place_object_if_area_free(pos, "key"):
                placed_keys.append(pos)

    # colocar potions (idéntico)
    for _ in range(n_potions):
        if random.random() < 0.5:
            pos = find_path_cell_free()
            if pos and place_object_if_area_free(pos, "potion"):
                placed_potions.append(pos)
                continue
        end = create_small_path_and_place()
        if end and place_object_if_area_free(end, "potion"):
            placed_potions.append(end)
            if random.random() < (200.0/3.0)/100.0:
                layer_dir = random.choice([-1,1])
                new_layer = layer_map[end[1]][end[0]] + layer_dir
                if 1 <= new_layer <= 7:
                    ext = create_small_path_and_place(layer_choice=new_layer)
                    if ext and place_object_if_area_free(ext, "potion"):
                        placed_potions.append(ext)
        else:
            pos = find_path_cell_free()
            if pos and place_object_if_area_free(pos, "potion"):
                placed_potions.append(pos)

    print(f"Llaves colocadas: {placed_keys}")
    print(f"Pociones colocadas: {placed_potions}")
    return placed_keys, placed_potions

# colocar trampas (5) en caminos existentes
def place_traps(n=5, player_pos=None):
    traps = []
    candidates = [(x,y) for y in range(N) for x in range(N) if walls[y][x] == 0 and objects[y][x] is None]
    random.shuffle(candidates)
    for c in candidates:
        if len(traps) >= n:
            break
        if player_pos and not not_adjacent_8(c, player_pos):
            continue
        if place_object_if_area_free(c, "trap"):
            traps.append(c)
    print(f"Trampas colocadas: {traps}")
    return traps

# colocar puertas: una bloqueando el TP y dos aleatorias que 'cierren caminos'
def place_doors(n=3, tp_pos=None):
    doors = []
    # 1) puerta bloqueando el TP: buscamos vecino en camino del tp
    if tp_pos:
        for nx,ny in neighbors(*tp_pos):
            if walls[ny][nx] == 0 and objects[ny][nx] is None:
                if place_object_if_area_free((nx,ny), "door"):
                    doors.append((nx,ny))
                    break
    # 2) el resto puertas en celdas de camino aleatorias que no colisionen
    candidates = [(x,y) for y in range(N) for x in range(N) if walls[y][x]==0 and objects[y][x] is None]
    random.shuffle(candidates)
    for c in candidates:
        if len(doors) >= n:
            break
        if place_object_if_area_free(c, "door"):
            doors.append(c)
    print(f"Puertas colocadas: {doors}")
    return doors

# colocar cofres (un cofre central)
def place_chest_center():
    center = (N//2, N//2)
    if place_object_if_area_free(center, "chest"):
        print("Cofre colocado en el centro.")
        return center
    else:
        # forzar posicion central si no libre
        x,y = center
        objects[y][x] = "chest"
        print("Cofre forzado en el centro.")
        return center

# ---------------- TRAZADO PRINCIPAL (usa reglas ya implementadas) ----------------
def trace_sequence_with_rules(start_pos, sequence, tp_pos):
    current = start_pos
    for target in sequence:
        if current == target:
            continue
        cx, cy = current
        tx, ty = target
        current_layer = layer_map[cy][cx]
        target_layer = layer_map[ty][tx]

        if current_layer == target_layer:
            # intentar conexión estricta
            path = connect_strict_same_layer(current, target, current_layer)
            if path:
                for (x,y) in path[1:]:
                    ok = carve_cell(x,y)
                    if not ok:
                        # intento pivote/detour
                        alt = connect_with_pivots(current, target, current_layer, attempts=50)
                        if alt:
                            for (ax,ay) in alt[1:]:
                                carve_cell(ax,ay)
                            break
                        # intentar bfs_no_2x2
                        path_safe = bfs_no_2x2(current, target)
                        if path_safe:
                            for (ax,ay) in path_safe[1:]:
                                carve_cell(ax,ay)
                            break
                        # fallback forzado
                        fb = bfs_shortest(current, target)
                        for (ax,ay) in fb[1:]:
                            if not carve_cell(ax,ay):
                                walls[ay][ax] = 0
                                print_map()
                                sleep_step()
                        break
                current = target
                continue
            # intentar pivotes/detour directo
            pivot_path = connect_with_pivots(current, target, current_layer, attempts=80)
            if pivot_path:
                for (x,y) in pivot_path[1:]:
                    carve_cell(x,y)
                current = target
                continue
            # si no fue posible -> intentar BFS seguro
        # diferente capa o no se pudo conectar en la misma -> BFS seguro
        path_safe = bfs_no_2x2(current, target)
        if path_safe:
            for (x,y) in path_safe[1:]:
                if not carve_cell(x,y):
                    # si no se pudo tallar por 2x2 -> intentar fallback local
                    fb = bfs_shortest(current, target)
                    for (ax,ay) in fb[1:]:
                        if walls[ay][ax] == 1:
                            walls[ay][ax] = 0
                            print_map()
                            sleep_step()
                    break
            current = target
            continue
        # fallback final
        fb = bfs_shortest(current, target)
        if fb:
            for (x,y) in fb[1:]:
                if not carve_cell(x,y):
                    walls[y][x] = 0
                    print_map()
                    sleep_step()
        current = target

# ---------------- FLUJO PRINCIPAL ----------------
def main():
    print_map()
    if not DEBUG_MODE:
        time.sleep(1)

    # colocar jugador
    jpos = place_player()

    # colocar TP (3-4 capas lejos preferible)
    tpos = place_tp(jpos)

    # generar puntos por capa (para la ruta principal)
    puntos_por_capa = generate_points_per_layer(jpos, tpos)
    print("Puntos por capa (resumen):")
    for c in sorted(puntos_por_capa.keys()):
        print(f" Capa {c}: {puntos_por_capa[c]}")

    # construir secuencia excluyendo capa del TP, que va al final
    tp_layer = layer_map[tpos[1]][tpos[0]]
    capas_disponibles = [c for c in puntos_por_capa.keys() if c != tp_layer]
    random.shuffle(capas_disponibles)
    sequence = []
    for capa in capas_disponibles:
        pts = puntos_por_capa[capa][:]
        random.shuffle(pts)
        sequence.extend(pts)
    sequence.append(tpos)

    print("Orden (J -> ... -> TP):")
    print(sequence)
    input("Pulsa ENTER para trazar la secuencia principal (aplicando reglas).")
    trace_sequence_with_rules(jpos, sequence, tpos)
    print("Secuencia principal trazada.")

    # generar puntos falsos (configurable)
    input("Pulsa ENTER para generar puntos falsos.")
    falsos = generate_false_points(FALSE_POINTS_COUNT, jpos)
    print(f"Puntos falsos generados: {falsos}")

    # colocar llave especial (misma lógica que TP)
    input("Pulsa ENTER para colocar la llave especial.")
    special_key_pos = place_special_key_like_tp(jpos)

    # colocar cofre central
    input("Pulsa ENTER para colocar el cofre central.")
    chest_pos = place_chest_center()

    # crear llaves y pociones
    input("Pulsa ENTER para generar 5 llaves y 5 pociones.")
    keys, potions = place_keys_and_potions(5,5)

    # colocar 5 trampas
    input("Pulsa ENTER para colocar 5 trampas.")
    traps = place_traps(5, player_pos=jpos)

    # colocar 3 puertas (una bloqueando TP)
    input("Pulsa ENTER para colocar 3 puertas.")
    doors = place_doors(3, tp_pos=tpos)

    print("\nGeneración finalizada. Resumen de objetos:")
    summary = {}
    for y in range(N):
        for x in range(N):
            o = objects[y][x]
            if o:
                summary.setdefault(o, []).append((x,y))
    for k,v in summary.items():
        print(f" {k}: {v}")

    input("\nPulsa ENTER para terminar.")
    print_map()

if __name__ == "__main__":
    main()
