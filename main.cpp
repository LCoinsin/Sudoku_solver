#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "vector"
#include <toml.hpp>
#include <list>

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

void print_vector_2d(const vector<vector<unsigned int>>& grid) {
    for (auto & i : grid) {
        for (unsigned int j : i) {
            cout << j << " | ";
        }
        cout << endl;
    }
}

void check_possibility_row(const vector<vector<unsigned int>>& grid, vector<int>& list_possibility, int row) {
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

void check_possibility_col(const vector<vector<unsigned int>>& grid, vector<int>& list_possibility, int col) {
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

void check_possibility_subgrid(const vector<vector<unsigned int>>& grid, vector<int>& list_possibility, int row, int col) {
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

void set_grid_possibility(const vector<vector<unsigned int>>& grid) {
    for (int row = 0; row < grid.size(); row++) {
        for (int col = 0; col < grid[row].size(); col++) {
            if (grid[row][col] == 0) {
                vector<int> list_possibility = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                check_possibility_row(grid, list_possibility, row);
                check_possibility_col(grid, list_possibility, col);
                check_possibility_subgrid(grid, list_possibility, row, col);
                cout << "Size : " << list_possibility.size() << " | ";
            }
        }
        cout << endl;
    }
}

int main() {
    cout << "Algorithme backtracking - solveur de sudoku ..." << endl;

    // Chargement du fichier de configuration
    auto config = toml::parse("config.toml");

    //Parsage du fichier XML dans un vecteur 2d
    vector<vector<unsigned int>> grid = parse_xml();

    print_vector_2d(grid);

    //Calcul des possibilitées par case disponible
    set_grid_possibility(grid);

    return 0;
}
