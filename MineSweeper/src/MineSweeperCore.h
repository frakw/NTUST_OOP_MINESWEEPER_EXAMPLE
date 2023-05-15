/*****************************************************************//**
 * File : MineSweeperCore.h
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-15
 * Description : This is the Core api header of MineSweeperExample
 *********************************************************************/

#pragma once
#ifndef _MINESWEEPERCORE_H_
#define _MINESWEEPERCORE_H_

#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <sstream>

#include "CellCore.h"
#include "BoardCore.h"

 //列舉出遊戲狀態
enum class MineSweeperState
{
	STANDBY,
	PLAYING,
	GAMEOVER,
};

class MineSweeperCore
{
public:
	//MineSweeperCore constructor
	MineSweeperCore();

	//MineSweeperCore destructor
	~MineSweeperCore();

	/**
	 * Intent : 執行一行的指令
	 * Pre :
	 * Post :
	 * \param command 指令字串
	 * \return 是否執行成功
	 */
	bool ExecuteCommand(std::string);

	/**
	 * Intent : 執行多行的指令
	 * Pre :
	 * Post :
	 * \param commandArray 指令字串陣列
	 */
	void ExecuteCommands(std::vector<std::string>);

	/**
	 * Intent : 讀取並執行指令檔
	 * Pre :
	 * Post :
	 * \param filename 指令檔檔名
	 */
	void ExecuteCommandFile(std::string);

	/**
	 * Intent : 將GameBoard用char組成的二維陣列輸出
	 * Pre :
	 * Post :
	 * \return GameBoard用char組成二維陣列
	 */
	std::vector<std::string> GameBoardOutput();

	/**
	 * Intent : 玩家是否贏得遊戲，false代表輸了
	 * Pre : 遊戲結束時進行呼叫
	 * Post :
	 * \return 玩家是否贏得遊戲，false代表輸了
	 */
	bool IsPlayerWin();

	/**
	 * Intent : 獲取當前遊戲狀態
	 * Pre :
	 * Post :
	 * \return 遊戲狀態
	 */
	MineSweeperState GetGameState();

	/**
	 * Intent : 獲取當前遊戲狀態(字串)
	 * Pre :
	 * Post :
	 * \return 遊戲狀態(字串)
	 */
	std::string GetGameStateStr();

	/**
	 * Intent : 獲取Row的數量
	 * Pre :
	 * Post :
	 * \return Row的數量
	 */
	int GetRowCount();

	/**
	 * Intent : 獲取Col的數量
	 * Pre :
	 * Post :
	 * \return Col的數量
	 */
	int GetColCount();

	/**
	 * Intent : 獲取炸彈數量
	 * Pre :
	 * Post :
	 * \return 炸彈數量
	 */
	int GetBombCount();

	/**
	 * Intent : 獲取當前旗幟數量
	 * Pre :
	 * Post :
	 * \return 當前旗幟數量
	 */
	int GetFlagCount();

	/**
	 * Intent : 獲取已開啟的空白格子數量
	 * Pre :
	 * Post :
	 * \return 已開啟的空白格子數量
	 */
	int GetOpenBlankCount();

	/**
	 * Intent : 獲取尚未開啟的空白格子數量
	 * Pre :
	 * Post :
	 * \return 尚未開啟的空白格子數量
	 */
	int GetRemainBlankCount();

private:

	/**
	 * Intent : 重新設定row col的數量
	 * Pre : 並非處於Playing狀態中
	 * Post : 設定完成
	 * \param _rows 新的row數量
	 * \param _cols 新的col數量
	 */
	void ResetRowCol(int, int);

	/**
	 * Intent : 動態配置記憶體，產生bomb map (2維bool陣列)
	 * Pre :
	 * Post : 記憶體配置完成
	 * \param rows row數量
	 * \param cols col數量
	 * \return bomb map陣列指標
	 */
	bool** NewBombMap(int rows, int cols);


	/**
	 * Intent : 釋放bomb map的記憶體
	 * Pre : 記憶體配置完成
	 * Post : 釋放完成
	 * \param isBombMap bomb map陣列指標
	 * \param rows row數量
	 */
	void DelBombMap(bool**, int);

	/**
	 * Intent :	清除資訊
	 * Pre :
	 * Post :
	 */
	void Clear();

	/**
	 * Intent :	開始遊戲
	 * Pre : Standby狀態
	 * Post : Playing狀態
	 */
	void StartGame();

	/**
	 * Intent :	重新遊玩
	 * Pre : GameOver狀態
	 * Post : Standby狀態
	 */
	void Replay();

	/**
	 * Intent : 用盤面檔模式來載入
	 * Pre :
	 * Post : 載入完成
	 * \param filename 盤面檔檔名
	 */
	void LoadFileBoard(std::string filename);

	/**
	 * Intent :	用RandomCount模式來載入
	 * Pre :
	 * Post : 載入完成
	 * \param _rows row數量
	 * \param _cols col數量
	 * \param bombCount 指令炸彈數量
	 */
	void LoadRandomCountBoard(int rows, int cols, int bombCount);

	/**
	 * Intent :	用RandomRate模式來載入
	 * Pre :
	 * Post : 載入完成
	 * \param _rows row數量
	 * \param _cols col數量
	 * \param bombRate 炸彈生成機率
	 */
	void LoadRandomRateBoard(int rows, int cols, float bombRate);

	/**
	 * Intent : 盤面是否存在 (是否已經執行Load指令了)
	 * Pre :
	 * Post :
	 * \return 盤面是否存在
	 */
	bool IsBoardExist();

	/**
	 * Intent : 執行print指令
	 * Pre :
	 * Post :
	 * \param printTarget 要印出的資訊名稱
	 */
	void ExecutePrint(std::string);

	/**
	 * Intent : 左鍵開啟格子
	 * Pre : Playing狀態
	 * Post : 成功/失敗 開啟，若成功開啟，則可能導致遊戲結束或繼續進行遊戲
	 * \param row row位置
	 * \param col col位置
	 */
	void LeftClick(int, int);

	/**
	 * Intent : 右鍵標註格子
	 * Pre :
	 * Post : 成功/失敗 標註
	 * \param row row位置
	 * \param col col位置
	 */
	void RightClick(int, int);

	/**
	 * Intent :	遊戲是否結束 (玩家是否贏得遊戲)
	 * Pre : Playing狀態
	 * Post :
	 * \return 遊戲是否結束
	 */
	bool IsGameFinished();

	/**
	 * Intent : 玩家贏了後要執行的程式
	 * Pre : 玩家贏了
	 * Post : 進入GameOver狀態
	 */
	void Win();

	/**
	 * Intent : 玩家輸了後要執行的程式
	 * Pre : 玩家輸了
	 * Post : 進入GameOver狀態
	 */
	void Lose();

	/**
	 * Intent :	輸入的row col是否合法 (在範圍內)
	 * Pre :
	 * Post :
	 * \param row 輸入的row
	 * \param col 輸入的col
	 * \return 輸入的row col是否合法
	 */
	bool ValidRowCol(int, int);

	//row的數量
	int rows = 9;

	//col的數量
	int cols = 9;

	//盤面處理api
	BoardCore* gameBoard = nullptr;

	//遊戲狀態
	MineSweeperState gameState = MineSweeperState::STANDBY;

	//儲存玩家輸或贏，遊戲結束後才會存取到
	bool playerWin = false;
};

#endif // !_MINESWEEPERCORE_H_