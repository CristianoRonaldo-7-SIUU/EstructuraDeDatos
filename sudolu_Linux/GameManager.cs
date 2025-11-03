// Services/GameManager.cs
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

public class GameManager
{
    private const int N = 9;
    private const int TOTAL_PUZZLES = 25;

    private SudokuGenerator generator = new SudokuGenerator();
    private RenderService renderer = new RenderService();
    private SaveLoadService saveLoadService = new SaveLoadService();
    private HighScoreService highScoreService = new HighScoreService();
    private GameState state = new GameState();
    private CancellationTokenSource? solverCts = null;

    private int CurrentPuzzleIndex => state.CurrentDifficulty * 5 + state.CurrentPuzzleInDifficulty;

    public void Run()
    {
        generator.LoadOrGenerateAll();
        ShowWelcome();
        MainLoop();
    }

    private void ShowWelcome()
    {
        Console.Clear();
        Console.WriteLine("SUDOKU - consola");
        Console.Write("Nombre de jugador: ");
        var name = Console.ReadLine();
        if (!string.IsNullOrWhiteSpace(name))
            state.PlayerName = name.Trim();
        Console.WriteLine("Pulsa G para GENERAR el primer sudoku.");
    }

    private void MainLoop()
    {
        while (true)
        {
            Console.Write("Comando (G=Generar, L=Cargar, S=Guardar, T=Tiempos, Q=Salir): ");
            var command = (Console.ReadLine() ?? "").Trim().ToUpperInvariant();

            switch (command)
            {
                case "Q":
                    StopSolver();
                    return;
                case "G":
                    if (!state.PuzzleActive) StartPuzzle();
                    else Console.WriteLine("Ya hay un sudoku activo.");
                    break;
                case "S":
                    saveLoadService.SaveGame(state);
                    break;
                case "L":
                    saveLoadService.LoadGame(state);
                    break;
                case "T":
                    ShowHighScores();
                    break;
                default:
                    Console.WriteLine("Comando no reconocido.");
                    break;
            }
        }
    }

    private void StartPuzzle()
    {
        if (CurrentPuzzleIndex >= TOTAL_PUZZLES)
        {
            OnGameWon();
            return;
        }

        InitializePuzzleState();
        state.LivesAtLevelStart = state.Lives; // Regla 3
        state.MaxLivesAchieved = Math.Max(state.MaxLivesAchieved, state.Lives);
        state.PuzzleStartTime = DateTime.UtcNow; // Regla 3
        state.TimeThisLevel = TimeSpan.Zero;
        state.PuzzleActive = true;
        state.SelectedQuadrant = 0;

        PuzzleLoop();
    }

    private void InitializePuzzleState()
    {
        for (int r = 0; r < N; r++)
        {
            for (int c = 0; c < N; c++)
            {
                state.CurrentGrid[r, c] = generator.Initials[r, c, CurrentPuzzleIndex];
                state.IsGiven[r, c] = state.CurrentGrid[r, c] != 0;
                state.IsUserCorrect[r, c] = false;
                state.IsUserIncorrect[r, c] = false;
            }
        }
    }

    private void UpdateTimers()
    {
        if (state.PuzzleActive && state.PuzzleStartTime.HasValue)
        {
            state.TimeThisLevel = DateTime.UtcNow - state.PuzzleStartTime.Value;
        }
    }

    private void PuzzleLoop()
    {
        while (state.PuzzleActive)
        {
            UpdateTimers();
            // Pasamos las soluciones al renderizador para la Regla 4 (comprobar cuadrante completo)
            renderer.RenderGame(state, generator.Solutions, CurrentPuzzleIndex);

            if (IsPuzzleComplete())
            {
                OnPuzzleCompleted();
                break;
            }

            ProcessPlayerInput();
        }
    }

    private void ProcessPlayerInput()
    {
        Console.Write("Introduce cuadrante (1-9) o X para salir al menú: ");
        var quadrantInput = (Console.ReadLine() ?? "").Trim();

        if (string.IsNullOrEmpty(quadrantInput)) return;

        // Comandos ocultos
        if (quadrantInput.Equals("B", StringComparison.OrdinalIgnoreCase))
        {
            ToggleSolver();
            return;
        }
        if (quadrantInput.Equals("R", StringComparison.OrdinalIgnoreCase))
        {
            RevealSolution();
            return;
        }
        if (quadrantInput.Equals("X", StringComparison.OrdinalIgnoreCase))
        {
            state.PuzzleActive = false;
            StopSolver();
            return;
        }

        if (!int.TryParse(quadrantInput, out int quadrant) || quadrant < 1 || quadrant > 9)
        {
            Console.WriteLine("Cuadrante inválido.");
            return;
        }

        state.SelectedQuadrant = quadrant;
        renderer.RenderGame(state, generator.Solutions, CurrentPuzzleIndex); // Re-render con selección
        ProcessCellSelection(quadrant);
    }

    private void ProcessCellSelection(int quadrant)
    {
        Console.Write("Introduce celda dentro del cuadrante (1-9) o C para cancelar: ");
        var cellInput = (Console.ReadLine() ?? "").Trim();

        if (cellInput.Equals("C", StringComparison.OrdinalIgnoreCase))
        {
            state.SelectedQuadrant = 0;
            return;
        }

        if (!int.TryParse(cellInput, out int position) || position < 1 || position > 9)
        {
            Console.WriteLine("Posición inválida.");
            return;
        }

        var (row, col) = CalculateCellCoordinates(quadrant, position);

        if (!CanModifyCell(row, col)) return;

        ProcessCellValue(row, col);
    }

    private (int row, int col) CalculateCellCoordinates(int quadrant, int position)
    {
        int quadRow = (quadrant - 1) / 3;
        int quadCol = (quadrant - 1) % 3;
        int localRow = (position - 1) / 3;
        int localCol = (position - 1) % 3;
        return (quadRow * 3 + localRow, quadCol * 3 + localCol);
    }

    private bool CanModifyCell(int row, int col)
    {
        if (state.IsGiven[row, col])
        {
            Console.WriteLine("No puede modificar una casilla generada automáticamente. Elija otra celda.");
            return false;
        }

        if (state.IsUserCorrect[row, col])
        {
            Console.WriteLine("No puede modificar una casilla ya colocada correctamente. Elija otra celda.");
            return false;
        }

        return true;
    }

    private void ProcessCellValue(int row, int col)
    {
        Console.Write("Introduce valor (1-9): ");
        var valueInput = (Console.ReadLine() ?? "").Trim();

        if (!int.TryParse(valueInput, out int value) || value < 1 || value > 9)
        {
            Console.WriteLine("Valor inválido.");
            return;
        }

        ValidateAndApplyMove(row, col, value);
    }

    private void ValidateAndApplyMove(int row, int col, int value)
    {
        int solution = generator.Solutions[row, col, CurrentPuzzleIndex];
        state.CurrentGrid[row, col] = value;

        if (value == solution)
        {
            state.IsUserCorrect[row, col] = true;
            state.IsUserIncorrect[row, col] = false;
            state.TotalCorrectMoves++; // Regla 5
            state.CurrentScore += 10; // Regla 5
        }
        else
        {
            state.IsUserIncorrect[row, col] = true;
            state.IsUserCorrect[row, col] = false;
            state.Lives--;
            state.TotalIncorrectMoves++; // Regla 5
            state.LostLifeThisDifficulty = true; // Regla 2
            if (state.Lives <= 0)
            {
                OnGameOver();
            }
        }
    }

    private bool IsPuzzleComplete()
    {
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                if (state.CurrentGrid[r, c] == 0)
                    return false;

        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                if (state.CurrentGrid[r, c] != generator.Solutions[r, c, CurrentPuzzleIndex])
                    return false;
        return true;
    }

    private void OnPuzzleCompleted()
    {
        state.PuzzleActive = false;
        UpdateTimers(); // Captura el tiempo final del nivel
        state.PuzzleStartTime = null; // Detener cronómetro
        StopSolver();

        // Actualizar tiempos totales
        state.LevelCompletionTimes.Add(state.TimeThisLevel); // NUEVO: Guardar tiempo del nivel
        state.TimeThisDifficulty += state.TimeThisLevel;
        state.TotalTime += state.TimeThisLevel;

        // --- LÓGICA DE VIDAS (Reglas 1 y 3) ---
        bool lostLifeThisLevel = state.Lives < state.LivesAtLevelStart;
        if (lostLifeThisLevel)
        {
            // Regla 3: "CTRL+Z"
            state.Lives = state.LivesAtLevelStart;
        }
        else
        {
            // Regla 1: Nivel perfecto
            state.Lives++;
            state.MaxLivesAchieved = Math.Max(state.MaxLivesAchieved, state.Lives);
        }
        
        // --- LÓGICA DE PUNTUACIÓN (Regla 5) ---
        CalculateTimeBonuses(isLevelEnd: true);

        renderer.RenderGame(state, generator.Solutions, CurrentPuzzleIndex); // Render final
        Console.WriteLine("Sudoku completado.");

        Console.WriteLine("Opciones: C=Guardar+Continuar, E=Guardar+Salir, Q=Salir sin guardar");
        Console.Write("Comando: ");
        var command = (Console.ReadLine() ?? "").Trim().ToUpperInvariant();
        if (command == "C")
        {
            saveLoadService.SaveGame(state);
            AdvanceToNextPuzzle();
            StartPuzzle();
        }
        else if (command == "E")
        {
            saveLoadService.SaveGame(state);
            Environment.Exit(0);
        }
    }

    private void AdvanceToNextPuzzle()
    {
        state.CurrentPuzzleInDifficulty++;
        if (state.CurrentPuzzleInDifficulty >= 5)
        {
            // --- FIN DE DIFICULTAD ---
            
            // Regla 5: Bonus por tiempo de dificultad
            CalculateTimeBonuses(isDifficultyEnd: true);
            
            // NUEVO: Mostrar informe de tiempos
            ShowDifficultySummaryReport(state, isGameOver: false);
            
            // NUEVO: Aplicar multiplicador de puntuación por dificultad
            long baseScore = state.CurrentScore;
            double multiplier = GetScoreMultiplier(state.Lives);
            state.CurrentScore = (long)(baseScore * multiplier);
            
            Console.Clear();
            Console.WriteLine("--- PUNTUACIÓN ACTUALIZADA ---");
            Console.WriteLine($"Puntos base: {baseScore}");
            Console.WriteLine($"Vidas: {state.Lives} (Multiplicador: x{multiplier})");
            Console.WriteLine($"Nueva Puntuación: {state.CurrentScore}");
            Console.WriteLine("\nPresiona ENTER para la siguiente dificultad...");
            Console.ReadLine();

            // Regla 2: Bonus de vidas por dificultad
            bool fasterTime = state.CurrentDifficulty > 0 && state.TimeThisDifficulty < state.TimeLastDifficulty;
            if (!state.LostLifeThisDifficulty || fasterTime)
            {
                state.Lives++;
                state.MaxLivesAchieved = Math.Max(state.MaxLivesAchieved, state.Lives);
            }

            // Reset para siguiente dificultad
            state.TimeLastDifficulty = state.TimeThisDifficulty;
            state.TimeThisDifficulty = TimeSpan.Zero;
            state.LostLifeThisDifficulty = false;
            
            state.CurrentDifficulty++;
            state.CurrentPuzzleInDifficulty = 0;

            if (state.CurrentDifficulty >= 5)
            {
                // El juego se ha ganado, pero StartPuzzle lo detectará
                return;
            }
        }
    }

    private void OnGameOver()
    {
        state.PuzzleActive = false;
        StopSolver();
        Console.WriteLine("Sin vidas. Fin de la partida.");
        
        // ¡CAMBIO CLAVE!
        // Llamar al informe de tiempos también al perder.
        ShowDifficultySummaryReport(state, isGameOver: true);
        
        FinalizeScoreAndSave(false);
    }

    private void OnGameWon()
    {
        state.PuzzleActive = false;
        StopSolver();
        Console.WriteLine("¡Felicidades! ¡Has completado todos los Sudokus!");
        
        // Regla 5: Bonus por tiempo total
        CalculateTimeBonuses(isGameEnd: true);
        
        // El informe de la última dificultad ya se mostró en AdvanceToNextPuzzle
        FinalizeScoreAndSave(true);
    }

    private void CalculateTimeBonuses(bool isLevelEnd = false, bool isDifficultyEnd = false, bool isGameEnd = false)
    {
        try
        {
            // Evitar error si el juego se gana (CurrentDifficulty podría ser 5)
            int diffIdx = Math.Min(state.CurrentDifficulty, 4); 
            double goodDifficultyTimeMins = GameState.GoodDifficultyTimes[diffIdx];
            double goodLevelTimeSecs = (goodDifficultyTimeMins * 60) / 5.0;
            
            if (isLevelEnd && state.TimeThisLevel.TotalSeconds <= goodLevelTimeSecs)
            {
                state.CurrentScore += 500;
                Console.WriteLine($"¡Bonus de tiempo de nivel! +500 puntos");
            }

            if (isDifficultyEnd && state.TimeThisDifficulty.TotalSeconds <= (goodDifficultyTimeMins * 60))
            {
                state.CurrentScore += 2500;
                Console.WriteLine($"¡Bonus de tiempo de dificultad! +2500 puntos");
            }

            if (isGameEnd)
            {
                double goodTotalTimeSecs = GameState.GoodDifficultyTimes.Values.Sum() * 60;
                if (state.TotalTime.TotalSeconds <= goodTotalTimeSecs)
                {
                    state.CurrentScore += 25000;
                    Console.WriteLine($"¡Bonus de tiempo total! +25000 puntos");
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error calculando bonus: {ex.Message}");
        }
    }

    // NUEVO: Método para mostrar el informe de tiempos
    private void ShowDifficultySummaryReport(GameState state, bool isGameOver)
    {
        Console.Clear();
        Console.WriteLine("--- RESUMEN DE LA DIFICULTAD ---");

        int totalLevelsCompleted = state.LevelCompletionTimes.Count;
        
        // Determina cuántos niveles de esta dificultad se completaron
        int levelsInThisDifficulty;
        if (isGameOver)
        {
            // Si es game over, solo los de esta dificultad
            // (que son los que están en la lista desde la última dificultad completada)
            int lastDifficultyBaseLevel = state.CurrentDifficulty * 5;
            levelsInThisDifficulty = totalLevelsCompleted - lastDifficultyBaseLevel;
        }
        else
        {
            // Si se completó la dificultad, fueron 5
            levelsInThisDifficulty = 5;
        }

        int startIdx = Math.Max(0, totalLevelsCompleted - levelsInThisDifficulty);
        var recentTimes = state.LevelCompletionTimes.Skip(startIdx).ToList();

        for (int i = 0; i < recentTimes.Count; i++)
        {
            // Nivel base 1 (Nivel 1, Nivel 2, ... Nivel 25)
            int levelNumber = startIdx + i + 1;
            Console.WriteLine($"  Tiempo Nivel {levelNumber}: {recentTimes[i].ToString(@"hh\:mm\:ss")}");
        }

        Console.WriteLine("\n+------------------------------+");
        Console.WriteLine($"  Tiempo Total Dificultad: {state.TimeThisDifficulty.ToString(@"hh\:mm\:ss")}");
        Console.WriteLine($"  Tiempo Total de Juego:   {state.TotalTime.ToString(@"hh\:mm\:ss")}");
        Console.WriteLine("+------------------------------+");
        
        Console.WriteLine("\nPresiona ENTER para continuar...");
        Console.ReadLine();
    }
    
    // NUEVO: Helper para obtener el multiplicador
    private double GetScoreMultiplier(int lives)
    {
        switch (lives)
        {
            case 0: return 0.25;
            case 1: return 0.5;
            case 2: return 1.5;
            case 3: return 3.0;
            default:
                return (lives > 3) ? lives : 3.0; // Si es > 3, es xN. Si es negativo (imposible), usa 3.
        }
    }

private void FinalizeScoreAndSave(bool won)
{
    long baseScore = state.CurrentScore;
    
    // CAMBIO: La multiplicación solo ocurre si se pierde.
    // Si se gana, la puntuación ya fue multiplicada al final de la última dificultad.
    if (!won)
    {
        double multiplier = GetScoreMultiplier(state.Lives); // Vidas serán 0
        state.CurrentScore = (long)(baseScore * multiplier);
        Console.WriteLine("--- FIN DE LA PARTIDA ---");
        Console.WriteLine($"Puntos base: {baseScore}");
        Console.WriteLine($"Vidas finales: {state.Lives} (Multiplicador: x{multiplier})");
    }
    else
    {
        Console.WriteLine("--- FIN DE LA PARTIDA ---");
    }

    Console.WriteLine($"PUNTUACIÓN FINAL: {state.CurrentScore}");
    Console.WriteLine($"Tiempo total: {state.TotalTime.ToString(@"hh\:mm\:ss")}");
    
    highScoreService.SavePlayerScore(state, won);
    Console.WriteLine("Récord guardado. Presiona ENTER para volver al menú.");
    Console.ReadLine();
}

    // Regla 7: Tablas de Tiempos
    private void ShowHighScores()
    {
        Console.Clear();
        var allScores = highScoreService.LoadAllScores();
        
        var myScores = allScores
            .Where(s => s.PlayerName.Equals(state.PlayerName, StringComparison.OrdinalIgnoreCase))
            .OrderByDescending(s => s.DateAchieved)
            .ToList();
            
        PrintScoreTable($"Mis Récords ({state.PlayerName})", myScores);
        
        Console.WriteLine("\nPresiona 'W' + ENTER para ver el Ranking Global, o solo ENTER para salir.");
        var cmd = Console.ReadLine() ?? "";

        if (cmd.Trim().Equals("W", StringComparison.OrdinalIgnoreCase))
        {
            Console.Clear();
            var globalRanked = allScores
                .OrderByDescending(s => s.DifficultyReached) // 1. Nivel
                .ThenBy(s => s.TotalTime)                     // 2. Tiempo
                .ThenByDescending(s => s.NetCorrect)          // 3. Aciertos - Fallos
                .ToList();
                
            PrintScoreTable("Ranking Global (Todos los jugadores)", globalRanked);
            Console.Write("\nPresiona ENTER para salir.");
            Console.ReadLine();
        }
    }

    private void PrintScoreTable(string title, List<HighScore> scores)
    {
        Console.WriteLine(title);
        string line = "+----------------------+----------+----------+---------+----------+----------+";
        Console.WriteLine(line);
        Console.WriteLine("| Nombre Jugador       | Puntos   | Dificult | Puzzles | Tiempo   | A-F      |");
        Console.WriteLine(line);

        if (!scores.Any())
        {
            Console.WriteLine("| No hay récords disponibles...                                         |");
        }

        foreach (var score in scores.Take(20)) // Mostrar max 20
        {
            string diff = score.DifficultyReached == 5 ? "TODOS" : (score.DifficultyReached + 1).ToString();
            string puzzles = $"{score.PuzzlesCompleted}/25";
            string net = (score.TotalCorrectMoves - score.TotalIncorrectMoves).ToString();

            Console.WriteLine($"| {score.PlayerName.PadRight(20).Substring(0, 20)} " +
                              $"| {score.FinalScore.ToString().PadLeft(8)} " +
                              $"| {diff.PadLeft(8)} " +
                              $"| {puzzles.PadLeft(7)} " +
                              $"| {score.TotalTime.ToString(@"hh\:mm\:ss").PadLeft(8)} " +
                              $"| {net.PadLeft(8)} |");
        }
        Console.WriteLine(line);
    }

    // --- Funciones del Solver (sin cambios) ---
    private void ToggleSolver()
    {
        state.SolverEnabled = !state.SolverEnabled;
        if (state.SolverEnabled) StartSolver();
        else StopSolver();
    }

    private void StartSolver()
    {
        if (solverCts != null) return;
        solverCts = new CancellationTokenSource();
        var token = solverCts.Token;

        Task.Run(async () =>
        {
            while (!token.IsCancellationRequested && state.PuzzleActive)
            {
                bool filled = false;
                for (int r = 0; r < N && !filled; r++)
                {
                    for (int c = 0; c < N; c++)
                    {
                        if (state.CurrentGrid[r, c] == 0)
                        {
                            state.CurrentGrid[r, c] = generator.Solutions[r, c, CurrentPuzzleIndex];
                            state.IsUserCorrect[r, c] = true;
                            filled = true;
                            break;
                        }
                    }
                }
                await Task.Delay(100, token);
            }
        }, token);
    }

    private void StopSolver()
    {
        solverCts?.Cancel();
        solverCts?.Dispose();
        solverCts = null;
        state.SolverEnabled = false;
    }

    private void RevealSolution()
    {
        for (int r = 0; r < N; r++)
        {
            for (int c = 0; c < N; c++)
            {
                state.CurrentGrid[r, c] = generator.Solutions[r, c, CurrentPuzzleIndex];
                state.IsUserCorrect[r, c] = true;
                state.IsUserIncorrect[r, c] = false;
            }
        }
    }
}
