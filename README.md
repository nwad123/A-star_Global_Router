# Global Routing Lab

*Nick Waddoups*

Notes on this project:

1. This was written for a project for my VLSI Design Automation class at Utah State.

2. Most of the code was pre-provided to me, I wrote the [A\*](https://en.wikipedia.org/wiki/A*_search_algorithm)
    search logic in the [MazeRouter](src/MazeRouter.cpp) file, and I also wrote the fixed vector
    class in the [detail header](src/detail.h). I adjusted a few functions in [SimpleGR](src/SimpleGR.h),
    but most of it is unchanged.

3. Because this was an assignment and we were supposed to contain most of our code to the `routeMaze` function in
   MazeRouter.cpp, I wrote a lot of lambda functions inside of the `routeMaze` function to keep my logic local to
   the `routeMaze` function. I think that this approach makes the code fairly readable as each operation in the
   A\* algorithm has been given a named lambda to go with it. A drawback of this approach is that none of the
   functions in the A\* algorithm are available for other code sources to use.

## Purpose

The purpose of this lab was to implement a 3-dimensional A\* search algorithm to solve chip routing
for a given chip layout. *Chip routing* is the process of determining the routes of connecting wires
through an integrated circuit. Integrated circuits (chips) are often designed by splitting the chip
into smaller blocks, then placing the blocks on the chip (using some kind of packing algorithm), and
then by connecting all the blocks. This program implements the third step of the process, and determines
the best routes for all the internal connection of the chip given a predetermined layout.

More information about the lab can be found in the [lab document](./lab_instructions.pdf).

## Installation

### Requirements

1. `make` - This project uses `make` to run the subsequent `CMake` build. If you don't have
    `make` you can still build this project by invoking `CMake` directly, but these instructions
    will assume you are letting make do it.

    Targets:
    
    - `make` : builds the `SimpleGR` and `mapper` executables
    - `make build` : same as `make`
    - `make clean` : cleans out the build directory
    - `make bundle` : bundles the code into a tar (this was used for assignment submission, so
        unless you're planning on submitting my code as an assignment this won't be too useful)

2. `cmake` - This project uses `cmake` to generate all the build files, required version is 3.5 or
    greater.

3. C++ compiler that has good support for C++17 or newer.

### Build

To build, simply type the following into your favorite shell (which should hopefully work first try,
as long as your shell isn't too esoteric):

```bash
make
```

## Usage

*After* building the project, you can run it with the following syntax:

```bash
./build/SimpleGR -f <design> [options]
```

You are required to provide design file (see attached PDF for design file format and
[simple.gr](benchmarks/simple.gr) for an example of the design files. If you don't want to
create your own design file (a wise decision) the you can use any of the designs provided by
my instructor in the [benchmarks](benchmarks/) directory.

```
-f <filename>
```

There are a few other parameters you can set as well when running from the command lin, as
shown below:

```
-o <filename>         Save routes in <filename>
-maxRipIter <uint>    Maximum rip-up and re-route iterations
-timeOut <double>     Rip-up and re-route timeout (seconds)
-maxGreedyIter <uint> Maximum greedy iterations
-h, -help             Show this page
```

## Output

The output that is directed to `stdout` provides stats about the solution that was generated:

```
GR Stats :
 Final nets routed 76927 (100%)
 Final total length for routed nets 648894
 Final total number of vias 109698
 Final total wire length 977988
 Final number of overflowing edges is 0 (0%)
 Final max overflow is 0
 Final total overflow is 0
 Final avg overflow is 0
 Final CPU time: 0.483391 seconds
```

In detail, here are the stats produced:

1. Final nets routed - the total number of wires routed on the chip

2. Final total length for routed nets - the length of all the wires that were routed

3. Final total number of vias - the number of vertical connections from one layer to the next 

4. \*overflow\* - Statistics about the overflow of the solution, see the lab document for more
    details on this.

5. Final CPU time - the run time of the program, typically lower is better.

## Answers to Lab Questions

### Part A

Completed comments in [IO.cpp](./src/IO.cpp).

### Part B

1. What is the purpose of each of the 3 stages in this routing algorithm? How are they different
    from each other? How do they work together as a flow?

    The three stages are each successively more focused than the last. The `initialRouting()`
        stage provides a baseline routing that is not very effecient, but it works. After that,
        the `doRRR()` stage performs large scale changes to the routing in order to develop a
        more effecient solution. Finally, after a better solution is obtained, the
        `greedyImprovement()` stage performs small changes to develop the best possible routing.

2. How does SimpleGR build a framework to allow the 3 stages? (Think high-level, it should be
    brief)

    The `SimpleGR` class allows for different routing algorithms to be implemented on the core
        routing data structure. This decouples the implementation of the data and the routing
        algorithm, similar to how the standard library separates *containers* and *algorithms*.
        This framework allows for arbitrary functions to implemented as separate stages, which
        allows you to build a pipeline for routing where you execute successive algorithms on the
        solution until you reach some confidence level or stopping point, then move on to the
        next algorithm.

### Part C

Completed implementing the A\* algorithm in [MazeRouter.cpp](./MazeRouter.cpp). I tested the
code on the four tests cases ([simple.gr](./benchmarks/simple.gr), [adaptec1-3.simple.gr](./benchmark))
and got results that were close to the reference outputs given in the [goldenRef](./goldenRef)
directory. The total path length for each input generated by my A\* algorithm was within 1% of the
total path lengths of the reference solution. The overflow for my solutions was also within 0.3%
of the overflow generated by the reference solutions.
