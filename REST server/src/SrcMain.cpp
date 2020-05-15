#include "SrcMain.h"



std::unique_ptr<ImdbHandler> g_httpimdb;


void ProcessCommandArgs(int argc, const char* argv[])
{ 

	utility::string_t port = U("12345");
    utility::string_t address = U("http://localhost:");
    address.append(port);

    std::string fileName;
    if(argc > 1){
        fileName = argv[1];
    }
    else{
        fileName = "../../data.tsv";
    }
    on_initialize(address, fileName);

    std::cout << "IMDB Server is up and running..." << std::endl;

    //use while instead for travis, however then on_shutdown will never be reached and the port will not be released?
    while(true){}

    on_shutdown();
}


void on_initialize(const string_t& address, std::string fileName)
{
    uri_builder uri(address);
    uri.append_path(U("/movie"));

    auto addr = uri.to_uri().to_string();
    ucout << "starting<<<>>>\n\n";
    std::cout << "starting<<<>>>\n\n";
    g_httpimdb = std::make_unique<ImdbHandler>(addr, fileName);
    ucout << "starting<<<>>>\n\n";
    std::cout << "starting<<<>>>\n\n";
    g_httpimdb->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
}

void on_shutdown()
{
    g_httpimdb->close().wait();
}
