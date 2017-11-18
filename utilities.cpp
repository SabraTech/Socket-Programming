//
// Created by sabra on 19/11/17.
//

#include "utilities.h"

using namespace std;


vector <string> split(string s, string delimiter) {
    size_t pos = 0;
    vector <string> ans;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        ans.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    ans.push_back(s);
    return ans;
}

vector <string> parse_request(string request) {
    /* request example
    * GET home.txt HTTP/1.1
    * HOST: xyz.com
    */
    vector <string> result;
    string new_line = "\n";
    string space = " ";
    vector <string> request_lines = split(request, new_line);
    for (int i = 0; i < request_lines.size(); i++) {
        vector <string> temp = split(request_lines[i], space);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    // tokens: GET, home.txt, HTTP/1.1, HOST:, xyz.com
    return result;
}

string get_file_format(vector <string> request) {
    string file_name = request[1];
    vector <string> tokens = split(file_name, ".");
    string format = tokens[1];
    if (format.compare("txt") != 0 && format.compare("html") != 0) {
        return "img";
    } else {
        return format;
    }
}