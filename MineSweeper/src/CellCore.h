/*****************************************************************//**
 * File : CellCore.h
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-15
 * Description : This is the Core api header of MineSweeperExample
 *********************************************************************/

#pragma once
#ifndef _CELLCORE_H_
#define _CELLCORE_H_

#include <iostream>
#include <string>

 //列舉出格子的顯示狀態
enum class CellState
{
	CLOSED, //未打開
	OPENED, //已打開
	FLAGGED, //標記旗子
	QUESTION_MARK, //標記問號
};

class CellCore
{
public:

	//CellCore constructor
	CellCore();

	//CellCore destructor
	~CellCore();

	/**
	 * Intent : 回傳此格是否為炸彈
	 * Pre :
	 * Post :
	 * \return 此格是否為炸彈
	 */
	bool IsBomb();

	/**
	 * Intent :	設定此格是否為炸彈
	 * Pre :
	 * Post :
	 * \param _isBomb 是否為炸彈
	 */
	void SetBomb(bool);

	/**
	 * Intent :	回傳格子顯示狀態
	 * Pre :
	 * Post :
	 * \return 格子顯示狀態
	 */
	CellState GetState();

	/**
	 * Intent : 設定格子顯示狀態
	 * Pre :
	 * Post : 設定完成
	 * \param newState 新的格子顯示狀態
	 */
	void SetState(CellState);

	/**
	 * Intent : 印出該格所表示的字元
	 * Pre :
	 * Post :
	 */
	void Print();

	/**
	 * Intent : 獲取該格所表示的字元
	 * Pre :
	 * Post :
	 * \return 該格所表示的字元
	 */
	char GetChar();

	/**
	 * Intent :	印出該格的解答 (炸彈為X 其餘為數字(near bomb count))
	 * Pre :
	 * Post :
	 */
	void PrintAnswer();

	/**
	 * Intent :	回傳周遭九宮格內的炸彈數量
	 * Pre :
	 * Post :
	 * \return 周遭九宮格內的炸彈數量
	 */
	int GetNearBombCount();

	/**
	 * Intent : 設定周遭九宮格內的炸彈數量
	 * Pre :
	 * Post :
	 * \param _nearBombCount 設定值
	 */
	void SetNearBombCount(int);

	/**
	 * Intent :	此格是否可以被執行LeftClick指令
	 * Pre :
	 * Post :
	 * \return 是否可以被執行LeftClick指令
	 */
	bool CanBeLeftClick();

	/**
	 * Intent : 此格是否可以被執行RightClick指令
	 * Pre :
	 * Post :
	 * \return 是否可以被執行RightClick指令
	 */
	bool CanBeRightClick();

	/**
	 * Intent : 執行RightClick指令 (無標註->旗幟, 旗幟->問號, 問號->無標註)
	 * Pre :
	 * Post : 標註完成
	 */
	void RightClick();

private:

	//是否為炸彈
	bool isBomb = false;

	//格子顯示狀態
	CellState state = CellState::CLOSED;

	//周遭九宮格內的炸彈數量
	int nearBombCount = 0;
};


#endif // !_CELLCORE_H_
