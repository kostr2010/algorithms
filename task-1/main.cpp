#include <iostream>

#include "graph.h"

const size_t NODE_FROM = 1;
const std::string GRAPH =
    "1: 2(7) 3(9) 6(14); 2: 1(7) 3(10) 4(15); 3: 1(9) 2(10) 4(11) 6(2); 4: 2(15) "
    "3(11) 5(6); 5: 4(6) 6(9); 6: 1(14) 3(2) 5(9)";

int main() {
    graph::Graph gr;
    if (!gr.Parse(GRAPH)) {
        return 1;
    }

    const auto res = gr.Deikstra(NODE_FROM);

    std::cout << "shortest paths from node " << NODE_FROM << " to anothers:\n";
    for (size_t i = 0; i < res.size(); i++) {
        if (i == NODE_FROM - 1) {
            continue;
        }

        std::cout << "to node " << std::setw(2) << i + 1 << ": " << res[i] << std::endl;
    }

    return 0;
}