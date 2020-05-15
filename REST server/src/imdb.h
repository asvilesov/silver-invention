#include "cpprest/asyncrt_utils.h"
#include "cpprest/http_listener.h"
#include "cpprest/json.h"
#include "cpprest/uri.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class ImdbHandler
{
    public:
        ImdbHandler(utility::string_t url, std::string fileName);

        pplx::task<void>open(){return m_listener.open();}
        pplx::task<void>close(){return m_listener.close();}

        std::map<std::string, std::vector<std::string>> imdbIdData;
        std::map<std::string, std::vector<std::vector<std::string>>> imdbMovieData;
        std::string baseUrl;

    protected: 

    private:
        void handle_get(http_request message);
        http_listener m_listener;
};