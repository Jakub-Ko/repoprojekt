#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

const int GRID_SIZE = 20;

struct Node {
    int x, y;
    double g;
    double h;
    double f;
    int order;
    int px, py;
};

double euclidean_distance(int x1, int y1, int x2, int y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main() {
    int grid[GRID_SIZE][GRID_SIZE];

    std::ifstream file("grid.txt");
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku grid.txt!" << std::endl;
        return 1;
    }

    for (int r = 0; r < GRID_SIZE; ++r) {
        int y = (GRID_SIZE - 1) - r;
        for (int x = 0; x < GRID_SIZE; ++x) {
            file >> grid[y][x];
        }
    }
    file.close();

    const int start_x = 0, start_y = 0;
    const int goal_x = 19, goal_y = 19;

    bool closed[GRID_SIZE][GRID_SIZE] = { false };
    double g_cost[GRID_SIZE][GRID_SIZE];
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            g_cost[y][x] = 1e9;
        }
    }

    std::vector<Node> open_list;
    int order_counter = 0;

    // punkt startowy
    double h_start = euclidean_distance(start_x, start_y, goal_x, goal_y);
    open_list.push_back({start_x, start_y, 0.0, h_start, h_start, ++order_counter, -1, -1});
    g_cost[start_y][start_x] = 0.0;

    int parent_x[GRID_SIZE][GRID_SIZE];
    int parent_y[GRID_SIZE][GRID_SIZE];
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            parent_x[y][x] = -1;
            parent_y[y][x] = -1;
        }
    }

    const int dx[] = { 0,  0, -1, 1 };
    const int dy[] = { 1, -1,  0, 0 };

    bool goal_reached = false;

    while (!open_list.empty()) {
        // o najmniejszym f
        int best_idx = 0;
        for (size_t i = 1; i < open_list.size(); ++i) {
            if (open_list[i].f < open_list[best_idx].f - 1e-9) {
                best_idx = i;
            } else if (std::abs(open_list[i].f - open_list[best_idx].f) <= 1e-9) {
                if (open_list[i].order > open_list[best_idx].order) {
                    best_idx = i;
                }
            }
        }

        Node current = open_list[best_idx];
        open_list.erase(open_list.begin() + best_idx);

        closed[current.y][current.x] = true;
        parent_x[current.y][current.x] = current.px;
        parent_y[current.y][current.x] = current.py;

        if (current.x == goal_x && current.y == goal_y) {
            goal_reached = true;
            break;
        }

        // sasiedzi
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            // granice, przeszkody, lista zamknieta
            if (nx < 0 || nx >= GRID_SIZE || ny < 0 || ny >= GRID_SIZE) continue;
            if (grid[ny][nx] == 5 || closed[ny][nx]) continue;

            double tentative_g = current.g + 1.0;

            if (tentative_g < g_cost[ny][nx]) {
                g_cost[ny][nx] = tentative_g;
                double h = euclidean_distance(nx, ny, goal_x, goal_y);
                double f = tentative_g + h;

                // sasiad na liscie otwartej
                auto it = std::find_if(open_list.begin(), open_list.end(),
                    [nx, ny](const Node& n) { return n.x == nx && n.y == ny; });

                if (it != open_list.end()) {
                    it->g = tentative_g;
                    it->f = f;
                    it->px = current.x;
                    it->py = current.y;
                    it->order = ++order_counter;
                } else {
                    open_list.push_back({nx, ny, tentative_g, h, f, ++order_counter, current.x, current.y});
                }
            }
        }
    }

    if (!goal_reached) {
        std::cout << "Brak mozliwosci dotarcia do celu!" << std::endl;
        return 0;
    }

    // Odtworzenie œcie¿ki
    int cur_x = goal_x, cur_y = goal_y;
    int steps = 0;
    while (cur_x != -1 && cur_y != -1) {
        grid[cur_y][cur_x] = 3;
        int next_px = parent_x[cur_y][cur_x];
        int next_py = parent_y[cur_y][cur_x];
        cur_x = next_px;
        cur_y = next_py;
        steps++;
    }

    std::cout << "Cel osiagniety! Dlugosc trasy: " << (steps - 1) << " krokow.\n\n";
    std::cout << "Wizualizacja planszy (o = wolne, 5 = przeszkoda, 3 = sciezka):\n";
    std::cout << "(Lewy dolny rog to punkt [0,0])\n\n";

    // Wyœwietlenie
    for (int y = GRID_SIZE - 1; y >= 0; --y) {
        std::cout << std::setw(2) << y << " | ";
        for (int x = 0; x < GRID_SIZE; ++x) {
            if (grid[y][x] == 3) {
                std::cout << "3 ";
            } else if (grid[y][x] == 5) {
                std::cout << "5 "; // przeszkoda
            } else {
                std::cout << "o "; // wolne pole
            }
        }
        std::cout << "\n";
    }

    std::cout << "     ";
    for (int x = 0; x < GRID_SIZE; ++x) std::cout << "--";
    std::cout << "\n     ";
    for (int x = 0; x < GRID_SIZE; ++x) std::cout << (x % 10) << " ";
    std::cout << "\n";

    return 0;
}
