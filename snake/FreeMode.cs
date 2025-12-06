using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace SnakeGame {
    public class FreeMode {
        readonly int w = Config.Width, h = Config.Height;
        SinglyList snake = null!;
        List<Point> traps = new List<Point>();
        Point? apple = null;
        double moveIntervalMs;
        Stopwatch sw = new Stopwatch();
        string appleSym, trapSym;
        Queue<Point> dirQueue = new Queue<Point>();
        Point dir = new Point(1, 0);

        int applesEaten = 0;
        int trapsHit = 0;

        public FreeMode(double movesPerSecond) {
            moveIntervalMs = 1000.0 / movesPerSecond;
            appleSym = Config.UseEmoji ? Config.AppleEmoji : Config.AppleFallback.ToString();
            trapSym = Config.UseEmoji ? Config.TrapEmoji : Config.TrapFallback.ToString();
        }

        Point Wrap(Point p) => new Point((p.X % w + w) % w, (p.Y % h + h) % h);

        bool IsFree(Point p) {
            if (snake.Contains(p)) return false;
            foreach (var t in traps) if (t.X == p.X && t.Y == p.Y) return false;
            if (apple.HasValue && apple.Value.X == p.X && apple.Value.Y == p.Y) return false;
            return true;
        }

        Point RandomFreeFarFromHead(int minDist) {
            var head = snake.Head;
            for (int it = 0; it < 3000; it++) {
                var p = new Point(Config.Rng.Next(w), Config.Rng.Next(h));
                if (!snake.Contains(p)) {
                    double dx = p.X - head.X; double dy = p.Y - head.Y;
                    double dist = Math.Sqrt(dx * dx + dy * dy);
                    if (dist >= minDist) return p;
                }
            }
            for (int i = 0; i < 3000; i++) {
                var p = new Point(Config.Rng.Next(w), Config.Rng.Next(h));
                if (!snake.Contains(p)) return p;
            }
            return new Point(0, 0);
        }

        Point RandomFree() {
            for (int i = 0; i < 3000; i++) {
                var p = new Point(Config.Rng.Next(w), Config.Rng.Next(h));
                if (IsFree(p)) return p;
            }
            return new Point(0, 0);
        }

        void Init() {
            applesEaten = 0;
            trapsHit = 0;
            snake = new SinglyList();
            int cx = w / 2, cy = h / 2;
            for (int i = 0; i < Config.StartLength - 1; i++) snake.InsertFront(new Point(cx - i - 1, cy));
            snake.InsertFront(new Point(cx, cy));
            traps.Clear();
            apple = null;
            for (int i = 0; i < Config.InitialTrapCount; i++) traps.Add(RandomFree());
            dir = new Point(1, 0);
            dirQueue.Clear();
        }

        int DrawX(int logicalX) => 1 + logicalX * Config.CellWidth;
        int DrawY(int logicalY) => 2 + logicalY;

        void DrawBorders() {
            Console.ForegroundColor = ConsoleColor.White;
            for (int x = 0; x < 1 + w * Config.CellWidth + 1; x++) {
                Console.SetCursorPosition(x, 1);
                if (x == 0 || x == 1 + w * Config.CellWidth) Console.Write(Config.BorderCorner);
                else Console.Write(Config.BorderHorizontal);
            }
            for (int x = 0; x < 1 + w * Config.CellWidth + 1; x++) {
                Console.SetCursorPosition(x, 2 + h);
                if (x == 0 || x == 1 + w * Config.CellWidth) Console.Write(Config.BorderCorner);
                else Console.Write(Config.BorderHorizontal);
            }
            for (int y = 0; y < h; y++) {
                Console.SetCursorPosition(0, 2 + y);
                Console.Write(Config.BorderVertical);
                Console.SetCursorPosition(1 + w * Config.CellWidth, 2 + y);
                Console.Write(Config.BorderVertical);
            }
            Console.ResetColor();
        }

        void DrawCell(Point p, string cellStr) {
            if (p.X < 0 || p.X >= w || p.Y < 0 || p.Y >= h) return;
            int sx = DrawX(p.X);
            int sy = DrawY(p.Y);
            try { Console.SetCursorPosition(sx, sy); } catch { return; }
            if (cellStr == null) cellStr = Config.EmptyCell;
            if (cellStr.Contains(Config.HeadChar.ToString()) || cellStr.Contains(Config.BodyChar.ToString())) Console.ForegroundColor = ConsoleColor.Green;
            else if (cellStr.Contains(Config.UseEmoji ? Config.AppleEmoji : Config.AppleFallback.ToString()) || cellStr.Contains(Config.AppleFallback.ToString())) Console.ForegroundColor = ConsoleColor.Red;
            else if (cellStr.Contains(Config.UseEmoji ? Config.TrapEmoji : Config.TrapFallback.ToString()) || cellStr.Contains(Config.TrapFallback.ToString())) Console.ForegroundColor = ConsoleColor.Magenta;
            else Console.ResetColor();
            if (cellStr.Length < Config.CellWidth) cellStr = cellStr.PadRight(Config.CellWidth, Config.EmptyChar);
            if (cellStr.Length > Config.CellWidth) cellStr = cellStr.Substring(0, Config.CellWidth);
            Console.Write(cellStr);
            Console.ResetColor();
            // DrawBorders eliminado de aquí para evitar lag/parpadeo
        }

        void EraseCell(Point p) {
            if (p.X < 0 || p.X >= w || p.Y < 0 || p.Y >= h) return;
            int sx = DrawX(p.X);
            int sy = DrawY(p.Y);
            try { Console.SetCursorPosition(sx, sy); } catch { return; }
            Console.Write(Config.EmptyCell);
            // DrawBorders eliminado de aquí
        }

        void RenderFull() {
            Console.SetCursorPosition(0, 0);
            Console.WriteLine($"Free mode    Length: {snake.Count}");
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    Console.SetCursorPosition(DrawX(x), DrawY(y));
                    Console.Write(Config.EmptyCell);
                }
            }
            DrawBorders(); // Se dibuja solo una vez
            var arr = snake.ToArray();
            for (int i = 0; i < arr.Length; i++) DrawCell(arr[i], Config.CellString(i == 0 ? Config.HeadChar.ToString() : Config.BodyChar.ToString()));
            foreach (var t in traps) DrawCell(t, Config.CellString(Config.UseEmoji ? Config.TrapEmoji : Config.TrapFallback.ToString()));
        }

        void RenderHeader() {
            Console.SetCursorPosition(0, 0);
            Console.Write(new string(' ', Math.Max(Console.WindowWidth, 40)));
            Console.SetCursorPosition(0, 0);
            Console.WriteLine($"Free mode    Length: {snake.Count}  Time: {sw.Elapsed.TotalSeconds:F1}s  Apples:{applesEaten}  Traps:{trapsHit}");
        }

        public void Run() {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            Console.CursorVisible = false;
            Init();
            sw.Restart();
            double lastMove = sw.Elapsed.TotalMilliseconds;
            double lastRender = 0;
            double appleAppearAt = sw.Elapsed.TotalSeconds + Config.AppleBaseDelaySeconds;
            RenderFull();
            bool running = true;

            while (running) {
                double nowMs = sw.Elapsed.TotalMilliseconds;
                double nowSec = sw.Elapsed.TotalSeconds;

                while (Console.KeyAvailable) {
                    var key = Console.ReadKey(true).Key;
                    Point newDir = dir;
                    if (key == ConsoleKey.UpArrow || key == ConsoleKey.W) newDir = new Point(0, -1);
                    else if (key == ConsoleKey.DownArrow || key == ConsoleKey.S) newDir = new Point(0, 1);
                    else if (key == ConsoleKey.LeftArrow || key == ConsoleKey.A) newDir = new Point(-1, 0);
                    else if (key == ConsoleKey.RightArrow || key == ConsoleKey.D) newDir = new Point(1, 0);
                    else if (key == ConsoleKey.Escape) { running = false; break; }
                    Point lastQueued = dir;
                    if (dirQueue.Count > 0) { var arr = dirQueue.ToArray(); lastQueued = arr[arr.Length - 1]; }
                    if (!(newDir.X == -lastQueued.X && newDir.Y == -lastQueued.Y) && !(newDir.X == lastQueued.X && newDir.Y == lastQueued.Y)) {
                        dirQueue.Enqueue(newDir);
                        if (dirQueue.Count > 4) dirQueue.Dequeue();
                    }
                }

                if (nowMs - lastMove >= moveIntervalMs) {
                    if (dirQueue.Count > 0) dir = dirQueue.Dequeue();
                    var head = snake.Head;
                    var newHead = Wrap(new Point(head.X + dir.X, head.Y + dir.Y));
                    if (snake.Contains(newHead)) { running = false; break; }
                    snake.InsertFront(newHead);
                    var prevHead = head;
                    bool grew = false;
                    if (apple.HasValue && apple.Value.X == newHead.X && apple.Value.Y == newHead.Y) {
                        grew = true;
                        apple = null;
                        applesEaten++;
                        var tp = RandomFreeFarFromHead(8);
                        traps.Add(tp);
                        DrawCell(tp, Config.CellString(Config.UseEmoji ? Config.TrapEmoji : Config.TrapFallback.ToString()));
                        // DrawBorders eliminado
                        appleAppearAt = nowSec + Config.AppleBaseDelaySeconds + applesEaten * 0.05;
                    }
                    Point? erasedTail = null;
                    if (!grew) erasedTail = snake.RemoveBack();
                    for (int i = 0; i < traps.Count; i++) {
                        var t = traps[i];
                        if (t.X == newHead.X && t.Y == newHead.Y) {
                            trapsHit++;
                            Point removed = snake.RemoveBack();
                            EraseCell(removed);
                            traps.RemoveAt(i);
                            break;
                        }
                    }
                    DrawCell(newHead, Config.CellString(Config.HeadChar.ToString()));
                    DrawCell(prevHead, Config.CellString(Config.BodyChar.ToString()));
                    if (erasedTail.HasValue) EraseCell(erasedTail.Value);
                    lastMove = nowMs;
                }

                if (!apple.HasValue && sw.Elapsed.TotalSeconds >= appleAppearAt) {
                    apple = RandomFree();
                    DrawCell(apple.Value, Config.CellString(Config.UseEmoji ? Config.AppleEmoji : Config.AppleFallback.ToString()));
                    // DrawBorders eliminado
                }

                if (nowMs - lastRender >= 1000.0 / Config.FramesPerSecond) {
                    RenderHeader();
                    lastRender = nowMs;
                }

                Thread.Sleep(1);
            }

            sw.Stop();
            Console.CursorVisible = true;
            Console.SetCursorPosition(0, 2 + h + 3);
            Console.WriteLine("Game over. Presiona cualquier tecla...");
            Console.ReadKey(true);
            ScoreStore.AddScore(new Score { Name = "Free", Level = 0, TimeSeconds = sw.Elapsed.TotalSeconds, Date = DateTime.Now, ApplesEaten = applesEaten, TrapsHit = trapsHit });
        }
    }
}
