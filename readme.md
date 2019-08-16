# CppTemplate

C++プロジェクトのテンプレート

## Features

* コーディング系
  * clang-format対応
  * ログ出力 (by spdlog)
* テスト系
  * ユニットテスト (by Catch2)
  * テストカバレッジ取得 (by gcovr)
* ビルド系
  * cmakeプロジェクト
  * ビルド時にCppCheck・clang-tidyを自動実行
  * プリコンパイルヘッダー (by cotire)
