#ifndef LOGGER_HPP
#define LOGGER_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include <quill/Backend.h>
#include <quill/Frontend.h>

#include <quill/LogFunctions.h>
#include <quill/Logger.h>

#include <quill/sinks/ConsoleSink.h>
#include <quill/sinks/FileSink.h>

#include <quill/core/PatternFormatterOptions.h>

/// \cond
#include <memory>
#include <string>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** CLASSES *****************************************************************/

class logger
{
    using logger_t = quill::Logger*;

public:
    template <typename Logger, typename... Args>
    using info = quill::info<Logger, Args...>;

    template <typename Logger, typename... Args>
    using warning = quill::warning<Logger, Args...>;

    template <typename Logger, typename... Args>
    using error = quill::error<Logger, Args...>;

    static void init(const std::string& file = "")
    {
        auto& self = instance();

        quill::Backend::start();
        quill::PatternFormatterOptions format("%(time) [%(thread_id)] %(short_source_location:<28) %(log_level:<9) %(message)");

        auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("console");
        self.m_console_logger = quill::Frontend::create_or_get_logger("console", std::move(console_sink), format);

        if (!file.empty())
        {
            auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>(file);
            self.m_file_logger = quill::Frontend::create_or_get_logger("file", std::move(file_sink), format);
        }
    }

    static logger_t file()
    {
        auto& self = instance();
        return self.m_file_logger;
    }

    static logger_t console()
    {
        auto& self = instance();
        return self.m_console_logger;
    }

private:
    logger() = default;

    static logger& instance()
    {
        static logger instance;
        return instance;
    }

    logger_t m_file_logger = nullptr;
    logger_t m_console_logger = nullptr;
};

#endif  // LOGGER_HPP
