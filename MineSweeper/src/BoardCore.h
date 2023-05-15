/*****************************************************************//**
 * File : BoardCore.h
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-15
 * Description : This is the Core api header of MineSweeperExample
 *********************************************************************/

#pragma once
#ifndef _BOARDCORE_H_
#define _BOARDCORE_H_

#include <iostream>
#include <vector>
#include <string>

#include "CellCore.h"

 //列舉出載入模式
enum class BoardCoreGenerateType
{
	FILE,
	RANDOM_RATE,
	RANDOM_COUNT,
};

class BoardCore
{
public:

	//BoardCore constructor
	BoardCore();

	//BoardCore constructor
	BoardCore(int, int);

	//BoardCore destructor
	~BoardCore();

	/**
	 * Intent : 載入空的盤面 (只配置記憶體)
	 * Pre :
	 * Post : 載入完成
	 * \param _rows row數量
	 * \param _cols col數量
	 */
	void Load(int, int);

	/**
	 * Intent : 用bomb map載入盤面
	 * Pre :
	 * Post : 載入完成
	 * \param isBombMap bomb map指標
	 * \param _rows row數量
	 * \param _cols col數量
	 */
	void Load(bool**, int, int);

	/**
	 * Intent :	清除已有資訊，並釋放記憶體
	 * Pre :
	 * Post :
	 */
	void Clear();

	/**
	 * Intent : 是否已載入過盤面
	 * Pre :
	 * Post :
	 * \return 是否已載入過盤面
	 */
	bool IsLoaded();

	/**
	 * Intent : 印出盤面
	 * Pre :
	 * Post :
	 * \param rowSplit row與row之間的分隔字串，預設為換行
	 * \param colSplit col與col之間的分隔字串，預設為一個空白
	 */
	void Print(std::string rowSplit = "\n", std::string colSplit = " ");

	/**
	 * Intent : 印出盤面解答 (含炸彈位置與每格數字)
	 * Pre :
	 * Post :
	 * \param rowSplit row與row之間的分隔字串，預設為換行
	 * \param colSplit col與col之間的分隔字串，預設為一個空白
	 */
	void PrintAnswer(std::string rowSplit = "\n", std::string colSplit = " ");

	/**
	 * Intent :	輸出盤面，用char組成2維陣列去表示
	 * Pre :
	 * Post :
	 * \return char組成的2維陣列
	 */
	std::vector<std::string> Output();

	/**
	 * Intent : 獲取該格的指標
	 * Pre :
	 * Post :
	 * \param row row的位置
	 * \param col col的位置
	 * \return 該格的指標
	 */
	CellCore* GetCell(int, int);

	/**
	 * Intent : 更新盤面 (重新計算count)
	 * Pre :
	 * Post :
	 */
	void Refresh();

	/**
	 * Intent : 計算該格周遭九宮格內的炸彈數量 (near bomb count)
	 * Pre :
	 * Post :
	 * \param row row位置
	 * \param col col位置
	 * \return 該格周遭九宮格內的炸彈數量
	 */
	int GetNearBombCount(int, int);

	/**
	 * Intent : 回傳炸彈總數量
	 * Pre :
	 * Post :
	 * \return 回傳炸彈總數量
	 */
	int GetTotalBombCount();

	/**
	 * Intent : 回傳旗幟總數量
	 * Pre :
	 * Post :
	 * \return 回傳旗幟總數量
	 */
	int GetTotalFlagCount();

	/**
	 * Intent : 回傳空白總數量
	 * Pre :
	 * Post :
	 * \return 回傳空白總數量
	 */
	int GetTotalBlankCount();

	/**
	 * Intent :	回傳已開啟的空白格子數量
	 * Pre :
	 * Post :
	 * \return 已開啟的空白格子數量
	 */
	int GetOpenBlankCount();

	/**
	 * Intent :	回傳剩餘未開啟的空白格子數量
	 * Pre :
	 * Post :
	 * \return 剩餘未開啟的空白格子數量
	 */
	int GetRemainBlankCount();

	/**
	 * Intent : 把所有格子都開啟，遊戲結束顯示解答時使用
	 * Pre :
	 * Post : 盤面上的所有格子都被開啟
	 */
	void UncoverAll();

private:

	/**
	 * Intent : 重新配置2維陣列記憶體
	 * Pre :
	 * Post : 配置完成
	 * \param _rows row數量
	 * \param _cols col數量
	 */
	void AllocateMem(int, int);

	/**
	 * Intent : 重新計算total類型的count
	 * Pre :
	 * Post :
	 */
	void RefreshTotalCount();

	/**
	 * Intent : 重新計算每格的數字(near bomb count)
	 * Pre :
	 * Post :
	 */
	void RefreshNearBombCount();

	/**
	 * Intent : 重新計算已開啟的格子數
	 * Pre :
	 * Post :
	 */
	void RefreshOpenBlankCount();

	/**
	 * Intent :	輸入的row col是否合法 (在範圍內)
	 * Pre :
	 * Post :
	 * \param row 輸入的row
	 * \param col 輸入的col
	 * \return 輸入的row col是否合法
	 */
	bool ValidRowCol(int, int);

	//儲存2維格子陣列
	CellCore** cells = nullptr;

	//row col 數量
	int rows = 0;
	int cols = 0;

	//紀錄count的變數
	int totalBombCount = 0;
	int totalFlagCount = 0;
	int totalBlankCount = 0;
	int openBlankCount = 0;
	int remainBlankCount = 0;
};


#endif // !_BOARDCORE_H_
