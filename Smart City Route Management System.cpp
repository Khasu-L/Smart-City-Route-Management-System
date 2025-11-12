/*
  Smart City Route Management System
  Student: Lebohang Khasu
  Student Number: 17955572
  */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

// Route structure to store each route
struct Route {
    string start;
    string end;
    int distance; // distance in km
};

// Comparator for sorting routes by distance
struct CompareRoute {
    bool operator()(const Route &r1, const Route &r2) {
        return r1.distance < r2.distance;
    }
};

// CityGraph class manages the network of routes
class CityGraph {
private:
    unordered_map<string, vector<pair<string,int>>> adjList;
    vector<Route> allRoutes; // Keep all routes for sorting/searching

public:
    // Add a new route
    void addRoute(string start, string end, int distance) {
        adjList[start].push_back({end, distance});
        allRoutes.push_back({start, end, distance});
        cout << "Route " << start << " -> " << end << " added with distance " << distance << " km.\n";
        // XAI: Route is added to adjacency list and allRoutes vector for management and sorting
    }

    // Remove a route
    void removeRoute(string start, string end) {
        if(adjList.count(start)) {
            auto &neighbors = adjList[start];
            neighbors.erase(remove_if(neighbors.begin(), neighbors.end(),
                                      [&](pair<string,int> p){ return p.first == end; }),
                            neighbors.end());
        }

        allRoutes.erase(remove_if(allRoutes.begin(), allRoutes.end(),
                                  [&](Route r){ return r.start==start && r.end==end; }),
                        allRoutes.end());

        cout << "Route " << start << " -> " << end << " removed.\n";
        // XAI: We remove the route from both the adjacency list and the route collection for accurate representation
    }

    // Update an existing route
    void updateRoute(string start, string end, int newDistance) {
        bool updated = false;
        if(adjList.count(start)) {
            for(auto &p : adjList[start]) {
                if(p.first == end) { p.second = newDistance; updated = true; break; }
            }
        }
        for(auto &r : allRoutes) {
            if(r.start == start && r.end == end) { r.distance = newDistance; updated = true; break; }
        }
        if(updated)
            cout << "Route " << start << " -> " << end << " updated to " << newDistance << " km.\n";
        else
            cout << "Route not found.\n";
        // XAI: Updating distance ensures the graph and route list remain consistent
    }

    // Display all routes
    void displayRoutes() {
        cout << "All routes in the city:\n";
        for(auto &r : allRoutes)
            cout << r.start << " -> " << r.end << " : " << r.distance << " km\n";
        // XAI: Display shows all known routes for transparency
    }

    // Sort routes by distance
    void sortRoutesByDistance() {
        sort(allRoutes.begin(), allRoutes.end(), CompareRoute());
        cout << "Routes sorted by distance:\n";
        for(auto &r : allRoutes)
            cout << r.start << " -> " << r.end << " : " << r.distance << " km\n";
        // XAI: Sorting allows officials to see shortest routes first
    }

    // Dijkstra algorithm to find shortest path
    void dijkstra(string start, string end) {
        unordered_map<string,int> dist;
        unordered_map<string,string> prev;
        set<string> visited;
        for(auto &p : adjList) dist[p.first] = INT_MAX;
        dist[start] = 0;

        // Min-heap priority queue: pair(distance, node)
        priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> pq;
        pq.push({0, start});

        while(!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if(visited.count(u)) continue;
            visited.insert(u);

            // XAI: Node selected with current shortest known distance
            cout << "Node " << u << " selected with distance " << d << "\n";

            for(auto &[v, w] : adjList[u]) {
                if(dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    pq.push({dist[v], v});
                    // XAI: Distance updated for neighbor " << v << " to " << dist[v] << " km
                }
            }
        }

        if(dist.find(end) == dist.end() || dist[end] == INT_MAX) {
            cout << "No path found from " << start << " to " << end << ".\n";
            return;
        }

        // Reconstruct path
        vector<string> path;
        string at = end;
        while(at != start) {
            path.push_back(at);
            at = prev[at];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        cout << "Shortest path from " << start << " to " << end << ":\n";
        for(size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if(i != path.size()-1) cout << " -> ";
        }
        cout << "\nTotal distance: " << dist[end] << " km\n";
        // XAI: Path chosen because it has the minimal cumulative distance using Dijkstra's algorithm
    }
};

// Menu interface
void menu() {
    CityGraph city;
    int choice;
    do {
        cout << "\nMENU:\n";
        cout << "1. Add a route\n";
        cout << "2. Remove a route\n";
        cout << "3. Update a route\n";
        cout << "4. View all routes\n";
        cout << "5. Find shortest path\n";
        cout << "6. Sort routes by distance\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        string start, end;
        int distance;
        switch(choice) {
            case 1:
                cout << "Enter start node: "; cin >> start;
                cout << "Enter end node: "; cin >> end;
                cout << "Enter distance: "; cin >> distance;
                city.addRoute(start, end, distance);
                break;
            case 2:
                cout << "Enter start node: "; cin >> start;
                cout << "Enter end node: "; cin >> end;
                city.removeRoute(start, end);
                break;
            case 3:
                cout << "Enter start node: "; cin >> start;
                cout << "Enter end node: "; cin >> end;
                cout << "Enter new distance: "; cin >> distance;
                city.updateRoute(start, end, distance);
                break;
            case 4:
                city.displayRoutes();
                break;
            case 5:
                cout << "Enter start node: "; cin >> start;
                cout << "Enter end node: "; cin >> end;
                city.dijkstra(start, end);
                break;
            case 6:
                city.sortRoutesByDistance();
                break;
            case 7:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while(choice != 7);
}

int main() {
    cout << "Smart City Route Management System\n";
    menu();
    return 0;
}

/*
Documentation:

Problem Analysis:
- The city network is represented as a directed graph with intersections as nodes and routes as edges.
- Routes have attributes such as distance, allowing route optimisation and management.

Data Structures Used:
1. unordered_map<string, vector<pair<string,int>>> -> Graph representation for O(1) node access and adjacency storage.
2. vector<Route> -> Store all routes for sorting and searching.
3. priority_queue -> Used in Dijkstra's algorithm to always select the node with minimal distance.
4. set -> To track visited nodes during Dijkstra.

Algorithms:
- Dijkstra's algorithm: Finds the shortest path from a source to a destination.
- Sorting routes using CompareRoute for clarity in route management.

XAI Principles Applied:
- Each key decision in Dijkstra is output with reasoning.
- Console outputs explain why nodes and routes are selected or updated.
- Comments in code indicate reasoning behind route selection, sorting, and updates.
*/
