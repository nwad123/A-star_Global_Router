#include <iostream>

#include "SimpleGR.h"

void printTitle()
{
    std::cout << "SimpleGR " << SimpleGRversion << " (" << sizeof(void *) * 8 << "-bit) Compiled on " << __DATE__
              << " at " << __TIME__ << "\n\n";
}

/* $begin main */
int main(int argc, char **argv)
{
    printTitle();
    // init parameters of SimpleGR
    SimpleGRParams params(argc, argv);
    // instantiate an instance of SimpleGR
    SimpleGR simplegr(params);

    // read in the nets and create the grid input file
    simplegr.parseInput();
    simplegr.printParams();

    // perform 3-stage global routing
    simplegr.initialRouting();
    simplegr.printStatistics();

    simplegr.doRRR();
    simplegr.printStatistics();

    simplegr.greedyImprovement();
    simplegr.printStatistics(true, true);

    // output solution file
    simplegr.writeRoutes();

    return 0;
}
/* $end main */
