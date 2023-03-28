#include "TitleDataBase.hpp"
#include "utils.hpp"

const int TITLE_FILE_MISSED = 1;
const int RATING_FILE_MISSED = 2;
const int INFO_FILE_MISSED = 3;
const int RUNTIME_MISSED = 4;

const int INCORRECT_DATA = -1;
const int FILE_OPENING_ERROR = -2;

//./build/dz1 -t title.akas.tsv -r title.ratings.tsv -i title.basics.tsv -d 200

int main(int argc, char* argv[])
{
    Args comm_line_args = parseCommLineArgs(argc, argv);

    if (comm_line_args.title_file_name.empty())
    {
        std::cerr << "Title file (-t) is missed!" << std::endl;
        return TITLE_FILE_MISSED;
    }

    if (comm_line_args.rating_file_name.empty())
    {
        std::cerr << "Rating file (-r) is missed!" << std::endl;
        return RATING_FILE_MISSED;
    }

    if (comm_line_args.info_file_name.empty())
    {
        std::cerr << "Info file (-i) is missed!" << std::endl;
        return INFO_FILE_MISSED;
    }

    if (comm_line_args.duration.empty())
    {
        std::cerr << "Duration time is missed!" << std::endl;
        return RUNTIME_MISSED;
    }

    TitleDataBase IMDb(comm_line_args.title_file_name, 8, comm_line_args.rating_file_name, 3, comm_line_args.info_file_name, 9, std::stoi(comm_line_args.duration));
    if (IMDb.isOpen())
    {
        if (!IMDb.isValid())
        {
            std::cerr << "Incorrect data!" << std::endl;
            return INCORRECT_DATA;
        }

        IMDb.skipHeaders();

        std::map<std::string, std::string>* id_primaryTitle = new std::map<std::string, std::string>();
        IMDb.getNotAdultMoviesWithProperRuntime(*id_primaryTitle);

        if (!id_primaryTitle->empty())
        {
            int top = (id_primaryTitle->size() >= 10) ? 10 : id_primaryTitle->size();

            std::vector<std::pair<std::string, double>> top10rating = IMDb.getTopRating(top, *id_primaryTitle);

            if (!top10rating.empty())
            {
                std::vector<std::string> id_of_top(top10rating.size());
                for (int i = 0; i < id_of_top.size(); ++i)
                    id_of_top[i] = top10rating[i].first;
    
                std::vector<std::string> top_titles = IMDb.getRusTitlesById(id_of_top);
                for (int i = 0; i < top_titles.size(); ++i)
                {
                    //Там, где не нашлось русского названия, подтягиваем primaryTitle
                    if (top_titles[i] == "\\N")
                        top_titles[i] = (*id_primaryTitle)[id_of_top[i]];
                }
    
                for (int i = 0; i < top_titles.size(); ++i)
                    std::cout << top_titles[i] << " - " << top10rating[i].second << std::endl;
            }
            else 
                std::cerr << "No Movies Found" << std::endl;
        }
        else
            std::cerr << "No Movies Found" << std::endl;

        delete id_primaryTitle;
    }
    else
    {
        std::cerr << "File opening error!" << std::endl;
        return FILE_OPENING_ERROR;
    }

    return 0;
}
