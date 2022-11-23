#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "vector"
#include <toml.hpp>
#include <list>
#include <map>

using namespace std;
using namespace rapidxml;

vector<vector<unsigned int>> parse_xml() {
    xml_document<> doc;
    xml_node<> * root_node;
    vector<vector<unsigned int>> grid;

    ifstream grid_xml ("grid_in.xml");
    vector<char> buffer((istreambuf_iterator<char>(grid_xml)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("root");

    int i = 0;
    for (xml_node<> * row_node = root_node->first_node("row"); row_node; row_node = row_node->next_sibling() ) {
        grid.emplace_back();

        for (xml_node<> * col_node = row_node->first_node("col"); col_node; col_node = col_node->next_sibling() ) {
            grid[i].push_back(static_cast<int>(*col_node->value())-48);
        }
        i++;
    }

    return grid;
}

// Print functions
void print_vector_2d(const vector<vector<unsigned int>>& grid) {
    for (auto & i : grid) {
        for (unsigned int j : i) {
            cout << j << " | ";
        }
        cout << endl;
    }
}

void print_grid_possibility(const vector<vector<int>>& grid_possibility) {
    for (const auto & row : grid_possibility) {
        for (int col : row) {
            cout << col << " | ";
        }
        cout << endl;
    }
}

void print_map_possibility(const map<int, vector<int>>& map_order) {
    for(const auto & elem : map_order)
    {
        cout << elem.first << " | "  << elem.second[0] << "-" << elem.second[1] << endl;
    }
}

// Algo functions
void check_possibility_row(const vector<vector<unsigned int>>& grid, vector<unsigned int>& list_possibility, int row) {
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] != 0) {
            for (int j = 0; j<list_possibility.size(); j++) {
                if (list_possibility[j] == grid[row][i]) {
                    list_possibility.erase(list_possibility.begin()+j);
                }
            }
        }
    }
}

void check_possibility_col(const vector<vector<unsigned int>>& grid, vector<unsigned int>& list_possibility, int col) {
    for (int i = 0; i<9; i++) {
        if(grid[i][col] != 0) {
            for (int j = 0; j<list_possibility.size(); j++) {
                if (list_possibility[j] == grid[i][col]) {
                    list_possibility.erase(list_possibility.begin()+j);
                }
            }
        }
    }
}

void check_possibility_subgrid(const vector<vector<unsigned int>>& grid, vector<unsigned int>& list_possibility, int row, int col) {
    double subgrid_row = floor(row/3);
    double subgrid_col = floor(col/3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i+3*subgrid_row][j+3*subgrid_col] != 0) {
                for (int k = 0; k<list_possibility.size(); ++k) {
                    if (list_possibility[k] == grid[i+3*subgrid_row][j+3*subgrid_col]) {
                        list_possibility.erase(list_possibility.begin()+k);
                    }
                }
            }
        }
    }
}

vector<vector<int>> setGridPossibility(const vector<vector<unsigned int>>& grid) {
    vector<vector<int>> grid_possibility;

    for (int row = 0; row < grid.size(); row++) {
        grid_possibility.emplace_back();
        int i = 0;
        for (int col = 0; col < grid[row].size(); col++) {
            if (grid[row][col] == 0) {
                vector<unsigned int> list_possibility = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                check_possibility_row(grid, list_possibility, row);
                check_possibility_col(grid, list_possibility, col);
                check_possibility_subgrid(grid, list_possibility, row, col);
                grid_possibility[row].push_back(static_cast<int>(list_possibility.size()));
                i++;
            }
        }
    }

    return grid_possibility;
}

map<int, vector<int>> setOrderFill(const vector<vector<int>>& grid_possibility, const vector<vector<unsigned int>>& grid) {
    map<int, vector<int>> map_order;
    int order = 0;
    for (int i = 1; i<=9; i++) {
        for (int row = 0; row < grid_possibility.size(); row++) {
            for (int col = 0; col < grid_possibility[row].size(); col++) {
                if (grid_possibility[row][col] == i) {
                    int col_grid = 0;
                    int sub_col = col;
                    for (int j = 0; j<9; j++) {
                        if (grid[row][j] == 0) {
                            if (sub_col==0) break;
                            sub_col--;
                        } else col_grid++;
                    }
                    vector<int> localisation = {row, col+col_grid};
                    map_order[order] = localisation;
                    order++;
                }
            }
        }
    }
    return map_order;
}

// Algo de backtracking
bool existeOnSubgrid(vector<vector<unsigned int>>& grid, int row, int col, const unsigned int value) {
    double subgrid_row = floor(row/3);
    double subgrid_col = floor(col/3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i+3*subgrid_row != row && j+3*subgrid_col != col) {
                if (grid[i+3*subgrid_row][j+3*subgrid_col] == value) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool existOnRow(vector<vector<unsigned int>>& grid, int row, int col, const unsigned int value) {
    for (int subCol = 0; subCol<9; subCol++) {
        if(subCol != col) {
            if(grid[row][subCol] == value) {
                return false;
            }
        }
    }
    return true;
}

bool existOnCol(vector<vector<unsigned int>>& grid, int row, int col, const unsigned int value) {
    for (int subRow = 0; subRow<9; subRow++) {
        if (subRow != row) {
            if(grid[subRow][col] == value) {
                return false;
            };
        }
    }
    return true;
}

bool isValide(vector<vector<unsigned int>>& grid, int row, int col) {
    unsigned int value = grid[row][col];
    if (value == 0) return false;
    else {
        if (existOnCol(grid, row, col, value) && existOnRow(grid, row, col, value) && existeOnSubgrid(grid, row, col, value)) return true;
        else return false;
    }
}

bool resolveSudoku(vector<vector<unsigned int>>& grid, map<int, vector<int>>& map_order, int& position) {
    if (map_order.size() > position) {

        vector<int>& pos = map_order.at(position);
        grid[pos[0]][pos[1]] += 1;

        if (isValide(grid, pos[0], pos[1])) {
            position += 1;
            resolveSudoku(grid, map_order, position);
        } else {
            if (grid[pos[0]][pos[1]] == 9) {
                grid[pos[0]][pos[1]] = 0;
                position -= 1;
            }
            resolveSudoku(grid, map_order, position);
        }
    }
    return true;
}

// Main
int main() {
    cout << "Algorithme backtracking - solveur de sudoku ..." << endl;

    // Chargement du fichier de configuration
    auto config = toml::parse("config.toml");

    //Parsage du fichier XML dans un vecteur 2d
    vector<vector<unsigned int>> grid = parse_xml();
    print_vector_2d(grid);

    cout << endl << "---------------------------" << endl;

    //Calcul des possibilités par case disponible
    vector<vector<int>> grid_possibility = setGridPossibility(grid);
    print_grid_possibility(grid_possibility);

    cout << endl << "---------------------------" << endl;

    //Determine l'ordre de passage des valeurs pour l'algorithme de backtracking
    map<int, vector<int>> map_order = setOrderFill(grid_possibility, grid);
    print_map_possibility(map_order);


    cout << "map size " << map_order.size() << endl;
    int position = 0;
    resolveSudoku(grid, map_order, position);

    print_vector_2d(grid);

    return 0;
}
