#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <utf8.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "\nUsage: docsample filename\n";
        return 0;
    }

    const char* test_file_path = argv[1];
    // Open the test file (contains UTF-8 encoded text)
    ifstream fs8(test_file_path);
    if (!fs8.is_open()) {
    cout << "Could not open " << test_file_path << endl;
    return 0;
    }

    unsigned line_count = 1;
    string line;
    // Play with all the lines in the file
    while (getline(fs8, line)) {
       // check for invalid utf-8 (for a simple yes/no check, there is also utf8::is_valid function)
        string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
        if (end_it != line.end()) {
            cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
            cout << "This part is fine: " << string(line.begin(), end_it) << "\n";
        }

        // Get the line length (at least for the valid part)
        int length = utf8::distance(line.begin(), end_it);
        cout << "Length of line " << line_count << " is " << length <<  "\n";

        // Convert it to utf-16
        vector<unsigned short> utf16line;
        utf8::utf8to16(line.begin(), end_it, back_inserter(utf16line));

        // And back to utf-8
        string utf8line;
        utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

        // Confirm that the conversion went OK:
        if (utf8line != string(line.begin(), end_it))
            cout << "Error in UTF-16 conversion at line: " << line_count << "\n";

        line_count++;
    }
    return 0;
}
