#include "Graph.hpp"
#include <iostream>
#include <random>
#include <set>
#include <utility>
using namespace std;

namespace graph {

/**
 * Creates a new graph with the given number of vertices.
 *
 * @param vertices Number of vertices in the graph.
 * @throws If the number of vertices is 0 or negative.
 */
    Graph::Graph(int vertices) {
        if (vertices <= 0) {
            throw "Graph must have at least one vertex.";
        }
        numVertices = vertices;
        adjList = new Neighbor*[vertices];
        for (int i = 0; i < vertices; ++i) {
            adjList[i] = nullptr;
        }
    }

 /**
 * Frees the memory used by the graph.
 * Deletes all the neighbors and the adjacency list.
 */

    Graph::~Graph() {
        for (int i = 0; i < numVertices; ++i) {
            Neighbor* current = adjList[i];
            while (current) {
                Neighbor* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] adjList;
    }
 /**
 * Adds an edge between two vertices with a given weight.
 * Since the graph is undirected, the edge is added in both directions.
 *
 * @param from The first vertex.
 * @param to The second vertex.
 * @param weight The weight of the edge.
 * @throws If one of the vertex indexes is not valid.
 */
    void Graph::addEdge(int from, int to, int weight) {
        if (from < 0 || from >= numVertices || to < 0 || to >= numVertices) {
            throw "Invalid vertex index.";
        }

        adjList[from] = new Neighbor(to, weight, adjList[from]);
        adjList[to] = new Neighbor(from, weight, adjList[to]);
    }

 /**
 * Removes one direction of an edge from the adjacency list of a vertex.
 *
 * @param from The vertex to remove the edge from.
 * @param to The neighbor to remove.
 * @throws If the edge does not exist.
 */
    void Graph::removeFromList(int from, int to) {
        Neighbor* current = adjList[from];
        Neighbor* prev = nullptr;

        while (current) {
            if (current->vertex == to) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    adjList[from] = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }

        throw "Edge does not exist.";
    }

 /**
 * Removes an edge between two vertices from both directions.
 *
 * @param from The first vertex.
 * @param to The second vertex.
 * @throws If one of the vertex indexes is not valid,
 *         or if the edge does not exist.
 */
    void Graph::removeEdge(int from, int to) {
        if (from < 0 || from >= numVertices || to < 0 || to >= numVertices) {
            throw "Invalid vertex index.";
        }

        removeFromList(from, to);
        removeFromList(to, from);
    }

 /**
 * Prints the graph as an adjacency list.
 * Each line shows a vertex and its connected neighbors with weights.
 */
    void Graph::print_graph() const {
        for (int i = 0; i < numVertices; ++i) {
            std::cout << "Vertex " << i << ":";
            Neighbor* current = adjList[i];
            while (current) {
                std::cout << " -> (" << current->vertex << ", w=" << current->weight << ")";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
 

      bool Graph::isEven() const {
        for (int i = 0; i < numVertices; ++i) {
            int numOfNeighbors =0;
            Neighbor* current = adjList[i];
            while (current) {
                numOfNeighbors++;
                current = current->next;
            }
            if (numOfNeighbors%2==1)
            {
                return false;
            }
        }
        return true;
    }


       int Graph::getNumOfNeighbors(int i) const {
            int numOfNeighbors =0;
            Neighbor* current = adjList[i];
            while (current) {
                numOfNeighbors++;
                current = current->next;
            }
        return numOfNeighbors;
    }


 /**
 * Returns the number of vertices in the graph.
 *
 * @return Number of vertices.
 */
    int Graph::getNumVertices() const {
    return numVertices;
    }

 /**
 * Returns the adjacency list of the graph.
 *
 * @return Pointer to the adjacency list.
 */
    Neighbor** Graph::getAdjList() const {
    return adjList;
    }

    Neighbor* Graph::cloneList(Neighbor* head) {
    Neighbor* newHead = nullptr;
    Neighbor* tail = nullptr;
    for (Neighbor* p = head; p; p = p->next) {
        Neighbor* node = new Neighbor(p->vertex, p->weight, nullptr);
        if (!newHead) newHead = node;
        else          tail->next = node;
        tail = node;
    }
    return newHead;
}


Graph::Graph(const Graph& other) : numVertices(other.numVertices) {
    adjList = new Neighbor*[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        adjList[i] = Graph::cloneList(other.adjList[i]); 
    }
}


Graph& Graph::operator=(const Graph& other) {
    if (this == &other) return *this;
    Graph tmp(other);                
    std::swap(numVertices, tmp.numVertices);
    std::swap(adjList,     tmp.adjList);
    return *this;                    
}

bool Graph::isInside(set<pair<int,int>>used_pairs,int a, int b ){
     for (pair<int,int> temp : used_pairs) {
    
           if (temp.first==b && temp.second == a)
           {
            return true;
           }
           
            
        }
        return false;
}

    Graph Graph::generateRandomGraph (int v , int e){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, v-1); 

    set<pair<int,int>> used_pairs; 

    while(static_cast<int>(used_pairs.size())<e)
    {
        int a = dist(gen);
        int b = dist(gen);

        

        while (a==b || isInside(used_pairs,a,b)){
            b = dist(gen); 
        }
        
        pair<int,int> p = {a,b};
        
        used_pairs.insert(p);
    }
    Graph g(v);
    int w=0;
    for (int i = 0; i < e; i++)
    {
        w=dist(gen); 
        auto it=used_pairs.begin();
        advance(it,i);
        g.addEdge(it->first,it->second,w);
        
    }
    return g;
    

}
}
