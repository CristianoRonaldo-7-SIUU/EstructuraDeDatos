// Services/SudokuGenerator.cs
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

public class SudokuGenerator
{
    private const int N = 9;
    private const int TOTAL_PUZZLES = 25;
    private Random rnd = new Random();
    public int[,,] Solutions { get; private set; } = new int[N, N, TOTAL_PUZZLES];
    public int[,,] Initials { get; private set; } = new int[N, N, TOTAL_PUZZLES];

    public void LoadOrGenerateAll()
    {
        string cacheFile = "sudokus_cache.json";
        if (File.Exists(cacheFile))
        {
            try
            {
                var doc = JsonSerializer.Deserialize<SudokuCache>(File.ReadAllText(cacheFile));
                if (doc?.Solutions?.Length == TOTAL_PUZZLES)
                {
                    for (int k = 0; k < TOTAL_PUZZLES; k++)
                        for (int r = 0; r < N; r++)
                            for (int c = 0; c < N; c++)
                            {
                                Solutions[r, c, k] = doc.Solutions[k][r][c];
                                Initials[r, c, k] = doc.Initials![k][r][c];
                            }
                    return;
                }
            }
            catch { }
        }
        Console.WriteLine("Generando 25 sudokus (puede tardar)...");
        GenerateAllPuzzles();
        SaveToCache(cacheFile);
    }

    private void GenerateAllPuzzles()
    {
        int[][] difficultyRanges = new int[][]
        {
            new int[] { 36, 44 }, new int[] { 32, 35 }, new int[] { 28, 31 }, new int[] { 24, 27 }, new int[] { 17, 23 }
        };
        int idx = 0;
        for (int diff = 0; diff < 5; diff++)
        {
            for (int j = 0; j < 5; j++)
            {
                var fullSolution = GenerateFullSolution();
                int clues = rnd.Next(difficultyRanges[diff][0], difficultyRanges[diff][1] + 1);
                var puzzle = MakePuzzle(fullSolution, clues);
                for (int r = 0; r < N; r++)
                    for (int c = 0; c < N; c++)
                    {
                        Solutions[r, c, idx] = fullSolution[r, c];
                        Initials[r, c, idx] = puzzle[r, c];
                    }
                idx++;
            }
        }
    }

    private void SaveToCache(string cacheFile)
    {
        var cacheObj = new SudokuCache
        {
            Solutions = new int[TOTAL_PUZZLES][][],
            Initials = new int[TOTAL_PUZZLES][][]
        };
        for (int k = 0; k < TOTAL_PUZZLES; k++)
        {
            cacheObj.Solutions[k] = new int[N][];
            cacheObj.Initials[k] = new int[N][];
            for (int r = 0; r < N; r++)
            {
                cacheObj.Solutions[k][r] = new int[N];
                cacheObj.Initials[k][r] = new int[N];
                for (int c = 0; c < N; c++)
                {
                    cacheObj.Solutions[k][r][c] = Solutions[r, c, k];
                    cacheObj.Initials[k][r][c] = Initials[r, c, k];
                }
            }
        }
        File.WriteAllText(cacheFile, JsonSerializer.Serialize(cacheObj));
    }

    public int[,] GenerateFullSolution()
    {
        int[,] baseGrid = new int[N, N];
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                baseGrid[r, c] = ((r * 3 + r / 3 + c) % 9) + 1;
        return ShuffleGrid(baseGrid);
    }

    private void Shuffle<T>(IList<T> list)
    {
        for (int i = list.Count - 1; i > 0; i--)
        {
            int j = rnd.Next(i + 1);
            var temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    }

    private int[,] ShuffleGrid(int[,] grid)
    {
        int[,] shuffled = (int[,])grid.Clone();
        // Shuffle rows within bands
        for (int band = 0; band < 3; band++)
        {
            var rows = new List<int> { 0, 1, 2 };
            Shuffle(rows);
            int[,] temp = (int[,])shuffled.Clone();
            for (int i = 0; i < 3; i++)
            {
                int srcRow = band * 3 + rows[i];
                int dstRow = band * 3 + i;
                for (int c = 0; c < N; c++)
                    temp[dstRow, c] = shuffled[srcRow, c];
            }
            shuffled = temp;
        }
        // Shuffle bands
        var bands = new List<int> { 0, 1, 2 };
        Shuffle(bands);
        shuffled = RearrangeBands(shuffled, bands);
        shuffled = Transpose(shuffled);
        // More shuffling after transpose
        for (int band = 0; band < 3; band++)
        {
            var rows = new List<int> { 0, 1, 2 };
            Shuffle(rows);
            int[,] temp = (int[,])shuffled.Clone();
            for (int i = 0; i < 3; i++)
            {
                int srcRow = band * 3 + rows[i];
                int dstRow = band * 3 + i;
                for (int c = 0; c < N; c++)
                    temp[dstRow, c] = shuffled[srcRow, c];
            }
            shuffled = temp;
        }
        var bands2 = new List<int> { 0, 1, 2 };
        Shuffle(bands2);
        shuffled = RearrangeBands(shuffled, bands2);
        shuffled = Transpose(shuffled);
        if (rnd.NextDouble() < 0.5) shuffled = Transpose(shuffled);
        return shuffled;
    }

    private int[,] RearrangeBands(int[,] grid, List<int> bandOrder)
    {
        int[,] result = (int[,])grid.Clone();
        for (int i = 0; i < 3; i++)
        {
            int srcBand = bandOrder[i];
            for (int r = 0; r < 3; r++)
                for (int c = 0; c < N; c++)
                    result[i * 3 + r, c] = grid[srcBand * 3 + r, c];
        }
        return result;
    }

    private int[,] Transpose(int[,] matrix)
    {
        int[,] transposed = new int[N, N];
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                transposed[r, c] = matrix[c, r];
        return transposed;
    }

    private bool IsSafe(int[,] grid, int row, int col, int value)
    {
        // Check row and column
        for (int i = 0; i < N; i++)
            if (grid[row, i] == value || grid[i, col] == value)
                return false;
        // Check 3x3 box
        int boxRow = row / 3 * 3;
        int boxCol = col / 3 * 3;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (grid[boxRow + i, boxCol + j] == value)
                    return false;
        return true;
    }

    private int[,] MakePuzzle(int[,] fullSolution, int clues)
    {
        int[,] puzzle = (int[,])fullSolution.Clone();
        var cells = new List<(int, int)>();
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                cells.Add((r, c));
        Shuffle(cells);
        int removalsTarget = N * N - clues;
        int removed = 0;
        foreach (var (r, c) in cells)
        {
            if (removed >= removalsTarget) break;
            int backup = puzzle[r, c];
            puzzle[r, c] = 0;
            int solutionsCount = CountSolutions(puzzle, 2);
            if (solutionsCount != 1)
                puzzle[r, c] = backup;
            else
                removed++;
        }
        return puzzle;
    }

    private int CountSolutions(int[,] grid, int limit)
    {
        int found = 0;
        void Backtrack(int[,] g)
        {
            if (found >= limit) return;
            int emptyRow = -1, emptyCol = -1;
            for (int r = 0; r < N; r++)
            {
                for (int c = 0; c < N; c++)
                {
                    if (g[r, c] == 0)
                    {
                        emptyRow = r;
                        emptyCol = c;
                        goto FoundEmpty;
                    }
                }
            }
            found++;
            return;
        FoundEmpty:
            for (int value = 1; value <= 9; value++)
            {
                if (IsSafe(g, emptyRow, emptyCol, value))
                {
                    g[emptyRow, emptyCol] = value;
                    Backtrack(g);
                    g[emptyRow, emptyCol] = 0;
                    if (found >= limit) return;
                }
            }
        }
        var copy = (int[,])grid.Clone();
        Backtrack(copy);
        return Math.Min(found, limit);
    }
}