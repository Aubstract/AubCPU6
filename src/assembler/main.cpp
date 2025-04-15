// Created 2025-04-14
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Line
{
private:
    int line_num;
    std::string line;
public:
    Line(int _line_num, std::string& _line) : line_num(_line_num), line(_line) {}
};

void sanitize_lines(std::vector<std::string>& lines)
{
    // For loop to iterate through the lines starting from the last line
    // and remove comments, leading/trailing whitespace, and empty lines
    for (int i = lines.size() - 1; i >= 0; --i)
    {
        std::string& line = lines[i];

        // Remove comments that start with "//"
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos)
        {
            line.erase(comment_pos);
        }

        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Remove empty lines
        if (line.empty())
        {
            lines.erase(lines.begin() + i);
        }
    }
}

int main()
{
    std::string file_path;
    std::cout << "Enter the source file path: ";
    std::cin >> file_path;

    std::ifstream src_file(file_path);
    if (!src_file.is_open())
    {
        std::cout << "Error: Could not open file " << file_path << std::endl;
        return 1;
    }
    // Copies the entire file into a string
    std::string src_code((std::istreambuf_iterator<char>(src_file)), std::istreambuf_iterator<char>());

    // Split the string into lines
    std::vector<std::string> lines;
    std::string line;
    std::istringstream stream(src_code);
    while (std::getline(stream, line))
    {
        lines.push_back(line);
    }

    // Sanitize the lines
    sanitize_lines(lines);



    src_file.close();
    return 0;
}
