//
// Created by naama on 14/01/2020.
//
#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <Client.h>
#include <readFromServ.h>
#include <thread>
#include <KeyboardReader.h>
#include "StompBookClubClient.h"
#include "connectionHandler.h"
#include <unordered_map>
#include <map>

using namespace std;
using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    bool shineOnYouCrazyDiamond = true;
//    cout << "entered main" << endl;
    while (shineOnYouCrazyDiamond) {
        const short bufsize = 1024;
        char buf[bufsize];
        try {
            cin.getline(buf, bufsize);
        } catch (exception) {
        }
        string line(buf);
        int len = line.length();
        string command[len];
        int i = 0;
        string msgToSend;
        for (char c:line) {
            if (c != ' ') {
                command[i] = command[i] + c;
            } else {
                i = i + 1;
            }
        }
        if (command[0] == "login") {
            string hostAndPort = command[1];
            string userName = command[2];
            string passcode = command[3];

            string serverHost = "";
            string serverPort = "";
            int j = 0;
            for (char c:hostAndPort) {
                if (c != ':') {
                    serverHost = serverHost + c;
                    j = j + 1;
                } else {
                    serverPort = hostAndPort.substr(j + 1, hostAndPort.size());
                    break;
                }
            }
            short port = stoi(serverPort);

            msgToSend =
                    string("CONNECT") + '\n' + string("accept-version:1.2") + '\n' + string("host:") + serverHost + '\n'
                    + string("login:") + userName + '\n'
                    + string("passcode:") + passcode + "\n\n";
//cout<<msgToSend<<endl;
            ConnectionHandler *handler = new ConnectionHandler(serverHost, port);
            handler->connect();
            handler->sendLine(msgToSend);
            string command = "";

            if (!(handler->getLine(command))) {
                cout << "Could not connect to server" << endl;
                shineOnYouCrazyDiamond = false;
                handler->close();
            } else {

                string words[command.size()];
                int i = 0;
                for (char c:command) {
                    if (c != '\n') {
                        words[i] = words[i] + c;
                    } else {
                        i = i + 1;
                    }
                }
                string firstWord = words[0];

                if (firstWord == "ERROR") {

                    if (words[1].at(0) == 'r') {
                        string output = words[2].substr(8, words[2].length());
                        cout << output << endl;
                    } else {
                        string output = words[1].substr(8, words[1].length());
                        cout << output << endl;
                    }
                    shineOnYouCrazyDiamond = false;
                    handler->close();
                } else if (firstWord == "CONNECTED") {
                    cout<<"Login successful"<<endl;
                    Client client = Client(userName, passcode);
                    auto *socketReader = new readFromServ(*handler, client);
                    auto *keyboardReader = new KeyboardReader(*handler, client);

                    thread socketThread(&readFromServ::run, socketReader);
                    thread keyboardThread(&KeyboardReader::run, keyboardReader);

                    keyboardThread.join();
//                    cout << "keyboard finish" << endl;
                    socketThread.join();
//                    cout << "socked finish" << endl;

                    //client->clearClient();
//                    cout << "clearClient finish" << endl;
                    handler->close();
//                    cout << "handler finish" << endl;
                    delete (socketReader);
//                    cout << " delete(socketReader)" << endl;
                    delete (keyboardReader);
//                    cout << "(keyboardReader)" << endl;
                    //delete(client);
                    delete (handler);
//                    cout << "delete(client)" << endl;
                    client.clearMe();
                    shineOnYouCrazyDiamond = false;
//                    cout << shineOnYouCrazyDiamond << endl;

                }
            }

        }
    }

}
