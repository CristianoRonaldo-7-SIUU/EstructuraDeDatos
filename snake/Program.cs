using System;
using System.Linq;

namespace SnakeGame {
    class Program {
        static void Main(string[] args) {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            while (true) {
                Console.Clear();
                Console.CursorVisible = true;
                Console.WriteLine("SNAKE - Menú");
                Console.WriteLine("1) Modo por Niveles");
                Console.WriteLine("2) Modo Libre");
                Console.WriteLine("3) Ver Top Scores");
                Console.WriteLine("0) Salir");
                Console.Write("Opción: ");
                var k = Console.ReadKey(true).Key;
                if (k == ConsoleKey.D0 || k == ConsoleKey.NumPad0) break;
                if (k == ConsoleKey.D1 || k == ConsoleKey.NumPad1) {
                    Console.Clear();
                    Console.Write("Nombre (para scoreboard): ");
                    string? inputName = Console.ReadLine();
                    string name = string.IsNullOrWhiteSpace(inputName) ? "Player" : inputName!;
                    var levelMode = new LevelMode(name);
                    levelMode.Run();
                } else if (k == ConsoleKey.D2 || k == ConsoleKey.NumPad2) {
                    Console.Clear();
                    Console.Write("Velocidad movimientos por segundo (ej. 6): ");
                    string? s = Console.ReadLine();
                    if (!double.TryParse(s, out double v)) v = Config.InitialMovesPerSecond;
                    var free = new FreeMode(v);
                    free.Run();
                } else if (k == ConsoleKey.D3 || k == ConsoleKey.NumPad3) {
                    Console.Clear();
                    var list = ScoreStore.Load();
                    var ordered = list.OrderByDescending(s => s.Level).ThenBy(s => s.TimeSeconds).Take(20);
                    Console.WriteLine("Top Scores:");
                    Console.WriteLine("Name\tLvl\tTime(s)\tApples\tTraps\tDate");
                    foreach (var sc in ordered) Console.WriteLine($"{sc.Name}\t{sc.Level}\t{sc.TimeSeconds:F1}\t{sc.ApplesEaten}\t{sc.TrapsHit}\t{sc.Date}");
                    Console.WriteLine("Presione ENTER...");
                    Console.ReadLine();
                }
            }
        }
    }
}

