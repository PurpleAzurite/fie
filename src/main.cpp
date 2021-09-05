#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <vector>

namespace ByteSize {

constexpr auto kb = 1'000;
constexpr auto mb = 1'000'000;
constexpr auto gb = 1'000'000'000;
constexpr auto tb = 1'000'000'000'000;

} // namespace ByteSize

struct Item
{
    char type;
    std::string permissions;
    std::string size;
    std::string time;
    std::string name;
};

/* Modify this function to change how the headers are displayed */
void printHeaders()
{
    fmt::print(fmt::emphasis::underline, "Permissions");
    fmt::print(" ");
    fmt::print(fmt::emphasis::underline, "Size");
    fmt::print("   ");
    fmt::print(fmt::emphasis::underline, "Last Modified");
    fmt::print("      ");
    fmt::print(fmt::emphasis::underline, "Name\n");
}

/* Symlinks should have priority over other types */
char getType(const std::filesystem::directory_entry& entry)
{
    if (entry.is_symlink())
        return 'l';

    else if (entry.is_directory())
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
        double size = static_cast<double>(entry.file_size());
        size = ceil(size);
        auto sizeStr = std::string();

        if (size < ByteSize::kb)
        {
            auto str = toStr(size);
            output.append(str);
        }

        else if (ByteSize::kb <= size && size < ByteSize::mb)
        {
            size /= ByteSize::kb;
            auto str = toStr(size);
            output.append(toStr(size));
            output.append("K");
        }

        else if (ByteSize::mb <= size && size < ByteSize::gb)
        {
            output.append(toStr(size / ByteSize::mb));
            output.append("M");
        }

        else if (ByteSize::gb <= size && size < ByteSize::tb)
        {
            output.append(toStr(size / ByteSize::gb));
            output.append("G");
        }

        else if (ByteSize::tb <= size)
        {
            output.append(toStr(size / ByteSize::tb));
            output.append("T");
        }
    }

    while (output.size() < 5)
        output.append(" ");

    return output;
}

std::string getTime(const std::filesystem::directory_entry& entry)
{
    using namespace std::chrono;

    auto output = std::string();

    if (entry.is_regular_file() || entry.is_directory())
    {
        auto time = system_clock::to_time_t(
            file_clock::to_sys(entry.last_write_time()));

        output = fmt::format("{:%y-%m-%d %H:%M:%S}", *std::localtime(&time));
    }

    while (output.size() < 17)
        output.append(" ");

    return output;
}

std::string getName(const std::filesystem::directory_entry& entry)
{
    auto output = entry.path().stem().string();
    output.append(entry.path().extension().string());

    if (entry.is_symlink())
    {
        output.append(" -> ");
        output.append(std::filesystem::read_symlink(entry));
    }

    return output;
}

int main(int argc, char* argv[])
{
    auto path = std::filesystem::current_path();

    if (argc > 1)
    {
        if (std::string(argv[1]).find("--version") != std::string::npos)
        {
            fmt::print(fg(fmt::color::aqua), "fie v0.3.0\n");

            return 0;
        }

        else if (std::string(argv[1]).find("--help") != std::string::npos)
        {
            fmt::print(fg(fmt::color::aqua), "Usage: fie <args>\n");
            fmt::print(fg(fmt::color::antique_white),
                       "\t--help\t\tShows this message\n");
            fmt::print(fg(fmt::color::antique_white),
                       "\t--version\tDisplays program version information\n\n");
            fmt::print(fg(fmt::color::antique_white),
                       "\t<path>\t\tShows the content of path instead of "
                       "current directory\n\n");

            return 0;
        }

        else
            path = argv[1];
    }

    if (!std::filesystem::exists(path))
    {
        fmt::print(fg(fmt::color::crimson), "No such path in filesystem.\n");
        return 1;
    }

    if (std::filesystem::is_empty(path))
    {
        fmt::print(fg(fmt::color::gold), "Directory is empty.\n");
        return 0;
    }

    auto dirItr = std::filesystem::directory_iterator(path);
    auto items = std::vector<Item>();

    for (const auto& i : dirItr)
    {
        auto type = getType(i);
        auto perm = getPerms(std::filesystem::status(i).permissions());
        auto size = getSize(i);
        auto time = getTime(i);
        auto name = getName(i);

        Item item{type, perm, size, time, name};
        items.push_back(item);
    }

    std::sort(items.begin(), items.end(),
              [](const Item& a, const Item& b) { return a.name < b.name; });

    // Remove this call if you don't want a header to be printed
    printHeaders();

    // Modify this loop to change the printout style
    for (const auto& i : items)
    {
        fmt::print("{0}{1}  {2}  {3}  ", i.type, i.permissions, i.size, i.time);

        if (i.type == 'l')
            fmt::print(fg(fmt::color::pink), "{}\n", i.name);

        else if (i.type == 'd')
            fmt::print(fg(fmt::color::aqua), "{}\n", i.name);

        else if (i.type == '.')
            fmt::print(fg(fmt::color::gold), "{}\n", i.name);
    }

    return 0;
}
