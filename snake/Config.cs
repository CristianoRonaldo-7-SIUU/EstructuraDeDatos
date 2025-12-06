using System;
namespace SnakeGame {
    public static class Config {
        public const int Width = 25;
        public const int Height = 21;
        public const int StartLength = 5;
        public const int LevelUpLength = 15;
        public const int InitialTrapCount = 3;
        public const double TrapSpawnIntervalSeconds = 12.0;
        public const double TrapRepositionSeconds = 15.0;
        public const double AppleBaseDelaySeconds = 1.0;
        public const double AppleLevelExtraPerLevelSeconds = 0.05;
        public const int FramesPerSecond = 30;
        public const double InitialMovesPerSecond = 6.0;
        public const double SpeedIncreasePerLevel = 0.1;
        public const char HeadChar = 'O';
        public const char BodyChar = '#';
        public const char EmptyChar = ' ';
        public const char BorderHorizontal = '-';
        public const char BorderVertical = '|';
        public const char BorderCorner = '+';
        public const int CellWidth = 2;
        public const string AppleEmoji = "🍎";
        public const string TrapEmoji = "💀";
        public const char AppleFallback = 'A';
        public const char TrapFallback = 'X';
        public static readonly Random Rng = new Random();
        public static bool UseEmoji = false;
        public static string CellString(string s) {
            if (string.IsNullOrEmpty(s)) return new string(EmptyChar, CellWidth);
            if (s.Length >= CellWidth) return s.Substring(0, CellWidth);
            return s.PadRight(CellWidth, EmptyChar);
        }
        public static string EmptyCell => new string(EmptyChar, CellWidth);
    }
}

