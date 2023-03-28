#include "TitleDataBase.hpp"
#include "utils.hpp"

const int TITLE_FILE_MISSED = 1;
const int RATING_FILE_MISSED = 2;
const int INFO_FILE_MISSED = 3;
const int RUNTIME_MISSED = 4;

const int INCORRECT_DATA = -1;
const int FILE_OPENING_ERROR = -2;

const int TOP_AMOUNT = 10;

const char NO_DATA[] = "\\N";

//./build/dz1 -t title.akas.tsv -r title.ratings.tsv -i title.basics.tsv -d 200

void setPrimaryTitleInEmpty(std::vector<std::string>& titles, const std::vector<std::string>& id, const std::map<std::string, std::string>& id_title)
{
    for (int i = 0; i < titles.size(); ++i)
        {
            //Там, где не нашлось русского названия, подтягиваем primaryTitle
            if (titles[i] == NO_DATA)
                titles[i] = id_title.find(id[i])->second;
        }
}

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
    
    if (!IMDb.isOpen())
    {
        std::cerr << "File opening error!" << std::endl;
        return FILE_OPENING_ERROR;
    }
    
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
        int top = (id_primaryTitle->size() >= TOP_AMOUNT) ? TOP_AMOUNT : id_primaryTitle->size();
        std::vector<std::pair<std::string, double>> top_rating = IMDb.getTopRating(top, *id_primaryTitle);
        if (!top_rating.empty())
        {
            //Запишем в отдельный вектор id найденных топ-фильмов
            std::vector<std::string> id_of_top = getFirstInPairs(top_rating);

            std::vector<std::string> top_titles = IMDb.getRusTitlesById(id_of_top);

            //Там, где не нашлось русского названия, подтягиваем primaryTitle
            setPrimaryTitleInEmpty(top_titles, id_of_top, *id_primaryTitle);

            for (int i = 0; i < top_titles.size(); ++i)
                std::cout << top_titles[i] << " - " << top_rating[i].second << std::endl;
        }
        else 
            std::cerr << "No Movies Found" << std::endl;
    }
    else
        std::cerr << "No Movies Found" << std::endl;

    delete id_primaryTitle;

    return 0;
}
