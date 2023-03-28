#include "utils.hpp"

std::tuple<std::string, std::string, std::string, std::string> parseCommLineArgs(int argc, char* argv[])
{
    const char* short_options = "t:r:i:d:";

    const struct option long_options[] = 
    {
        {"title_file", required_argument, nullptr, 't'},
        {"rating_file", required_argument, nullptr, 'r'},
        {"info_file", required_argument, nullptr, 'i'},
        {"duration", required_argument, nullptr, 'd'},
        {nullptr, 0, nullptr, 0}
    };

    std::string title_file_name;
    std::string rating_file_name;
    std::string info_file_name;
    std::string duration;

    int option_index = -1;
    int found_option = -1;
    while ((found_option = getopt_long(argc, argv, short_options,
                                         long_options, &option_index)) != -1)
    {
        switch (found_option)
        {
        case 't':
        {
            if (optarg != nullptr)
            {
                title_file_name = optarg;
            }
            else
                std::cout << "Filename is not found!" << std::endl;
            break;
        };
        case 'r':
        {
            if (optarg != nullptr)
            {
                rating_file_name = optarg;
            }
            else
                std::cout << "Filename is not found!" << std::endl;
            break;
        }
        case 'i':
        {
            if (optarg != nullptr)
            {
                info_file_name = optarg;
            }
            else
                std::cout << "Filename is not found!" << std::endl;
            break;
        }
        case 'd':
        {
            if (optarg != nullptr)
            {
                duration = optarg;
            }
            else
                std::cout << "Duration time is not found!" << std::endl;
            break;
        }
        default:
        {
            std::cout << "Incorrect flag!" << std::endl;
            break;
        }
        }
    }

    return std::make_tuple(title_file_name, rating_file_name, info_file_name, duration);
}
