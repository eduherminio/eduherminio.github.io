using FileParser;
using System;
using System.IO;
using System.Linq;

namespace RockPaperScissors
{
    public static class Program
    {
        private enum Item { R, P, S }

        public static void Main()
        {
            using var outputFile = new StreamWriter("results.out");
            ParsedFile inputFile = new ParsedFile("Inputs/testInput.txt");

            var lines = inputFile.NextLine().NextElement<int>();
            for (int i = 0; i < lines; ++i)
            {
                var line = inputFile.NextLine();

                var player1 = (int)Enum.Parse(typeof(Item), line.NextElement<string>());
                var player2 = (int)Enum.Parse(typeof(Item), line.NextElement<string>());
                var players = new[] { player1, player2 };

                var result = Math.Abs(player2 - player1)
                    switch
                    {
                        0 => "-",
                        1 => ((Item)players.Max()).ToString(),
                        2 => ((Item)players.Min()).ToString(),
                        _ => throw new Exception(@"¯\_(ツ)_/¯")
                    };

                outputFile.WriteLine($"Case #{i + 1}: {result}");
            }
        }
    }
}
