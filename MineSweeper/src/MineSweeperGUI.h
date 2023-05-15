/*****************************************************************//**
 * File : MineSweeperGUI.h
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-10
 * Description : This is the GUI header of MineSweeperExample
 *********************************************************************/
#pragma once
#ifndef _MINESWEEPERGUI_H_
#define _MINESWEEPERGUI_H_

#include <iostream>
#include <string>
#include <vector>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QMessageBox>
#include <QMovie>
#include <QIcon>
#include <QtMultimedia/QMediaPlayer>

#include "MineSweeperCore.h"

class MineSweeperGUI : public QMainWindow
{
	//Qt用於signal與slot定義的macro，但我都是使用callback的方式來實作，這裡其實可以註解掉
	Q_OBJECT

public:

	//MineSweeperGUI constructor
	MineSweeperGUI(QWidget* parent = nullptr);

	//MineSweeperGUI destructor
	virtual ~MineSweeperGUI();

private:

	/**
	 * Intent : 創建Standby介面
	 * Pre :
	 * Post : 創建完畢
	 */
	void CreateStandbyLayout();

	/**
	 * Intent : 創建Playing介面
	 * Pre :
	 * Post : 創建完畢
	 */
	void CreatePlayingLayout();

	/**
	 * Intent : 創建2D格狀盤面GUI
	 * Pre :
	 * Post :
	 */
	void CreateBoardGridGUI();

	/**
	 * Intent : 切換成Standby介面
	 * Pre : 處於Playing介面
	 * Post : 處於Standby介面
	 */
	void SwitchStandbyLayout();

	/**
	 * Intent : 切換成Playing介面
	 * Pre : 處於Standby介面
	 * Post : 處於Playing介面
	 */
	void SwitchPlayingLayout();

	/**
	 * Intent : 更新GUI (GameState、版面、Count)
	 * Pre :
	 * Post :
	 */
	void UpdateGUI();

	/**
	 * Intent : 對按鈕按下右鍵的callback
	 * Pre :
	 * Post :
	 * \param pos 滑鼠點擊的位置，不需使用
	 */
	void RightClickCallback(const QPoint& pos);

	/**
	 * Intent : 進入遊戲結束彈出式訊息框
	 * Pre : 遊戲結束
	 * Post : 重玩或關閉遊戲
	 */
	void EnterGameOverMessageBox();

	//一些預設的常數
	const QSize STANDBY_LAYOUT_SIZE = QSize(400, 160);
	const int CELL_BUTTON_SIZE_LENGTH = 50;
	const QSize CELL_BUTTON_SIZE = QSize(CELL_BUTTON_SIZE_LENGTH, CELL_BUTTON_SIZE_LENGTH);

	//遊戲核心api
	MineSweeperCore* gameCore = nullptr;

	//主視窗的基底widget
	QStackedWidget* centralWidget = nullptr;

	//Standby介面的widget與其layout
	QWidget* standbyWidget = nullptr;
	QVBoxLayout* standbyLayout = nullptr;

	//Playing介面的widget與其layout
	QWidget* playingWidget = nullptr;
	QVBoxLayout* playingLayout = nullptr;

	//GameState相關的UI物件
	QLabel* gameStateTitle = nullptr;
	QLabel* gameStateContent = nullptr;
	QHBoxLayout* gameStateLayout = nullptr;

	//2D格狀盤面的layout
	QGridLayout* boardGridLayout = nullptr;

	//用2維陣列存放按鈕與其label，方便存取
	std::vector<std::vector<QPushButton*>> cellButtons;
	std::vector<std::vector<QLabel*>> cellButtonLabels;

	//顯示Count相關的UI物件
	QHBoxLayout* showBombFlagCountLayout = nullptr;
	QLabel* bombCountLabel = nullptr;
	QLabel* flagCountLabel = nullptr;
	QHBoxLayout* showBlankCountLayout = nullptr;
	QLabel* openBlankCountLabel = nullptr;
	QLabel* remainBlankCountLabel = nullptr;

	//遊戲結束彈出式訊息框
	QMessageBox* gameOverMessageBox = nullptr;

	//音效播放器
	QMediaPlayer* soundMediaPlayer = nullptr;

	//音檔位置
	QString openBlankCellSoundLocation = "./sounds/OpenCell.wav";
	QString bombExplosionSoundLocation = "./sounds/BombExplosion.wav";

	//動畫播放器
	QMovie* bombExplosionMovie = nullptr;

	//動畫位置
	QString bombExplosionAnimationLocation = "./animations/BombExplosion.gif";

	//圖片相關變數
	QIcon flagIcon = QIcon(QString("./images/flag.png"));
	QPixmap flagPixmap;
	QIcon questionMarkIcon = QIcon(QString("./images/question_mark.png"));
	QPixmap questionMarkPixmap;
	QIcon bombIcon = QIcon(QString("./images/bomb.png"));
	QPixmap bombPixmap;

	//暫存觸發動畫的炸彈格label，用於之後停止播放
	QLabel* triggerBombCellButtonLabel = nullptr;
};

#endif