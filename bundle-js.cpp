// TODO: overall, make the code more pretty, esp. regexes

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <format>
#include <regex>
#include <sstream>
using namespace std;

string getFileNameWoExt(string fileName);
string substringFileNameFromScript(string scriptTagLine);
string jsName(string fileName);
string jsNameB(string fileName);

int main() {
  // Prepare structure - create necessary folders
  CreateDirectory("build", nullptr);
  CreateDirectory("build/js", nullptr);

  string fileNames[] = {"index.html", "another.html"};

  for (string& fileName : fileNames) {
    // Copy html files along with their main JS file inside js/ folder
    CopyFile(fileName.c_str(), ("build/" + fileName).c_str(), false);
    CopyFile((jsName(fileName)).c_str(), ("build/" + jsNameB(fileName)).c_str(), false);
    
    ifstream file("build/" + fileName);
    string fileNameWoExt = getFileNameWoExt(fileName);
    string searchStr;
    if (fileName == "index.html") {
      searchStr = format("<script src=\"js/{}\"></script>", "main.js");
    } else {
      searchStr = format("<script src=\"js/{}\"></script>", fileNameWoExt + ".js");
    }

    // Get (TODO: process?) secondary JS files, omitting main file
    string line;
    while (getline(file, line)) {
      if (line.find(searchStr) != string::npos) {
        continue;
      }

      if (line.find("<script") != string::npos) {
        string secondaryFileName = substringFileNameFromScript(line);
        ifstream secondaryFile(secondaryFileName, ios::app);
        ostringstream buffer;
        buffer << secondaryFile.rdbuf();

        cout << jsName(fileName) << endl;
       
        // TODO: поместить bundled в js/...-bundled.js
      }
    }
  }
}

string getFileNameWoExt(string fileName) {
  size_t dotPos = fileName.find('.');
  string fileNameWoExt = fileName.substr(0, dotPos);

  return fileNameWoExt;
}

string substringFileNameFromScript(string scriptTagLine) {
  smatch match;
  // TODO: learn way to be able to CAPTURE content WITHIN quotes; rn it's kinda ugly bc i need to trim quotes after
  regex r("\".*\"");
  regex_search(scriptTagLine, match, r);

  string secondaryFileName = match.str();
  // Remove quotes
  secondaryFileName.erase(0, 1);
  secondaryFileName.erase(secondaryFileName.size() - 1);

  return secondaryFileName;
}

string jsName(string fileName) {
  if (fileName == "index.html") return "js/main.js";
  return "js/" + getFileNameWoExt(fileName) + ".js";
}
// jsNameBundled
string jsNameB(string fileName) {
  if (fileName == "index.html") return "js/main-bundled.js";
  return "js/" + getFileNameWoExt(fileName) + "-bundled" + ".js";
}