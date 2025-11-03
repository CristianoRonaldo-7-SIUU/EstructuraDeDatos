// Services/HighScoreService.cs
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

public class HighScoreService
{
    private const string HIGH_SCORE_FILE = "highscores.json";

    // Carga todos los récords del archivo
    public List<HighScore> LoadAllScores()
    {
        if (!File.Exists(HIGH_SCORE_FILE))
        {
            return new List<HighScore>();
        }

        try
        {
            string json = File.ReadAllText(HIGH_SCORE_FILE);
            return JsonSerializer.Deserialize<List<HighScore>>(json) ?? new List<HighScore>();
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error al cargar high scores: {ex.Message}");
            return new List<HighScore>();
        }
    }

    // Guarda el estado final de una partida como un nuevo récord
    public void SavePlayerScore(GameState state, bool wonGame)
    {
        var scores = LoadAllScores();

        var newScore = new HighScore
        {
            PlayerName = state.PlayerName,
            FinalScore = state.CurrentScore,
            DifficultyReached = wonGame ? 5 : state.CurrentDifficulty, // 5 = completado
            PuzzlesCompleted = state.CurrentDifficulty * 5 + state.CurrentPuzzleInDifficulty,
            TotalTime = state.TotalTime,
            TotalCorrectMoves = state.TotalCorrectMoves,
            TotalIncorrectMoves = state.TotalIncorrectMoves,
            FinalLives = state.Lives,
            MaxLivesAchieved = state.MaxLivesAchieved,
            DateAchieved = DateTime.UtcNow
        };

        scores.Add(newScore);

        try
        {
            string json = JsonSerializer.Serialize(scores, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(HIGH_SCORE_FILE, json);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error al guardar high score: {ex.Message}");
        }
    }
}