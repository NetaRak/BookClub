//
// Created by magami@wincs.cs.bgu.ac.il on 13/01/2020.
//
//#include <boost/algorithm/string.hpp>dc++.h>
#include <boost/algorithm/string.hpp>
#include <Client.h>
#include "readFromServ.h"
#include "connectionHandler.h"

#include <array>

using namespace std;

readFromServ::readFromServ(ConnectionHandler &_handler1, Client &_client) : handler(_handler1), client(_client) {
//    connected=true;
}

//readFromServ::~readFromServ() {
//    close();
//}

void readFromServ::run() {
    Book myBook("","","");
    bool connected = true;
    while (connected) {
        string command = "";
        handler.getLine(command);

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
//        cout << command << endl;
        if (firstWord == "MESSAGE") {
            string body = words[5];
            string topic = words[3].substr(12, words[3].size());
            string toPrint = topic + ":" + body;
            cout << body << endl;
            string bodyArray[body.size()];
            int bodyS=bodyArray->size();
            for (int i = 0; i < bodyS; ++i) {
                bodyArray[i] = "";
            }
            int j = 0;
            for (char c:body) {
                if (c == '\n') {
                    break;
                }
                if (c != ' ') {
                    bodyArray[j] = bodyArray[j] + c;
                } else {
                    j = j + 1;
                }
            }
//            int b1=bodyArray->size();
//            for (int i = 0; i < b1; ++i) {
//                cout << "[" + bodyArray[i] + "]" << endl;
//            }

            if (bodyArray[1] == "wish") {
                string bookName = "";
                int b2 =  body.size();
                for (int k = 4; k <b2; ++k) {
                    if (bodyArray[k] != "") {
                        bookName = bookName + " " + bodyArray[k];
                    }
                }
//                cout << "----------" << endl;
//                cout << bookName << endl;
                bookName = client.fixName(bookName);
//                cout << bookName << endl;
//                cout << "----------" << endl;
                myBook = client.containesBook(bookName);

                if (!myBook.empty()) {
//                    cout << "not empty" << endl;
                    string msgToSend = "SEND\ndestination:" + myBook.getGenere() + "\n\n" +
                                       client.getName() + " has the book " + myBook.getName() +
                                       "\n";
                    handler.sendFrameAscii(msgToSend, '\0');
                }
            } else if (bodyArray[1] == "has" && bodyArray[2] != "added") {
                string bookName = "";
                int b3=body.size();
                for (int k = 4; k < b3; ++k) {
                    if (bodyArray[k] != "") {
                        bookName = bookName + bodyArray[k] + " ";
                    }
                }
                bookName = client.fixName(bookName);
                //bookName=bookName.substr(0,bookName.size()-1);
                // cout<<"book name- "+bookName+""<<endl;
                myBook= client.containesBook(bookName);
                if (myBook.getName()=="") {
                    bool wishListContain = client.wishListContain(bookName);
                    if (wishListContain) {
                        string msgToSend = "SEND\ndestination:";
                        msgToSend += topic;
                        msgToSend += "\n\nTaking ";
                        msgToSend += bookName;
                        msgToSend += " from ";
                        msgToSend += bodyArray[0];
                        msgToSend += "\n";

                        bool wasMineBefore = client.containedBeforeBook(bookName);
                        client.removeFromWishList(bookName);
                        if (wasMineBefore) {
//                            cout << "im in the if" << endl;
                            client.returnToMe(bookName);
                        } else {
//                            cout << "im in the else" << endl;
                            Book myNewBook = Book(bookName, bodyArray[0], topic);
//                            cout << "im after book" << endl;
                            client.addBook(topic, myNewBook);
//                            cout << "im after add book" << endl;
                        }
                        handler.sendLine(msgToSend);

                    }
                }
            } else if (bodyArray[0] == "Taking") {
                int from = 2;
                int t1= bodyArray->size();
                for (i = 0; i < t1; ++i) {
                    if (bodyArray[i] == "from") {
                        from = i;
                    }
                }
                string fromWhomToTake = bodyArray[from + 1];
                fromWhomToTake = client.fixName(fromWhomToTake);
                if (fromWhomToTake == client.getName()) {
                    string bookName;
                    int k = 1;
                    while (bodyArray[k] != "from") {
                        bookName += bodyArray[k] + " ";
                        ++k;
                    }
                    bookName = client.fixName(bookName);
//                    cout<<"Taking the book "+bookName+"from "+fromWhomToTake<<endl;
                    //   client.SetPrevOwner(bookName,fromWhomToTake);
                    if (!client.containesBook(bookName).empty()) {
//                        cout << "wants to remove " + bookName + "from" + client.getName() << endl;
                        client.removeBook(topic, client.containesBook(bookName));
                    } else {
                        cout << "ERROR: CLIENT GAVE A BOOK BUT DOES NOT OWN IT!";
                    }
                }
            } else if (bodyArray[0] == "Returning") {
                int k = 1;
                string bookToReturn;
                while (bodyArray[k] != "to") {
                    bookToReturn += bodyArray[k++] + " ";
                }
                bookToReturn = client.fixName(bookToReturn);
                int userNamePosition;
                int b4 = bodyArray->size();
                for (int i = 2; i < b4; ++i) {
                    if (bodyArray[i] == "to") {
                        userNamePosition = i + 1;
                        break;
                    }
                }
//                cout << "book to return name:" + bookToReturn << endl;
//                cout << "user to return to:" + bodyArray[userNamePosition] << endl;
//                cout << "my name:" + client.getName() << endl;
                if (bodyArray[userNamePosition] == client.getName()) {
//                    cout << client.getName() + " entered here" << endl;
                    if (client.containedBeforeBook(bookToReturn)) {
                        client.returnToMe(bookToReturn);
                    } else {
                        cout << "ERROR: CLIENT GAVE A BOOK BUT DOES NOT HAVE IT ON CONTAINED BEFORE!";
                    }
                }
            } else if (bodyArray[0] == "book") {
                string msgToSend = "SEND\n"
                                   "destination:" + topic + "\n\n" +
                                   client.getInventory(topic) +
                                   "\n";
//                cout << client.getName() + ":" << endl;
//                client.printInventory();
                handler.sendLine(msgToSend);

            }
        } else if (firstWord == "RECEIPT") {
            string receiptId = words[1].substr(11, words[1].size());
//            cout << "got receipt id from server  " + receiptId << endl;
            string action = client.getReceipt(stoi(receiptId));
//            cout << "***after action  " + receiptId << endl;
            string recieptArray[action.size()];
            int j = 0;
            for (char c:action) {
                if (c == '\n') {
                    break;
                }
                if (c != ' ') {
                    recieptArray[j] = recieptArray[j] + c;
                } else {
                    j = j + 1;
                }
            }
            // of niv login 132.72.45.154:6666 naama 34
            if(recieptArray->size()>0) {
                string first = recieptArray[0];
//                cout << first << endl;
                if (first == "join") {
                    string topic = recieptArray[1];
                    client.addToSubs(topic);
                    cout << "Joined club " + recieptArray[1] << endl;
                } else if (first == "logout") {
                    connected = false;
//                    cout << "**** inside logout" + receiptId << endl;
//                handler.close();
                } else if ("exit") {
                    client.removeFromSubs(recieptArray[1]);
                    cout << "Exited club " + recieptArray[1] << endl;

                }
            }
        }

    }
//    cout << "finished read" << endl;
}