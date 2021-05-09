#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <utility>
#include <vector>

#define DEBUG
#undef DEBUG

namespace graph {

typedef std::vector<std::vector<size_t>> table_t;

enum class TokenType {
    NUMBER,
    DELIM_L_BRACE,
    DELIM_R_BRACE,
    DELIM_SEMICOLON,
    DELIM_COLON,

    INVALID,
};

struct Token {
    Token(const std::string& str, const TokenType t) : token(str), type(t), token_num(0) {
    }

    Token(const std::string& str, const size_t num, const TokenType t)
        : token(str), type(t), token_num(num) {
    }

    ~Token() = default;

    std::string token;
    size_t token_num;
    TokenType type;
};

typedef std::vector<Token> context_t;

class Graph {
  public:
    Graph() = default;
    ~Graph() = default;

    bool Parse(const std::string& list) {
        if (list.empty()) {
            true;
        }

        context_t con{};

        if (!Tokenize(list, &con)) {
            err = 1;

            return false;
        }

        if (!ParseList(con)) {
            err = 1;

            return false;
        }

#ifdef DEBUG
        std::cout << "parsed graph:\n";

        for (int i = 0; i < links.size(); i++) {
            for (int j = 0; j < links.size(); j++) {
                std::cout << std::setw(2) << links[i][j] << ", ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
#endif

        return true;
    }

    std::vector<size_t> Deikstra(size_t from) {
        if (links.empty()) {
            return {};
        }

        // because real life indexation starts from 1
        from--;

        // size_t tag - shortest path length
        // bool is_visited - if this node was already visited
        std::vector<std::pair<size_t, bool>> nodes;
        size_t n_nodes = links.size();
        nodes.resize(n_nodes);

        for (size_t i = 0; i < n_nodes; i++) {
            nodes[i].first = std::numeric_limits<size_t>::max();
            nodes[i].second = false;
        }
        nodes[from].first = 0;

        // call function for each connected node - O(n * func_complexity)
        for (int i = from; i != -1; i = TagNeighbours(i, &nodes)) {
        }

        std::vector<size_t> res{};
        for (const auto& pair : nodes) {
            res.push_back(pair.first);
        }

        return res;
    }

  private:
    Graph(const table_t& link_list) : links(link_list) {
    }

    int TagNeighbours(size_t node, std::vector<std::pair<size_t, bool>>* nodes) {
        std::vector<size_t> queue{};

#ifdef DEBUG
        std::cout << "tagging neighbours of " << node << std::endl;
#endif

        // NOTE: select all connected and unvisited nodes - O(n)
        for (size_t i = 0; i < links.size(); i++) {
            size_t cost = links[node][i];

            if (cost != 0 && !(*nodes)[i].second) {
                queue.push_back(i);
            }
        }

#ifdef DEBUG
        std::cout << "neighbours are:" << std::endl;
        std::cout << "<";
        for (size_t i = 0; i < queue.size(); i++) {
            if (i != queue.size() - 1) {
                std::cout << queue[i] << ", ";
            } else {
                std::cout << queue[i];
            }
        }
        std::cout << ">" << std::endl;
#endif

        if (queue.empty()) {
            return -1;
        }

        // NOTE: bubble sort - O(n^2)
        bool is_sorted = false;

        while (!is_sorted) {
            is_sorted = true;

            for (size_t i = 0; i < queue.size() - 1; i++) {
                if ((*nodes)[queue[i]].first > (*nodes)[queue[i + 1]].first) {
                    std::swap(queue[i], queue[i + 1]);

                    is_sorted = false;
                }
            }
        }

#ifdef DEBUG
        std::cout << "sorted neighbours:" << std::endl;
        std::cout << "<";
        for (size_t i = 0; i < queue.size(); i++) {
            if (i != queue.size() - 1) {
                std::cout << queue[i] << ", ";
            } else {
                std::cout << queue[i];
            }
        }
        std::cout << ">" << std::endl;
#endif

        // NOTE: updating tags of every neigbour - O(n)
        for (const auto& i : queue) {
            size_t cost = links[node][i];
            size_t new_cost = (*nodes)[node].first + cost;

            if (new_cost < (*nodes)[i].first) {
                (*nodes)[i].first = new_cost;
            }
        }

        (*nodes)[node].second = true;

#ifdef DEBUG
        std::cout << "updated map is: (tag|is_visited)" << std::endl;

        for (size_t i = 0; i < nodes->size(); i++) {
            std::cout << (*nodes)[i].first << "|" << (*nodes)[i].second << ", ";
        }
        std::cout << std::endl;
#endif

        // NOTE: select next node with minimal tag - O(n)
        bool first = true;
        int next = -1;

        for (size_t i = 0; i < links.size(); i++) {
            if (!(*nodes)[i].second) {
                if (first) {
                    next = i;
                    first = false;
                }

                if ((*nodes)[i].first < (*nodes)[next].first) {
                    next = i;
                }
            }
        }

        return next;
    }

    bool Tokenize(const std::string& list, context_t* con) {
        size_t iter = 0;

        while (iter < list.size()) {
            switch (list[iter]) {
            case ':': {
                con->emplace_back(std::string(1, list[iter]), TokenType::DELIM_COLON);
                iter++;
                break;
            }
            case ';': {
                con->emplace_back(std::string(1, list[iter]), TokenType::DELIM_SEMICOLON);
                iter++;
                break;
            }
            case ')': {
                con->emplace_back(std::string(1, list[iter]), TokenType::DELIM_R_BRACE);
                iter++;
                break;
            }
            case '(': {
                con->emplace_back(std::string(1, list[iter]), TokenType::DELIM_L_BRACE);
                iter++;
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                size_t num = atoi(list.data() + iter);
                std::string num_str{};

                while (isdigit(list[iter])) {
                    num_str.push_back(list[iter]);
                    iter++;
                }

                con->emplace_back(num_str, num, TokenType::NUMBER);
                break;
            }
            case ' ': {
                while (isblank(list[iter])) {
                    iter++;
                }
                break;
            }
            default: {
                return false;
            }
            }
        }

        return true;
    }

    bool ParseList(const context_t& con) {
        size_t iter = 0;

        while (ParseNode(con, &iter)) {
        }

        return true;
    }

    bool ParseNode(const context_t& con, size_t* iter) {
        if (err != 0) {
            return false;
        }

        if (con[*iter].type != TokenType::NUMBER) {
            return false;
        }

        const size_t node_n = con[*iter].token_num;

        if (node_n > links.size()) {
            links.resize(node_n);
            for (size_t i = 0; i < node_n; i++) {
                links[i].resize(node_n);
            }
        }

        (*iter)++;

        if (con[*iter].type != TokenType::DELIM_COLON) {
            return false;
        }

        (*iter)++;

        while (ParseLink(con, iter, node_n)) {
        }

        if (con[*iter].type != TokenType::DELIM_SEMICOLON) {
            return false;
        }

        (*iter)++;

        return true;
    }

    bool ParseLink(const context_t& con, size_t* iter, const size_t node_n) {
        if (err != 0) {
            return false;
        }

        if (con[*iter].type != TokenType::NUMBER) {
            return false;
        }

        const size_t link_to = con[*iter].token_num;

        if (link_to > links.size()) {
            links.resize(link_to);
            for (size_t i = 0; i < link_to; i++) {
                links[i].resize(link_to);
            }
        }

        (*iter)++;

        if (con[*iter].type != TokenType::DELIM_L_BRACE) {
            return false;
        }

        (*iter)++;

        if (con[*iter].type != TokenType::NUMBER) {
            return false;
        }

        links[node_n - 1][link_to - 1] = con[*iter].token_num;

        (*iter)++;

        if (con[*iter].type != TokenType::DELIM_R_BRACE) {
            return false;
        }

        (*iter)++;

        return true;
    }

    bool ParseWhitespace(const std::string& list, size_t* iter) {
        if (err != 0) {
            return false;
        }

        return true;
    }

    table_t links;
    int err = 0;
};
}; // namespace graph

#endif