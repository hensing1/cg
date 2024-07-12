#include <vector>
#include <string>

class CSV {
    public:
        static std::vector<std::vector<float>> read_csv(std::string filepath);
};
