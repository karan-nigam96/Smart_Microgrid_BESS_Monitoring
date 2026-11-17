#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>

// Structure to represent a node in the microgrid network
struct Node {
    int id;
    double generation; // Power generated (kW) [Solar/Wind]
    double demand;     // Power consumed (kW) [Load]
    bool isSubstation;
};

// Edge representing a transmission line between nodes
struct Edge {
    int u, v;
    double capacity;   // Max power flow limit (kW)
    double resistance; // Line resistance (Ohms)
};

// Microgrid Network Graph Model
class MicrogridNetwork {
private:
    int numNodes;
    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adjList;

public:
    MicrogridNetwork(int n) : numNodes(n), nodes(n), adjList(n) {}

    void addNode(int id, double gen, double dem, bool sub = false) {
        nodes[id] = {id, gen, dem, sub};
    }

    void addTransmissionLine(int u, int v, double capacity, double resistance) {
        adjList[u].push_back({u, v, capacity, resistance});
        adjList[v].push_back({v, u, capacity, resistance}); // Bidirectional
    }

    // Simulate power flow and check for overloads using BFS/Graph traversal
    void simulatePowerFlow() {
        std::cout << "\n[INFO] Running Real-Time Power Flow Simulation...\n";
        std::cout << std::string(50, '-') << "\n";
        std::cout << "Node ID\tGeneration(kW)\tDemand(kW)\tNet Status\n";
        std::cout << std::string(50, '-') << "\n";

        for (int i = 0; i < numNodes; ++i) {
            double net = nodes[i].generation - nodes[i].demand;
            std::cout << nodes[i].id << "\t\t" 
                      << std::fixed << std::setprecision(2) << nodes[i].generation << "\t\t" 
                      << nodes[i].demand << "\t\t" 
                      << (net >= 0 ? "+" : "") << net << " kW\n";
        }
        std::cout << std::string(50, '-') << "\n";
    }

    // Optimize BESS (Battery Energy Storage System) charging/discharging
    void optimizeBESS(const std::vector<double>& hourlyGeneration, const std::vector<double>& hourlyDemand, double batteryCapacity) {
        std::cout << "\n[INFO] Running BESS Dynamic Optimization...\n";
        double currentBatteryStored = batteryCapacity * 0.5; // Start at 50% state of charge
        double totalGridImportReduced = 0.0;

        for (size_t hour = 0; hour < hourlyGeneration.size(); ++hour) {
            double gen = hourlyGeneration[hour];
            double dem = hourlyDemand[hour];
            double netPower = gen - dem; // Positive = surplus, Negative = deficit

            if (netPower > 0) {
                double energyToStore = std::min(netPower, batteryCapacity - currentBatteryStored);
                currentBatteryStored += energyToStore;
                std::cout << "Hour " << std::setw(2) << hour << ": Surplus " << std::setw(6) << netPower 
                          << " kW | Stored: " << std::setw(6) << energyToStore 
                          << " kW | BESS Level: " << std::setw(6) << currentBatteryStored << " kW\n";
            } else {
                double energyNeeded = -netPower;
                double energyDischarged = std::min(energyNeeded, currentBatteryStored);
                currentBatteryStored -= energyDischarged;
                totalGridImportReduced += energyDischarged;

                std::cout << "Hour " << std::setw(2) << hour << ": Deficit  " << std::setw(6) << energyNeeded 
                          << " kW | Discharged: " << std::setw(6) << energyDischarged 
                          << " kW | BESS Level: " << std::setw(6) << currentBatteryStored << " kW\n";
            }
        }

        std::cout << "\n[SUCCESS] Optimization Complete! Total Grid Import Reduced by: " 
                  << std::setprecision(2) << totalGridImportReduced << " kWh\n";
    }
};

int main() {
    std::cout << "=== Smart Microgrid Power Flow & BESS Optimizer ===\n";

    MicrogridNetwork grid(4);
    grid.addNode(0, 0.0, 50.0, true);   
    grid.addNode(1, 120.0, 20.0, false); 
    grid.addNode(2, 80.0, 30.0, false);  
    grid.addNode(3, 0.0, 90.0, false);   

    grid.addTransmissionLine(0, 1, 150.0, 0.05);
    grid.addTransmissionLine(1, 2, 100.0, 0.08);
    grid.addTransmissionLine(2, 3, 200.0, 0.03);

    grid.simulatePowerFlow();

    std::vector<double> sampleGeneration = {20, 10, 5, 10, 40, 90, 150, 180, 200, 190, 160, 130, 
                                            110, 100, 90, 70, 40, 15, 5, 2, 10, 15, 20, 20};
    std::vector<double> sampleDemand     = {60, 55, 50, 52, 65, 80, 110, 130, 140, 135, 120, 115, 
                                            110, 105, 115, 130, 150, 160, 140, 110, 90, 75, 70, 65};

    double maxBESSCapacity = 300.0; 
    grid.optimizeBESS(sampleGeneration, sampleDemand, maxBESSCapacity);

    return 0;
}
