#pragma once

#include "stradegy.hpp"
#include<string>
#include <memory>
#include <algorithm>  // std::transform, std::replace
#include <cctype>


class factory{
    public:
    factory(){}

    //  std::unique_ptr<stradegy> create(string name){
    //     if(name=="Euler circle"){return std::make_unique<Euler_circle>();}
    //     if(name=="max flow"){return std::make_unique<fordFulkerson>();}
    //     if(name=="count cliques"){return std::make_unique<countCliques>();}
    //     if(name=="find Max clique"){return std::make_unique<findMaxClique>();}
    //     if(name=="MST"){return std::make_unique<kruskalMST>();}

    //     throw std::invalid_argument("Unknown strategy: " + name);
    // }
 std::unique_ptr<stradegy> create(const std::string& name) {
        std::string norm = name;
        std::transform(norm.begin(), norm.end(), norm.begin(), ::tolower);

        if (norm == "euler circle" || norm == "euler")
            return std::make_unique<Euler_circle>();

        if (norm == "max flow" || norm == "ford fulkerson")
            return std::make_unique<fordFulkerson>();

        if (norm == "max clique" || norm == "find max clique" || norm == "count cliques")
            return std::make_unique<findMaxClique>();

        if (norm == "mst" || norm == "kruskal")
            return std::make_unique<kruskalMST>();

        throw std::invalid_argument("Unknown strategy: " + name);
    }




};
