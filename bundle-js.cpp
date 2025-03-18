// TODO: overall, make the code more pretty, esp. regexes

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <windows.h>
#include <regex>
#include <sstream>
using namespace std;

string substringFileNameFromScript(string scriptTagLine);
string jsNameB(string fileName);

int main() {
  // Prepare structure - create necessary folders
  CreateDirectory("build", nullptr);
  CreateDirectory("build/js", nullptr);

  vector<array<string, 2>> htmlFileToMainJsFile = {
    {"index.html", "js/main.js"},
    {"another.html", "js/another.js"},
  };

  for (auto [htmlFileName, mainJsFileName] : htmlFileToMainJsFile) {
    // Copy html files along with their main JS file inside js/ folder
    CopyFile(htmlFileName.c_str(), ("build/" + htmlFileName).c_str(), false);
    CopyFile(mainJsFileName.c_str(), ("build/" + mainJsFileName).c_str(), false);
    
    fstream htmlFile("build/" + htmlFileName);
    ofstream jsFile("build/" + mainJsFileName, ios::app);

    // Get (TODO: process?) secondary JS files, omitting main file
    string line;
    int lineIdx;
    while (getline(htmlFile, line)) {
      if (line.contains("<script") && !line.contains(mainJsFileName)) {
        string secondaryFileName = substringFileNameFromScript(line);
        ifstream secondaryFile(secondaryFileName);
        
        jsFile << "\n" << "// From " << secondaryFileName << "\n" << secondaryFile.rdbuf();



        // TODO: поместить bundled в js/....js
        // TODO: rename в ..-bundled.js
      }
    }
  }
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

string getFileNameWoExt(string fileName) {
  size_t dotPos = fileName.find('.');
  string fileNameWoExt = fileName.substr(0, dotPos);

  return fileNameWoExt;
}
// jsNameBundled
string jsNameB(string fileName) {
  if (fileName == "index.html") return "js/main-bundled.js";
  return "js/" + getFileNameWoExt(fileName) + "-bundled" + ".js";
}