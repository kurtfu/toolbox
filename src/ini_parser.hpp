#ifndef INI_PARSER_HPP
#define INI_PARSER_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#include "utils.hpp"

#include <cstddef>
#include <format>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

#if defined(__GNUC__) || defined(__clang__)
    #include <cxxabi.h>
    #include <memory>
#endif

/*****************************************************************************/
/*** CLASSES *****************************************************************/

namespace ini_parser
{
    namespace detail
    {
        [[nodiscard]] inline std::string_view trim(std::string_view text) noexcept
        {
            constexpr std::string_view whitespace = "\t\r\n";

            const auto begin = text.find_first_not_of(whitespace);
            const auto end = text.find_last_not_of(whitespace);

            if (begin == std::string_view::npos)
            {
                return {};
            }

            return text.substr(begin, end - begin + 1);
        }

        inline std::string demangle(const char* name)
        {
#if defined(__GNUC__) || defined(__clang__)
            int status = 0;

            std::unique_ptr<char> result{abi::__cxa_demangle(name, nullptr, nullptr, &status)};
            return (status == 0) ? result.get() : name;
#else
            return name;
#endif
        }

        template <typename T>
        T convert(const std::string& input)
        {
            std::istringstream iss(input);
            T value;

            if (!(iss >> std::boolalpha >> value) || !(iss >> std::ws).eof())
            {
                panic(std::format("error: invalid conversion from '{}' to '{}'", input, demangle(typeid(T).name())));
            }

            return value;
        }

        template <>
        inline std::string convert<std::string>(const std::string& input)
        {
            return input;
        }
    }  // namespace detail

    template <typename T>
    struct converter_t
    {
        static T from(const std::vector<std::string>& input)
        {
            return detail::convert<T>(input.back());
        }
    };

    template <typename T>
    struct converter_t<std::vector<T>>
    {
        static std::vector<T> from(const std::vector<std::string>& input)
        {
            std::vector<T> result;

            for (const auto& entry : input)
            {
                std::istringstream iss(entry);
                std::string item;

                while (std::getline(iss, item, ','))
                {
                    result.emplace_back(std::move(detail::convert<T>(item)));
                }
            }
        }
    };

    class result_t
    {
    public:
        static result_t from_string(const std::string& content)
        {
            std::istringstream iss(content);

            std::size_t line = 0;
            std::string section;

            result_t result{};

            for (std::string input; std::getline(iss, input);)
            {
                line += 1;
                std::string_view text = input;

                if (auto comment = text.find_first_of("#;"); comment != std::string_view::npos)
                {
                    text = text.substr(0, comment);
                }

                if (text = detail::trim(text); text.empty())
                {
                    continue;
                }

                if (text.front() == '[')
                {
                    if (text.back() != ']')
                    {
                        panic(std::format("error: invalid section from at line {}", line));
                    }

                    section = std::string(detail::trim(text.substr(1, text.size() - 2)));
                    continue;
                }

                auto equals = text.find('=');

                if (equals == std::string_view::npos)
                {
                    panic(std::format("error: invalid key-value form at line {}", line));
                }

                auto key = std::string(detail::trim(text.substr(0, equals)));
                auto value = std::string(detail::trim(text.substr(equals + 1)));

                if (key.empty())
                {
                    panic(std::format("error: empty key at line {}", line));
                }

                result.m_data[section][key].emplace_back(std::move(value));
            }

            return result;
        }

        static result_t from_file(const char* path)
        {
            std::ifstream file(path);

            if (!file.is_open())
            {
                panic(std::format("error: file '{}' does not exists", path));
            }

            std::stringstream ss;
            ss << file.rdbuf();

            return from_string(ss.str());
        }

        static result_t from_file(const std::string& path)
        {
            return from_file(path.data());
        }

        static result_t from_file(std::string_view path)
        {
            return from_file(path.data());
        }

        [[nodiscard]] bool has(const std::string& section) const
        {
            return m_data.contains(section);
        }

        [[nodiscard]] bool has(const std::string& section, const std::string& key) const
        {
            auto iter = m_data.find(section);
            return iter != m_data.end() && iter->second.contains(key);
        }

        template <typename T>
        [[nodiscard]] T get(const std::string& section, const std::string& key) const
        {
            return converter_t<T>::from(raw(section, key));
        }

        template <typename T>
        [[nodiscard]] T get(const std::string& section, const std::string& key, const T& fallback) const
        {
            return has(section, key) ? get<T>(section, key) : fallback;
        }

    private:
        [[nodiscard]] const std::vector<std::string>& raw(const std::string& section, const std::string& key) const
        {
            if (!has(section, key))
            {
                panic(std::format("error: missing key '{}' in section '{}'", key, section));
            }

            return m_data.at(section).at(key);
        }

        std::map<std::string, std::map<std::string, std::vector<std::string>>> m_data;
    };
}  // namespace ini_parser

#endif  // INI_PARSER_HPP
