#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(std::getline(fileStream, line)){
        content+= line+ "\n";
    }
    content += "\0";
    fileStream.close();

	return content;
}

static std::string read_shader_file (const char *shader_file)
{
    // no feedback is provided for stream errors / exceptions.

    std::ifstream file (shader_file);
    if (!file) return std::string ();

    file.ignore(std::numeric_limits<std::streamsize>::max());
    auto size = file.gcount();

    if (size > 0x10000) // 64KiB sanity check for shaders:
        return std::string ();

    file.clear();
    file.seekg(0, std::ios_base::beg);

    std::stringstream sstr;
    sstr << file.rdbuf();
    file.close();

    return sstr.str();
}

const char* read_file2(const char *path)
{
	std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
if (file.is_open())
{
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    char *contents = new char [size];
    file.seekg (0, std::ios::beg);
    file.read (contents, size);
    file.close();
    contents[size] = '\0';
    return contents;
}
}
