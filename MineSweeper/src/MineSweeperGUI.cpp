/*****************************************************************//**
 * File : MineSweeperGUI.cpp
 * Author : SHENG-HAO LIAO (frakwu@gmail.com)
 * Create Date : 2023-04-01
 * Editor : SHENG-HAO LIAO (frakwu@gmail.com)
 * Update Date : 2023-05-10
 * Description : This is the GUI implementation of MineSweeperExample
 *********************************************************************/

#include <sstream>
#include <iomanip>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include "MineSweeperGUI.h"

using namespace std;

//MineSweeperGUI constructor
MineSweeperGUI::MineSweeperGUI(QWidget* parent)
	: QMainWindow(parent)
{
	//動態配置記憶體生成處理遊戲邏輯的核心
	gameCore = new MineSweeperCore();

	//本GUI共有兩大區塊，分別為載入盤面用的介面 : Standby 與 遊戲遊玩時的介面 : Playing
	//建立必要的Widget，以centralWidget當作基底，切換成standbyWidget或playingWidget顯示
	centralWidget = new QStackedWidget();
	standbyWidget = new QWidget();
	playingWidget = new QWidget();

	//建立顯示GameState的UI物件，將其掛入gameStateLayout底下，切換區塊顯示時會一併將其導入
	gameStateTitle = new QLabel("GameState : ");
	gameStateTitle->setAlignment(Qt::AlignLeft);
	gameStateContent = new QLabel("Standby");
	gameStateContent->setAlignment(Qt::AlignLeft);
	gameStateLayout = new QHBoxLayout();
	gameStateLayout->addWidget(gameStateTitle);
	gameStateLayout->addWidget(gameStateContent);
	//在最後面加上stretch，使前面的UI可以置左顯示
	gameStateLayout->addStretch(1);



	//建立遊戲結束時的彈出式訊息視窗
	gameOverMessageBox = new QMessageBox();
	gameOverMessageBox->setText(QString(u8""));
	gameOverMessageBox->setStandardButtons(QMessageBox::Reset | QMessageBox::Close);
	gameOverMessageBox->setDefaultButton(QMessageBox::Reset);
	//把QMessageBox::Reset當作Replay指令，QMessageBox::Close當作Quit指令
	gameOverMessageBox->setButtonText(QMessageBox::Reset, "Replay");
	gameOverMessageBox->setButtonText(QMessageBox::Close, "Quit");

	//播放音效的mediaplayer
	soundMediaPlayer = new QMediaPlayer();
	soundMediaPlayer->setVolume(100);

	//創建爆炸動畫
    bombExplosionMovie = new QMovie(bombExplosionAnimationLocation);
	bombExplosionMovie->setScaledSize(QSize(30, 30));

	//獲取圖片(icon)的pixel map
	flagPixmap = flagIcon.pixmap(QSize(32, 32));
	questionMarkPixmap = questionMarkIcon.pixmap(QSize(32, 32));
	bombPixmap = bombIcon.pixmap(QSize(32, 32));

	//設定一個callback，在播放動畫到最後一幀時，結束播放動畫，並設定成炸彈圖片
	connect(bombExplosionMovie, &QMovie::frameChanged, [this](int frame) {
		if (frame == bombExplosionMovie->frameCount() - 1) {
			bombExplosionMovie->stop();
			triggerBombCellButtonLabel->setMovie(nullptr);
			triggerBombCellButtonLabel->setPixmap(bombPixmap);
		}
		});

	//呼叫創建Standby與Playing介面的函式
	CreateStandbyLayout();
	CreatePlayingLayout();

	//在基底Widget中加入Standby與Playing介面
	centralWidget->addWidget(standbyWidget);
	centralWidget->addWidget(playingWidget);

	// centralWidget->widget(0) ===> Standby介面
	// centralWidget->widget(1) ===> Playing介面
	//一開始先顯示Standby介面，並隱藏與Playing介面
	centralWidget->widget(0)->show();
	centralWidget->widget(1)->close();

	//把視窗大小固定到standbyLayoutSize (預設的固定值)
	this->setMinimumSize(STANDBY_LAYOUT_SIZE);
	this->setMaximumSize(STANDBY_LAYOUT_SIZE);
	this->resize(STANDBY_LAYOUT_SIZE);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	//把基底Widget指定給主視窗 (this)
	this->setCentralWidget(centralWidget);
}

//MineSweeperGUI destructor
MineSweeperGUI::~MineSweeperGUI()
{
	//釋放記憶體
	gameCore->~MineSweeperCore();
	QWidget::~QWidget();
}

/**
 * Intent : 創建Standby介面
 * Pre :
 * Post : 創建完畢
 */
void MineSweeperGUI::CreateStandbyLayout()
{
	//以standbyWidget為基底，創建其直屬的layout (垂直)
	standbyLayout = new QVBoxLayout(standbyWidget);

	//第一行加入GameState顯示介面 (row0)
	standbyLayout->addLayout(gameStateLayout, 0);

	//創建接下來4行的水平layout
	QHBoxLayout* row1Layout = new QHBoxLayout();
	QHBoxLayout* row2Layout = new QHBoxLayout();
	QHBoxLayout* row3Layout = new QHBoxLayout();
	QHBoxLayout* row4Layout = new QHBoxLayout();

	//創建下拉式選單前面的標題
	QLabel* loadTitle = new QLabel(QString(u8"載入類型 : "));

	//載入模式的下拉式選單

	QComboBox* loadModeComboBox = new QComboBox();
	loadModeComboBox->addItem(u8"盤面檔");
	loadModeComboBox->addItem(u8"指定炸彈數量盤面");
	loadModeComboBox->addItem(u8"指定炸彈生成率盤面");

	//新增UI物件到row1
	row1Layout->addWidget(loadTitle);
	row1Layout->addWidget(loadModeComboBox);
	row1Layout->addStretch(1);

	//文字輸入框，用於輸入盤面檔路徑
	QLineEdit* boardFilePath = new QLineEdit();
	boardFilePath->setText("./boards/board1.txt");

	//變數的標題，根據載入模式切換對應標題
	QLabel* varTitle = new QLabel(QString(u8"盤面檔位置 : "));
	QLabel* rowsTitle = new QLabel(QString(u8"Rows : "));
	QLabel* colsTitle = new QLabel(QString(u8"Cols : "));

	//rows的int輸入框
	QSpinBox* rowsInput = new QSpinBox();
	rowsInput->setValue(9);
	rowsInput->setMinimum(5);

	//cols的int輸入框
	QSpinBox* colsInput = new QSpinBox();
	colsInput->setValue(9);
	colsInput->setMinimum(5);

	//炸彈數量的int輸入框
	QSpinBox* randomCountInput = new QSpinBox();
	randomCountInput->setValue(10);
	randomCountInput->setMinimum(1);
	randomCountInput->setMaximum(2147483647);

	//炸彈生成率的float輸入框
	QDoubleSpinBox* randomRateInput = new QDoubleSpinBox();
	randomRateInput->setValue(0.2f);
	randomRateInput->setMinimum(0.0f);
	randomRateInput->setMaximum(1.0f);
	randomRateInput->setSingleStep(0.1f);

	//創建載入按鈕，並設定callback
	QPushButton* loadButton = new QPushButton("Load");
	connect(loadButton, &QPushButton::clicked, [=]() {
		//根據不同載入模式，執行Load指令
		switch (loadModeComboBox->currentIndex())
		{
		case 0:
		{
			gameCore->ExecuteCommand(string("Load BoardFile ") + boardFilePath->text().toStdString());
			break;
		}
		case 1:
		{
			gameCore->ExecuteCommand(string("Load RandomCount ") + to_string(rowsInput->value()) + ' ' + to_string(colsInput->value()) + ' ' + to_string(randomCountInput->value()));
			break;
		}
		case 2:
		{
			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << randomRateInput->value();
			gameCore->ExecuteCommand(string("Load RandomRate ") + to_string(rowsInput->value()) + ' ' + to_string(colsInput->value()) + ' ' + ss.str());
			break;
		}
		default:
			break;
		}

		});

	//新增UI物件到row2
	row2Layout->addWidget(varTitle);
	row2Layout->addWidget(boardFilePath);
	row2Layout->addWidget(loadButton);

	//在row2中加入Load模式為BoardFile時所需的UI物件
	auto AddLoadBoardFileWidget = [=](QLayout* layout) {
		row2Layout->addWidget(varTitle);
		row2Layout->addWidget(boardFilePath);
		row2Layout->addWidget(loadButton);
	};

	//在row2中加入Load模式為RandomCount時所需的UI物件
	auto AddLoadRandomCountWidget = [=](QLayout* layout) {
		row2Layout->addWidget(rowsTitle);
		row2Layout->addWidget(rowsInput);
		row2Layout->addWidget(colsTitle);
		row2Layout->addWidget(colsInput);
		row2Layout->addWidget(varTitle);
		row2Layout->addWidget(randomCountInput);
		row2Layout->addWidget(loadButton);
	};

	//在row2中加入Load模式為RandomRate時所需的UI物件
	auto AddLoadRandomRateWidget = [=](QLayout* layout) {
		row2Layout->addWidget(rowsTitle);
		row2Layout->addWidget(rowsInput);
		row2Layout->addWidget(colsTitle);
		row2Layout->addWidget(colsInput);
		row2Layout->addWidget(varTitle);
		row2Layout->addWidget(randomRateInput);
		row2Layout->addWidget(loadButton);
	};

	//清除並隱藏layout中所有物件
	auto RemoveHideAllFromLayout = [=](QLayout* layout) {
		QLayoutItem* item;
		while ((item = layout->takeAt(0))) {
			QWidget* widget = item->widget();
			layout->removeWidget(widget);
			widget->setVisible(false);
		}
	};

	//顯示layout中所有物件
	auto ShowAllFromLayout = [=](QLayout* layout) {
		for (int i = 0; i < layout->count(); i++) {
			QWidget* widget = layout->itemAt(i)->widget();
			if (widget) {
				widget->setVisible(true);
			}
		}
	};

	//設定切換load模式時的callback -> currentIndexChanged
	connect(loadModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		RemoveHideAllFromLayout(row2Layout); //先清除row2中的所有UI物件
		//根據模式加入所需的UI物件
		switch (index) {
		case 0:
			varTitle->setText(u8"盤面檔位置 : ");
			AddLoadBoardFileWidget(row2Layout);
			break;
		case 1:
			varTitle->setText(u8"炸彈數量 : ");
			AddLoadRandomCountWidget(row2Layout);
			break;
		case 2:
			varTitle->setText(u8"炸彈生成機率 : ");
			AddLoadRandomRateWidget(row2Layout);
			break;
		}
		ShowAllFromLayout(row2Layout); //顯示新加入的這些物件
		});

	//在row3中加入3個按鈕，用於執行Print指令
	QPushButton* printGameBoardButton = new QPushButton("Print GameBoard");
	QPushButton* printGameAnswerButton = new QPushButton("Print GameAnswer");
	QPushButton* printGameStateButton = new QPushButton("Print GameState");
	row3Layout->addWidget(printGameBoardButton);
	row3Layout->addWidget(printGameAnswerButton);
	row3Layout->addWidget(printGameStateButton);

	//設定按鈕的callback，執行Print指令
	connect(printGameBoardButton, &QPushButton::clicked, [=]() {
		gameCore->ExecuteCommand("Print GameBoard");
		});
	connect(printGameAnswerButton, &QPushButton::clicked, [=]() {
		gameCore->ExecuteCommand("Print GameAnswer");
		});
	connect(printGameStateButton, &QPushButton::clicked, [=]() {
		gameCore->ExecuteCommand("Print GameState");
		});

	//創建開始遊戲按鈕
	QPushButton* startGameButton = new QPushButton("StartGame");
	startGameButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//前後都加stretch 使按鈕置中
	row4Layout->addStretch(1);
	row4Layout->addWidget(startGameButton);
	row4Layout->addStretch(1);

	//設定開始遊戲按鈕的callback
	connect(startGameButton, &QPushButton::clicked, [this]() {
		//ExecuteCommand會回傳是否執行成功
		if (gameCore->ExecuteCommand("StartGame") == true)
		{
			//創建2D格狀盤面(以PuhButton組成)
			//因每次玩的盤面大小都可能不同，因此要全部重新創建
			CreateBoardGridGUI();
			//更新GUI (每格的輸出顯示)
			UpdateGUI();
			//從Standby介面切換成Playing介面
			SwitchPlayingLayout();
			//重置上一場遊戲失敗後爆炸的按鈕label
			triggerBombCellButtonLabel = nullptr;
		}

		});

	//在Standby介面中加入前面創建的4個row
	standbyLayout->addLayout(row1Layout);
	standbyLayout->addLayout(row2Layout);
	standbyLayout->addLayout(row3Layout);
	standbyLayout->addLayout(row4Layout);
	standbyLayout->addStretch(1); //置上顯示
}


/**
 * Intent : 創建Playing介面
 * Pre :
 * Post : 創建完畢
 */
void MineSweeperGUI::CreatePlayingLayout()
{
	//以playingWidget為基底，創建其直屬的layout (垂直)
	playingLayout = new QVBoxLayout(playingWidget);

	//建立顯示BombCount與FlagCount的水平layout
	showBombFlagCountLayout = new QHBoxLayout();
	bombCountLabel = new QLabel("Bomb Count : 0");
	flagCountLabel = new QLabel("Flag Count : 0");
	showBombFlagCountLayout->addWidget(bombCountLabel);
	showBombFlagCountLayout->addWidget(flagCountLabel);

	//建立顯示OpenBlankCount與RemainBlankCount的水平layout
	showBlankCountLayout = new QHBoxLayout();
	openBlankCountLabel = new QLabel("Open Blank Count : 0");
	remainBlankCountLabel = new QLabel("Remain Blank Count : 0");
	showBlankCountLayout->addWidget(openBlankCountLabel);
	showBlankCountLayout->addWidget(remainBlankCountLabel);

	//跟前面一樣，創建3個按鈕，用於執行Print指令
	QHBoxLayout* print3btnLayout = new QHBoxLayout();
	QPushButton* printGameBoardButton = new QPushButton("Print GameBoard");
	QPushButton* printGameAnswerButton = new QPushButton("Print GameAnswer");
	QPushButton* printGameStateButton = new QPushButton("Print GameState");

	connect(printGameBoardButton, &QPushButton::clicked, [=]() {
		gameCore->ExecuteCommand("Print GameBoard");
		});
	connect(printGameAnswerButton, &QPushButton::clicked, [=]() {
		gameCore->ExecuteCommand("Print GameAnswer");
		});
	connect(printGameStateButton, &QPushButton::clicked, [=]() {
		gameCore->ExecuteCommand("Print GameState");
		});

	print3btnLayout->addWidget(printGameBoardButton);
	print3btnLayout->addWidget(printGameAnswerButton);
	print3btnLayout->addWidget(printGameStateButton);

	//創建盤面的2D格狀layout
	boardGridLayout = new QGridLayout();
	boardGridLayout->setSpacing(0);
	boardGridLayout->setAlignment(Qt::AlignLeft);

	//在Playing介面中加入前面創建的4個row的layout
	playingLayout->addLayout(showBombFlagCountLayout);
	playingLayout->addLayout(showBlankCountLayout);
	playingLayout->addLayout(print3btnLayout);
	playingLayout->addLayout(boardGridLayout);
}

/**
 * Intent : 創建2D格狀盤面GUI
 * Pre :
 * Post :
 */
void MineSweeperGUI::CreateBoardGridGUI()
{
	//先把原先有的盤面上的按鈕清除
	if (!cellButtons.empty())
	{
		for (int i = 0; i < cellButtons.size(); i++)
		{
			for (int j = 0; j < cellButtons[i].size(); j++)
			{
				boardGridLayout->removeWidget(cellButtons[i][j]);
				delete cellButtons[i][j];
			}
		}
	}

	//根據rows cols計算出合適的視窗大小，並resize
	playingWidget->resize(gameCore->GetColCount() * CELL_BUTTON_SIZE_LENGTH + 20, gameCore->GetRowCount() * CELL_BUTTON_SIZE_LENGTH + 120);

	//清空button的2維陣列
	cellButtons.clear();
	cellButtonLabels.clear();

	//在Grid Layout中塞入按鈕，並設定callback
	for (int row = 0; row < gameCore->GetRowCount(); ++row)
	{
		//配置2維陣列空間
		cellButtons.push_back(std::vector<QPushButton*>());
		cellButtonLabels.push_back(std::vector<QLabel*>());

		for (int col = 0; col < gameCore->GetColCount(); ++col)
		{
			//創建按鈕
			QPushButton* cellButton = new QPushButton();

			//在按鈕之中創建水平layout，用於擺放label
			QHBoxLayout* layoutInsideButton = new QHBoxLayout(cellButton);

			//創建按鈕的label
			QLabel* cellButtonLabel = new QLabel();

			//置中擺放
			layoutInsideButton->addStretch(1);
			layoutInsideButton->addWidget(cellButtonLabel);
			layoutInsideButton->addStretch(1);

			//設定固定的按鈕大小
			cellButton->setFixedSize(CELL_BUTTON_SIZE);

			//變更字體大小
			QFont font = cellButtonLabel->font();
			font.setPointSize(CELL_BUTTON_SIZE_LENGTH * 0.5f);
			cellButtonLabel->setFont(font);

			//設定按鈕左鍵callback
			connect(cellButton, &QPushButton::clicked, [=]() {
				//執行LeftClick指令
				bool openSuccess = gameCore->ExecuteCommand("LeftClick " + to_string(row) + ' ' + to_string(col));

				//若沒有執行成功，直接擋掉
				if (openSuccess == false)
				{
					return;
				}

				//更新版面GUI
				UpdateGUI();

				//若遊戲沒有結束
				if (gameCore->GetGameState() == MineSweeperState::PLAYING)
				{
					//播放開啟空白格子的音效
                    soundMediaPlayer->setMedia(openBlankCellSoundLocation);
					soundMediaPlayer->play();
				}
				//若遊戲結束了 (輸or贏)
				else if (gameCore->GetGameState() == MineSweeperState::GAMEOVER)
				{
					//贏了
					if (gameCore->IsPlayerWin() == true)
					{
						//播放開啟空白格子的音效
                        soundMediaPlayer->setMedia(openBlankCellSoundLocation);
						soundMediaPlayer->play();
					}
					//輸了
					else
					{
						//播放炸彈爆炸聲
                        soundMediaPlayer->setMedia(QUrl(bombExplosionSoundLocation));
						soundMediaPlayer->play();

						//播放炸彈爆炸動畫
						cellButtonLabel->clear();
						cellButtonLabel->setMovie(bombExplosionMovie);
						bombExplosionMovie->start();

						//預先把播放爆炸動畫的label存起來，以便後續進行動畫停止播放
						triggerBombCellButtonLabel = cellButtonLabel;
					}

					//進入遊戲結束彈出式訊息框
					EnterGameOverMessageBox();
				}

				});

			//設定按鈕右鍵callback
			cellButton->setContextMenuPolicy(Qt::CustomContextMenu);
			connect(cellButton, &QPushButton::customContextMenuRequested, this, &MineSweeperGUI::RightClickCallback);

			//把按鈕與其label塞入2維陣列，以方便後續存取
			cellButtons[row].push_back(cellButton);
			cellButtonLabels[row].push_back(cellButtonLabel);

			//把按鈕加入Grid Layout
			boardGridLayout->addWidget(cellButton, row, col);
		}
	}
}

/**
 * Intent : 切換成Standby介面
 * Pre : 處於Playing介面
 * Post : 處於Standby介面
 */
void MineSweeperGUI::SwitchStandbyLayout()
{
	//把gameState介面從Playing介面中移除
	playingLayout->removeItem(gameStateLayout);

	//把gameState介面加到Standby的開頭 (也就是row0)
	standbyLayout->insertLayout(0, gameStateLayout);

	//從基底中，關閉Playing介面，並顯示Standby介面
	centralWidget->widget(1)->close();
	centralWidget->widget(0)->show();

	//設定主視窗使其不可縮放
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	//設定最小最大Size
	this->setMinimumSize(STANDBY_LAYOUT_SIZE);
	this->setMaximumSize(STANDBY_LAYOUT_SIZE);
	this->resize(STANDBY_LAYOUT_SIZE);
}

/**
 * Intent : 切換成Playing介面
 * Pre : 處於Standby介面
 * Post : 處於Playing介面
 */
void MineSweeperGUI::SwitchPlayingLayout()
{
	//把gameState介面從Standby介面中移除
	standbyLayout->removeItem(gameStateLayout);

	//把gameState介面加到Playing的開頭 (也就是row0)
	playingLayout->insertLayout(0, gameStateLayout);

	//從基底中，關閉Standby介面，並顯示Playing介面
	centralWidget->widget(0)->close();
	centralWidget->widget(1)->show();

	//設定主視窗使其可以縮放
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//設定最小最大Size
	this->setMinimumSize(playingWidget->size());
	this->setMaximumSize(playingWidget->size() * 2);

	//設定最小最大Size
	this->resize(playingWidget->size());
}



/**
 * Intent : 更新GUI (GameState、版面、Count)
 * Pre :
 * Post :
 */
void MineSweeperGUI::UpdateGUI()
{
	//獲取GameBoard的輸出
	vector<string> gameBoardOutput = gameCore->GameBoardOutput();

	//更新2D格狀盤面
	for (int i = 0; i < gameCore->GetRowCount(); i++)
	{
		for (int j = 0; j < gameCore->GetColCount(); j++)
		{
			QLabel* buttonLabel = cellButtonLabels[i][j];

			//預設先把所有已經顯示的東西清除
			buttonLabel->clear();

			//棋子
			if (gameBoardOutput[i][j] == 'f')
			{
				//把顯示label設定成棋幟的圖片
				buttonLabel->setPixmap(flagPixmap);
			}
			//問號
			else if (gameBoardOutput[i][j] == '?')
			{
				//把顯示label設定成問號的圖片
				buttonLabel->setPixmap(questionMarkPixmap);
			}
			//已開啟格(0~9)
			else if (gameBoardOutput[i][j] >= '0' && gameBoardOutput[i][j] <= '9')
			{
				//0的話，不特別顯示
				if (gameBoardOutput[i][j] == '0')
				{
					buttonLabel->setText(QString(""));
				}
				else
				{
					//顯示9宮格內炸彈數量
					buttonLabel->setText(QString(gameBoardOutput[i][j]));
				}

				//已開啟的格子背景色設成灰色
				cellButtons[i][j]->setStyleSheet("background-color: #969696;");
			}
			//炸彈，玩家踩到炸彈之後會顯示出來
			else if (gameBoardOutput[i][j] == 'X')
			{
				//如果該格就是被觸發的炸彈，就播放
				buttonLabel->setPixmap(bombPixmap);

				//炸彈格子的背景色設成紅色
				cellButtons[i][j]->setStyleSheet("background-color: #d95252;");
			}
		}
	}

	//更新state與count相關label
	gameStateContent->setText(QString::fromStdString(gameCore->GetGameStateStr()));
	bombCountLabel->setText(QString("Bomb Count : %1").arg(gameCore->GetBombCount()));
	flagCountLabel->setText(QString("Flag Count : %1").arg(gameCore->GetFlagCount()));
	openBlankCountLabel->setText(QString("Open Blank Count : %1").arg(gameCore->GetOpenBlankCount()));
	remainBlankCountLabel->setText(QString("Remain Blank Count : %1").arg(gameCore->GetRemainBlankCount()));
}

/**
 * Intent : 對按鈕按下右鍵的callback
 * Pre :
 * Post :
 * \param pos 滑鼠點擊的位置，不需使用
 */
void MineSweeperGUI::RightClickCallback(const QPoint& pos)
{
	//獲取被右鍵點擊到的按鈕
	QPushButton* clickTargetCellButton = qobject_cast<QPushButton*>(sender());

	//搜尋所有按鈕，找到是哪個row col的按鈕，並執行RightClick指令
	for (int i = 0; i < cellButtons.size(); i++)
	{
		for (int j = 0; j < cellButtons[i].size(); j++)
		{
			if (clickTargetCellButton == cellButtons[i][j])
			{
				gameCore->ExecuteCommand("RightClick " + to_string(i) + ' ' + to_string(j));
			}
		}
	}

	//更新GUI
	UpdateGUI();
}

/**
 * Intent : 進入遊戲結束彈出式訊息框
 * Pre : 遊戲結束
 * Post : 重玩或關閉遊戲
 */
void MineSweeperGUI::EnterGameOverMessageBox()
{
	if (gameCore->GetGameStateStr() != "GameOver")
	{
		return;
	}

	//顯示贏或輸
	gameOverMessageBox->setText(gameCore->IsPlayerWin() ? "You win the game" : "You lose the game");

	//進入彈出式訊息框，當使用者選擇按鈕按下後才會往下執行
	int ret = gameOverMessageBox->exec();

	//重新遊玩
	if (ret == QMessageBox::Reset)
	{
		if (gameCore->ExecuteCommand("Replay"))
		{
			//切換成Standby介面
			SwitchStandbyLayout();
			gameStateContent->setText(QString::fromStdString(gameCore->GetGameStateStr()));
		}
	}
	//離開遊戲
	else if (ret == QMessageBox::Close)
	{
		gameCore->ExecuteCommand("Quit");
	}
}


