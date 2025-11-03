// Services/SaveLoadService.cs
using System;
using System.IO;
using System.Text.Json;
using System.Collections.Generic; // Necesario para List<T>

public class SaveLoadService
{
    private const string SAVE_FOLDER = "saves";
    private const int N = 9;

    public void SaveGame(GameState state)
    {
        Directory.CreateDirectory(SAVE_FOLDER);
        
        // Regla 6: Guardar estado completo
        var saveData = new SaveData
        {
            PlayerName = state.PlayerName,
            CurrentDifficulty = state.CurrentDifficulty,
            CurrentPuzzleInDifficulty = state.CurrentPuzzleInDifficulty,
            Lives = state.Lives,
            CurrentGrid = ConvertToJaggedArray(state.CurrentGrid),
            IsGiven = ConvertToJaggedArray(state.IsGiven),

            // Nuevas propiedades
            LivesAtLevelStart = state.LivesAtLevelStart,
            MaxLivesAchieved = state.MaxLivesAchieved,
            LostLifeThisDifficulty = state.LostLifeThisDifficulty,
            TimeThisLevel = state.TimeThisLevel,
            TimeThisDifficulty = state.TimeThisDifficulty,
            TimeLastDifficulty = state.TimeLastDifficulty,
            TotalTime = state.TotalTime,
            CurrentScore = state.CurrentScore,
            TotalCorrectMoves = state.TotalCorrectMoves,
            TotalIncorrectMoves = state.TotalIncorrectMoves,
            
            // NUEVO: Guardar la lista de tiempos
            LevelCompletionTimes = state.LevelCompletionTimes
        };

        string filePath = Path.Combine(SAVE_FOLDER, state.PlayerName + "_save.json");
        File.WriteAllText(filePath, JsonSerializer.Serialize(saveData, new JsonSerializerOptions { WriteIndented = true }));

        Console.WriteLine($"Guardado en: {filePath}");
    }

    public void LoadGame(GameState state)
    {
        Console.WriteLine($"Cargar archivo (ENTER para {state.PlayerName}_save.json):");
        string input = Console.ReadLine() ?? "";
        string filePath = string.IsNullOrWhiteSpace(input)
            ? Path.Combine(SAVE_FOLDER, state.PlayerName + "_save.json")
            : input;

        if (!File.Exists(filePath))
        {
            Console.WriteLine("No existe archivo.");
            return;
        }

        try
        {
            var saveData = JsonSerializer.Deserialize<SaveData>(File.ReadAllText(filePath));
            if (saveData != null)
            {
                ApplySaveData(state, saveData);
                Console.WriteLine("Partida cargada.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error carga: " + ex.Message);
        }
    }

    private void ApplySaveData(GameState state, SaveData saveData)
    {
        // Regla 6: Cargar estado completo
        state.PlayerName = saveData.PlayerName ?? state.PlayerName;
        state.CurrentDifficulty = saveData.CurrentDifficulty;
        state.CurrentPuzzleInDifficulty = saveData.CurrentPuzzleInDifficulty;
        state.Lives = saveData.Lives;

        if (saveData.CurrentGrid != null)
            state.CurrentGrid = ConvertToMultiArray(saveData.CurrentGrid);
        if (saveData.IsGiven != null)
            state.IsGiven = ConvertToMultiArray(saveData.IsGiven);

        // Nuevas propiedades
        state.LivesAtLevelStart = saveData.LivesAtLevelStart;
        state.MaxLivesAchieved = saveData.MaxLivesAchieved;
        state.LostLifeThisDifficulty = saveData.LostLifeThisDifficulty;
        state.TimeThisLevel = saveData.TimeThisLevel;
        state.TimeThisDifficulty = saveData.TimeThisDifficulty;
        state.TimeLastDifficulty = saveData.TimeLastDifficulty;
        state.TotalTime = saveData.TotalTime;
        state.CurrentScore = saveData.CurrentScore;
        state.TotalCorrectMoves = saveData.TotalCorrectMoves;
        state.TotalIncorrectMoves = saveData.TotalIncorrectMoves;

        // NUEVO: Cargar la lista de tiempos
        state.LevelCompletionTimes = saveData.LevelCompletionTimes ?? new List<TimeSpan>();

        // Reiniciar estado de puzzle (se carga inactivo)
        state.PuzzleActive = false;
        state.PuzzleStartTime = null;
    }

    // --- Métodos ayudantes de conversión (Sin Cambios) ---

    private int[][] ConvertToJaggedArray(int[,] multiArray)
    {
        int[][] jaggedArray = new int[N][];
        for (int i = 0; i < N; i++)
        {
            jaggedArray[i] = new int[N];
            for (int j = 0; j < N; j++)
            {
                jaggedArray[i][j] = multiArray[i, j];
            }
        }
        return jaggedArray;
    }

    private bool[][] ConvertToJaggedArray(bool[,] multiArray)
    {
        bool[][] jaggedArray = new bool[N][];
        for (int i = 0; i < N; i++)
        {
            jaggedArray[i] = new bool[N];
            for (int j = 0; j < N; j++)
            {
                jaggedArray[i][j] = multiArray[i, j];
            }
        }
        return jaggedArray;
    }

    private int[,] ConvertToMultiArray(int[][] jaggedArray)
    {
        int[,] multiArray = new int[N, N];
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                multiArray[i, j] = jaggedArray[i][j];
            }
        }
        return multiArray;
    }


    private bool[,] ConvertToMultiArray(bool[][] jaggedArray)
    {
        bool[,] multiArray = new bool[N, N];
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                multiArray[i, j] = jaggedArray[i][j];
            }
        }
        return multiArray;
    }
}