// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
#include <iostream>
#include <cassert>
#include <iomanip>
#include "amongus.h"


using namespace std;

int main(int argc, char *argv[]) {

    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2);
    cout << std::fixed;
    Graph graph;

    /*-------------------read the command---------------------*/

    for (int i = 1; i < argc; ++i) {
        if (argv[i][1] == 'h' || argv[i][2] == 'h'){
            cout << "Are you kidding me ?\n";
            exit(0);
        } else if (argv[i][1] == 'm' || argv[i][2] == 'm'){
            i += 1;
            graph.mode = argv[i][0];
        }
    }

    /*-------------------read the nodes---------------------*/
    graph.readVertex();

    /*-------------------run the program---------------------*/
    if (graph.mode == 'M')
        graph.runMST();
    else if (graph.mode == 'F')
        graph.runFASTTSP();
    else
        graph.runOPTTSP();

    return 0;
}
