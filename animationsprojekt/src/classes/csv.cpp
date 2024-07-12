#include "csv.hpp"
#include <fstream>
#include <sstream>
#include <string>

std::vector<std::vector<float>> CSV::read_csv(std::string filepath) {
    std::ifstream csv_file(filepath);
    std::vector<std::vector<float>> result{};

    std::string line;
    std::string token;
    std::string::size_type pos;

    while (std::getline(csv_file, line)) {
        std::stringstream line_stream(line);
        std::vector<float> line_numbers;
        while (std::getline(line_stream, token, ',')) {
            line_numbers.push_back(std::stof(token));
        }
        result.push_back(line_numbers);
    }

    return result;
}
