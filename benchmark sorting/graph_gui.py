import sys
import json
import threading
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk

# --- Configuración de la Ventana y Gráfica ---
root = tk.Tk()
root.title("Gráfica de Benchmark")

fig, ax = plt.subplots(figsize=(10, 6))
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack(fill=tk.BOTH, expand=1)

root.configure(bg='#2e2e2e')
fig.patch.set_facecolor('#2e2e2e')
ax.set_facecolor('#3e3e3e')
ax.tick_params(colors='white')
ax.xaxis.label.set_color('white')
ax.yaxis.label.set_color('white')
ax.title.set_color('white')
for spine in ax.spines.values():
    spine.set_edgecolor('white')

def update_graph(data):
    """
    Limpia y redibuja la gráfica de Matplotlib con los nuevos datos.
    """
    try:
        labels = data.get('labels', [])
        dataset = data.get('datasets', [{}])[0]
        
        plot_data = dataset.get('data', [])
        title = dataset.get('label', 'Gráfica de Benchmark')
        colors = dataset.get('backgroundColor', '#00FFFF')
        
        # Limpiar gráfica anterior
        ax.clear()

        # Dibujar nueva gráfica
        ax.bar(labels, plot_data, color=colors)
        
        # Usar escala logarítmica si los valores son muy dispares
        ax.set_yscale('log')
        
        ax.set_title(title, color='white')
        ax.tick_params(axis='x', rotation=45, labelsize=9)
        
        # Redibujar
        fig.tight_layout()
        canvas.draw()
    except Exception as e:
        # Imprime errores en la terminal
        print(f"[Python-Error] Error al actualizar gráfica: {e}", file=sys.stderr)

def listen_for_data():
    """
    Escucha en un hilo separado por nuevos datos JSON desde stdin.
    """
    while True:
        try:
            # Lee una línea desde Node.js
            line = sys.stdin.readline()
            if not line:
                break
            
            data = json.loads(line)
            
            root.after(0, update_graph, data)
            
        except json.JSONDecodeError:
            print(f"[Python-Error] No se pudo decodificar JSON.", file=sys.stderr)
        except Exception:
            break

if __name__ == "__main__":
    listener_thread = threading.Thread(target=listen_for_data, daemon=True)
    listener_thread.start()
    root.mainloop()
