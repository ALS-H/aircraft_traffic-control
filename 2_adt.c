#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    int aircraft_id;
    double position[3];
    double velocity[3];
    double fuel;
} Aircraft;

typedef struct GraphNode {
    Aircraft aircraft;
    struct GraphNode* next;
} GraphNode;

typedef struct {
    GraphNode** adjacency_list;
    int aircraft_count;
} AirTrafficControlGraph;

typedef struct {
    AirTrafficControlGraph graph;
    // Add additional data structures for real-time tracking, routing, monitoring, and control
    // ...
} AirTrafficControlSystem;

double calculateDistance(Aircraft aircraft1, Aircraft aircraft2) {
    return sqrt(pow(aircraft1.position[0] - aircraft2.position[0], 2) +
                pow(aircraft1.position[1] - aircraft2.position[1], 2) +
                pow(aircraft1.position[2] - aircraft2.position[2], 2));
}

Aircraft createAircraft(int id, double x, double y, double z, double vx, double vy, double vz) {
    Aircraft newAircraft;
    newAircraft.aircraft_id = id;
    newAircraft.position[0] = x;
    newAircraft.position[1] = y;
    newAircraft.position[2] = z;
    newAircraft.velocity[0] = vx;
    newAircraft.velocity[1] = vy;
    newAircraft.velocity[2] = vz;
    newAircraft.fuel = 5000.0; // Initial fuel value (you can adjust this)
    return newAircraft;
}

GraphNode* createGraphNode(Aircraft aircraft) {
    GraphNode* newNode = (GraphNode*)malloc(sizeof(GraphNode));
    newNode->aircraft = aircraft;
    newNode->next = NULL;
    return newNode;
}

AirTrafficControlSystem initializeATCSystem(int capacity) {
    AirTrafficControlSystem atcSystem;
    atcSystem.graph.adjacency_list = (GraphNode*)malloc(capacity * sizeof(GraphNode));
    atcSystem.graph.aircraft_count = 0;
    // Initialize other components of the air traffic control system
    // ...
    return atcSystem;
}

void addAircraftToGraph(AirTrafficControlSystem* system, Aircraft newAircraft) {
    GraphNode* newNode = createGraphNode(newAircraft);
    system->graph.adjacency_list[system->graph.aircraft_count++] = newNode;
}

void updateAircraftPositions(AirTrafficControlSystem* system) {
    for (int i = 0; i < system->graph.aircraft_count; ++i) {
        Aircraft* currentAircraft = &system->graph.adjacency_list[i]->aircraft;
        currentAircraft->position[0] += currentAircraft->velocity[0];
        currentAircraft->position[1] += currentAircraft->velocity[1];
        currentAircraft->position[2] += currentAircraft->velocity[2];

        // Deduct fuel based on velocity (example: fuel consumption rate of 0.1 per unit velocity)
        double fuelConsumption = 0.1 * (fabs(currentAircraft->velocity[0]) +
                                        fabs(currentAircraft->velocity[1]) +
                                        fabs(currentAircraft->velocity[2]));
        currentAircraft->fuel -= fuelConsumption;
    }
}

void detectAndResolveCollisions(AirTrafficControlSystem* system) {
    double collisionThreshold = 1.0;

    for (int i = 0; i < system->graph.aircraft_count; ++i) {
        Aircraft* currentAircraft = &system->graph.adjacency_list[i]->aircraft;

        for (int j = i + 1; j < system->graph.aircraft_count; ++j) {
            Aircraft* otherAircraft = &system->graph.adjacency_list[j]->aircraft;

            double distance = calculateDistance(*currentAircraft, *otherAircraft);

            if (distance < collisionThreshold) {
                printf("Collision detected between aircraft %d and %d!\n",
                       currentAircraft->aircraft_id, otherAircraft->aircraft_id);

                // Resolve collision (simplified, reverse direction)
                currentAircraft->velocity[0] = -currentAircraft->velocity[0];
                currentAircraft->velocity[1] = -currentAircraft->velocity[1];
                currentAircraft->velocity[2] = -currentAircraft->velocity[2];

                otherAircraft->velocity[0] = -otherAircraft->velocity[0];
                otherAircraft->velocity[1] = -otherAircraft->velocity[1];
                otherAircraft->velocity[2] = -otherAircraft->velocity[2];
            }
        }
    }
}

void changeAircraftVelocity(Aircraft* aircraft) {
    // Ask user for velocity changes
    printf("Enter new X velocity for aircraft %d: ", aircraft->aircraft_id);
    scanf("%lf", &aircraft->velocity[0]);

    printf("Enter new Y velocity for aircraft %d: ", aircraft->aircraft_id);
    scanf("%lf", &aircraft->velocity[1]);

    printf("Enter new Z velocity for aircraft %d: ", aircraft->aircraft_id);
    scanf("%lf", &aircraft->velocity[2]);
}

const char* checkFuelEfficiency(const Aircraft* aircraft) {
    // Placeholder logic for fuel efficiency assessment
    if (aircraft->fuel > 4000.0) {
        return "Efficient";
    } else if (aircraft->fuel > 2000.0) {
        return "Moderate";
    } else {
        return "Inefficient";
    }
}

void simulateAirTraffic(AirTrafficControlSystem* system, int simulation_steps) {
    for (int step = 0; step < simulation_steps; ++step) {
        updateAircraftPositions(system);
        detectAndResolveCollisions(system);

        // Ask user for velocity changes to avoid collisions
        for (int i = 0; i < system->graph.aircraft_count; ++i) {
            Aircraft* currentAircraft = &system->graph.adjacency_list[i]->aircraft;
            changeAircraftVelocity(currentAircraft);
        }

        // Check and display fuel efficiency
        for (int i = 0; i < system->graph.aircraft_count; ++i) {
            Aircraft* currentAircraft = &system->graph.adjacency_list[i]->aircraft;
            const char* efficiencyStatus = checkFuelEfficiency(currentAircraft);
            printf("Aircraft %d fuel efficiency: %s\n", currentAircraft->aircraft_id, efficiencyStatus);
        }

        // Add calls to other functions for routing, monitoring, and control
        // ...
    }
}

void freeATCSystem(AirTrafficControlSystem* system) {
    for (int i = 0; i < system->graph.aircraft_count; ++i) {
        free(system->graph.adjacency_list[i]);
    }
    free(system->graph.adjacency_list);
    // Free memory for other components
    //
    // ...
}


int main() {
    // User input for capacity and aircraft
    int capacity;
    printf("Enter the initial capacity: ");
    scanf("%d", &capacity);

    int numAircraft;
    printf("Enter the number of aircraft: ");
    scanf("%d", &numAircraft);

    if (numAircraft > capacity) {
        printf("Error: Number of aircraft exceeds the initial capacity.\n");
        return 1; // Exit with an error code
    }

    // Initialize air traffic control system
    AirTrafficControlSystem atcSystem = initializeATCSystem(capacity);

    for (int i = 0; i < numAircraft; ++i) {
        int id;
        double x, y, z, vx, vy, vz;

        printf("Enter details for Aircraft %d:\n", i + 1);
        printf("Aircraft ID: ");
        scanf("%d", &id);

        printf("Initial X position: ");
        scanf("%lf", &x);

        printf("Initial Y position: ");
        scanf("%lf", &y);

        printf("Initial Z position: ");
        scanf("%lf", &z);

        printf("X velocity: ");
        scanf("%lf", &vx);

        printf("Y velocity: ");
        scanf("%lf", &vy);

        printf("Z velocity: ");
        scanf("%lf", &vz);

        // Add aircraft to the graph
        addAircraftToGraph(&atcSystem, createAircraft(id, x, y, z, vx, vy, vz));
    }

    // Simulation steps
    int simulationSteps;
    printf("Enter the number of simulation steps: ");
    scanf("%d", &simulationSteps);

    // Simulate air traffic
    simulateAirTraffic(&atcSystem, simulationSteps);

    // Free allocated memory
    freeATCSystem(&atcSystem);

    return 0;
}