#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <initializer_list>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>


namespace utils
{
std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}

using highlighter_table_t = std::map<size_t, std::pair<std::string, std::string>>;
template<class T>
std::ostream& print_array_with_indices(std::ostream& out, const std::vector<T>& A, const highlighter_table_t& highlight_indices = {})
{
    std::stringstream values;
    std::stringstream indices;

    for(size_t i = 0; i < A.size(); i ++)
    {
        std::string value_formatted_str;
        std::string index_str = std::to_string(i);

        auto highlighted_pos_it = highlight_indices.find(i);
        if (highlighted_pos_it != highlight_indices.end())
        {
            value_formatted_str = std::get<0>(highlighted_pos_it->second) + std::to_string(A[i]) + std::get<1>(highlighted_pos_it->second);
        }
        else
        {
            value_formatted_str = std::to_string(A[i]);
        }

        auto max_column_size = std::max(value_formatted_str.size(), index_str.size());
        value_formatted_str.resize(max_column_size, ' ');

        std::string index_formatted_str(max_column_size - index_str.size(), ' ');
        index_formatted_str.insert(index_str.size() / 2, index_str);

        if (highlighted_pos_it != highlight_indices.end())
        {
            values << bold_on << value_formatted_str << bold_off << ", ";
            indices << bold_on << index_formatted_str << bold_off << "  ";
        }
        else
        {
            values << value_formatted_str << ", ";
            indices << index_formatted_str << "  ";
        }
    }

    out << "array: " << values.str() << std::endl;
    out << "index: " << indices.str() << std::endl;

    return out;
}


template<class T>
std::ostream& print_array_with_indices(std::ostream& out, const std::vector<T>& array)
{
    return print_array_with_indices(out, array);
}

template<class T>
std::ostream& print_array_with_indices(std::ostream& out, const std::vector<T>& A, std::initializer_list<size_t> highlight_indices = {})
{
    highlighter_table_t highlight_indices_with_text;
    for(size_t i : highlight_indices)
    {
        if(highlight_indices_with_text.empty())
        {
            std::get<0>(highlight_indices_with_text[i]) = "[";
            std::get<1>(highlight_indices_with_text[i]) = "]";
        }
        else
        {
            std::get<0>(highlight_indices_with_text[i]) = "(";
            std::get<1>(highlight_indices_with_text[i]) = ")";
        }
    }
    return print_array_with_indices(out, A, highlight_indices_with_text);
}
}

#endif
