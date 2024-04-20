# Banker's algorithm: Cappy's implementation

A friend asked me to help them copy someone else's assignment on implementing Djikstra's Banker's Algorithm, and to "change it up a bit so it doesn't look obvious I copied".

So I answered.

Featuring... The Object-oriented(?) Banker solver. Written in robust C code, and a modular design.

The original code can be found [here](./reference.c).

## Differences

- Refactored most of the functions, no more camelCase.
- To honor E. W. Dijkstra, I've completely removed the use of `goto` statements.
- The aforementioned `goto` statements were replaced with `while` loops, which are much more readable, and significantly reduced the LOC count.
- The massive `allocate_resources` function was refactored into smaller functions, with descriptive names. Now the function is just a wrapper for the smaller functions.

## Afterthoughts

Honestly, this is actually the first actual C program I've written. I've been writing Rust for a while now, and I've been spoiled by the compiler's helpful error messages. And funnily enough, it compiles on the first try.

This was a fun exercise and actually taught me a lot about C. I hope I don't have to write C again, but if I do, at least I know how to do it.

Rust version coming soon to a GitHub near you.
