#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "vector"
#include "fstream"
#include <filesystem>
#include <toml.hpp>

using namespace std;
using namespace rapidxml;

vector<vector<unsigned int>> parse_xml() {
    xml_document<> doc;
    xml_node<> * root_node;
    vector<vector<unsigned int>> grid;

    ifstream grid_xml ("/Users/leocoinsin/Documents/Personnel/C++/Sudoku/grid.xml");
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

int main() {
    cout << "Algorithme solveur de sudoku ..." << endl;

    // Chargement du fichier de configuration
    auto config = toml::parse("config.toml");

    vector<vector<unsigned int>> grid = parse_xml();

    print_vector_2d(grid);

    return 0;
}
