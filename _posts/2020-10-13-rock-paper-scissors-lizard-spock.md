---
title: "Rock, Paper, Scissors, Lizard, Spock"
excerpt: "Solving "Rock, Paper, Scissors, Lizard, Spock" using an alternative, 'mathematical' approach"
date: 2020-10-13T00:00:00+00:00
categories:
  - blog
tags:
  - csharp
  - maths
series: "Rock, Paper, Scissors"
---

{% include series.html.liquid content=" " %}

## Introduction

In [my very first post](https://eduherminio.github.io/blog/rock-paper-scissors/) we went through solving _Rock, Paper, Scissors_ game using a 'mathematical' approach, and that entry ended with an open question:

> Can you find a similar, mathematical approach for [_Rock, Paper, Scissors, Lizard, Spock_](https://bigbangtheory.fandom.com/wiki/Rock,_Paper,_Scissors,_Lizard,_Spock)?

I invested no time at all in thinking about this variant until a few days ago, when [@jotagarciaz](https://github.com/jotagarciaz) asked that same question to me and sparked my curiosity.

## Game analysis: "Lizard, Spock" or "Spock, Lizard?"

_Rock, Paper, Scissors, Lizard, Spock_ is an expansion of _Rock, Paper, Scissors_ created by [Sam Kass and Karen Bryla](http://www.samkass.com/theories/RPSSL.html).
It was popularized by [_The Big Bang Theory_](https://www.youtube.com/watch?v=x5Q6-wMx-K8) TV series.

Let's start from the original game:

>- Scissors cuts Paper
>- Paper covers Rock
>- (and as it always has) Rock crushes Scissors

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/1_rock-paper-scissors-rock-paper-scissors.svg"
  description="'Rock, Paper, Scissors' arrow diagram" %}

Those traditional _Rock, Paper, Scissors_ rules still apply, but two new options are added to the game: Lizard and Spock.

Let's introduce the simplest rules related to those new options:

>- Rock crushes Lizard
>- Lizard poisons Spock
>- Spock smashes Scissors

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/2_rock-paper-scissors-add-lizard-spock.svg"
  description="'Rock, Paper, Scissors, Lizard, Spock' incomplete arrow diagram with Rock → Lizard, Lizard → Spock and Spock → Scissors arrows (besides traditional Rock, Paper, Scissors arrows)" %}

No huge surprises for now. Let's add another rule:

>- Lizard eats Paper

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/3_rock-paper-scissors-discover-pattern.svg"
  description="'Rock, Paper, Scissors, Lizard, Spock' incomplete arrow diagram with Rock → Lizard, Lizard → Spock, Spock → Scissors and Lizard → Paper arrows, with the latter one highlighted (besides traditional Rock, Paper, Scissors arrows)" %}

We can already imagine a possible pattern in the arrow diagram, which gets confirmed when adding the next one:

>- Spock vaporizes Rock

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/4_rock-paper-scissors-follow-pattern-1.svg"
  description="'Rock, Paper, Scissors, Lizard, Spock' incomplete arrow diagram with Rock → Lizard, Lizard → Spock, Spock → Scissors, Lizard → Paper and Spock → Rock arrows, with the latter one highlighted (besides traditional Rock, Paper, Scissors arrows)" %}

>- Scissors decapitates Lizard

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/5_rock-paper-scissors-follow-pattern-2.svg"
  description="'Rock, Paper, Scissors, Lizard, Spock' incomplete arrow diagram with Rock → Lizard, Lizard → Spock, Spock → Scissors and Lizard → Paper, Spock → Rock and Scissors → Lizard arrows, with the latter one highlighted (besides traditional Rock, Paper, Scissors arrows)" %}

>- Paper disproves Spock

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/6_rock-paper-scissors-follow-pattern-3.svg"
  description="'Rock, Paper, Scissors, Lizard, Spock' complete arrow diagram" %}

After going through all the new rules one by one, we can conclude that there's definitely a pattern.

We can also observe such pattern by reflecting those rules in a double entry table, **providing we use _Rock, Paper, Scissors, Spock, Lizard_ order** (that is, swapping Lizard and Spock positions in the game's name).

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/table.png"
  description="Double entry table comparing 'Rock, Paper, Scissors, Spock, Lizard' 'battles'" %}

Shocking, right? The game's most popular name is _**Rock, Paper, Scissors, Lizard, Spock**_, as that's how it appears in _The Big Bang Theory_; but according to the acronyms used in [Sam's page](http://www.samkass.com/theories/RPSSL.html), their creators originally named it 'right' (_**Rock, Paper, Scissors, Spock, Lizard**_).

I wonder how Doctor Sheldon Lee Cooper missed that!

## Solving the game

Once all rules are applied, we end up with this final diagram:

{% include image.html
  url="/assets/images/2020-10-13-rock-paper-scissors-lizard-spock/7_rock-paper-scissors-pattern-complete.svg"
  description="'Rock, Paper, Scissors, Lizard, Spock' complete arrow diagram" %}

Analyzing this figure and/or the table we saw above, we can write the pattern we've identified as:

- Any given option alternates between winning and losing against the other ones if you evaluate it following this (cyclic) order: Rock, Paper, Scissors, Spock, Lizard.
- The sequence starts by either losing against the next option in the list (or the first one clockwise in the diagram) or winning against the previous one (or the first one anti/counterclockwise in the diagram).

That's a really solid start, but still not enough. We need some mathematical relationship to be able to transform that into code.

Let's remember [what we concluded for _Rock, Paper, Scissors_](https://eduherminio.github.io/blog/rock-paper-scissors/#putting-everything-together):

>- If both numbers are the same, no one wins
>- If both numbers are consecutive, the bigger one wins
>- If both numbers aren’t consecutive, the smaller one wins

We have to find something similar for our pattern, and that's why we added numbers to the diagram above.

Let's write down some examples and try to induct a general rule that satisfies them all:

- Spock beats Scissors  → 4 beats 3 → > number wins
- Spock beats Rock      → 4 beats 1 → > number wins
- Paper beats Spock     → 2 beats 4 → < number wins
- Lizard beats Spock    → 5 beats 4 → > number wins
- Lizard beats Paper    → 5 beats 2 → > number wins
- Scissors beat[s](https://ell.stackexchange.com/questions/32268/scissors-cut-cuts-paper-which-is-correct/32288#32288) Lizard     → 3 beats 5 → < number wins
- Rock beats Lizard     → 1 beats 5 → < number wins

By carefully observing the examples, we can induct that:

- **If the difference between both numbers is odd, the bigger one wins**
- **If the difference between both numbers is even:**
  - **If both numbers are the same, no one wins**
  - **If both numbers are not the same, the smaller one wins**

Feel free to take any other example and verify that it satisfies those statements.

If we implement them using C# 8, we get the following function:

```csharp
int CalculateWinner(int player1, int player2)
{
    return (Math.Abs(player1 - player2) % 2)
        switch
    {
        0 => player1 == player2
            ? -1
            : new[] { player1, player2 }.Min(),
        1 => new[] { player1, player2 }.Max(),
        _ => throw new Exception(@"¯\_(ツ)_/¯")
    };
}
```

When we compare it with [_Rock, Paper, Scissors_ solution](https://eduherminio.github.io/blog/rock-paper-scissors/#putting-everything-together), we see that this algorithm is a generalization of the algorithm we used there, exactly how it's supposed to be, given that we're now solving an expansion of that game.

## Console application

Here is an example of an interactive console application that plays _Rock, Paper, Scissors, Spock, Lizard_ with us, and which is based on the previously defined algorithm.

Note that this code can also be used for _Rock, Paper, Scissors_ game if we remove Spock and Lizard from `enum Item` (line 6).

```csharp
using System;
using System.Linq;

public static class Program
{
    private enum Item { Rock, Paper, Scissors, Spock, Lizard }

    private static readonly string[] ItemArray = Enum.GetNames(typeof(Item));
    private static readonly Random Rnd = new Random((int)DateTime.Now.Ticks & 0x0000FFFF);

    public static void Main()
    {
        while (true)
        {
            Console.WriteLine($"Let's play! Type '{string.Join("', '", ItemArray[..^1])}' or '{ItemArray.Last()}'");

            if (!Enum.TryParse(typeof(Item), Console.ReadLine().Trim('\''), ignoreCase: true, out var humanInput))
            {
                Console.WriteLine("\tComputer says \"no\": invalid input\n");
                continue;
            }

            var computerInput = Enum.Parse(typeof(Item), ItemArray[Rnd.Next(0, ItemArray.Length)]);
            Console.WriteLine($"\tYou've chosen {humanInput}\n\tComputer has chosen {computerInput}");

            var result = CalculateWinner((int)humanInput, (int)computerInput);

            PrintResultMessage((int)humanInput, result);
        }
    }

    private static int CalculateWinner(int player1, int player2)
    {
        return (Math.Abs(player1 - player2) % 2)
            switch
        {
            0 => player1 == player2
                ? -1
                : new[] { player1, player2 }.Min(),
            1 => new[] { player1, player2 }.Max(),
            _ => throw new Exception(@"¯\_(ツ)_/¯")
        };
    }

    private static void PrintResultMessage(int humanInput, int result)
    {
        var resultMessage = result == -1
            ? "It's a draw!"
            : (result == humanInput) ? "You win!" : "You lose!";

        Console.WriteLine($"\t{resultMessage}\n");
    }
}
```

You can also find this code [here](https://github.com/eduherminio/eduherminio.github.io/tree/master/code/2020-10-13-rock-paper-scissors-lizard-spock).

---

I hope you've enjoyed this analysis of _Rock, Paper, Scissors, Spock, Lizard_ and/or that you've found the provided solution interesting!
