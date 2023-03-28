#include "TitleDataBase.hpp"
#include "utils.hpp"

//./build/dz1 -t title.akas.tsv -r title.ratings.tsv -i title.basics.tsv -d 200

int main(int argc, char* argv[])
{
    std::string title_file_name, rating_file_name, info_file_name, duration;

    std::tie(title_file_name, rating_file_name, info_file_name, duration) = parseCommLineArgs(argc, argv);

    if (title_file_name.empty())
    {
        std::cout << "Title file (-t) is missed!" << std::endl;
        return 1;
    }

    if (rating_file_name.empty())
    {
        std::cout << "Rating file (-r) is missed!" << std::endl;
        return 2;
    }

    if (info_file_name.empty())
    {
        std::cout << "Info file (-i) is missed!" << std::endl;
        return 3;
    }

    if (duration.empty())
    {
        std::cout << "Duration time is missed!" << std::endl;
        return 4;
    }

    TitleDataBase IMDb(title_file_name, 8, rating_file_name, 3, info_file_name, 9, std::stoi(duration));
    if (IMDb.isOpen())
    {
        if (!IMDb.isValid())
        {
            std::cout << "Incorrect data!" << std::endl;
            return -1;
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
                std::cout << "No Movies Found" << std::endl;
        }
        else
            std::cout << "No Movies Found" << std::endl;

        delete id_primaryTitle;
    }
    else
    {
        std::cout << "File opening error!" << std::endl;
        return -2;
    }

    return 0;
}
