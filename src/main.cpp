/* Copyright © 2021 PurpleAzurite

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <vector>

struct Item {
    char type;
    std::string permissions;
    std::string size;
    std::time_t time;
    std::string name;
};

void printHeaders()
{
#ifdef FIE_COMPILER_MSVC
    fmt::print("Permissions");
    fmt::print(" ");
    fmt::print("Size");
    fmt::print("  ");
    fmt::print("Last Modified");
    fmt::print("     ");
    fmt::print("Name\n");
#else
    fmt::print(fmt::emphasis::underline, "Permissions");
    fmt::print(" ");
    fmt::print(fmt::emphasis::underline, "Size");
    fmt::print("  ");
    fmt::print(fmt::emphasis::underline, "Last Modified");
    fmt::print("     ");
    fmt::print(fmt::emphasis::underline, "Name\n");
#endif
}

char getType(const std::filesystem::directory_entry& entry)
{
    if (entry.is_directory())
        return 'd';

    else
        return '.';
}

std::string getPerms(std::filesystem::perms permission)
{
    auto output = std::string();

    // clang-format off
        (permission & std::filesystem::perms::owner_read)
        != std::filesystem::perms::none ? output.append("r") : output.append("-");
        (permission & std::filesystem::perms::owner_write)
        != std::filesystem::perms::none ? output.append("w") : output.append("-");
        (permission & std::filesystem::perms::owner_exec)
        != std::filesystem::perms::none ? output.append("x") : output.append("-");
        (permission & std::filesystem::perms::group_read)
        != std::filesystem::perms::none ? output.append("r") : output.append("-");
        (permission & std::filesystem::perms::group_write)
        != std::filesystem::perms::none ? output.append("w") : output.append("-");
        (permission & std::filesystem::perms::group_exec)
        != std::filesystem::perms::none ? output.append("x") : output.append("-");
        (permission & std::filesystem::perms::others_read)
        != std::filesystem::perms::none ? output.append("r") : output.append("-");
        (permission & std::filesystem::perms::others_write)
        != std::filesystem::perms::none ? output.append("w") : output.append("-");
        (permission & std::filesystem::perms::others_exec)
        != std::filesystem::perms::none ? output.append("x") : output.append("-");
    // clang-format on

    return output;
}

std::string toStr(double value)
{
    auto stream = std::ostringstream();
    stream.precision(3);
    stream << value;

    return stream.str();
}

std::string getSize(const std::filesystem::directory_entry& entry)
{
    auto output = std::string();

    if (entry.is_regular_file())
    {
        double size = entry.file_size();
        size = ceil(size);
        auto sizeStr = std::string();

        if (size < 1024)
        {
            auto str = toStr(size);
            output.append(str);
        }

        else if (1024 <= size && size < 1048576)
        {
            size /= 1024;
            auto str = toStr(size);
            output.append(toStr(size));
            output.append("K");
        }

        else if (1048576 <= size && size < 1073741824)
        {
            output.append(toStr(size / 1048576));
            output.append("M");
        }

        else if (1073741824 <= size)
        {
            output.append(toStr(size / 1073741824));
            output.append("G");
        }
    }

    while (output.size() < 5)
        output.append(" ");

    return output;
}

std::time_t getDate(const std::filesystem::directory_entry& entry)
{
    using namespace std::chrono;

    auto time =
        system_clock::to_time_t(file_clock::to_sys(entry.last_write_time()));

    return time;
}

std::string getName(const std::filesystem::directory_entry& entry)
{
    auto output = entry.path().stem().string();
    output.append(entry.path().extension().string());

    return output;
}

int main(int argc, char* argv[])
{

    auto path = argc == 2 ? argv[1] : std::filesystem::current_path();

    if (!std::filesystem::exists(path))
    {
        fmt::print(fg(fmt::color::crimson), "No such path in filesystem.\n");
        return 1;
    }

    auto items = std::vector<Item>();

    printHeaders();

    auto dirItr = std::filesystem::directory_iterator(path);
    auto entries = std::vector<std::filesystem::directory_entry>();

    for (const auto& i : dirItr)
    {
        auto type = getType(i);
        auto perm = getPerms(std::filesystem::status(i).permissions());
        auto size = getSize(i);
        auto time = getDate(i);
        auto name = getName(i);

        Item item{type, perm, size, time, name};
        items.push_back(item);
    }

    std::sort(items.begin(), items.end(),
              [](const Item& a, const Item& b) { return a.name < b.name; });

    for (const auto& i : items)
    {
        fmt::print(
            "{0}{1}  {2} {3} {4}\n", i.type, i.permissions, i.size,
            fmt::format("{:%y-%m-%d %H:%M:%S}", *std::localtime(&i.time)),
            i.name);
    }

    return 0;
}
