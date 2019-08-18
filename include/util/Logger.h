#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H

#include <spdlog/fmt/fmt.h>  // fmt::format

#include <sstream>  // std::ostringstream

namespace util
{
/// @brief ログ出力
class Logger
{
    std::size_t m_memory_usage_at_start{0};  ///< メソッド開始時点のメモリ使用量

public:
    /// @brief ログ出力レベル
    /// pre系以外のメソッドで指定
    enum class Level
    {
        INF,  ///< infoレベル
        WRN,  ///< warnレベル
        ERR   ///< errorレベル
    };

    /// @brief デフォルトのロガーを設定する
    struct LoggersRegister
    {
        LoggersRegister() = delete;
        explicit LoggersRegister(const std::string& the_category_name);
    };

    /// @brief メソッド開始時のログ出力 (引数なし)
    /// infoレベルで、日時、ファイル名、行番号、メソッド名、開始メッセージ、開始時点のメモリ使用量を出力する
    /// @param [i] the_file_name ファイル名 (マクロでは__FILE__を指定)
    /// @param [i] the_line_number 行番号 (マクロでは__LINE__を指定)
    /// @param [i] the_function_name メソッド名 (マクロでは__func__を指定)
    void pre(
        const std::string& the_file_name,
        int the_line_number,
        const std::string& the_function_name);

    /// @brief メソッド開始時のログ出力 (引数あり)
    /// infoレベルで、日時、ファイル名、行番号、メソッド名、開始メッセージ、引数の内容、開始時点のメモリ使用量を出力する
    /// @tparam Args 引数群(可変)の型
    /// @param [i] the_file_name ファイル名 (マクロでは__FILE__を指定)
    /// @param [i] the_line_number 行番号 (マクロでは__LINE__を指定)
    /// @param [i] the_function_name メソッド名 (マクロでは__func__を指定)
    /// @param [i] the_args 引数群
    /// @note 引数に対応したop<<の定義が必要
    template <typename... Args>
    void pre(
        const std::string& the_file_name,
        int the_line_number,
        const std::string& the_function_name,
        Args... the_args)
    {
        std::ostringstream oss;
        oss << "start";
        if (0 < sizeof...(the_args))
        {
            oss << ", with args: ";
            args(oss, std::forward<Args>(the_args)...);
        }

        pre_impl(the_file_name, the_line_number, the_function_name, oss.str());
    }

    /// @brief メソッド終了時のログ出力 (戻り値指定なし)
    /// 指定レベルで、日時、ファイル名、行番号、メソッド名、終了メッセージ、終了時点のメモリ使用量を出力する
    /// @param [i] the_level ログ出力レベル
    /// @param [i] the_file_name ファイル名 (マクロでは__FILE__を指定)
    /// @param [i] the_line_number 行番号 (マクロでは__LINE__を指定)
    /// @param [i] the_function_name メソッド名 (マクロでは__func__を指定)
    void post(
        Level the_level,
        const std::string& the_file_name,
        int the_line_number,
        const std::string& the_function_name);

    /// @brief メソッド終了時のログ出力 (戻り値指定あり)
    /// 指定レベルで、日時、ファイル名、行番号、メソッド名、終了メッセージ、戻り値、終了時点のメモリ使用量を出力する
    /// @tparam U 戻り値の型
    /// @param [i] the_level ログ出力レベル
    /// @param [i] the_file_name ファイル名 (マクロでは__FILE__を指定)
    /// @param [i] the_line_number 行番号 (マクロでは__LINE__を指定)
    /// @param [i] the_function_name メソッド名 (マクロでは__func__を指定)
    /// @param [i] the_result 戻り値
    /// @note 戻り値に対応したop<<の定義が必要
    template <typename U>
    void post(
        Level the_level,
        const std::string& the_file_name,
        int the_line_number,
        const std::string& the_function_name,
        const U& the_result)
    {
        post_impl(
            the_level,
            the_file_name,
            the_line_number,
            the_function_name,
            fmt::format(
                "end, result={}",
                the_result));
    }

private:
    /// @brief メソッド開始時のログ出力 共通処理
    /// @param [i] the_file_name ファイル名
    /// @param [i] the_line_number 行番号
    /// @param [i] the_function_name メソッド名
    /// @param [i] the_message ログメッセージ文字列
    void pre_impl(
        const std::string& the_file_name,
        int the_line_number,
        const std::string& the_function_name,
        const std::string& the_message);

    /// @brief メソッド終了時のログ出力 共通処理
    /// @param [i] the_level ログ出力レベル
    /// @param [i] the_file_name ファイル名
    /// @param [i] the_line_number 行番号
    /// @param [i] the_function_name メソッド名
    /// @param [i] the_message ログメッセージ文字列
    void post_impl(
        Level the_level,
        const std::string& the_file_name,
        int the_line_number,
        const std::string& the_function_name,
        const std::string& the_message);

    /// @brief 引数出力 共通処理 (引数がないケース用)
    void args(
        std::ostream& the_ostream);

    /// @brief 引数出力 共通処理 (1個以上の引数があるケース用)
    /// @tparam Arg1 引数(1)の型
    /// @tparam Args 2つ目以降(0個以上)の引数群の型
    /// @param [i] the_arg1 引数1
    /// @param [i] the_args 2つ目以降の引数群
    template <typename Arg1, typename... Args>
    void args(
        std::ostream& the_ostream,
        Arg1&& the_arg1,
        Args&&... the_args)
    {
        the_ostream << the_arg1;
        if (sizeof...(the_args) == 0)
        {
            return;
        }
        the_ostream << ",";
        args(the_ostream, std::forward<Args>(the_args)...);
    }
};

/// シングルトンパターンによりLoggerのインスタンスを提供
Logger& theLogger();

}  // namespace util

/// メソッド開始ログ・引数なし
#define UTIL_LOGGING_PRE0() util::theLogger().pre(__FILE__, __LINE__, __func__)

/// メソッド開始ログ・引数あり
#define UTIL_LOGGING_PRE(...) util::theLogger().pre(__FILE__, __LINE__, __func__, __VA_ARGS__)

/// メソッド終了ログ・戻り値なし
#define UTIL_LOGGING_POST0(the_level) util::theLogger().post(the_level, __FILE__, __LINE__, __func__)

/// メソッド終了ログ・戻り値あり
#define UTIL_LOGGING_POST(the_level, the_result) util::theLogger().post(the_level, __FILE__, __LINE__, __func__, the_result)

#endif  // UTIL_LOGGER_H
