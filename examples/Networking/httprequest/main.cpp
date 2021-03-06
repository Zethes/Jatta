#include <iostream>
#include <vector>
#include <CGUL.hpp>
using namespace std;
using namespace CGUL;

int main()
{
	try
    {
        Network::HTTPRequest http;

        //Http example
        /*cout << "-- Connecting..." << endl;
        http.Http("api.tumblr.com");

        if (!http.IsConnected())
        {
            cout << "-- Failed to connect..." << endl;
            return 1;
        }

        cout << "-- Connected!" << endl;

        cout << "-- Performing basic GET..." << endl;
        http.Get("/v2/blog/good.tumblr.com/info?api_key=fuiKNFp9vQFvjLNvx4sUwti4Yb5yGutBN4Xh10LXZhhRKjWlV4", 5000);
        cout << "Request: " << endl << "---------------\n" << http.GetRequest() << "---------------" << endl << endl;
        cout << "Response Head: " << endl << "---------------\n" << http.GetResponseHead() << "---------------" << endl << endl;
        cout << "Response Body: " << endl << "---------------\n" << http.GetResponseBody() << "\n---------------" << endl << endl;

        if (!http.IsConnected())
        {
            std::cout << "-- The connection was closed, reconnecting for next request..." << std::endl;
            http.Http("api.tumblr.com");
        }

        cout << "-- Performing basic POST..." << endl;
        if (!http.Post("/v2/user/follow?url=good.tumblr.com", "", 5000))
            cout << "Failed to perform POST." << endl;
        cout << "Request: " << endl << "---------------\n" << http.GetRequest() << "---------------" << endl << endl;
        cout << "Response Head: " << endl << "---------------\n" << http.GetResponseHead() << "---------------" << endl << endl;
        cout << "Response Body: " << endl << "---------------\n" << http.GetResponseBody() << "\n---------------" << endl << endl;

        http.Close();*/

        //Https example
        cout << "-- Connecting..." << endl;
        http.Https("api.github.com");
        cout << "-- Connecting..." << endl;
        if (!http.IsConnected())
        {
            cout << "-- Failed to connect..." << endl;
            return 1;
        }

        cout << "-- Connected!" << endl;

        cout << "-- Performing basic GET..." << endl;
        http.Get("/repos/Zethes/CGUL", 5000);
        cout << "Request: " << endl << "---------------\n" << http.GetRequest() << "---------------" << endl << endl;
        cout << "Response Head: " << endl << "---------------\n" << http.GetResponseHead() << "---------------" << endl << endl;
        cout << "Response Body: " << endl << "---------------\n" << http.GetResponseBody() << "\n---------------" << endl << endl;

        if (!http.IsConnected())
        {
            std::cout << "-- The connection was closed, reconnecting for next request..." << std::endl;
            http.Http("api.tumblr.com");
        }

        CGUL::String content = "{\r\n";
        content += "\t\"name\": \"Hello-World\",\r\n";
        content += "\t\"homepage\": \"https://github.com\",\r\n";
        content += "\t\"public\": true,\r\n";
        content += "\t\"has_wiki\": true,\r\n";
        content += "\t\"has_downloads\": true\r\n";
        content += "}\r\n";

        cout << "-- Performing basic POST..." << endl;
        if (!http.Post("/user/repos", content, 5000))
            cout << "Failed to perform POST." << endl;
        cout << "Request: " << endl << "---------------\n" << http.GetRequest() << "---------------" << endl << endl;
        cout << "Response Head: " << endl << "---------------\n" << http.GetResponseHead() << "---------------" << endl << endl;
        cout << "Response Body: " << endl << "---------------\n" << http.GetResponseBody() << "\n---------------" << endl << endl;

        http.Close();
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
    }
	
	return 0;
}
