#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>

using namespace std;


// Position for each element
//Position class stores the coordinates of a cell in a maze

class Position
{
public:
    int row;
    int col;

    Position(int r = 0, int c = 0) : row(r), col(c) {}

    bool operator==(const Position &p) const    //== is overloaded to compare two cells
    {
        return (row == p.row && col == p.col);
    }
};


// Overload hash function as the default hash function do not generate hash values for user defined functions or classes
//Unordered_set and Unordered_map rely only on hash values

namespace std
{
    template <>
    struct hash<Position>
    {
        size_t operator()(const Position &pos) const
        {
            return (hash<int>()(pos.row) * 31) ^ hash<int>()(pos.col);//Generate unique hash values for each cell in a maze
        }
    };
}


//This class handles the functions of the maze
class MazeGraph
{

private:

    vector<vector<char>> grid;                                  //A 2D GRID
    unordered_map<Position, vector<Position>> adjacencyList;    //Key:Cell  Value:Vector containing its neighbours
    int rows, cols;                                             //Contains number of rows and columns in the grid
    Position start, end;                                        //Stores the coordinates of S cell and E cell
    const vector<pair<int, int>> direction = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};    //Stores the possible movement of the cell


public:

    MazeGraph(const vector<string> &inp)    //MazeGraph() is the constructor
    {
//Calculates the size of row,column
        rows = inp.size();
        cols = rows > 0 ? inp[0].length() : 0;
        grid.resize(rows, vector<char>(cols));

//Iterates through each cell checks for S and E in the Grid and creates a Position object for start and end
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                grid[i][j] = inp[i][j];
                if (grid[i][j] == 'S')
                    start = Position(i, j);
                else if (grid[i][j] == 'E')
                    end = Position(i, j);
            }
        }

        build_graph(); // Start to build graph
    }

/*
build_graph() function is used for 3 main functions
    1.To check for the cells that can be used for traversing the maze
    2.To check for the cells that are possible to move from a particular cell among the four possible directions
    3.To check for the valid cells (i.e) without walls and add its neighbour to the adjacency list for the particular cell
*/
    void build_graph()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (grid[i][j] == '#' || grid[i][j] == '1')     //Avoids walls
                    continue;

                Position cur_node(i, j);
                vector<Position> neighbours;

                for (const auto &direc : direction)
                {
                    int row_nei = i + direc.first;
                    int col_nei = j + direc.second;

                    if (isValid(row_nei, col_nei))
                    {
                        neighbours.push_back(Position(row_nei, col_nei));
                    }
                }
                adjacencyList[cur_node] = neighbours;
            }
        }
    }

    bool isValid(int row, int col)
    {
        return (row >= 0 && row < rows && col >= 0 && col < cols && grid[row][col] != '#' && grid[row][col] != '1');
    }

    void printOriMaze()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void printPathMaze(const vector<Position> &path)
    {
        vector<vector<char>> temp = grid;

        for (const auto &pos : path)
        {
            if (temp[pos.row][pos.col] != 'S' && temp[pos.row][pos.col] != 'E')
            {
                temp[pos.row][pos.col] = '*';
            }
        }

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << temp[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    vector<Position> solveDFS()
    {
        unordered_map<Position, Position> parent;
        unordered_set<Position> visited;
        stack<Position> stk;
        int nodesExplored = 0;

        stk.push(start);
        visited.insert(start);
        parent[start] = start;

        while (!stk.empty())
        {
            Position current = stk.top();
            stk.pop();
            nodesExplored++;

            if (current == end)
            {
                vector<Position> path;
                while (!(current == start))
                {
                    path.push_back(current);
                    current = parent[current];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());

                cout << "DFS Stats:" << endl;
                cout << "Path length: " << path.size() << endl;
                cout << "Nodes explored: " << nodesExplored << endl;

                return path;
            }

            for (const auto &neighbour : adjacencyList[current])
            {
                if (visited.find(neighbour) == visited.end())
                {
                    visited.insert(neighbour);
                    parent[neighbour] = current;
                    stk.push(neighbour);
                }
            }
        }

        cout << "DFS: No path found!" << endl;
        return {};
    }

    vector<Position> solveBFS()
    {
        unordered_map<Position, Position> parent;
        unordered_set<Position> visited;
        queue<Position> que;
        int nodesExplored = 0;

        que.push(start);
        visited.insert(start);
        parent[start] = start;

        while (!que.empty())
        {
            Position current = que.front();
            que.pop();
            nodesExplored++;

            if (current == end)
            {
                vector<Position> path;
                while (!(current == start))
                {
                    path.push_back(current);
                    current = parent[current];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());

                cout << "BFS Stats:" << endl;
                cout << "Path length: " << path.size() << endl;
                cout << "Nodes explored: " << nodesExplored << endl;

                return path;
            }

            for (const auto &neighbour : adjacencyList[current])
            {
                if (visited.find(neighbour) == visited.end())
                {
                    visited.insert(neighbour);
                    parent[neighbour] = current;
                    que.push(neighbour);
                }
            }
        }

        cout << "BFS: No path found!" << endl;
        return {};
    }
};

int main()
{
    int size_mat_row,size_mat_col;
    int choice,play=1;
    string get_row;
    vector<string> mazeStr;
    MazeGraph* maze = NULL;

    cout<<"\t\t\t\t\t\t\tWELCOME TO THE MAZE SOLVER!"<<endl;
    cout<<"\t\t\tHere you can solve a maze using the algorithms like DFS(Depth First Search) and BFS(Breadth First Search)"<<endl<<endl;

    while(play)
    {
        cout<<endl<<"OPERATIONS FOR THE MAZE: "<<endl;
        cout<<"1.INPUT THE MAZE"<<endl;
        cout<<"2.VIEW THE MAZE"<<endl;
        cout<<"3.PERFORM DFS FOR THE MAZE"<<endl;
        cout<<"4.PERFORM BFS FOR THE MAZE"<<endl;
        cout<<"5.CLEAR THE SCREEN"<<endl;
        cout<<"6.EXIT"<<endl<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;

        while(choice>6 || choice<1)
        {
            cout<<"\nEntered choice is out of range!"<<endl;
            cout<<"Enter your choice: ";
            cin>>choice;
        }

        switch(choice)
        {
            case 1:
            {
                if(!mazeStr.empty())
                {
                    mazeStr.clear();
                }
                cout<<"Enter the dimension of the matrix(row x column): ";
                cin>>size_mat_row>>size_mat_col;
                cout<<endl<<endl<<"S - Starting Cell"<<endl;
                cout<<"E - Ending Cell"<<endl;
                cout<<". - Movable Cell"<<endl;
                cout<<"# - Wall"<<endl<<endl;
                for(int i=0;i<size_mat_row;i++)
                {
                    cout<<"Enter row "<<i+1<<" for the maze: ";
                    cin>>get_row;

                    if(get_row.length()>size_mat_col)
                    {
                        cout<<"Row given has column size greater than the required size!"<<endl;
                        i--;
                        continue;
                    }
                    if(get_row.length()<size_mat_col)
                    {
                        cout<<"Row given has length smaller than the required size!"<<endl;
                        i--;
                        continue;
                    }

                    mazeStr.push_back(get_row);
                }
                /*vector<string> mazeStr =
                {
                    "S....",
                    "#.#..",
                    "..#.#",
                    "#.#E.",
                    "#...#"
                };*/

                delete maze;
                maze = new MazeGraph(mazeStr);
                cout<<"MAZE CREATED SUCCESSFULLY!!!"<<endl<<endl;
                break;
            }
            case 2:
                if(mazeStr.empty())
                {
                    cout<<endl<<"Please fill the maze and try to view the maze"<<endl<<endl;
                    break;
                }
                cout << "Original Maze:" << endl;
                maze->printOriMaze();
                break;

            case 3:
            {
                if(mazeStr.empty())
                {
                    cout<<endl<<"Please fill the maze and try to perform DFS for the maze"<<endl<<endl;
                    break;
                }
                cout << "\nSolving maze using DFS:" << endl;
                vector<Position> dfsPath = maze->solveDFS();
                if (!dfsPath.empty())
                    maze->printPathMaze(dfsPath);
                break;
            }

            case 4:
            {
                if(mazeStr.empty())
                {
                    cout<<endl<<"Please fill the maze and try to perform BFS for the maze"<<endl<<endl;
                    break;
                }
                cout << "\nSolving maze using BFS:" << endl;
                vector<Position> bfsPath = maze->solveBFS();
                if (!bfsPath.empty())
                    maze->printPathMaze(bfsPath);
                break;
            }

            case 5:
            {
                system("cls");
                break;
            }

            case 6:
            {
                cout<<endl<<"THANK YOU FOR PLAYING THE MAZE SOLVER!!!";
                play = 0;
                break;
            }
        }
    }

    return 0;
}
