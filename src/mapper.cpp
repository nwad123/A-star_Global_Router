#include <iostream>
#include <string>

#include "SimpleGR.h"

void usage(const char *exename)
{
    std::cout << "Usage: " << exename << " <benchname> <solnname> [options]\n";
    std::cout << "Available options:\n";
    std::cout << "  -o <filename>         Save using base <filename>\n" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        usage(argv[0]);
        return 0;
    }

    std::string outputName = "congestion";

    for (int i = 3; i < argc; ++i) {
        if (argv[i] == std::string("-h") || argv[i] == std::string("-help")) {
            usage(argv[0]);
            return 0;
        }
        if (argv[i] == std::string("-o")) {
            if (i + 1 < argc) {
                outputName = argv[i + 1];
            } else {
                std::cout << "option -o requires an argument" << std::endl;
                usage(argv[0]);
                return 0;
            }
        }
    }

    SimpleGRParams parms;
    SimpleGR simplegr(parms);

    simplegr.parseInputMapper(argv[1]);

    simplegr.parseSolution(argv[2]);

    bool noCheckRouted = false;
    simplegr.printStatistics(noCheckRouted);

    simplegr.plotXPM(outputName);

    return 0;
}
