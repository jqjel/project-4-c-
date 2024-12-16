// Jazmin Elenes
// None
// 026
// 12.09.2024

//------------------------------------------------------
// AUTOGRADER INFO -- IGNORE BUT DO NOT REMOVE 
// test_cases: true
// feedback('all')
// 35b24e49-2842-4603-8ba7-0f656200c2d1
//------------------------------------------------------

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <cmath>


using namespace std;
 

struct Map {
    int rows;           
    int cols;           
    int startRow;       
    int startCol;       
    int endRow;         
    int endCol;         
};

struct Planet {
    int row;            
    int col;            
    char mapSymbol;     
    int ID;             
    bool seen = 0;  
};



void readFileName(ifstream &file, vector<int> &ids, vector<string> &names){
    int id;
    string name;
    while (file >> id >> name) {
        ids.push_back(id);
        names.push_back(name);
    }
}


void processFileName(vector<string> &names) {
    for (int i = 0; i < int(names.size()); ++i) {
      
        string &word = names.at(i);

        word.erase(remove(word.begin(), word.end(), 'X'), word.end());

        for (char &c : word) {
            if (c == '_') {
                c = ' '; 
            }
        }
    }

}

double calcDist(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int nearNeigh(int currentRow, int currentCol, vector<Planet> &planets){
    int nearI = -1;
    double minDist = 9999999999.0; // Humongous number
    for(int i = 0; i < planets.size(); i++) {
        if(!planets[i].seen){
            double dist = calcDist(currentRow, currentCol, planets[i].row, planets[i].col);
            if (dist < minDist) {
                minDist = dist;
                nearI = i;
            }
        }
    }
    return nearI;
}

int main() {

    Map map;
    vector<Planet> planets;
    vector<int> ids;
    vector<string> names;

    string locFile, fileName;
    cout << "Enter Locations Filename: ";
    cin >> locFile;
    cout << "Enter Names Filename: ";
    cin >> fileName;

    ifstream finLoc(locFile);
    finLoc >> map.rows >> map.cols;
    finLoc >> map.startRow >> map.startCol;
    finLoc >> map.endRow >> map.endCol;
    
    Planet locPlanet;
    while (finLoc >> locPlanet.row >> locPlanet.col >> locPlanet.mapSymbol >> locPlanet.ID) {
        if (locPlanet.row >= 1 && locPlanet.row <= map.rows && locPlanet.col >= 1 && locPlanet.col <= map.cols) {
            planets.push_back(locPlanet);
        } else {
            cout << locPlanet.ID << " out of range - ignoring" << endl;
        }
        
    }
    
    ifstream finName(fileName);

    readFileName(finName, ids, names);
    processFileName(names);

    if (!(finLoc).is_open() || !(finName).is_open()) {
        cout << "Input file could not be opened" << endl;
        return 1;
    }


    vector<vector<char>> grid(map.rows, vector<char>(map.cols, '.'));
    grid[map.startRow - 1][map.startCol - 1] = 'S';
    grid[map.endRow - 1][map.endCol - 1] = 'E';

    for (const auto& planet : planets){
        grid[planet.row - 1][planet.col - 1] = planet.mapSymbol;
    }

    vector<int> route;
    int currentRow = map.startRow, currentCol = map.startCol;

    while (route.size() < planets.size ()){
        int nearPlanet = nearNeigh(currentRow, currentCol, planets);
        if (nearPlanet != -1) {
            planets[nearPlanet].seen = true;
            route.push_back(planets[nearPlanet].ID);
            currentRow = planets[nearPlanet].row;
            currentCol = planets[nearPlanet].col;
        }
    }

    ofstream coutFile("journey.txt");

    for(int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            coutFile << grid[i][j];
        }
        coutFile << endl;
    }

    coutFile << endl;
    
    coutFile << "Start at " << map.startRow << " " << map.startCol << endl;
    for (int planetID : route) {
        int planetI = find(ids.begin(), ids.end(), planetID) - ids.begin();
        coutFile << "Go to " << names[planetI] << " at " << planets[planetI].row << " " << planets[planetI].col << endl;
    }
    
    coutFile << "End at " << map.endRow << " " << map.endCol << endl;

    coutFile.close();

    return 0;

}


