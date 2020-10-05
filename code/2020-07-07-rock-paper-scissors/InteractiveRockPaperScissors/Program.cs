using System;

namespace InteractiveRockPaperScissors
{
    public static class Program
    {
        private enum Item { Rock, Paper, Scissors }

        private static readonly string[] ItemArray = Enum.GetNames(typeof(Item));
        private static readonly Random Rnd = new Random((int)DateTime.Now.Ticks & 0x0000FFFF);

        public static void Main()
        {
            while (true)
            {
                Console.WriteLine("Let's play Rock, paper, scissors! Type 'Rock', 'Paper', or 'Scissors'");

                if (!Enum.TryParse(typeof(Item), Console.ReadLine().Trim('\''), ignoreCase: true, out var humanInput))
                {
                    Console.WriteLine("\tComputer says \"no\": invalid input\n");
                    continue;
                }

                var computerInput = Enum.Parse(typeof(Item), ItemArray[Rnd.Next(0, ItemArray.Length)]);
                Console.WriteLine($"\tYou've chosen {humanInput}\n\tComputer has chosen {computerInput}");

                var result = Math.Abs((int)humanInput - (int)computerInput)
                    switch
                {
                    0 => "It's a tie!",
                    1 => "You win!",
                    2 => "You lose!",
                    _ => throw new Exception(@"¯\_(ツ)_/¯")
                };

                Console.WriteLine($"\t{result}\n");
            }
        }
    }
}
