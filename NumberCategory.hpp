#pragma once

#include <set>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>

class NumberCategory
{
public:
    NumberCategory() = default;

    // Return true if the number matches the category
    virtual bool Match(unsigned int number) const = 0;
    // Handle the number (must match the cagetory!)
    virtual void Handle(unsigned int number) = 0;

    void send_output()
    {
        if (_pipe == -1)
            return;

        // std::cout << _stream.str();
        std::string buffer(_stream.str());
        write(_pipe, buffer.c_str(), buffer.size() + 1);
        _stream.str(std::string());
    }

    static bool set_pipe(const char *pipename)
    {
        _pipe = open(pipename, O_WRONLY);
        if (_pipe == -1)
            return false;

        return true;
    }

protected:
    std::stringstream _stream;
    static int _pipe;
};

class NumberCategoryOdd : public NumberCategory
{
public:
    NumberCategoryOdd() = default;

    bool Match(unsigned int number) const override
    {
        return number % 2 == 1;
    }

    void Handle(unsigned int number) override
    {
        assert(Match(number));
        // Just write it out
        _stream << number << ": this is an odd number" << std::endl;
        send_output();
    }
};

class NumberCategoryStartOne : public NumberCategory
{
public:
    NumberCategoryStartOne() = default;

    bool Match(unsigned int number) const override
    {
        while (number > 9)
            number /= 10;

        return number == 1;
    }

    void Handle(unsigned int number) override
    {
        assert(Match(number));
        // Generate "number" random numbers
        std::vector<unsigned int> numbers;
        for (unsigned int i = 0; i < number; i++)
            numbers.emplace_back(std::rand() % 999 + 1);

        // print 'em out
        _stream << number << ": starts with 1 [origin list:";
        for (auto value : numbers)
            _stream << " " << value;

        // then print 'em out again in ascending order
        std::sort(numbers.begin(), numbers.end());
        _stream << " Ordered list: ";
        for (auto value : numbers)
            _stream << " " << value;

        _stream << "]" << std::endl;
        send_output();
    }
};

class NumberCategoryHundreds : public NumberCategory
{
public:
    NumberCategoryHundreds() = default;

    bool Match(unsigned int number) const override
    {
        return number > 99 && number < 1000;
    }

    void Handle(unsigned int number) override
    {
        assert(Match(number));

        auto orig = number;
        // Calculate multiplication of digits
        unsigned int sum = number % 10;
        while (number > 9)
        {
            number /= 10;
            sum *= number % 10;
        }

        // Print it out
        _stream << orig << ": the digit’s multiplications are equal to " << sum << std::endl;
        send_output();
    }
};

class NumberCategoryOther : public NumberCategory
{
public:
    NumberCategoryOther() = default;

    bool Match(unsigned int number) const override
    {
        return true;
    }

    void Handle(unsigned int number) override
    {
        assert(Match(number));
        // Just write it out
        _stream << number << ": this number belongs to the others" << std::endl;
        send_output();
    }
};
