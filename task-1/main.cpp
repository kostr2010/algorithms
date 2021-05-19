#include <fstream>
#include <iostream>
#include <time.h>

#include "graph.h"

const size_t NODE_FROM = 1;
const std::string GRAPH =
    "1: 2(7) 3(9) 6(14); 2: 1(7) 3(10) 4(15); 3: 1(9) 2(10) 4(11) 6(2); 4: 2(15) "
    "3(11) 5(6); 5: 4(6) 6(9); 6: 1(14) 3(2) 5(9)";

int main() {
    // graph::Graph gr;
    // if (!gr.Parse(GRAPH)) {
    //     return 1;
    // }

    // clock_t start = clock();
    // const auto res = gr.Deikstra(NODE_FROM);
    // clock_t end = clock();

    // std::cout << "shortest paths from node " << NODE_FROM << " to anothers:\n";
    // for (size_t i = 0; i < res.size(); i++) {
    //     if (i == NODE_FROM - 1) {
    //         continue;
    //     }

    //     std::cout << "to node " << std::setw(2) << i + 1 << ": " << res[i] << std::endl;
    // }

    // std::cout << "execution time: " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";

    std::ofstream out("res.csv");
    out << "nodes,time\n";

    for (size_t i = 10; i < 1000; i += 10) {
        double time = 0.0;

        for (size_t n = 0; n < 100; n++) {
            graph::Graph gr(i);

            clock_t start = clock();
            const auto res = gr.Deikstra(NODE_FROM);
            clock_t end = clock();

            time += (double)(end - start) / CLOCKS_PER_SEC;
        }

        out << i << "," << time / 100 << "\n";
    }

    return 0;
}