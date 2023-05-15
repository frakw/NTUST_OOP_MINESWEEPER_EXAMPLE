# NTUST_OOP_MINESWEEPER_EXAMPLE
 Project1-MineSweeper-助教範例-Source Code

## 環境
* Qt 5.15.2 msvc2019_64
* Visual Studio 2019 16.11.24
* Windows 10

## 編譯方式 (需先安裝好Qt)
1. 使用Qt Creator開啟MineSweeper.pro
2. 指定Qt版本
3. 將resources資料夾中的四個子資料夾放入建置資料夾中 EX: build-MineSweeperQt-Desktop_Qt_5_15_2_MSVC2019_64bit-Release
4. 按下Qt Creator左下角三角形按鈕後，即可編譯與執行

## 靜態編譯方式 (Windows) <--- 推薦使用
1. 下載Qt 5.15.2靜態編譯版本壓縮檔，下載連結 : http://gg.gg/qt-static-5-15-2
2. 解壓縮到MineSweeper資料夾內
3. 設定cl.exe到環境變數中
    - 開啟x64 Native Tools Command Prompt for VS 2019 / 2022 (可用win10的搜尋去找)
    - 輸入where cl得到cl.exe位置
    - 複製 cl.exe 的路徑(不含cl.exe)，「進階系統設定」->「環境變數」
    - 找到「系統變數」中的「Path」變數，編輯此變數，在最後面加上 cl.exe 的路徑。
4. 點兩下執行 ./MineSweeper/CLICKME.bat 產生Visual Studio專案
5. 點兩下執行 ./MineSweeper/VSProject/MineSweeper.vcxproj
6. 自動創建Visual Studio專案，可將.sln檔存起來

> P.S. 如果出現"新行字元..."相關錯誤訊息，請將MineSweeperGUI.cpp以UTF-8(有簽章)格式儲存檔案，不可使用BIG5或UTF-8(無簽章)