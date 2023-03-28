#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

class TitleDataBase
{
private:

    std::ifstream titles;
    int num_title_cols;

    std::ifstream ratings;
    int num_rating_cols;

    std::ifstream add_info;
    int num_info_cols;

    int duration_time;

public:

    TitleDataBase() : titles(), num_title_cols(0), ratings(), num_rating_cols(0), 
                      add_info(), num_info_cols(0), duration_time(0)
    {
    }

    TitleDataBase(const std::string& titles_file, int title_cols, const std::string& ratings_file, int rating_cols, 
                  const std::string& info_file, int info_cols, int _duration_time) 
                  : 
                  titles(titles_file), num_title_cols(title_cols), ratings(ratings_file), num_rating_cols(rating_cols),
                  add_info(info_file), num_info_cols(info_cols), duration_time(_duration_time)
    {
    }

    ~TitleDataBase()
    {
        this->close();
    }

    void getNotAdultMoviesWithProperRuntime(std::map<std::string, std::string>& id_primaryTitle);

    void getRatingMap(std::map<std::string, double>& id_rating);
    
    std::vector<std::pair<std::string, double>> getTopRating(int top, std::map<std::string, std::string>& id_title);

    std::vector<std::string> getRusTitlesById(const std::vector<std::string> id_vector);

    bool isValid();

    void close()
    {
        titles.close();
        ratings.close();
        add_info.close();
    }

    void skipHeaders();

    bool isOpen()
    {
        return (titles.is_open() && ratings.is_open() && add_info.is_open());
    }

};
