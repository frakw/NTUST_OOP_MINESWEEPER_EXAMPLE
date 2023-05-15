/*****************************************************************//**
 * File : BoardCore.cpp
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-15
 * Description : This is the Core api implementation of MineSweeperExample
 *********************************************************************/

#include "BoardCore.h"

using namespace std;

//BoardCore constructor
BoardCore::BoardCore()
{

}

//BoardCore constructor
BoardCore::BoardCore(int _rows, int _cols) : rows(_rows), cols(_cols)
{
	AllocateMem(_rows, _cols);
}

//BoardCore destructor
BoardCore::~BoardCore()
{
	if (cells != nullptr)
	{
		Clear();
	}
}

/**
 * Intent : 重新配置2維陣列記憶體
 * Pre :
 * Post : 配置完成
 * \param _rows row數量
 * \param _cols col數量
 */
void BoardCore::AllocateMem(int _rows, int _cols)
{
	//若與原本配置的記憶體大小相同，則不另外配置
	if (_rows == rows && _cols == cols)
	{
		return;
	}

	//如果已經配置過記憶體了，先釋放掉
	if (cells != nullptr)
	{
		//釋放記憶體
		Clear();
	}

	//配置2維陣列記憶體
	cells = new CellCore * [_rows];

	for (int i = 0; i < _rows; i++)
	{
		cells[i] = new CellCore[_cols];
	}
}

/**
 * Intent : 載入空的盤面 (只配置記憶體)
 * Pre :
 * Post : 載入完成
 * \param _rows row數量
 * \param _cols col數量
 */
void BoardCore::Load(int _rows, int _cols)
{
	AllocateMem(_rows, _cols);
	rows = _rows;
	cols = _cols;
}

/**
 * Intent : 用bomb map載入盤面
 * Pre :
 * Post : 載入完成
 * \param isBombMap bomb map指標
 * \param _rows row數量
 * \param _cols col數量
 */
void BoardCore::Load(bool** isBombMap, int _rows, int _cols)
{
	AllocateMem(_rows, _cols);
	rows = _rows;
	cols = _cols;
	totalBombCount = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cells[i][j].SetBomb(isBombMap[i][j]);
		}
	}
	Refresh();
}

/**
 * Intent :	清除已有資訊，並釋放記憶體
 * Pre :
 * Post :
 */
void BoardCore::Clear()
{
	if (cells != nullptr)
	{
		for (int i = 0; i < rows; i++)
		{
			delete[] cells[i];
		}

		delete[] cells;
		cells = nullptr;
		rows = 0;
		cols = 0;
		totalBombCount = 0;
		totalFlagCount = 0;
		totalBlankCount = 0;
		openBlankCount = 0;
		remainBlankCount = 0;
	}
}

/**
 * Intent : 是否已載入過盤面
 * Pre :
 * Post :
 * \return 是否已載入過盤面
 */
bool BoardCore::IsLoaded()
{
	return cells != nullptr;
}

/**
 * Intent : 印出盤面
 * Pre :
 * Post :
 * \param rowSplit row與row之間的分隔字串，預設為換行
 * \param colSplit col與col之間的分隔字串，預設為一個空白
 */
void BoardCore::Print(std::string rowSplit, std::string colSplit)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cells[i][j].Print();
			cout << colSplit;
		}
		cout << rowSplit;
	}
}

/**
 * Intent : 印出盤面解答 (含炸彈位置與每格數字)
 * Pre :
 * Post :
 * \param rowSplit row與row之間的分隔字串，預設為換行
 * \param colSplit col與col之間的分隔字串，預設為一個空白
 */
void BoardCore::PrintAnswer(std::string rowSplit, std::string colSplit)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cells[i][j].PrintAnswer();
			cout << colSplit;
		}
		cout << rowSplit;
	}
}

/**
 * Intent :	輸出盤面，用char組成2維陣列去表示
 * Pre :
 * Post :
 * \return char組成的2維陣列
 */
vector<string> BoardCore::Output()
{
	vector<string> result;
	for (int i = 0; i < rows; i++)
	{
		string line;
		for (int j = 0; j < cols; j++)
		{
			line.push_back(cells[i][j].GetChar());
		}
		result.push_back(line);
	}
	return result;
}

/**
 * Intent : 獲取該格的指標
 * Pre :
 * Post :
 * \param row row的位置
 * \param col col的位置
 * \return 該格的指標
 */
CellCore* BoardCore::GetCell(int row, int col)
{
	//防呆機制
	if (row < 0 || row >= rows)
	{
		return nullptr;
	}

	//防呆機制
	if (col < 0 || col >= cols)
	{
		return nullptr;
	}

	return &cells[row][col];
}

/**
 * Intent : 更新盤面 (重新計算count)
 * Pre :
 * Post :
 */
void BoardCore::Refresh()
{
	RefreshTotalCount();
	RefreshNearBombCount();
	RefreshOpenBlankCount();
}

/**
 * Intent : 重新計算total類型的count
 * Pre :
 * Post :
 */
void BoardCore::RefreshTotalCount()
{
	totalBombCount = 0;
	totalBlankCount = 0;
	totalFlagCount = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//計算炸彈數量
			if (cells[i][j].IsBomb())
			{
				totalBombCount++;
			}
			//計算空白數量
			else
			{
				totalBlankCount++;
			}

			//計算旗幟數量
			if (cells[i][j].GetState() == CellState::FLAGGED)
			{
				totalFlagCount++;
			}
		}
	}
}

/**
 * Intent : 重新計算每格的數字(near bomb count)
 * Pre :
 * Post :
 */
void BoardCore::RefreshNearBombCount()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cells[i][j].SetNearBombCount(GetNearBombCount(i, j));
		}
	}
}

/**
 * Intent : 重新計算已開啟的格子數
 * Pre :
 * Post :
 */
void BoardCore::RefreshOpenBlankCount()
{
	openBlankCount = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (cells[i][j].GetState() == CellState::OPENED)
			{
				openBlankCount++;
			}
		}
	}
	remainBlankCount = totalBlankCount - openBlankCount;
}


/**
 * Intent :	輸入的row col是否合法 (在範圍內)
 * Pre :
 * Post :
 * \param row 輸入的row
 * \param col 輸入的col
 * \return 輸入的row col是否合法
 */
bool BoardCore::ValidRowCol(int row, int col)
{
	if (row < 0 || row >= rows)
	{
		return false;
	}

	if (col < 0 || col >= cols)
	{
		return false;
	}

	return true;
}

/**
 * Intent : 計算該格周遭九宮格內的炸彈數量 (near bomb count)
 * Pre :
 * Post :
 * \param row row位置
 * \param col col位置
 * \return 該格周遭九宮格內的炸彈數量
 */
int BoardCore::GetNearBombCount(int row, int col)
{
	//防呆機制
	if (!ValidRowCol(row, col))
	{
		return 0;
	}

	int nearBombCount = 0;

	//掃過周遭九宮格
	for (int i = -1; i <= 1; i++)
	{
		int checkRow = row + i;
		for (int j = -1; j <= 1; j++)
		{
			int checkCol = col + j;

			//跳過自己
			if (checkRow == row && checkCol == col)
			{
				continue;
			}

			//如果超出邊界，忽略掉
			if (!ValidRowCol(checkRow, checkCol))
			{
				continue;
			}

			//如果是炸彈，計數器加1
			if (cells[checkRow][checkCol].IsBomb())
			{
				nearBombCount++;
			}
		}
	}
	return nearBombCount;
}

/**
 * Intent : 回傳炸彈總數量
 * Pre :
 * Post :
 * \return 回傳炸彈總數量
 */
int BoardCore::GetTotalBombCount()
{
	return totalBombCount;
}

/**
 * Intent : 回傳旗幟總數量
 * Pre :
 * Post :
 * \return 回傳旗幟總數量
 */
int BoardCore::GetTotalFlagCount()
{
	return totalFlagCount;
}

/**
 * Intent : 回傳空白總數量
 * Pre :
 * Post :
 * \return 回傳空白總數量
 */
int BoardCore::GetTotalBlankCount()
{
	return totalBlankCount;
}



/**
 * Intent :	回傳已開啟的空白格子數量
 * Pre :
 * Post :
 * \return 已開啟的空白格子數量
 */
int BoardCore::GetOpenBlankCount()
{
	return openBlankCount;
}

/**
 * Intent :	回傳剩餘未開啟的空白格子數量
 * Pre :
 * Post :
 * \return 剩餘未開啟的空白格子數量
 */
int BoardCore::GetRemainBlankCount()
{
	return remainBlankCount;
}

/**
 * Intent : 把所有格子都開啟，遊戲結束顯示解答時使用
 * Pre :
 * Post : 盤面上的所有格子都被開啟
 */
void BoardCore::UncoverAll()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cells[i][j].SetState(CellState::OPENED);
		}
	}
}
