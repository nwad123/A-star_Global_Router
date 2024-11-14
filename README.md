# Global Routing Lab

*Nick Waddoups*

## Part A

Completed comments in [IO.cpp](./src/IO.cpp).

## Part B

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
