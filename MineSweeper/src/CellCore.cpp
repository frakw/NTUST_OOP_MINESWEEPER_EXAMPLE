/*****************************************************************//**
 * File : CellCore.cpp
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-15
 * Description : This is the Core api implementation of MineSweeperExample
 *********************************************************************/

#include "CellCore.h"

using namespace std;

//CellCore constructor
CellCore::CellCore()
{

}

//CellCore destructor
CellCore::~CellCore()
{

}

/**
 * Intent : 回傳此格是否為炸彈
 * Pre :
 * Post :
 * \return 此格是否為炸彈
 */
bool CellCore::IsBomb()
{
	return isBomb;
}

/**
 * Intent :	設定此格是否為炸彈
 * Pre :
 * Post :
 * \param _isBomb 是否為炸彈
 */
void CellCore::SetBomb(bool _isBomb)
{
	isBomb = _isBomb;
}

/**
 * Intent :	回傳格子顯示狀態
 * Pre :
 * Post :
 * \return 格子顯示狀態
 */
CellState CellCore::GetState()
{
	return state;
}

/**
 * Intent : 設定格子顯示狀態
 * Pre :
 * Post : 設定完成
 * \param newState 新的格子顯示狀態
 */
void CellCore::SetState(CellState newState)
{
	state = newState;
}

/**
 * Intent : 印出該格所表示的字元
 * Pre :
 * Post :
 */
void CellCore::Print()
{
	cout << GetChar();
}

/**
 * Intent : 獲取該格所表示的字元
 * Pre :
 * Post :
 * \return 該格所表示的字元
 */
char CellCore::GetChar()
{
	switch (state)
	{
	case CellState::CLOSED:
		//尚未開啟(關閉)狀態
		return '#';
		break;
	case CellState::OPENED:
		//已開啟狀態
		if (isBomb)
		{
			//若為炸彈
			return 'X';
		}
		else
		{
			//若為空白格子，顯示數字(0~9)
			return nearBombCount + '0';
		}
		break;
	case CellState::FLAGGED:
		//標註為旗幟
		return 'f';
		break;
	case CellState::QUESTION_MARK:
		//標註為問號
		return '?';
		break;
	default:
		break;
	}
	return -1;
}

/**
 * Intent :	印出該格的解答 (炸彈為X 其餘為數字(near bomb count))
 * Pre :
 * Post :
 */
void CellCore::PrintAnswer()
{
	cout << (isBomb ? "X" : to_string(nearBombCount));
}

/**
 * Intent :	回傳周遭九宮格內的炸彈數量
 * Pre :
 * Post :
 * \return 周遭九宮格內的炸彈數量
 */
int CellCore::GetNearBombCount()
{
	return nearBombCount;
}

/**
 * Intent : 設定周遭九宮格內的炸彈數量
 * Pre :
 * Post :
 * \param _nearBombCount 設定值
 */
void CellCore::SetNearBombCount(int _nearBombCount)
{
	if (_nearBombCount >= 0)
	{
		nearBombCount = _nearBombCount;
	}
}

/**
 * Intent :	此格是否可以被執行LeftClick指令
 * Pre :
 * Post :
 * \return 是否可以被執行LeftClick指令
 */
bool CellCore::CanBeLeftClick()
{
	return state == CellState::CLOSED || state == CellState::QUESTION_MARK;
}

/**
 * Intent : 此格是否可以被執行RightClick指令
 * Pre :
 * Post :
 * \return 是否可以被執行RightClick指令
 */
bool CellCore::CanBeRightClick()
{
	return state == CellState::CLOSED || state == CellState::FLAGGED || state == CellState::QUESTION_MARK;
}

/**
 * Intent : 執行RightClick指令 (無標註->旗幟, 旗幟->問號, 問號->無標註)
 * Pre :
 * Post : 標註完成
 */
void CellCore::RightClick()
{
	if (!CanBeRightClick()) return;
	switch (state)
	{
	case CellState::CLOSED:
		state = CellState::FLAGGED;
		break;
	case CellState::OPENED:
		break;
	case CellState::FLAGGED:
		state = CellState::QUESTION_MARK;
		break;
	case CellState::QUESTION_MARK:
		state = CellState::CLOSED;
		break;
	default:
		break;
	}
}