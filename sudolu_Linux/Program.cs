using System;

class Program
{
    static GameManager gameManager = new GameManager();
    static void Main()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;
        gameManager.Run();
    }
}