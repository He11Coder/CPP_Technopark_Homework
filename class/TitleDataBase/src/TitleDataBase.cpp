#include "TitleDataBase.hpp"

bool comparator(std::pair<std::string, double>& a, std::pair<std::string, double>& b)
{
    return a.second > b.second;
}

void TitleDataBase::getNotAdultMoviesWithProperRuntime(std::map<std::string, std::string>& id_primaryTitle)
{
    id_primaryTitle.clear();

    std::string line_buff;
    while (add_info)
    {
        std::getline(add_info, line_buff);

        int col_count = 0;
        std::string id;
        std::string title_type;
        std::string primary_title;
        bool is_adult;
        int run_time = -1;

        std::string::size_type begin = 0;
        std::string::size_type end = 0;
        std::string current_substr;

        //Парсим каждую отдельную строчку
        while (end != std::string::npos)
        {
            end = line_buff.find('\t', begin);
            current_substr = line_buff.substr(begin, end - begin);
            begin = end + 1;
            ++col_count;

            //Записываем данные в нужную переменную, в зависимости от того, из какого столбца они получены
            switch (col_count)
            {
            case 1:
                id = current_substr;
                break;
            case 2:
                title_type = current_substr;
                break;
            case 3:
                primary_title = current_substr;
                break;
            case 5:
                is_adult = (current_substr == "0") ? false : true;
                break;
            case 8:
                run_time = (current_substr != "\\N") ? std::stoi(current_substr) : -1;
                break;
            default:
                break;
            }
        }

        if ((title_type == "movie") && (!is_adult) && (run_time <= duration_time) && (run_time > 0))
            id_primaryTitle.insert(std::pair<std::string, std::string>(id, primary_title));
    }
}

void TitleDataBase::getRatingMap(std::map<std::string, double>& id_rating)
{
    std::string line_buff;
    while (ratings)
    {
        std::getline(ratings, line_buff);

        int col_count = 0;
        std::string id;
        double rating = -1.0;
        int numVotes = -1;

        std::string::size_type begin = 0;
        std::string::size_type end = 0;
        std::string current_substr;
        while (end != std::string::npos)
        {
            end = line_buff.find('\t', begin);
            current_substr = line_buff.substr(begin, end - begin);
            begin = end + 1;
            ++col_count;

            switch (col_count)
            {
            case 1:
                id = current_substr;
                break;
            case 2:
                rating = (current_substr != "\\N") ? std::stod(current_substr) : -1.0;
                break;
            case 3:
                numVotes = (current_substr != "\\N") ? std::stoi(current_substr) : -1;
                break;
            default:
                break;
            }
        }

        if ((numVotes >= 1000) && (rating != -1))
            id_rating.insert(std::pair<std::string, double>(id, rating));
    }
}

std::vector<std::pair<std::string, double>> TitleDataBase::getTopRating(int top, std::map<std::string, std::string>& id_title)
{
    //Создаем отображение id->rating
    std::map<std::string, double>* id_rating = new std::map<std::string, double>();
    getRatingMap(*id_rating);

    //В этих двух for сначала исключаем из мапы id->title записи, которых нет в id->rating, и потом наоборот,
    //чтобы остались только записи, которые есть и там, и там
    for (auto id_title_iter = id_title.begin(); id_title_iter != id_title.end();)
    {
        if (id_rating->find(id_title_iter->first) == id_rating->end())
            id_title_iter = id_title.erase(id_title_iter);
        else
            ++id_title_iter;   
    }

    for (auto id_rating_iter = id_rating->begin(); id_rating_iter != id_rating->end();)
    {
        if (id_title.find(id_rating_iter->first) == id_title.end())
            id_rating_iter = id_rating->erase(id_rating_iter);
        else
            ++id_rating_iter;
    }

    std::vector<std::pair<std::string, double>>* id_rating_vector = new std::vector<std::pair<std::string, double>>();

    for (auto& it : *id_rating) 
        id_rating_vector->push_back(it);

    std::sort(id_rating_vector->begin(), id_rating_vector->end(), comparator);

    //Из вектора рейтингов берем топ, переданный в функцию, или просто весь вектор рейтингов, если его размер меньше переданного аргумента
    std::vector<std::pair<std::string, double>> topNrating(id_rating_vector->begin(), id_rating_vector->begin() + ((top <= id_rating_vector->size()) ? top : id_rating_vector->size()));

    delete id_rating;
    delete id_rating_vector;

    return topNrating;
}

std::vector<std::string> TitleDataBase::getRusTitlesById(const std::vector<std::string> id_vector)
{
    add_info.seekg(0);

    std::vector<std::string> titles_vector(id_vector.size());
    for(int i = 0; i < titles_vector.size(); ++i)
    {
        titles.seekg(0);

        std::string line_buff;
        std::string id;
        //Скипаем строчки до нужного id
        while (id != id_vector[i])
        {
            std::getline(titles, line_buff);
            id = line_buff.substr(0, line_buff.find('\t', 0));
        }

        //Перебираем все строчки с найденным id и разными локализациями данного фильма
        while (id == id_vector[i])
        {
            int col_count = 0;
            std::string::size_type begin = 0;
            std::string::size_type end = 0;
            std::string current_substr;
            std::string local_title;
            std::string region;
            while (end != std::string::npos)
            {
                end = line_buff.find('\t', begin);
                current_substr = line_buff.substr(begin, end - begin);
                begin = end + 1;
                ++col_count;

                switch (col_count)
                {
                case 3:
                    local_title = current_substr;
                    break;
                case 4:
                    region = current_substr;
                    break;
                default:
                    break;
                }
            }

            if (region == "RU")
                titles_vector[i] = local_title;

            std::getline(titles, line_buff);
            id = line_buff.substr(0, line_buff.find('\t', 0));
        }

        //Если записи нет, значит не нашлось русского названия
        if (titles_vector[i].empty())
            titles_vector[i] = "\\N";
    }

    return titles_vector;
}

bool TitleDataBase::isValid()
{
    //first file validation
    std::string line_buff;

    titles.seekg(0);

    std::getline(titles, line_buff);
    
    int title_cols = 0;
    for (int i = 0; i < line_buff.size(); ++i)
    {
        if (line_buff[i] == '\t')
            ++title_cols;
    }
    ++title_cols;
    titles.seekg(0);

    //second file validation
    ratings.seekg(0);

    std::getline(ratings, line_buff);
    
    int rating_cols = 0;
    for (int i = 0; i < line_buff.size(); ++i)
    {
        if (line_buff[i] == '\t')
            ++rating_cols;
    }
    ++rating_cols;
    ratings.seekg(0);

    //third file validation
    add_info.seekg(0);

    std::getline(add_info, line_buff);
    
    int info_cols = 0;
    for (int i = 0; i < line_buff.size(); ++i)
    {
        if (line_buff[i] == '\t')
            ++info_cols;
    }
    ++info_cols;
    add_info.seekg(0);

    return ((title_cols == num_title_cols) && (rating_cols == num_rating_cols) && (info_cols == num_info_cols) && (duration_time > 0));
}

void TitleDataBase::skipHeaders()
{
    std::string line_buff;

    titles.seekg(0);
    std::getline(titles, line_buff);

    ratings.seekg(0);
    std::getline(ratings, line_buff);

    add_info.seekg(0);
    std::getline(add_info, line_buff);
}
