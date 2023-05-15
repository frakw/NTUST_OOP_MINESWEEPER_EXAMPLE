/*****************************************************************//**
 * File : MineSweeperCore.cpp
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-15
 * Description : This is the Core api implementation of MineSweeperExample
 *********************************************************************/

#include "MineSweeperCore.h"

using namespace std;

//MineSweeperCore constructor
MineSweeperCore::MineSweeperCore()
{
	gameBoard = new BoardCore();
}

//MineSweeperCore destructor
MineSweeperCore::~MineSweeperCore()
{
	gameBoard->~BoardCore();
	gameBoard = nullptr;
}

/**
 * Intent : 執行一行的指令
 * Pre :
 * Post :
 * \param command 指令字串
 * \return 是否執行成功
 */
bool MineSweeperCore::ExecuteCommand(std::string command)
{
	//防呆機制
	if (command.empty())
	{
		return false;
	}

	cout << '<' << command << "> : ";

	//使用stringstream來分割指令
	stringstream commandStream(command);
	string action;
	commandStream >> action;

	//用try catch來將所有執行失敗的指令導到同一個地方
	try {

		//Load指令
		if (action == "Load")
		{
			//防呆機制
			if (gameState != MineSweeperState::STANDBY)
			{
				throw - 1;
			}

			string generateType;
			commandStream >> generateType;

			//若已經載入過盤面，則先將已有的盤面清除
			if (gameBoard->IsLoaded())
			{
				gameBoard->Clear();
			}

			if (generateType == "BoardFile")
			{
				string boardFilename;
				commandStream >> boardFilename;
				LoadFileBoard(boardFilename);
			}
			else if (generateType == "RandomCount")
			{
				int _row, _col, _bombCount;
				commandStream >> _row >> _col >> _bombCount;
				LoadRandomCountBoard(_row, _col, _bombCount);
			}
			else if (generateType == "RandomRate")
			{
				int _row, _col;
				float _bombRate;
				commandStream >> _row >> _col >> _bombRate;
				LoadRandomRateBoard(_row, _col, _bombRate);
			}
			if (!gameBoard->IsLoaded())
			{
				throw - 1;
			}
		}
		//StartGame指令
		else if (action == "StartGame")
		{
			//防呆機制
			if (gameState != MineSweeperState::STANDBY)
			{
				throw - 1;
			}

			if (gameBoard->IsLoaded() == false)
			{
				throw - 1;
			}

			StartGame();
		}
		//Print指令
		else if (action == "Print")
		{
			string printTarget;
			commandStream >> printTarget;
			ExecutePrint(printTarget);
		}
		//LeftClick指令
		else if (action == "LeftClick")
		{
			//防呆機制
			if (gameState != MineSweeperState::PLAYING)
			{
				throw - 1;
			}

			int clickRow, clickCol;
			commandStream >> clickRow >> clickCol;

			//防呆機制
			if (!ValidRowCol(clickRow, clickCol))
			{
				throw - 1;
			}

			CellState cellState = gameBoard->GetCell(clickRow, clickCol)->GetState();

			//防呆機制
			if (cellState == CellState::FLAGGED || cellState == CellState::OPENED)
			{
				throw - 1;
			}

			cout << "Success" << endl;
			LeftClick(clickRow, clickCol);
			return true;
		}
		//RightClick指令
		else if (action == "RightClick")
		{
			if (gameState != MineSweeperState::PLAYING)
			{
				throw - 1;
			}
			int clickRow, clickCol;
			commandStream >> clickRow >> clickCol;

			//防呆機制
			if (!ValidRowCol(clickRow, clickCol))
			{
				throw - 1;
			}

			CellState cellState = gameBoard->GetCell(clickRow, clickCol)->GetState();

			//防呆機制
			if (cellState == CellState::OPENED)
			{
				throw - 1;
			}

			RightClick(clickRow, clickCol);
		}
		//Replay指令
		else if (action == "Replay")
		{
			//防呆機制
			if (gameState != MineSweeperState::GAMEOVER)
			{
				throw - 1;
			}

			Replay();
		}
		//Quit指令
		else if (action == "Quit")
		{
			//防呆機制
			if (gameState != MineSweeperState::GAMEOVER)
			{
				throw - 1;
			}

			this->~MineSweeperCore();
			cout << "Success" << endl;
			exit(0);
		}
		else
		{
			throw - 1;
		}

	}
	catch (int failed)
	{
		//執行失敗，印出failed
		cout << "Failed" << endl;
		return false;
	}

	if (action != "Print")
	{
		//執行成功，印出Success
		cout << "Success" << endl;
	}

	return true;
}

/**
 * Intent : 執行多行的指令
 * Pre :
 * Post :
 * \param commandArray 指令字串陣列
 */
void MineSweeperCore::ExecuteCommands(std::vector<std::string> commandArray)
{
	for (int i = 0; i < commandArray.size(); i++)
	{
		ExecuteCommand(commandArray[i]);
	}
}
/**
 * Intent : 讀取並執行指令檔
 * Pre :
 * Post :
 * \param filename 指令檔檔名
 */
void MineSweeperCore::ExecuteCommandFile(std::string filename)
{
	ifstream commandFile(filename);

	//防呆機制
	if (!commandFile.is_open())
	{
		return;
	}

	vector<string> commands;
	string line;

	while (getline(commandFile, line))
	{
		commands.push_back(line);
	}

	ExecuteCommands(commands);
}

/**
 * Intent : 將GameBoard用char組成的二維陣列輸出
 * Pre :
 * Post :
 * \return GameBoard用char組成二維陣列
 */
vector<string> MineSweeperCore::GameBoardOutput()
{
	return gameBoard->Output();
}

/**
 * Intent : 玩家是否贏得遊戲，false代表輸了
 * Pre : 遊戲結束時進行呼叫
 * Post :
 * \return 玩家是否贏得遊戲，false代表輸了
 */
bool MineSweeperCore::IsPlayerWin()
{
	return playerWin;
}


/**
 * Intent : 獲取當前遊戲狀態
 * Pre :
 * Post :
 * \return 遊戲狀態
 */
MineSweeperState MineSweeperCore::GetGameState()
{
	return gameState;
}

/**
 * Intent : 獲取當前遊戲狀態(字串)
 * Pre :
 * Post :
 * \return 遊戲狀態(字串)
 */
string MineSweeperCore::GetGameStateStr()
{
	switch (gameState)
	{
	case MineSweeperState::STANDBY:
		return "Standby";
		break;
	case MineSweeperState::PLAYING:
		return "Playing";
		break;
	case MineSweeperState::GAMEOVER:
		return "GameOver";
		break;
	default:
		break;
	}
	return "";
}

/**
 * Intent : 獲取Row的數量
 * Pre :
 * Post :
 * \return Row的數量
 */
int MineSweeperCore::GetRowCount()
{
	return rows;
}

/**
 * Intent : 獲取Col的數量
 * Pre :
 * Post :
 * \return Col的數量
 */
int MineSweeperCore::GetColCount()
{
	return cols;
}

/**
 * Intent : 獲取炸彈數量
 * Pre :
 * Post :
 * \return 炸彈數量
 */
int MineSweeperCore::GetBombCount()
{
	return gameBoard->GetTotalBombCount();
}

/**
 * Intent : 獲取當前旗幟數量
 * Pre :
 * Post :
 * \return 當前旗幟數量
 */
int MineSweeperCore::GetFlagCount()
{
	return gameBoard->GetTotalFlagCount();
}

/**
 * Intent : 獲取已開啟的空白格子數量
 * Pre :
 * Post :
 * \return 已開啟的空白格子數量
 */
int MineSweeperCore::GetOpenBlankCount()
{
	return gameBoard->GetOpenBlankCount();
}

/**
 * Intent : 獲取尚未開啟的空白格子數量
 * Pre :
 * Post :
 * \return 尚未開啟的空白格子數量
 */
int MineSweeperCore::GetRemainBlankCount()
{
	return gameBoard->GetRemainBlankCount();
}

/**
 * Intent : 重新設定row col的數量
 * Pre : 並非處於Playing狀態中
 * Post : 設定完成
 * \param _rows 新的row數量
 * \param _cols 新的col數量
 */
void MineSweeperCore::ResetRowCol(int _rows, int _cols)
{
	if (gameState != MineSweeperState::PLAYING)
	{
		rows = _rows;
		cols = _cols;
	}
}

/**
 * Intent : 動態配置記憶體，產生bomb map (2維bool陣列)
 * Pre :
 * Post : 記憶體配置完成
 * \param rows row數量
 * \param cols col數量
 * \return bomb map陣列指標
 */
bool** MineSweeperCore::NewBombMap(int rows, int cols)
{
	bool** isBombMap = new bool* [rows];
	for (int i = 0; i < rows; i++)
	{
		isBombMap[i] = new bool[cols];
	}
	return isBombMap;
}

/**
 * Intent : 釋放bomb map的記憶體
 * Pre : 記憶體配置完成
 * Post : 釋放完成
 * \param isBombMap bomb map陣列指標
 * \param rows row數量
 */
void MineSweeperCore::DelBombMap(bool** isBombMap, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		delete[] isBombMap[i];
	}
	delete[] isBombMap;
}

/**
 * Intent : 用盤面檔模式來載入
 * Pre :
 * Post : 載入完成
 * \param filename 盤面檔檔名
 */
void MineSweeperCore::LoadFileBoard(std::string filename)
{
	//防呆機制
	if (gameState == MineSweeperState::PLAYING)
	{
		return;
	}

	std::ifstream mapFile(filename);

	if (!mapFile.is_open())
	{
		return;
	}

	mapFile >> rows >> cols;

	//創建bomb map
	bool** isBombMap = NewBombMap(rows, cols);

	//讀掉換行字元
	mapFile.ignore();

	//讀取盤面檔
	for (int i = 0; i < rows; i++)
	{
		char cellChar;
		for (int j = 0; j < cols; j++)
		{
			mapFile >> cellChar;

			if (cellChar == 'O')
			{
				isBombMap[i][j] = false;
			}
			else if (cellChar == 'X')
			{
				isBombMap[i][j] = true;
			}

		}

		//讀掉換行字元
		mapFile.ignore();
	}

	//把bomb map丟給BoardCore的載入函式
	gameBoard->Load(isBombMap, rows, cols);

	//釋放記憶體
	DelBombMap(isBombMap, rows);
}

/**
 * Intent :	用RandomCount模式來載入
 * Pre :
 * Post : 載入完成
 * \param _rows row數量
 * \param _cols col數量
 * \param bombCount 指令炸彈數量
 */
void MineSweeperCore::LoadRandomCountBoard(int _rows, int _cols, int bombCount)
{
	//防呆機制
	if (bombCount < 0 || bombCount > _rows * _cols)
	{
		return;
	}

	//防呆機制
	if (gameState == MineSweeperState::PLAYING)
	{
		return;
	}

	//重設row col數量
	ResetRowCol(_rows, _cols);

	//先載入空的盤面
	gameBoard->Load(_rows, _cols);

	//設定C++的隨機產生器
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> randRow(0, _rows - 1);
	std::uniform_int_distribution<> randCol(0, _cols - 1);

	//產生指令數量的炸彈
	for (int i = 0; i < bombCount; i++)
	{
		while (true)
		{
			//隨機選一個位置
			int row = randRow(gen);
			int col = randCol(gen);
			CellCore* cell = gameBoard->GetCell(row, col);

			//如果已經是炸彈了，就繼續挑下個位置
			if (cell->IsBomb())
			{
				continue;
			}
			else
			{
				//這個位置目前不是炸彈，將其設定為炸彈格
				cell->SetBomb(true);
				break;
			}
		}
	}

	//更新GameBoard，會計算出GameAnswer與Count
	gameBoard->Refresh();
}

/**
 * Intent :	用RandomRate模式來載入
 * Pre :
 * Post : 載入完成
 * \param _rows row數量
 * \param _cols col數量
 * \param bombRate 炸彈生成機率
 */
void MineSweeperCore::LoadRandomRateBoard(int _rows, int _cols, float bombRate)
{
	//防呆機制
	if (bombRate < 0.0f || bombRate > 1.0f)
	{
		return;
	}

	//防呆機制
	if (gameState == MineSweeperState::PLAYING)
	{
		return;
	}

	//重設row col數量
	ResetRowCol(_rows, _cols);

	//創建bomb map
	bool** isBombMap = NewBombMap(_rows, _cols);

	//設定C++的隨機產生器
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1

	//對每個格子隨機產生炸彈，放到bomb map中
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			if (dis(gen) > bombRate)
			{
				isBombMap[i][j] = false;
			}
			else
			{
				isBombMap[i][j] = true;
			}
		}
	}

	//把bomb map丟給BoardCore的載入函式
	gameBoard->Load(isBombMap, _rows, _cols);

	//釋放記憶體
	DelBombMap(isBombMap, _rows);
}

/**
 * Intent :	開始遊戲
 * Pre : Standby狀態
 * Post : Playing狀態
 */
void MineSweeperCore::StartGame()
{
	gameState = MineSweeperState::PLAYING;
}

/**
 * Intent :	重新遊玩
 * Pre : GameOver狀態
 * Post : Standby狀態
 */
void MineSweeperCore::Replay()
{
	gameBoard->Clear();
	gameState = MineSweeperState::STANDBY;
}

/**
 * Intent :	清除資訊
 * Pre :
 * Post :
 */
void MineSweeperCore::Clear()
{
	gameBoard->Clear();
	gameState = MineSweeperState::STANDBY;
}

/**
 * Intent : 盤面是否存在 (是否已經執行Load指令了)
 * Pre :
 * Post :
 * \return 盤面是否存在
 */
bool MineSweeperCore::IsBoardExist()
{
	return gameBoard != nullptr;
}

/**
 * Intent : 執行print指令
 * Pre :
 * Post :
 * \param printTarget 要印出的資訊名稱
 */
void MineSweeperCore::ExecutePrint(std::string printTarget)
{
	if (printTarget == "GameBoard")
	{
		cout << endl;
		gameBoard->Print("\n", " ");
	}
	else if (printTarget == "GameAnswer")
	{
		cout << endl;
		gameBoard->PrintAnswer("\n", " ");
	}
	else if (printTarget == "GameState")
	{
		switch (gameState)
		{
		case MineSweeperState::STANDBY:
			cout << "Standby" << endl;
			break;
		case MineSweeperState::PLAYING:
			cout << "Playing" << endl;
			break;
		case MineSweeperState::GAMEOVER:
			cout << "GameOver" << endl;
			break;
		default:
			break;
		}
	}
	else if (printTarget == "BombCount")
	{
		cout << gameBoard->GetTotalBombCount() << endl;
	}
	else if (printTarget == "FlagCount")
	{
		cout << gameBoard->GetTotalFlagCount() << endl;
	}
	else if (printTarget == "OpenBlankCount")
	{
		cout << gameBoard->GetOpenBlankCount() << endl;
	}
	else if (printTarget == "RemainBlankCount")
	{
		cout << gameBoard->GetRemainBlankCount() << endl;
	}
}

/**
 * Intent : 左鍵開啟格子
 * Pre : Playing狀態
 * Post : 成功/失敗 開啟，若成功開啟，則可能導致遊戲結束或繼續進行遊戲
 * \param row row位置
 * \param col col位置
 */
void MineSweeperCore::LeftClick(int row, int col)
{
	//防呆機制
	if (ValidRowCol(row, col) == false)
	{
		return;
	}

	//防呆機制
	if (gameState != MineSweeperState::PLAYING || gameBoard == nullptr)
	{
		return;
	}

	//獲取該格的指標
	CellCore* cell = gameBoard->GetCell(row, col);

	//防呆機制
	if (cell == nullptr)
	{
		return;
	}

	//如果該格不能被開啟，擋掉
	//CanBeLeftClick ---> 關閉狀態 or 標註問號
	if (!cell->CanBeLeftClick())
	{
		return;
	}

	//如果該格是炸彈，則輸掉遊戲
	if (cell->IsBomb())
	{
		//呼叫失敗後函式
		Lose();
		return;
	}

	//設定該格為開啟狀態
	cell->SetState(CellState::OPENED);

	//判斷遊戲是否結束 (玩家獲勝)
	if (IsGameFinished())
	{
		//呼叫獲勝後函式
		Win();
		return;
	}

	//如果該格是0，則往周遭8個方向的格子遞迴開啟下去
	if (cell->GetNearBombCount() == 0)
	{
		LeftClick(row - 1, col - 1);
		LeftClick(row - 1, col);
		LeftClick(row - 1, col + 1);
		LeftClick(row, col - 1);
		LeftClick(row, col + 1);
		LeftClick(row + 1, col - 1);
		LeftClick(row + 1, col);
		LeftClick(row + 1, col + 1);
	}
}

/**
 * Intent : 右鍵標註格子
 * Pre :
 * Post : 成功/失敗 標註
 * \param row row位置
 * \param col col位置
 */
void MineSweeperCore::RightClick(int row, int col)
{
	//防呆機制
	if (gameState != MineSweeperState::PLAYING || gameBoard == nullptr)
	{
		return;
	}

	//獲取該格的指標
	CellCore* cell = gameBoard->GetCell(row, col);

	//防呆機制
	if (cell == nullptr)
	{
		return;
	}

	//如果該格不能被標註，擋掉
	//CanBeRightClick ---> 關閉狀態 or 標註問號 or 標註旗幟
	if (!cell->CanBeRightClick())
	{
		return;
	}

	//對該格執行標註
	cell->RightClick();

	//重整 (重新計算Count數)
	gameBoard->Refresh();
}

/**
 * Intent :	遊戲是否結束 (玩家是否贏得遊戲)
 * Pre : Playing狀態
 * Post :
 * \return 遊戲是否結束
 */
bool MineSweeperCore::IsGameFinished()
{
	//先重整 (重新計算Count數)
	gameBoard->Refresh();

	//判斷所有空白格子是否都已被開啟
	return gameBoard->GetRemainBlankCount() == 0;
}

/**
 * Intent : 玩家贏了後要執行的程式
 * Pre : 玩家贏了
 * Post : 進入GameOver狀態
 */
void MineSweeperCore::Win()
{
	cout << "You win the game" << endl;
	gameState = MineSweeperState::GAMEOVER;
	playerWin = true;

	//將所有沒被開啟的格子打開 (顯示解答給玩家)
	gameBoard->UncoverAll();
}

/**
 * Intent : 玩家輸了後要執行的程式
 * Pre : 玩家輸了
 * Post : 進入GameOver狀態
 */
void MineSweeperCore::Lose()
{
	cout << "You lose the game" << endl;
	gameState = MineSweeperState::GAMEOVER;
	playerWin = false;

	//將所有沒被開啟的格子打開 (顯示解答給玩家)
	gameBoard->UncoverAll();
}


/**
 * Intent :	輸入的row col是否合法 (在範圍內)
 * Pre :
 * Post :
 * \param row 輸入的row
 * \param col 輸入的col
 * \return 輸入的row col是否合法
 */
bool MineSweeperCore::ValidRowCol(int row, int col)
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