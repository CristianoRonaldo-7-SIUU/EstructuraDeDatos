using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Linq;

namespace SnakeGame
{
    public class LevelMode
    {
        readonly int w = Config.Width, h = Config.Height;
        
        LinkedList<Point> snake = new LinkedList<Point>();
        
        List<Point> traps = new List<Point>();
        List<double> trapTimes = new List<double>();
        List<double> trapRepositionSchedule = new List<double>();
        Point? apple = null;
        double appleSpawnAt = 0;
        double moveIntervalMs;
        int level = 1;
        Stopwatch sw = new Stopwatch();
        string appleSym, trapSym;
        string name = string.Empty;

        Queue<Point> dirQueue = new Queue<Point>();
        Point dir = new Point(1, 0);

        int applesEaten = 0;
        int trapsHit = 0;

        public LevelMode(string playerName)
        {
            name = string.IsNullOrWhiteSpace(playerName) ? "Player" : playerName;
            moveIntervalMs = 1000.0 / Config.InitialMovesPerSecond;
            appleSym = Config.UseEmoji ? Config.AppleEmoji : Config.AppleFallback.ToString();
            trapSym = Config.UseEmoji ? Config.TrapEmoji : Config.TrapFallback.ToString();
        }

        Point Wrap(Point p)
        {
            int x = (p.X % w + w) % w;
            int y = (p.Y % h + h) % h;
            return new Point(x, y);
        }

        bool IsFreeCell(Point p)
        {
            if (snake.Contains(p)) return false;
            foreach (var t in traps) if (t.X == p.X && t.Y == p.Y) return false;
            if (apple.HasValue && apple.Value.X == p.X && apple.Value.Y == p.Y) return false;
            return true;
        }

        Point RandomFree()
        {
            for (int i = 0; i < 3000; i++)
            {
                var p = new Point(Config.Rng.Next(w), Config.Rng.Next(h));
                if (IsFreeCell(p)) return p;
            }
            return new Point(0, 0);
        }

        Point RandomFreeFarFromHead(int minDist)
        {
            // LinkedList.First puede ser null si está vacía, pero en juego normal no debería pasar.
            var head = snake.Count > 0 ? snake.First.Value : new Point(0,0);

            for (int it = 0; it < 3000; it++)
            {
                var p = new Point(Config.Rng.Next(w), Config.Rng.Next(h));
                
                bool hitsApple = apple.HasValue && apple.Value.X == p.X && apple.Value.Y == p.Y;
                
                if (!snake.Contains(p) && !hitsApple)
                {
                    double dx = p.X - head.X;
                    double dy = p.Y - head.Y;
                    double dist = Math.Sqrt(dx * dx + dy * dy);
                    if (dist >= minDist) return p;
                }
            }
            // Fallback
            for (int i = 0; i < 3000; i++)
            {
                var p = new Point(Config.Rng.Next(w), Config.Rng.Next(h));
                bool hitsApple = apple.HasValue && apple.Value.X == p.X && apple.Value.Y == p.Y;
                if (!snake.Contains(p) && !hitsApple) return p;
            }
            return new Point(0, 0);
        }

        void SpawnAppleScheduled(double nowSec)
        {
            double extra = applesEaten * 0.05;
            double delay = Config.AppleBaseDelaySeconds + Config.Rng.NextDouble() * extra;
            appleSpawnAt = nowSec + delay;
        }

        void SpawnTrapNow(double nowSec)
        {
            // Esto usa la lógica corregida que evita la manzana
            var p = RandomFreeFarFromHead(8);
            traps.Add(p);
            trapTimes.Add(nowSec);
            trapRepositionSchedule.Add(nowSec + Config.TrapRepositionSeconds);
            DrawCell(p, Config.CellString(trapSym));
            DrawBorders();
        }

        void ScheduleNextTrapRepositionFromIndex(int idx, double nowSec)
        {
            if (traps.Count == 0) return;
            int next = (idx + 1) % traps.Count;
            trapRepositionSchedule[next] = nowSec + 8.0;
        }

        void InitSnakeCenter()
        {
            snake = new LinkedList<Point>();
            int cx = w / 2;
            int cy = h / 2;
            // Insertamos el cuerpo
            for (int i = 0; i < Config.StartLength - 1; i++)
                snake.AddFirst(new Point(cx - i - 1, cy));
            // Insertamos la cabeza al final (AddFirst empuja lo anterior hacia atrás)
            snake.AddFirst(new Point(cx, cy));
            
            dir = new Point(1, 0);
            dirQueue.Clear();
        }

        void InitGame()
        {
            applesEaten = 0;
            trapsHit = 0;
            level = 1;
            moveIntervalMs = 1000.0 / Config.InitialMovesPerSecond;

            InitSnakeCenter();

            traps.Clear();
            trapTimes.Clear();
            trapRepositionSchedule.Clear();
            apple = null;

            sw.Restart();
            double nowSec = sw.Elapsed.TotalSeconds;
            SpawnAppleScheduled(nowSec);

            for (int i = 0; i < Config.InitialTrapCount; i++)
            {
                SpawnTrapNow(nowSec);
            }
        }

        int DrawX(int logicalX) => 1 + logicalX * Config.CellWidth;
        int DrawY(int logicalY) => 2 + logicalY;

        void DrawBorders()
        {
            Console.ForegroundColor = ConsoleColor.White;

            int topRow = DrawY(0) - 1;
            int bottomRow = DrawY(h - 1) + 1;
            int leftCol = 0;
            int rightCol = 1 + w * Config.CellWidth;

            for (int x = leftCol; x <= rightCol; x++)
            {
                try { 
                    Console.SetCursorPosition(x, topRow);
                    Console.Write(x == leftCol || x == rightCol ? Config.BorderCorner : Config.BorderHorizontal);
                    
                    Console.SetCursorPosition(x, bottomRow);
                    Console.Write(x == leftCol || x == rightCol ? Config.BorderCorner : Config.BorderHorizontal);
                } catch { }
            }

            for (int y = 0; y < h; y++)
            {
                try {
                    Console.SetCursorPosition(leftCol, DrawY(y));
                    Console.Write(Config.BorderVertical);
                    Console.SetCursorPosition(rightCol, DrawY(y));
                    Console.Write(Config.BorderVertical);
                } catch { }
            }

            Console.ResetColor();
        }

        void DrawCell(Point p, string cellStr)
        {
            if (p.X < 0 || p.X >= w || p.Y < 0 || p.Y >= h) return;

            if (cellStr == null) cellStr = Config.EmptyCell;
            int sx = DrawX(p.X);
            int sy = DrawY(p.Y);
            try { Console.SetCursorPosition(sx, sy); } catch { return; }

            bool isSnake = cellStr.Contains(Config.HeadChar.ToString()) || cellStr.Contains(Config.BodyChar.ToString());
            bool isApple = cellStr.Contains(Config.AppleFallback.ToString()) || cellStr.Contains(Config.AppleEmoji);
            bool isTrap = cellStr.Contains(Config.TrapFallback.ToString()) || cellStr.Contains(Config.TrapEmoji);

            if (isSnake) Console.ForegroundColor = ConsoleColor.Green;
            else if (isApple) Console.ForegroundColor = ConsoleColor.Red;
            else if (isTrap) Console.ForegroundColor = ConsoleColor.Magenta;
            else Console.ResetColor();

            if (cellStr.Length < Config.CellWidth) cellStr = cellStr.PadRight(Config.CellWidth, Config.EmptyChar);
            if (cellStr.Length > Config.CellWidth) cellStr = cellStr.Substring(0, Config.CellWidth);
            Console.Write(cellStr);
            Console.ResetColor();
        }

        void EraseCell(Point p)
        {
            if (p.X < 0 || p.X >= w || p.Y < 0 || p.Y >= h) return;
            int sx = DrawX(p.X);
            int sy = DrawY(p.Y);
            try { Console.SetCursorPosition(sx, sy); } catch { return; }
            Console.Write(Config.EmptyCell);
        }

        void RenderHeader()
        {
            try {
                Console.SetCursorPosition(0, 0);
                Console.Write(new string(' ', Math.Max(Console.WindowWidth, 60)));
                Console.SetCursorPosition(0, 0);
                Console.Write($"Player: {name}  Level: {level}  Length: {snake.Count}  Time: {sw.Elapsed.TotalSeconds:F1}s");
                Console.Write($"  Apples:{applesEaten}  Traps:{trapsHit}");
            } catch { }
        }

        void RenderFull()
        {
            RenderHeader();
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    Console.SetCursorPosition(DrawX(x), DrawY(y));
                    Console.Write(Config.EmptyCell);
                }
            }
            DrawBorders();

            int i = 0;
            foreach (var p in snake)
            {
                string s = (i == 0) ? Config.HeadChar.ToString() : Config.BodyChar.ToString();
                DrawCell(p, Config.CellString(s));
                i++;
            }

            if (apple.HasValue) DrawCell(apple.Value, Config.CellString(appleSym));

            for (int k = 0; k < traps.Count; k++)
            {
                DrawCell(traps[k], Config.CellString(trapSym));
            }
            
            try { Console.SetCursorPosition(0, DrawY(h - 1) + 2); } catch { }
        }

        public void Run()
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            Console.CursorVisible = false;

            try {
                if (OperatingSystem.IsWindows())
                {
                    int neededHeight = h + 6;
                    if (Console.WindowHeight < neededHeight) Console.WindowHeight = Math.Min(neededHeight, Console.LargestWindowHeight);
                    if (Console.BufferHeight < neededHeight) Console.BufferHeight = Math.Min(neededHeight, short.MaxValue);
                }
            } catch { }

            Console.Clear();
            InitGame();
            RenderFull();

            double lastMove = sw.Elapsed.TotalMilliseconds;
            double lastRender = 0;

            bool running = true;

            while (running)
            {
                double nowMs = sw.Elapsed.TotalMilliseconds;
                double nowSec = sw.Elapsed.TotalSeconds;

                while (Console.KeyAvailable)
                {
                    var key = Console.ReadKey(true).Key;
                    Point newDir = dir;
                    if (key == ConsoleKey.UpArrow || key == ConsoleKey.W) newDir = new Point(0, -1);
                    else if (key == ConsoleKey.DownArrow || key == ConsoleKey.S) newDir = new Point(0, 1);
                    else if (key == ConsoleKey.LeftArrow || key == ConsoleKey.A) newDir = new Point(-1, 0);
                    else if (key == ConsoleKey.RightArrow || key == ConsoleKey.D) newDir = new Point(1, 0);
                    else if (key == ConsoleKey.Escape) { running = false; break; }

                    Point lastQueued = dir;
                    if (dirQueue.Count > 0)
                    {
                        var arrQ = dirQueue.ToArray();
                        lastQueued = arrQ[arrQ.Length - 1];
                    }

                    if (!(newDir.X == -lastQueued.X && newDir.Y == -lastQueued.Y) &&
                        !(newDir.X == lastQueued.X && newDir.Y == lastQueued.Y))
                    {
                        dirQueue.Enqueue(newDir);
                        if (dirQueue.Count > 4) dirQueue.Dequeue();
                    }
                }

                if (nowMs - lastMove >= moveIntervalMs)
                {
                    if (dirQueue.Count > 0) dir = dirQueue.Dequeue();
                    
                    var head = snake.First.Value;

                    int rawX = head.X + dir.X;
                    int rawY = head.Y + dir.Y;

                    int newX = (rawX % w + w) % w;
                    int newY = (rawY % h + h) % h;

                    var newHead = new Point(newX, newY);

                    // Verifica colisión con el cuerpo
                    if (snake.Contains(newHead))
                    {
                        running = false;
                        break;
                    }

                    snake.AddFirst(newHead);
                    var prevHead = head;
                    bool grew = false;

                    if (apple.HasValue && apple.Value.X == newHead.X && apple.Value.Y == newHead.Y)
                    {
                        grew = true;
                        apple = null;
                        applesEaten++;
                        SpawnTrapNow(nowSec);
                        SpawnAppleScheduled(nowSec);
                    }

                    Point? erasedTail = null;
                    if (!grew)
                    {
                        erasedTail = snake.Last.Value;
                        snake.RemoveLast();
                    }

                    bool hadTrapCollision = false;
                    for (int i = 0; i < traps.Count; i++)
                    {
                        var t = traps[i];
                        if (t.X == newHead.X && t.Y == newHead.Y)
                        {
                            trapsHit++;
                            
                            if (snake.Count > 0)
                            {
                                Point removed = snake.Last.Value;
                                snake.RemoveLast();
                                EraseCell(removed);
                            }

                            if (snake.Count == 0)
                            {
                                running = false;
                                break; // Sale del for
                            }

                            traps.RemoveAt(i);
                            trapTimes.RemoveAt(i);
                            trapRepositionSchedule.RemoveAt(i);
                            if (traps.Count > 0) ScheduleNextTrapRepositionFromIndex(Math.Max(0, i - 1), nowSec);
                            hadTrapCollision = true;
                            break;
                        }
                    }

                    // Verificación extra por si murió dentro del loop de trampas
                    if (!running) break;

                    if (hadTrapCollision)
                    {
                        RenderFull();
                    }
                    else
                    {
                        DrawCell(newHead, Config.CellString(Config.HeadChar.ToString()));
                        DrawCell(prevHead, Config.CellString(Config.BodyChar.ToString()));
                        if (erasedTail.HasValue) EraseCell(erasedTail.Value);
                        DrawBorders();
                    }

                    lastMove = nowMs;
                }

                if (!apple.HasValue && nowSec >= appleSpawnAt)
                {
                    apple = RandomFree();
                    DrawCell(apple.Value, Config.CellString(appleSym));
                    DrawBorders();
                }

                int desiredTraps = Config.InitialTrapCount + Math.Max(0, level - 3);
                if (traps.Count < desiredTraps)
                {
                    double lastSpawn = trapTimes.Count > 0 ? trapTimes[trapTimes.Count - 1] : 0.0;
                    if (nowSec - lastSpawn >= Config.TrapSpawnIntervalSeconds)
                    {
                        SpawnTrapNow(nowSec);
                    }
                }

                for (int i = 0; i < traps.Count; i++)
                {
                    if (nowSec >= trapRepositionSchedule[i])
                    {
                        var old = traps[i];
                        // Lógica corregida dentro de RandomFreeFarFromHead evita la manzana
                        var np = RandomFreeFarFromHead(8);
                        traps[i] = np;
                        trapTimes[i] = nowSec;
                        trapRepositionSchedule[i] = nowSec + Config.TrapRepositionSeconds;
                        if (traps.Count > 1)
                        {
                            int next = (i + 1) % traps.Count;
                            trapRepositionSchedule[next] = nowSec + 8.0;
                        }
                        EraseCell(old);
                        DrawCell(np, Config.CellString(trapSym));
                        DrawBorders();
                    }
                }

                if (snake.Count >= Config.LevelUpLength)
                {
                    level++;
                    moveIntervalMs *= (1.0 - Config.SpeedIncreasePerLevel);

                    foreach (var t in traps) EraseCell(t);
                    traps.Clear();
                    trapTimes.Clear();
                    trapRepositionSchedule.Clear();

                    foreach (var c in snake) EraseCell(c);

                    InitSnakeCenter();

                    // Dibujar nueva serpiente
                    int idx = 0;
                    foreach (var p in snake)
                    {
                        string s = (idx == 0) ? Config.HeadChar.ToString() : Config.BodyChar.ToString();
                        DrawCell(p, Config.CellString(s));
                        idx++;
                    }

                    double now2 = sw.Elapsed.TotalSeconds;
                    for (int i = 0; i < Config.InitialTrapCount; i++) SpawnTrapNow(now2);
                    SpawnAppleScheduled(now2);

                    RenderHeader();
                    DrawBorders();
                    lastMove = sw.Elapsed.TotalMilliseconds + 300.0;
                }

                if (nowMs - lastRender >= 1000.0 / 6.0)
                {
                    RenderHeader();
                    lastRender = nowMs;
                }

                Thread.Sleep(1);
            }

            sw.Stop();
            Console.CursorVisible = true;
            try { Console.SetCursorPosition(0, DrawY(h - 2) + 3); } catch { }
            Console.WriteLine("Game over. Presiona cualquier tecla...");
            Console.ReadKey(true);

            double totalTime = sw.Elapsed.TotalSeconds;
            int reachedLevel = level;

            ScoreStore.AddScore(new Score
            {
                Name = name,
                Level = reachedLevel,
                TimeSeconds = totalTime,
                Date = DateTime.Now,
                ApplesEaten = applesEaten,
                TrapsHit = trapsHit
            });
        }
    }
}
