//
// Created by sabra on 19/11/17.
//

#include "utilities.h"

using namespace std;


vector <string> split(string line, string delimiter) {
    size_t pos = 0;
    vector <string> ans;
    string token;
    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        ans.push_back(token);
        line.erase(0, pos + delimiter.length());
    }
    ans.push_back(line);
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

vector <string> read_file(string file_name) {
    vector <string> vec;
    ifstream myFile(file_name);
    string line;
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            vec.push_back(line);
        }
    } else {
        cout << "Can not open the file" << endl;
    }
    myFile.close();
    return vec;
}

string make_request(string request) {
    string result;
    string file_format;
    vector <string> request_lines = split(request, " ");
    for (int i = 0; i < request_lines.size() - 1; i++) {
        if (i == 2) {
            result.append("HTTP/1.1");
            result.append("\n");
            result.append("Host:");
            result.append(" ");
        }
        result.append(request_lines[i]);
        result.append(" ");
    }
    result.append("\n");
    return result;
}