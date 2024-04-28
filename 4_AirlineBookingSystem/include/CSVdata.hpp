#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class CSVdata
{
public:
    void loadFromFile(std::string filepath)
    {
        std::ifstream file(filepath);
        if (!file) return;
        m_data.clear();
        std::string strLine;
        while (std::getline(file, strLine))
        {
            m_data.emplace_back();
            std::string strCell;
            for (auto &&ch : strLine)
            {
                if (ch == '\r') break;
                if (ch == ',')
                {
                    m_data.back().emplace_back(strCell);
                    strCell.clear();
                    continue;
                }
                strCell += ch;
            }
            m_data.back().emplace_back(strCell);
            strCell.clear();
        }
        file.close();
    }

    void saveToFile(std::string filepath)
    {
        std::ofstream file(filepath);
        for (auto &&row : m_data)
        {
            for (size_t i = 0; i < row.size(); ++ i)
                file << (i > 0 ? "," : "") << row[i];
            file << "\n";
        }
        file.close();
    }

    std::vector<std::vector<std::string>> m_data;

};