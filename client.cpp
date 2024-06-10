#include <vector>
#include <memory>

#include "NumberCategory.hpp"

int NumberCategory::_pipe = -1;

class NumberHandler : protected std::vector<std::unique_ptr<NumberCategory>>
{
public:
    NumberHandler()
    {
        // Initialize the categories array
        emplace_back(std::make_unique<NumberCategoryOdd>());
        emplace_back(std::make_unique<NumberCategoryStartOne>());
        emplace_back(std::make_unique<NumberCategoryHundreds>());
        emplace_back(std::make_unique<NumberCategoryOther>());
    }

    void Handle(unsigned int value) const
    {
        for (auto &category : *this)
        {
            if (!category->Match(value))
                continue;

            category->Handle(value);
            break;
        }
    }
};

int main(int argc, char *argv[])
{
    NumberHandler handler;
    std::srand(time(NULL));

    // Retrieve the number of random numbers we shall generate
    if (argc != 3)
    {
        std::cout << "Please provide the amount of numbers to generate (between 1 and 999), and the pipe name to send results on" << std::endl;
        return -1;
    }

    char *end;
    auto amount = std::strtoul(argv[1], &end, 0);
    if (amount < 1 || *end != '\0' || amount > 999)
    {
        std::cout << "Invalid input: " << argv[1] << std::endl;
        return -2;
    }

    // Retrieve the name of the pipe to send the output on
    if (!NumberCategory::set_pipe(argv[2]))
    {
        std::cout << "Failed to open pipe at " << argv[2] << std::endl;
        return -3;
    }

    for (unsigned int i = 0; i < amount; i++)
    {
        unsigned int value = std::rand() % 999 + 1;
        handler.Handle(value);
    }

    return 0;
}
