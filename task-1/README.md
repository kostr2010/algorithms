# DESCRIPTION

this program takes string-like representation of graph and source node to start from, prints out shortest paths from this node to all the others. to change values, change variables `NODE_FROM` & `GRAPH` in file `main.cpp`

# EXAMPLE

```C
// main.cpp

const size_t NODE_FROM = 1;
const std::string GRAPH =
    "1: 2(7) 3(9) 6(14); 2: 1(7) 3(10) 4(15); 3: 1(9) 2(10) 4(11) 6(2); 4: 2(15) "
    "3(11) 5(6); 5: 4(6) 6(9); 6: 1(14) 3(2) 5(9)";
```

output:

```
$ ./main
shortest paths from node 1 to anothers:
to node  2: 7
to node  3: 9
to node  4: 20
to node  5: 20
to node  6: 11
```

# BUILD & RUN

to build, execute

```bash
$ g++ -o main main.c
```

to run, execute

```bash
$ ./main
```
