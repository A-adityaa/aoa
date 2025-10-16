#include <stdio.h>
#include <limits.h>

#define MAX 10
#define INF 9999

int n;                              // number of tourist spots
int graph[MAX][MAX];                // adjacency matrix
int visited[MAX];                   // visited array for backtracking
int bestPath[MAX];                  // store best (minimum cost) path
int tempPath[MAX];                  // temporary path during recursion
int minCost = INF;                  // store minimum cycle cost

// ------------ Function to check if graph is connected ------------
void dfs(int start, int visitedNodes[]) {
    visitedNodes[start] = 1;
    for (int i = 0; i < n; i++) {
        if (graph[start][i] != INF && !visitedNodes[i])
            dfs(i, visitedNodes);
    }
}

// ------------ Backtracking function to find Hamiltonian Cycle ------------
void hamiltonian(int pos, int count, int cost) {
    if (count == n && graph[pos][0] != INF) {
        int totalCost = cost + graph[pos][0];
        if (totalCost < minCost) {
            minCost = totalCost;
            for (int i = 0; i < n; i++)
                bestPath[i] = tempPath[i];
        }
        return;
    }

    for (int city = 0; city < n; city++) {
        if (!visited[city] && graph[pos][city] != INF) {
            visited[city] = 1;
            tempPath[count] = city;
            hamiltonian(city, count + 1, cost + graph[pos][city]);
            visited[city] = 0; // backtrack
        }
    }
}

// ------------ Simple MST Approximation (Prim’s Algorithm) ------------
int mstCost() {
    int selected[MAX] = {0};
    int edges = 0, total = 0;

    selected[0] = 1;

    while (edges < n - 1) {
        int min = INF, x = 0, y = 0;
        for (int i = 0; i < n; i++) {
            if (selected[i]) {
                for (int j = 0; j < n; j++) {
                    if (!selected[j] && graph[i][j] < min) {
                        min = graph[i][j];
                        x = i;
                        y = j;
                    }
                }
            }
        }
        total += min;
        selected[y] = 1;
        edges++;
    }
    return total;
}

int main() {
    printf("Enter number of tourist spots: ");
    scanf("%d", &n);

    printf("Enter cost matrix (-1 if no path):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            scanf("%d", &x);
            if (x == -1) graph[i][j] = INF;
            else graph[i][j] = x;
        }
    }

    // ---------- Check if graph is connected ----------
    int visitedNodes[MAX] = {0};
    dfs(0, visitedNodes);

    int disconnected = 0;
    for (int i = 0; i < n; i++) {
        if (!visitedNodes[i]) disconnected = 1;
    }

    if (disconnected) {
        printf("\n⚠️  Graph is disconnected! Some spots are unreachable.\n");
        printf("No single Hamiltonian Cycle possible.\n");
    } else {
        printf("\n✅ Graph is connected.\n");

        // ---------- Run Backtracking ----------
        for (int i = 0; i < n; i++) visited[i] = 0;
        visited[0] = 1;
        tempPath[0] = 0;

        hamiltonian(0, 1, 0);

        if (minCost == INF) {
            printf("\nNo Hamiltonian Cycle exists.\n");
        } else {
            printf("\nHamiltonian Cycle (Backtracking): ");
            for (int i = 0; i < n; i++) printf("%d -> ", bestPath[i]);
            printf("0\n");
            printf("Total Cost: %d\n", minCost);
        }

        // ---------- Run MST Approximation ----------
        int mst = mstCost();
        int approxCost = 2 * mst; // MST approx TSP ≈ 2×MST
        printf("\nMST Approximation Cost: %d\n", approxCost);

        printf("\nComparison:\n");
        printf("Exact (Backtracking) Cost: %d\n", minCost);
        printf("MST Approximation Cost: %d\n", approxCost);
    }

    return 0;
}
