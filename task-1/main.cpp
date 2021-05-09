#include <iostream>
#include <time.h>

#include "graph.h"

const size_t NODE_FROM = 1;
// const std::string GRAPH =
//     "1: 2(7) 3(9) 6(14); 2: 1(7) 3(10) 4(15); 3: 1(9) 2(10) 4(11) 6(2); 4: 2(15) "
//     "3(11) 5(6); 5: 4(6) 6(9); 6: 1(14) 3(2) 5(9)";

const std::string GRAPH =
    "1: 2(7) 3(9) 6(14) 8(11); 2: 1(7) 3(10) 4(15); 3: 1(9) 2(10) 4(11) 6(2) 7(7) 8(6); 4: 2(15) "
    "3(11) 5(6) 7(10); 5: 4(6) 6(9) 7(9); 6: 1(14) 3(2) 5(9) 7(10) 8(3); 7: 5(9) 3(7) 4(10) 6(10); "
    "8: 1(11) 3(6) 6(3)";

int main() {
    graph::Graph gr;
    if (!gr.Parse(GRAPH)) {
        return 1;
    }

    clock_t start = clock();
    const auto res = gr.Deikstra(NODE_FROM);
    clock_t end = clock();

    std::cout << "shortest paths from node " << NODE_FROM << " to anothers:\n";
    for (size_t i = 0; i < res.size(); i++) {
        if (i == NODE_FROM - 1) {
            continue;
        }

        std::cout << "to node " << std::setw(2) << i + 1 << ": " << res[i] << std::endl;
    }

    std::cout << "execution time: " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";

    return 0;
}