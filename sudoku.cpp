#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

// Grid<T>，表示类型为T的二维网格
template<class T>
class Grid
{
public:
	T **grid;
	int boxSize, gridSize;

	Grid()
	{
		boxSize = 0;
		gridSize = 0;
		grid = NULL;
	}

	void init(int i_boxsize, int i_gridsize)
	{
		clear();

		boxSize = i_boxsize;
		gridSize = i_gridsize;
		createGrid();
	}

	Grid(T ** i_grid, int i_boxsize, int i_gridsize)
	{
		boxSize = i_boxsize;
		gridSize = i_gridsize;
		createGrid();

		for(int i = 0; i < gridSize; i++)
			for(int j = 0; j < gridSize; j++)
				grid[i][j] = i_grid[i][j];
	}
	Grid(const Grid& other)
	{
		*this = other;
	}
	Grid& operator=(const Grid& other)
	{
		clear();

		boxSize = other.boxSize;
		gridSize = other.gridSize;
		createGrid();

		for(int i = 0; i < gridSize; i++)
			for(int j = 0; j < gridSize; j++)
				grid[i][j] = other.grid[i][j];
	}
	// 析构时销毁grid
	~Grid()
	{
		clear();
	}
	void clear()
	{
		if(grid == NULL) return ;
		for(int i = 0; i < gridSize; i++)
			delete []grid[i];
		delete []grid; 
		boxSize = 0;
		gridSize = 0;
	}
	// 创建网格
	void createGrid()
	{
		if(gridSize == 0) return;

		grid = new T*[gridSize];
		for(int i = 0; i < gridSize; i++)
			grid[i] = new T[gridSize];
	}

	// 求行已有值
	set<T> getRowSet(int rowIndex)
	{
		set<T> row_nums;
		for(int i = 0; i < gridSize; i++)
			row_nums.insert(grid[rowIndex][i]);
		return row_nums;
	}
	// 求列已有值
	set<T> getColSet(int colIndex)
	{
		set<T> col_nums;
		for(int i = 0; i < gridSize; i++)
			col_nums.insert(grid[i][colIndex]);
		return col_nums;
	}
	// 求九宫格已有值
	set<T> getBoxSet(int rowIndex, int colIndex)
	{
		set<T> box_nums;
		int boxStartRow = rowIndex - rowIndex % boxSize;
		int boxStartCol = colIndex - colIndex % boxSize;
		for(int i = boxStartRow; i < boxStartRow + boxSize; i++)
			for(int j = boxStartCol; j < boxStartCol + boxSize; j++)
				box_nums.insert(grid[i][j]);
		return box_nums;
	}

	void printGrid()
	{
		if(grid == NULL)
		{
			cout << "printGrid failed, grid is NULL\n";
			return ;
		}
		cout << "Grid:\n";
		for(int i = 0; i < gridSize; i++)
		{
			for(int j = 0; j < gridSize; j++)
				cout << grid[i][j] << " ";
			cout << "\n";
		}
	}
};

// 数独为int的9*9二维网格
class Sudoku: public Grid<int>
{
public:
	// 候选值为每个位置是set<int>的二维网格
	Grid< set<int> > candidate;
	Sudoku(): Grid(), candidate()
	{

	}
	void parse(string input)
	{
		if(input.size() != 9 * 9)
		{
			cout << "Parse failed, input length is not a sudoku size\n";
			return ;
		}
		init(3, 9);
		candidate.init(3, 9);

		for(int i = 0; i < gridSize; i++)
			for(int j = 0; j < gridSize; j++)
				grid[i][j] = input[i * gridSize + j] - '0';
	}

	void getInference()
	{
		// 如果grid为空，则报错并返回
		if(grid == NULL)
		{
			cout << "getInference failed, grid is NULL\n";
			return ;
		}

		// full_digits是1-9的集合，用于求补集
		set<int> full_digits;
		for(int i = 1; i <= gridSize; i++)
			full_digits.insert(i);

		for(int i = 0; i < gridSize; i++)
			for(int j = 0; j < gridSize; j++)
			{
				if(grid[i][j] != 0)
				{
					candidate.grid[i][j].insert(grid[i][j]);
					continue;
				}
				set<int> exist_nums;

				// 求row集合, 合并进入exist
				set<int> row_nums = getRowSet(i);
				set_union(exist_nums.begin(), exist_nums.end(), row_nums.begin(), row_nums.end(),\
						  inserter(exist_nums, exist_nums.begin()));

				// 求col集合, 合并进入exist
				set<int> col_nums = getColSet(j);
				set_union(exist_nums.begin(), exist_nums.end(), col_nums.begin(), col_nums.end(),\
						  inserter(exist_nums, exist_nums.begin()));

				// 求box集合, 合并进入exist
				set<int> box_nums = getBoxSet(i, j);
				set_union(exist_nums.begin(), exist_nums.end(), box_nums.begin(), box_nums.end(),\
						  inserter(exist_nums, exist_nums.begin()));

				// 求补集, 得出剩余候选值
				set_difference(full_digits.begin(), full_digits.end(), exist_nums.begin(), exist_nums.end(),\
							   inserter(candidate.grid[i][j], candidate.grid[i][j].begin()));
			}
	}
	void printCandidates()
	{
		// 如果grid为空，则报错并返回
		if(grid == NULL)
		{
			cout << "printCandidates failed, grid is NULL\n";
			return ;
		}
		cout << "Sudoku candidates:\n";
		for(int i = 0; i < gridSize; i++)
			for(int j = 0; j < gridSize; j++)
			{
				cout << "row, col(" << i << ", " << j << "): ";
				for(set<int>::iterator it = candidate.grid[i][j].begin(); it != candidate.grid[i][j].end(); it++)
					cout << *it << " ";
				cout << "\n";
			}
	}
	bool testCandidates()
	{
		// 如果grid为空，则报错并返回
		if(grid == NULL)
		{
			cout << "testCandidates failed, grid is NULL\n";
			return 0;
		}		
		// 测试开始
		cout << "Sudoku test:\n";
		for(int i = 0; i < gridSize; i++)
			for(int j = 0; j < gridSize; j++)
			{
				cout << "row, col(" << i << ", " << j << "): ";
				bool flag = 1;
				set<int> row_nums = getRowSet(i);
				set<int> col_nums = getColSet(j);
				set<int> box_nums = getBoxSet(i, j);
				// 检查每个位置是否与行、列、九宫格冲突
				for(set<int>::iterator it = candidate.grid[i][j].begin(); grid[i][j] == 0 && it != candidate.grid[i][j].end(); it++)
				{
					int temp_candidate = *it;
					if(row_nums.count(temp_candidate) > 0)
					{
						flag = 0;
						cout << "wrong because " << temp_candidate << "in row\n";
						break; 
					}
					if(col_nums.count(temp_candidate) > 0)
					{
						flag = 0;
						cout << "wrong because " << temp_candidate << "in col\n";
						break; 
					}
					if(box_nums.count(temp_candidate) > 0)
					{
						flag = 0;
						cout << "wrong because " << temp_candidate << "in box\n";
						break; 
					}
				}
				if(flag)
				{
					cout << "accept\n";
				}
			}		
	}
};

int main()
{
	Sudoku sudoku;
	// string input = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";
	// 123456789000000000000000000000000000000000000000000000000000000000000000000000000
	string input;
	cin >> input;
	sudoku.parse(input); // 扫描输入
	sudoku.getInference(); // 推理每个位置候选值
	sudoku.printGrid(); // 输出网格
	sudoku.printCandidates(); // 输出候选值
	sudoku.testCandidates(); // 检查候选值是否正确

	return 0;
}