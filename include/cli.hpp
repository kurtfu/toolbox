#ifndef CLI_HPP
#define CLI_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include <replxx.hxx>

/// \cond
#include <string>
#include <string_view>

/// \endcond

/*****************************************************************************/
/*** CLASSES *****************************************************************/

class cli
{
public:
    static void println(std::string_view message)
    {
        auto& self = instance();

        self.m_terminal.print(message.data());
        self.m_terminal.print("\n");
    }

    static std::string input()
    {
        auto& self = instance();
        const auto* prompt = self.m_terminal.input("> ");

        if (!prompt)
        {
            return "";
        }

        self.m_terminal.history_add(prompt);
        return prompt;
    }

    static void save(std::string_view file)
    {
        auto& self = instance();
        self.m_terminal.history_load(file.data());
    }

    static void load(std::string_view file)
    {
        auto& self = instance();
        self.m_terminal.history_save(file.data());
    }

private:
    cli() = default;

    static cli& instance()
    {
        static cli instance;
        return instance;
    }

    replxx::Replxx m_terminal;
};

#endif  // CLI_HPP
