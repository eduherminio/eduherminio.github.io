---
title: "Getting ready for Advent of Code 2020"
date: 2020-11-26T00:00:00+00:00
categories:
  - blog
tags:
  - adventofcode
  - competitiveprogramming
---

## What's all the fuzz about

It's almost December and [Advent of Code](https://adventofcode.com/) is back! For those of you who don't know it yet, AoC is an initiative by the great [Eric Wastl](https://twitter.com/ericwastl) that consists of a series of 25 puzzles released daily between 1st and 25th of December.

In 2019 [almost 100,000 people](https://adventofcode.com/2019/stats) completed at least one of the two parts of the first problem and almost 3,000 of them completed them all; and I expect those numbers to increase this year due to the multiple lock-downs/stay-at-home recommendations in place worldwide.

People use Advent of Code as an excuse to learn a new language or paradigm, challenge their friends/colleagues (it has [public](https://adventofcode.com/2019/leaderboard) and private leaderboards) or simply challenge themselves every day for a few minutes/hours and starting/ending the day with the satisfaction of getting stuff done.

![Advent of Code 2015 gif animation](/assets/images/2020-11-26-getting-ready-for-aoc-2020/aoc_2015.gif)

However, AoC is not only about solving problems, but also about learning from (or directly admiring) other people's solutions and creativity:

- Some people solve certain problems using [**Minecraft**](https://www.reddit.com/r/adventofcode/comments/e7ylwd/i_solved_day_8_entirely_in_minecraft/).

- Others manage to solve a good number of them using [**Excel**](https://www.youtube.com/playlist?list=PL5ZJ5xCmOlkmsxkFbYj4qtQzPmRFfM451) (which includes implementing [made-up programming languages](https://esolangs.org/wiki/Intcode#:~:text=Intcode%20is%20an%20esoteric%20programming,%2C%2021%2C%2023%20and%2025.) in a [spreadsheet](https://www.reddit.com/r/adventofcode/comments/ea8mif/2019_day_13_excel_did_you_think_i_would_give_up/)).

- A few geniuses stick to more canonical languages/approaches, but [**solve all 25 problems in 9ms**](https://www.reddit.com/r/adventofcode/comments/jf03dr/2019_optimized_solutions_in_c_9_ms_total/).

- Did anyone mentioned [Arduino](https://www.youtube.com/playlist?list=PLm-JYoU3uw-aIWvjuzHk2KOQSjLQT6Ac-)?

- And what about Unity [here](https://www.reddit.com/r/adventofcode/comments/a6sej7/day_15_unity_visualization/) or [there](https://www.reddit.com/r/adventofcode/comments/eb79s0/2019_day_10_blowing_up_asteroids_in_unity/)?

But please don't let those amazing examples intimidate you, most of us solve the problems in more down-to-earth ways and just enjoy reading about other people's solutions afterwards. And of once a problem is out, you can solve whenever you find time for it, not only during that day (or year!).

You can listen to [this inspiring talk](https://youtu.be/gibVyxpi-qA) by Eric himself where he speaks about why he decided to organize it for the first time, the unexpected success he got the first advent day of the first year (his estimation about the number of participants was of by... 5,600% after the first 12 hours), how he keeps preparing it every year and [the amazing community](https://www.reddit.com/r/adventofcode) that has arisen around it.

## My personal Advent of Code preparation

As stated, the beauty of Advent of Code is that each individual does it in own their way.

In my case, I don't focus on competing in any leaderboards, but just having fun writing some code and trying to make it both understandable and relatively performant.

I discovered AoC in 2018, and that same year while going through the problems I started creating some kind of ['mini-framework' for solving AoC problems using .NET](https://github.com/eduherminio/AoCHelper).

I have to admit I had almost as much fun solving the actual problems as I did writing that util, which I released in 2019 as [AoCHelper NuGet library](https://www.nuget.org/packages/AoCHelper/).

This year I've tried to improve AoCHelper before starting the actual problem solving, to try to focus on finding the solutions to those problems rather than on tweaking the framework, but we'll see 😉.
**And yeah, that has been all my 'preparation'.**

In case anyone is curious about it, here you can see the entrypoint to my console app (on the left) and how all my problem classes look like (on the right).

![Personal workspace while using AoCHelper library: main program class on the left and problem class on the right](/assets/images/2020-11-26-getting-ready-for-aoc-2020/workspace_with_aochelper.png)

If I name the problem classes correctly and have the input files in the right folder, I just need to invoke:

```csharp
new ProblemSolver().solver.SolveAllProblemsWithMetrics()
```

and I'll get some output that measured the performance of each solution. 

Something like this, but with colors associated to different ranges of ms:

```bash
$> dotnet run -c Release

Day 1, part 1:          1234                  1 ms
Day 1, part 2:          5678                  9 ms

Day 2, part 1:          sol21                 4 ms
Day 2, part 2:          sol22                 20 ms

Day 3, part 1:          20                    125 ms
Day 3, part 2:          3200000               400 ms
```

## Come, join us 😏

There are tons of tools like mine out there written in/for multiple languages, and honestly most people don't bother or care about doing anything like that, because the only thing that [adventofcode.com](adventofcode.com) needs is final solution to the problem!

For the last two years [this repo](https://github.com/Bogdanp/awesome-advent-of-code) has been aggregating other repositories with solutions and utilities in different languages: Python, JS, TS, Ruby, Java, C++, Ada, Clojure, Prolog and a long etc. You can go there and just have a look, or open a PR to share your own ones!

**After all that wording, did I manage to convince you to challenge yourself or your classmates/friends/colleagues from December 1st?**
