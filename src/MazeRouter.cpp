/*
 * MazeRouter.cpp
 * Work on this file to complete your maze router
 */

#include "SimpleGR.h"

#include <iomanip>

///////////////////////////////////////////////////////////////////////////////
// Implement an A* search based maze routing algorithm
// and a corresponding back-trace procedure
// The function needs to correctly deal with the following conditions:
// 1. Only search within a bounding box defined by botleft and topright points
// 2. Control if any overflow on the path is allowed or not
///////////////////////////////////////////////////////////////////////////////
CostType SimpleGR::routeMaze(Net &net,
    bool allowOverflow,
    const Point &botleft,
    const Point &topright,
    const EdgeCost &edgeCost,
    std::vector<Edge *> &path)
{
    // find out the ID of the source and sink gcells
    const IdType sourceGCellId = getGCellId(net.gCellOne);
    const IdType destGCellId = getGCellId(net.gCellTwo);
    const auto &snkGCell = getGCell(destGCellId);


#ifndef NDEBUG
    std::cout << "Calling routeMaze()\n"
              << "Bounding box:\n"
              << "\tBotLeft:  [" << botleft.x << ", " << botleft.y << ", " << botleft.z << "]\n"
              << "\tTopRight: [" << topright.x << ", " << topright.y << ", " << topright.z << "]\n"
              << "Source: " << sourceGCellId << "\n"
              << "Dest:   " << destGCellId << "\n"
              << " ----- \n\n";
#endif

    // insert the source gcell into the priority queue
    priorityQueue.setGCellCost(sourceGCellId, 0., 0., NULLID);

    // Instantiate the Cost function to calculate the Manhattan distance between
    // two gcells. This distance is used as the heuristic cost for A* search.
    ManhattanCost &manhattanDistance = ManhattanCost::getFunc();

    //@brief checks if a cell is in the bounding box
    //  Note: this function ignores the z-boundary, as when `routeMaze` is called
    //  from SimpleGR `0` is always passed for the bot left and top right coord.
    //
    //@param cell Checks if `cell` is in the bounding box
    //@return Whether or not `cell` is in the bounding box
    auto in_bounding_box = [&botleft, &topright](const Point &cell) {
        return (botleft.x <= cell.x && cell.x <= topright.x) && (botleft.y <= cell.y && cell.y <= topright.y);
    };

    //@brief given a GCell and an edgeId returns the connecting GCell
    //  Note: this function has zero error handling, so call it with
    //  valid args or else :)
    auto get_connecting_cell = [this](const GCell &from, const IdType across_edge_id) -> GCell & {
        const auto &edge = grEdgeArr[across_edge_id];
        const auto *from_ptr = &from;
        if (from_ptr == edge.gcell1) {
            return *edge.gcell2;
        } else {
            return *edge.gcell1;
        }
    };

    //@brief checks if we have reached the destination cell, this function is only
    //  to provide readability, and it's expected that it will be inlined
    //  at compile time
    auto reached_destination = [&destGCellId](const IdType &cellId) { return destGCellId == cellId; };

    //@brief gets a pointer to the edge between two cells
    //  Note: As usual, there is no error handling :)
    auto get_edge = [this](const IdType cell1_id, const IdType cell2_id) -> Edge & {
        const auto &cell1 = getGCell(cell1_id);

        const auto cell1_coord = gcellIdtoCoord(cell1_id);
        const auto cell2_coord = gcellIdtoCoord(cell2_id);

        IdType edgeId{};

        // check all possible directions the edge could be (as there isn't a better
        // way to get the edge id, which we need to get the edge reference)
        if (cell1_coord.x != cell2_coord.x) {
            if (cell1_coord.x < cell2_coord.x) {
                edgeId = cell1.incX;
            } else {
                edgeId = cell1.decX;
            }
        } else if (cell1_coord.y != cell2_coord.y) {
            if (cell1_coord.y < cell2_coord.y) {
                edgeId = cell1.incY;
            } else {
                edgeId = cell1.decY;
            }
        } else if (cell1_coord.z != cell2_coord.z) {
            if (cell1_coord.z < cell2_coord.z) {
                edgeId = cell1.incZ;
            } else {
                edgeId = cell1.decZ;
            }
        }

        auto &edge = grEdgeArr[edgeId];
        return edge;
    };

    // A* search kernel
    // Loop until all "frontiers" in the priority queue are exhausted, or when
    // the sink gcell is found.
    do {
        const auto this_cell_id = priorityQueue.getBestGCell();
        const auto this_cell_data = priorityQueue.getGCellData(this_cell_id);

        priorityQueue.rmBestGCell();

#ifndef NDEBUG
        std::cout << "\tCurrent best cell: " << this_cell_id << "\n";
#endif

        // if the current cell is the dest cell we can pop right out of this
        // loop
        if (reached_destination(this_cell_id)) { break; }

        // get all the edges from the current cell
        auto edges = getGCellEdges(this_cell_id);

        for (const auto &edgeId : edges) {
            // find the adjacent cell
            const auto &this_cell = getGCell(this_cell_id);
            const GCell &connecting_cell = get_connecting_cell(this_cell, edgeId);
            const auto connecting_cell_id = getGCellId(connecting_cell);

            // if the cell is out of the bounding box we can skip this loop
            if (!in_bounding_box(connecting_cell)) {
#ifndef NDEBUG
                std::cout << "\tSkipping cell " << connecting_cell_id << " because it has left the bounding box.\n"
                          << "\t  Cell " << connecting_cell_id << " coords: [" << connecting_cell.x << ", "
                          << connecting_cell.y << ", " << connecting_cell.z << "]\n";
#endif
                continue;
            }

            // calculate the two types of cost
            // manh_cost : heuristic cost between the connecting cell and the destination
            // edge_cost : the cost from the source cell to the connecting cell
            const auto manh_cost = manhattanDistance(connecting_cell, snkGCell);
            const auto path_cost = edgeCost(edgeId) + this_cell_data.pathCost;

            // Calculate the total cost as detailed in the PQueue.setGCellCost function
            const auto total_cost = manh_cost + path_cost;

#ifndef NDEBUG
            std::cout << "\tCost to: " << connecting_cell_id << " = " << manh_cost << " + " << path_cost << " = "
                      << manh_cost + path_cost << "\n";
#endif

            // insert the neighbor cell into the priority queue
            priorityQueue.setGCellCost(connecting_cell_id, manh_cost, total_cost, this_cell_id);
        }

#ifndef NDEBUG
        std::cout << "\n";
#endif

    } while (!priorityQueue.isEmpty());

    // now backtrace and build up the path, if we found one
    // back-track from sink to source, and fill up 'path' vector with all the edges that are traversed
    if (priorityQueue.isGCellVsted(destGCellId)) {
        // YOUR backtrace CODE GOES IN HERE
        auto current_id = destGCellId;

        while (current_id != sourceGCellId) {
            const auto &current_node = priorityQueue.getGCellData(current_id);
            const auto parent_id = current_node.parentGCell;

            auto &edge = get_edge(current_id, parent_id);
            path.push_back(&edge);// this vector expects a pointer, for some reason

            current_id = parent_id;
        }
    }

    // calculate the accumulated cost of the path
    const CostType finalCost = priorityQueue.isGCellVsted(destGCellId)
                                   ? priorityQueue.getGCellData(destGCellId).pathCost
                                   : std::numeric_limits<CostType>::max();

    // clean up
    priorityQueue.clear();

    return finalCost;
}
