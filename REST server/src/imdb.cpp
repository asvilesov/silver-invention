#include "imdb.h"

//Constructor loads in data for server and starts it
ImdbHandler::ImdbHandler(utility::string_t url, std::string fileName)
    :m_listener(url)
    ,baseUrl(url)
{
    m_listener.support(methods::GET, std::bind(&ImdbHandler::handle_get, this, std::placeholders::_1));

    std::ifstream file;
    file.open(fileName);
    std::string word;
    std::string id;
    std::string title;
    //get rid of first line
    std::getline(file, word);
    std::vector<std::string> info;

    //diagnostic
    int count = 0;
    double perc;
    while(!file.eof()){
        ++count;
        std::getline(file, word, '\t');
        id = word;
        info.emplace_back(word);
        std::getline(file, word, '\t'); //skip title type
        if(word == "movie"){
            std::getline(file, word, '\t');
            title = word;
            info.emplace_back(word);
            std::getline(file, word, '\t');//skip original title
            std::getline(file, word, '\t');//skip is adult
            std::getline(file, word, '\t');
            info.emplace_back(word);
            std::getline(file, word, '\t');//skip end year
            std::getline(file, word, '\t');
            info.emplace_back(word);
            std::getline(file, word); //go to the rest of the line skipping genres

            //std::cout << id << " " << title << "\n";

            imdbIdData.insert(std::pair<std::string, std::vector<std::string>>(id, info));
            if(imdbMovieData.find(title) != imdbMovieData.end()){
                imdbMovieData.at(title).emplace_back(info);
            }
            else{
                std::vector<std::vector<std::string>> vecInfo;
                vecInfo.emplace_back(info);
                imdbMovieData.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(title, vecInfo));
            }
        }
        else{
            std::getline(file, word);
        }    
        info.clear();
        if(count % 1000000 == 0){
            perc = static_cast<double>(count)/6735233.00;
            std::cout <<"Percentage of data set parsed: " << perc << "\n";
            std::cout << "curr title: " << title << "\n";
        }
    }
    std::cout <<"The count is: " << count << "\n";

}

//Handling errors and returning various status codes was specifically taken from the REST web/black jack in lines such as 131-141, for example "NotFound"
void ImdbHandler::handle_get(http_request message)
{
    //ucout <<  message.to_string() << std::endl;
    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    // std::cout << "what" << "\n";
    // for(auto& i : paths){
    //     std::cout << i << "\n";
    // }
    // std::cout << "what" << "\n\n";

    int val = 0;
    ucout << "<< "<< val << " >>\n";
    if(paths[0] == "id"){
        if( imdbIdData.find(paths[1]) != imdbIdData.end()){
            web::json::value returnJson;
            std::vector<std::string> information = imdbIdData.at(paths[1]);
            ucout << "<< "<< information [2] << " >>\n";
            returnJson["ID"] = json::value::string(information[0]);
            if(information[3] != "\\N"){
                returnJson["Runtime"] = std::stoi(information[3]);
            }
            else{
                returnJson["Runtime"] = 0;
            }
            returnJson["Title"] = json::value::string(information[1]);
            returnJson["URL"] = json::value::string("https://www.imdb.com/title/" + information[0] + "/");
            ucout << "<< "<< information [2] << " >>\n";
            if(information[2] != "\\N"){
                returnJson["Year"] = std::stoi(information[2]);
            }
            else{
                returnJson["Year"] = 0;
            }

            message.relative_uri().path();
            message.reply(status_codes::OK, returnJson);
        }
        else{
            web::json::value returnError;
            returnError["Error"] = json::value("No movie with that ID found");
            message.reply(status_codes::NotFound, returnError);
        }
    }
    else if(paths[0] == "name"){
        if( imdbMovieData.find(paths[1]) != imdbMovieData.end()){
            web::json::value returnArr;
            web::json::value obj;
            std::vector<std::vector<std::string>> information = imdbMovieData.at(paths[1]);
            int count = 0;
            for(auto& i : information){
                obj["ID"] = json::value::string(i[0]);
                if(i[3] != "\\N"){
                    obj["Runtime"] = std::stoi(i[3]);
                }
                else{
                    obj["Runtime"] = 0;
                }
                obj["Title"] = json::value::string(i[1]);
                obj["URL"] = json::value::string("https://www.imdb.com/title/" + i[0] + "/");
                if(i[2] != "\\N"){
                    obj["Year"] = std::stoi(i[2]);
                }
                else{
                    obj["Year"] = 0;
                }
                returnArr[count] = obj;
                ++count;
            }
            message.reply(status_codes::OK, returnArr);
        }
        else{
            web::json::value returnError;
            returnError["Error"] = json::value("No movie by that name found");
            message.reply(status_codes::NotFound, returnError);
        }
    }
    else{
        web::json::value returnError;
        returnError["Error"] = json::value("No movie with that ID found");
        message.reply(status_codes::NotFound, returnError);
    }
}; 

