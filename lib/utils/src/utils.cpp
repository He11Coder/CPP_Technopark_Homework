#include "utils.hpp"

Args parseCommLineArgs(int argc, char* argv[])
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

    Args comm_line;

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
                comm_line.title_file_name = optarg;
            }
            else
                std::cout << "Filename is not found!" << std::endl;
            break;
        };
        case 'r':
        {
            if (optarg != nullptr)
            {
                comm_line.rating_file_name = optarg;
            }
            else
                std::cout << "Filename is not found!" << std::endl;
            break;
        }
        case 'i':
        {
            if (optarg != nullptr)
            {
                comm_line.info_file_name = optarg;
            }
            else
                std::cout << "Filename is not found!" << std::endl;
            break;
        }
        case 'd':
        {
            if (optarg != nullptr)
            {
                comm_line.duration = optarg;
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

    return comm_line;
}
