#include <iostream>
#include <string>
#include <istream>
#include <ostream>
#include <string>
#include <cstdlib>
#include <boost/asio/ts/internet.hpp>

using namespace std;

int main()
{
    using boost::asio::ip::tcp;

    try
    {
        string host = "jsonplaceholder.typicode.com"; // argv[1]
        string path = "/todos/1";                     // argv[2]

        boost::asio::ip::tcp::iostream s;

        // The entire sequence of I/O operations must complete within 60 seconds.
        // If an expiry occurs, the socket is automatically closed and the stream
        // becomes bad.
        s.expires_after(chrono::seconds(60));

        // Establish a connection to the server.
        s.connect(host, "http");
        if (!s)
        {
            cout << "Unable to connect: " << s.error().message() << "\n";
            return EXIT_FAILURE;
        }

        // Send the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        s << "GET " << path << " HTTP/1.0\r\n";
        s << "Host: " << host << "\r\n";
        s << "Accept: */*\r\n";
        s << "Connection: close\r\n\r\n";

        // By default, the stream is tied with itself. This means that the stream
        // automatically flush the buffered output before attempting a read. It is
        // not necessary not explicitly flush the stream at this point.

        // Check that response is OK.
        string http_version;
        s >> http_version;
        unsigned int status_code;
        s >> status_code;
        string status_message;
        getline(s, status_message);
        if (!s || http_version.substr(0, 5) != "HTTP/")
        {
            cout << "Invalid response\n";
            return EXIT_FAILURE;
        }
        if (status_code != 200)
        {
            cout << "Response returned with status code " << status_code << "\n";
            return EXIT_FAILURE;
        }
        
        // Process the response headers, which are terminated by a blank line.
        string header;
        while (getline(s, header) && header != "\r")
            cout << header << "\n";
        cout << "\n";

        // Write the remaining data to output.
        cout << s.rdbuf() << "\n";
    }
    catch (exception &e)
    {
        cout << "Exception: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    cin.get();
    return EXIT_SUCCESS;
}
