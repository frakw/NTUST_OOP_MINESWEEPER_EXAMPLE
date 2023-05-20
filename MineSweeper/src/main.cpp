/*****************************************************************//**
 * File : main.cpp
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-10
 * Description : This is the entry point of MineSweeperExample program
 *********************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

#include "MineSweeperCore.h"
#include "MineSweeperGUI.h"

using namespace std;

/**
 * Intent : 執行指令檔模式
 * Pre : Start Program
 * Post : End Program
 * \param commandFilename 指令檔檔名(含相對路徑)
 * \param outputFilename  輸出檔檔名
 */
void RunCommandFile(string commandFilename, string outputFilename)
{
	MineSweeperCore game;
	ofstream outputFile(outputFilename);
	std::cout.rdbuf(outputFile.rdbuf()); // 把cout的輸出都導入到輸出檔案，這樣就可以統一使用cout來印出結果，而不用進行stream的切換
	game.ExecuteCommandFile(commandFilename);
	outputFile.close();
}

/**
 * Intent : 執行指令輸入模式
 * Pre : Start Program
 * Post : End Program
 */
void RunCommandInput()
{
	MineSweeperCore game;
	string commandLine;

	//一行一行輸入指令，並執行
	while (getline(cin, commandLine))
	{
		game.ExecuteCommand(commandLine);
	}
}

/**
 * Intent : 執行GUI模式
 * Pre : Start Program
 * Post : End Program
 */
int RunGUI(int argc, char* argv[])
{
	//初始化Qt GUI程式
	QApplication app(argc, argv);
	MineSweeperGUI* gameGUI = new MineSweeperGUI();
	gameGUI->setWindowTitle("MineSweeper Example");
	gameGUI->show();

	//bgm播放器
	QMediaPlayer* bgmMediaPlayer = new QMediaPlayer();

	//bgm播放清單
	QMediaPlaylist* bmgPlaylist = new QMediaPlaylist(bgmMediaPlayer);

	//加入一首BGM
    bmgPlaylist->addMedia(QUrl("qrc:/resources/sounds/Road to Dazir.mp3"));

	//設定BGM，使其無限循環撥放
	bmgPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

	// 設定播放清單
	bgmMediaPlayer->setPlaylist(bmgPlaylist);
	bgmMediaPlayer->setVolume(100);

	//開始播放bgm
	bgmMediaPlayer->play();

	return app.exec();
}

int main(int argc, char* argv[])
{

	if (argc == 0)
	{
		cout << "arg error!" << endl;
		return 0;
	}
	else if (argc == 1)
	{
		//如果沒有指定模式，預設用GUI模式執行
		return RunGUI(argc, argv);
	}
	else if (string(argv[1]) == string("CommandFile") && argc == 4)
	{
		//執行指令檔模式
		RunCommandFile(argv[2], argv[3]);
	}
	else if (string(argv[1]) == string("CommandInput") && argc == 2)
	{
		//執行指令輸入模式
		RunCommandInput();
	}
	else if (string(argv[1]) == string("GUI") && argc == 2)
	{
		//執行GUI模式
		return RunGUI(argc, argv);
	}
	else
	{
		cout << "arg error!" << endl;
		return 0;
	}

	return 0;
}
