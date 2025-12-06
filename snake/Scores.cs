using System;
using System.Collections.Generic;

namespace SnakeGame {
    public class Score {
        public string Name { get; set; } = string.Empty;
        public int Level { get; set; }
        public double TimeSeconds { get; set; }
        public DateTime Date { get; set; }
        public int ApplesEaten { get; set; } = 0;
        public int TrapsHit { get; set; } = 0;
    }

    public static class ScoreStore {
        const string Path = "scores.json";
        public static void AddScore(Score s) {
            var list = Load();
            list.Add(s);
            Save(list);
        }
        public static List<Score> Load() {
            try {
                if (!System.IO.File.Exists(Path)) return new List<Score>();
                var j = System.IO.File.ReadAllText(Path);
                return System.Text.Json.JsonSerializer.Deserialize<List<Score>>(j) ?? new List<Score>();
            } catch {
                return new List<Score>();
            }
        }
        public static void Save(List<Score> list) {
            try {
                var j = System.Text.Json.JsonSerializer.Serialize(list);
                System.IO.File.WriteAllText(Path, j);
            } catch { }
        }
    }
}

