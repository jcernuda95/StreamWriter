#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>
#include <sys/time.h>
#include <condition_variable>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <string>

typedef enum {
    INTEGER,
    FLOAT,
    DOUBLE,
    NUMERIC,
    STRING
} DataType;

typedef struct {
    DataType type;
    size_t size;    // # of bytes of data
    void *data;
} Message;

std::vector<Message> readMessages(char const* path, int offset, int num_msg);
std::ifstream& GotoLine(std::ifstream& file, unsigned int num);
void writeMessages(std::vector<Message> msgs, char const* path);
std::string mes2str(Message m);
void printMessages(std::vector<Message> msgs);

int main() {
    std::vector<Message> msgs;
    char const* path = "/home/jaime/CLionProjects/FileWriter/partition";
    char const* path2 = "/home/jaime/CLionProjects/FileWriter/partition2";

    msgs = readMessages(path, 1, 10);

    printMessages(msgs);

    writeMessages(msgs, path2);
}

std::ifstream& GotoLine(std::ifstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}


std::vector<Message> readMessages(char const* path, int offset, int num_msg){
    std::ifstream myfile (path);
    std::vector<Message> msgs;
    if (myfile.is_open()) {
        GotoLine(myfile, offset);
        for (int i = 0; i < num_msg; i++) {
            Message msg;
            std::string line;
            std::string type;

            myfile >> type;

            if (type == "INTEGER") {
                msg.type = INTEGER;
                myfile >> msg.size;
                int ft;
                myfile >> ft;
                msg.data = new int(ft);
            } else if (type == "FLOAT") {
                msg.type = FLOAT;
                myfile >> msg.size;
                float ft;
                myfile >> ft;
                msg.data = new float(ft);
            } else if (type == "DOUBLE") {
                msg.type = DOUBLE;
                myfile >> msg.size;
                double ft;
                myfile >> ft;
                msg.data = new double(ft);
            } else if (type == "STRING") {
                msg.type = STRING;
                myfile >> msg.size;
                std::string st;
                myfile >> st;
                auto char_array = new char[st.size() + 1];
                strcpy(char_array, st.c_str());
                msg.data = char_array;

            } else {
                std::cout << "Error, no type definition on file line";
            }
            msgs.push_back(msg);
        }
    }
    myfile.close();
    return msgs;
}

void writeMessages(std::vector<Message> msgs, char const* path){
    std::ofstream myfile (path);
    if (myfile.is_open()) {
        for (int i = 0; i < msgs.size(); i++) {
            if (msgs[i].type == INTEGER) {
                myfile << "INTEGER";
                myfile << " ";
                myfile << msgs[i].size;
                myfile << " ";
                myfile << mes2str(msgs[i]);
                myfile << std::endl;
            } else if (msgs[i].type == FLOAT) {
                myfile << "FLOAT";
                myfile << " ";
                myfile << msgs[i].size;
                myfile << " ";
                myfile << mes2str(msgs[i]);
                myfile << std::endl;
            } else if (msgs[i].type == DOUBLE) {
                myfile << "DOUBLE";
                myfile << " ";
                myfile << msgs[i].size;
                myfile << " ";
                myfile << mes2str(msgs[i]);
                myfile << std::endl;
            } else if (msgs[i].type == STRING) {
                myfile << "STRING";
                myfile << " ";
                myfile << msgs[i].size;
                myfile << " ";
                myfile << mes2str(msgs[i]);
                myfile << std::endl;
            }
        }
    }
    myfile.close();
}

void printMessages(std::vector<Message> msgs){
    for (int i = 0; i < msgs.size(); ++i) {
        std::cout << mes2str(msgs[i]) << " ";
    }
    std::cout << " " << std::endl;
}

std::string mes2str(Message m) {
    std::ostringstream os;

    switch (m.type) {
        case INTEGER:
            os << *(int*) m.data;
            break;
        case FLOAT:
            os << *(float*) m.data;
            break;
        case DOUBLE:
            os << *(double*) m.data;
            break;
        case STRING:
            os << (char*) m.data;
            break;
    }

    return os.str();
}