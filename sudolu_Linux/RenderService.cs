// Services/RenderService.cs
using System;

public class RenderService
{
    private const int N = 9;
    private readonly string[] difficultyNames = { "Muy Fácil", "Fácil", "Medio", "Difícil", "Muy Difícil" };

    // Firma actualizada: Necesita las soluciones para la lógica de "Cuadrante Completo"
    public void RenderGame(GameState state, int[,,] solutions, int puzzleIndex)
    {
        Console.Clear();
        Console.WriteLine("SUDOKU - consola");

        // Regla 3 y 5: Mostrar Vidas, Puntos y Tiempos
        string timeLevel = (state.TimeThisLevel).ToString(@"hh\:mm\:ss");
        
        // El tiempo de Dificultad es el tiempo ya acumulado en esa dificultad + el tiempo del nivel actual
        string timeDiff = (state.TimeThisDifficulty + state.TimeThisLevel).ToString(@"hh\:mm\:ss");
        // El tiempo Total es el tiempo de todas las dificultades pasadas + el tiempo del nivel actual
        string timeTotal = (state.TotalTime + state.TimeThisLevel).ToString(@"hh\:mm\:ss");

        Console.WriteLine($"Jugador: {state.PlayerName}  Vidas: {FormatLives(state.Lives)}  Puntos: {state.CurrentScore}");
        Console.WriteLine($"Nivel: {timeLevel} | Dificultad: {timeDiff} | Total: {timeTotal}");

        // Regla 4: Renderizar Grid Principal CON barra lateral
        RenderMainGridWithSidebar(state);

        Console.WriteLine();
        
        // Renderizar Grid de Referencia (Pasando soluciones)
        RenderReferenceGrid(state, solutions, puzzleIndex);
        
        Console.WriteLine();
    }

    // Regla 2: Formato de Vidas (Ya estaba correcto)
    private string FormatLives(int lives)
    {
        return lives <= 5 ?
            new string('❤', Math.Max(0, lives)) : $"❤ x {lives}";
    }

    // Regla 4: Grid + Barra Lateral
    private void RenderMainGridWithSidebar(GameState state)
    {
        var originalColor = Console.ForegroundColor;
        string hBorder = "+-------+-------+-------+";

        Console.WriteLine(hBorder);
        for (int row = 0; row < N; row++)
        {
            Console.Write("|");
            for (int col = 0; col < N; col++)
            {
                if (col % 3 == 0) Console.Write(" ");
                if (state.CurrentGrid[row, col] == 0)
                {
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.Write(". ");
                }
                else if (state.IsGiven[row, col] || state.IsUserCorrect[row, col])
                {
                    Console.ForegroundColor = ConsoleColor.Cyan;
                    Console.Write(state.CurrentGrid[row, col] + " ");
                }
                else if (state.IsUserIncorrect[row, col])
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Write(state.CurrentGrid[row, col] + " ");
                }
                else
                {
                    Console.ForegroundColor = originalColor;
                    Console.Write(state.CurrentGrid[row, col] + " ");
                }

                Console.ForegroundColor = originalColor;
                if ((col + 1) % 3 == 0) Console.Write("|");
            }
            
            // --- INICIO BARRA LATERAL (Regla 4) ---
            string sidebar = "";
            if (row == 0)      sidebar = "   Dificultades:";
            else if (row >= 2 && row <= 6) // Filas 2 a 6 (índice 0-4)
            {
                int diffIndex = row - 2;
                string name = difficultyNames[diffIndex];
                string prefix = (diffIndex == state.CurrentDifficulty) ? " > " : "   ";
                string progress;

                if (diffIndex < state.CurrentDifficulty)
                    progress = "5/5"; // Completado
                else if (diffIndex == state.CurrentDifficulty)
                    progress = $"{state.CurrentPuzzleInDifficulty}/5"; // Actual
                else
                    progress = "0/5"; // Pendiente

                sidebar = $"{prefix}[{progress}] {name}";
            }
            Console.Write(sidebar);
            // --- FIN BARRA LATERAL ---
            
            Console.WriteLine(); //
            if ((row + 1) % 3 == 0)
                Console.WriteLine(hBorder);
        }
    }

    private void RenderReferenceGrid(GameState state, int[,,] solutions, int puzzleIndex)
    {
        var originalColor = Console.ForegroundColor;
        string border = "+---+---+---+";
        Console.WriteLine(border);
        for (int blockRow = 0; blockRow < 3; blockRow++)
        {
            Console.Write("|");
            for (int blockCol = 0; blockCol < 3; blockCol++)
            {
                int quadrantIndex = blockRow * 3 + blockCol + 1;
                Console.Write(" ");
                // Pasamos las soluciones a la celda
                RenderReferenceCell(state, quadrantIndex, blockRow, blockCol, solutions, puzzleIndex);
                Console.Write(" |");
            }
            Console.WriteLine();
            Console.WriteLine(border);
        }
        Console.ForegroundColor = originalColor;
    }

    private void RenderReferenceCell(GameState state, int quadrantIndex, int blockRow, int blockCol, int[,,] solutions, int puzzleIndex)
    {
        var originalColor = Console.ForegroundColor;
        if (state.SelectedQuadrant == 0)
        {
            // Modo normal: mostrar números 1-9
            if (IsQuadrantComplete(state, quadrantIndex, solutions, puzzleIndex))
            {
                Console.ForegroundColor = ConsoleColor.Cyan;
                Console.Write(quadrantIndex);
            }
            else
            {
                Console.Write(quadrantIndex);
            }
        }
        else
        {
            // Modo selección: mostrar estado de las celdas
            int selectedQuadrant = state.SelectedQuadrant - 1;
            int baseRow = (selectedQuadrant / 3) * 3;
            int baseCol = (selectedQuadrant % 3) * 3;
            int actualRow = baseRow + blockRow;
            int actualCol = baseCol + blockCol;
            int positionNumber = blockRow * 3 + blockCol + 1;
            
            if (state.IsGiven[actualRow, actualCol] || state.IsUserCorrect[actualRow, actualCol])
            {
                Console.ForegroundColor = ConsoleColor.Cyan;
                Console.Write(positionNumber);
            }
            else
            {
                // Rojo si está vacía O incorrecta (para que sepa dónde falta)
                Console.ForegroundColor = ConsoleColor.Red;
                Console.Write(positionNumber);
            }
        }
        Console.ForegroundColor = originalColor;
    }

    private bool IsQuadrantComplete(GameState state, int quadrantIndex, int[,,] solutions, int puzzleIndex)
    {
        int quadrant = quadrantIndex - 1;
        int baseRow = (quadrant / 3) * 3;
        int baseCol = (quadrant % 3) * 3;
        for (int r = 0; r < 3; r++)
        {
            for (int c = 0; c < 3; c++)
            {
                int row = baseRow + r;
                int col = baseCol + c;

                // Comprobación completa: no solo si está vacío, sino si es correcto
                if (state.CurrentGrid[row, col] == 0) return false;
                if (state.CurrentGrid[row, col] != solutions[row, col, puzzleIndex]) return false;
            }
        }
        return true;
    }

}
