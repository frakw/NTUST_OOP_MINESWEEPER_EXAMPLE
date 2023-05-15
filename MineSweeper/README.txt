###此資料夾存放路徑中，不可有中文或其他unicode字元###
1. 設定cl.exe到環境變數中
	(1) 開啟x64 Native Tools Command Prompt for VS 2019 / 2022 (可用win10的搜尋去找)
	(2) 輸入where cl得到cl.exe位置
	(3) 複製 cl.exe 的路徑(不含cl.exe)，「進階系統設定」->「環境變數」
	(4) 找到「系統變數」中的「Path」變數，編輯此變數，在最後面加上 cl.exe 的路徑。
2. 點兩下執行 CLICKME.bat 產生Visual Studio專案
3. 點兩下執行 ./VSProject/MineSweeper.GUI.Qt.vcxproj
4. 自動創建Visual Studio專案，可將.sln檔存起來
5. 執行專案
6. 如果希望看到qDebug輸出的訊息，右鍵專案 -> 屬性 -> 連結器 -> 子系統 改成Console (/SUBSYSTEM:CONSOLE)

P.S. 如果出現"新行字元..."相關錯誤訊息，請將MineSweeperGUI.cpp以UTF-8(有簽章)格式儲存檔案，不可使用BIG5或UTF-8(無簽章)