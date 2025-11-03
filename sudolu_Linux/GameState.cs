// Models/GameState.cs
using System;
using System.Collections.Generic;
using System.Text.Json;

public class GameState
{
    public string PlayerName { get; set; } = "Jugador";
    public int CurrentDifficulty { get; set; } = 0;
    public int CurrentPuzzleInDifficulty { get; set; } = 0;
    public int Lives { get; set; } = 3;
    public int[,] CurrentGrid { get; set; } = new int[9, 9];
    public bool[,] IsGiven { get; set; } = new bool[9, 9];
    public bool[,] IsUserCorrect { get; set; } = new bool[9, 9];
    public bool[,] IsUserIncorrect { get; set; } = new bool[9, 9];
    
    public DateTime? PuzzleStartTime { get; set; }
    public bool PuzzleActive { get; set; } = false;
    public int SelectedQuadrant { get; set; } = 0;
    public bool SolverEnabled { get; set; } = false;

    // --- NUEVAS PROPIEDADES PARA REGLAS ---

    // Regla Vidas
    public int LivesAtLevelStart { get; set; } = 3;
    public int MaxLivesAchieved { get; set; } = 3;
    public bool LostLifeThisDifficulty { get; set; } = false;

    // Regla Tiempos
    public TimeSpan TimeThisLevel { get; set; } = TimeSpan.Zero;
    public TimeSpan TimeThisDifficulty { get; set; } = TimeSpan.Zero;
    public TimeSpan TimeLastDifficulty { get; set; } = TimeSpan.Zero;
    public TimeSpan TotalTime { get; set; } = TimeSpan.Zero; // Acumula solo niveles COMPLETADOS
    
    // NUEVO: Almacena los tiempos de cada nivel individual
    public List<TimeSpan> LevelCompletionTimes { get; set; } = new List<TimeSpan>();


    // Regla Puntuación
    public long CurrentScore { get; set; } = 0;
    public int TotalCorrectMoves { get; set; } = 0;
    public int TotalIncorrectMoves { get; set; } = 0;

    // Tiempos objetivo (en minutos)
    public static readonly Dictionary<int, double> GoodDifficultyTimes = new Dictionary<int, double>
    {
        { 0, 12.5 }, // Muy Fácil
        { 1, 22.5 }, // Fácil
        { 2, 45.0 }, // Medio
        { 3, 92.5 }, // Difícil
        { 4, 175.0 } // Muy Difícil
    };
}

public class SudokuCache
{
    public int[][][]? Solutions { get; set; }
    public int[][][]? Initials { get; set; }
}

public class SaveData
{
    public string? PlayerName { get; set; }
    public int CurrentDifficulty { get; set; }
    public int CurrentPuzzleInDifficulty { get; set; }
    public int Lives { get; set; }
    public int[][]? CurrentGrid { get; set; }
    public bool[][]? IsGiven { get; set; }

    // --- NUEVAS PROPIEDADES PARA GUARDAR ---
    public int LivesAtLevelStart { get; set; }
    public int MaxLivesAchieved { get; set; }
    public bool LostLifeThisDifficulty { get; set; }

    public TimeSpan TimeThisLevel { get; set; }
    public TimeSpan TimeThisDifficulty { get; set; }
    public TimeSpan TimeLastDifficulty { get; set; }
    public TimeSpan TotalTime { get; set; }
    
    // NUEVO: Guardar la lista de tiempos
    public List<TimeSpan>? LevelCompletionTimes { get; set; }

    public long CurrentScore { get; set; }
    public int TotalCorrectMoves { get; set; }
    public int TotalIncorrectMoves { get; set; }
}

public class HighScore
{
    public string PlayerName { get; set; } = "N/A";
    public long FinalScore { get; set; }
    public int DifficultyReached { get; set; } // 0-5 (5 = completado)
    public int PuzzlesCompleted { get; set; }
    public TimeSpan TotalTime { get; set; }
    public int TotalCorrectMoves { get; set; }
    public int TotalIncorrectMoves { get; set; }
    public int FinalLives { get; set; }
    public int MaxLivesAchieved { get; set; }
    public DateTime DateAchieved { get; set; } = DateTime.UtcNow;

    // Para ranking (Aciertos - Fallos)
    public int NetCorrect => TotalCorrectMoves - TotalIncorrectMoves;
}