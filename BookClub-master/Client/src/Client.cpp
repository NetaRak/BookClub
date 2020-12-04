//
// Created by magami@wincs.cs.bgu.ac.il on 12/01/2020.
//
#include <iostream>
#include <connectionHandler.h>
#include "Client.h"
#include <utility>
#include <vector>
#include <unordered_map>
#include <mutex>
using namespace std;
#pragma ()
Client::Client(string _name, string _passcode) :
        emptyBook("", "", ""),
        name(std::move(_name)),
        passcode(std::move(_passcode)),
        wishList(new vector<Book>),
        booksByGenere(new unordered_map<string, vector<Book>>()),
        subs(new vector<string>),
        messsageByReceipt(new unordered_map<int, string>),
        topicsAndSubsId(new unordered_map<string, string>)
{
}

//Client::Client(Client& other)=default;
//                        name(other.name),
//                        passcode(other.passcode),
//                        wishList(other.wishList),
//                        booksByGenere(other.booksByGenere),
//                        subs(other.subs),
//                        messsageByReceipt(other.messsageByReceipt),
//                        topicsAndSubsId(other.topicsAndSubsId){
//    cout<<"COPY CONST CLIENT--->> if so, we need to deep-copy messsageByReceipt and booksByGenere"<<endl;
//}
//
//Client& Client::operator=(const Client&)=default;
//    cout<<"COPY =OPERATOR CLIENT--->> if so, we need to deep-copy messsageByReceipt and booksByGenere"<<endl;
//}

void Client::clearMe() {
    if (this->wishList)
        delete (this->wishList);
    this->wishList = nullptr;
    if (this->booksByGenere)
        delete (this->booksByGenere);
    this->booksByGenere = nullptr;
    if (this->subs)
        delete (this->subs);
    this->subs = nullptr;
    if (this->messsageByReceipt)
        delete (this->messsageByReceipt);
    this->messsageByReceipt = nullptr;
    if (this->topicsAndSubsId)
        delete (this->topicsAndSubsId);
    this->topicsAndSubsId = nullptr;
}

string Client::getByFirstKey(int first) {
    for (pair<int, string> mapPair: *messsageByReceipt) {
        if (mapPair.first == first)
            return mapPair.second;
    }
    return "";
}

void Client::addMessage(int receipt, string message) {
    messsageByReceipt->insert(pair<int, string>(receipt, message));
}

void Client::addBook(string topic, Book book) {
//    cout<<"add book"<<endl;
    if (booksByGenere->count(topic) == 0)
        booksByGenere->insert({topic, vector<Book>()});

    if (!containedBeforeBook(book.getName())) {
        booksByGenere->at(topic).push_back(book);
    }
    else
        book.setcurrentlyOnInventory(true);
}

void Client::removeBook(const string &genere, Book &book) {
//    cout << "enterd remove book " + book.getName() << endl;
    vector<Book>::iterator iter;
    if (booksByGenere->count(genere) > 0) {
        for (iter = booksByGenere->at(genere).begin(); iter < booksByGenere->at(genere).end(); ++iter) {
            if ((*iter).getName() == book.getName()) {
//                cout << "i setted the book " + book.getName() + "to false" << endl;
                (*iter).setcurrentlyOnInventory(false);
            }
        }
    }
}

int Client::getReceiptNum() {
    receiptNum += 1;
    return receiptNum;
}

string Client::getReceipt(int num) {
    string output = getByFirstKey(num);
    return output;
}

const string &Client::getName() const {
    return name;
}

string Client::fixName(string oldName) {
    std::string newName = oldName;
    bool first = false;
    bool last = false;
    int i = 0;
    char curr;
    int len =  newName.length();
    while ((!first) & (i <len)) {
        curr = newName.at(i);
        if (curr != ' ') {

            newName = newName.substr(i, newName.length());
            first = true;
        }
        i = i + 1;
    }
    i = newName.length() - 1;
    while ((i >= 0) & (!last)) {
        curr = newName.at(i);
        if (curr != ' ') {
            newName = newName.substr(0, i + 1);
            last = true;
        }
        i = i - 1;
    }
    return newName;
}

void Client::addToSubs(const string &topic) {
    subs->push_back(topic);
    vector<Book> vec;
    booksByGenere->insert(make_pair(topic, vec));
}

void Client::addTotopicsAndSubsId(const string &topic, string subId) {
    (*topicsAndSubsId)[topic] = std::move(subId);
}

void Client::removeFromTopicsAndSubsId(const string &topic) {
    topicsAndSubsId->erase(topic);
}

string Client::getSubIdByTopic(string topic) {
    string result;
    for (pair<string, string> mapPair: *topicsAndSubsId) {
        if (mapPair.first == topic)
            return mapPair.second;
    }
    return "";
}

void Client::removeFromSubs(string topic) {
    vector<string>::iterator iter;
    for (iter = subs->begin(); iter < subs->end(); ++iter) {
        if ((*iter) == topic) {
            break;//not working
        }
    }
    subs->erase(iter);
}

void Client::returnToMe( string bookName) {
//    cout<<"returning to me the book:"+bookName<<endl;
    for (auto it = booksByGenere->begin(); it != booksByGenere->end(); ++it) {
        for (Book &book : it->second) {
            if (book.getName().compare(bookName) == 0) {
                book.setcurrentlyOnInventory(true);
            }
        }
    }
}
//
//void Client::SetPrevOwner( string bookName,string owner) {
//    cout<<"returning to me the book:"+bookName<<endl;
//    for (auto it = booksByGenere->begin(); it != booksByGenere->end(); ++it) {
//        for (Book &book : it->second) {
//            if (book.getName().compare(bookName) == 0) {
//                book.setpreviousOwner(owner);
//            }
//        }
//    }
//}

Book &Client::containesBook(string bookName) {
//    cout << "----in containesBook---" << endl;
//    cout << bookName << endl;
//    cout << "----in containesBook---" << endl;
    for (auto it = booksByGenere->begin(); it != booksByGenere->end(); ++it) {
        for (Book &book : it->second) {
            if (book.getName().compare(bookName) == 0) {
                if (book.getcurrentlyOnInventory())
                    return book;
            }
        }
    }
    return emptyBook;
}

string Client::getInventory(const string &topic) {
    string temp;
    if (booksByGenere->find(topic) == booksByGenere->end())
        return "";
    for (Book &book : (booksByGenere->at(topic))) {
        if (book.getcurrentlyOnInventory()) {
            temp += book.getName() + ",";
        }
    }
    string answer = getName() + ":" + temp.substr(0, temp.size() - 1);
//    cout << "user inventory:" + answer << endl;
    return answer;
}

bool Client::containedBeforeBook(const string &bookName) {
    for (auto &it : *booksByGenere) {
        for (Book book : it.second) {
            if ((book.getName() == bookName) & (!book.getcurrentlyOnInventory())) {
                return true;
            }
        }
    }
    return false;

}

//login 132.72.45.28:7777 neta 123


Book &Client::getFromBooksByGenere(const string &gen, string bookName) {

    if (booksByGenere->count(gen) == 0)
        return emptyBook;
    else {
        for (Book &b: booksByGenere->at(gen)) {
            if (b.getName() == bookName)
                return b;
        }
    }
    //added
    return emptyBook;
}

//void Client::printInventory() {
//    for (auto it = booksByGenere->begin(); it != booksByGenere->end(); ++it) {
//        int size = (it)->second.size();
//        for (int i = 0; (i < size); ++i) {
//            if ((*it).second.at(i).getcurrentlyOnInventory()) {
//                cout << "[" + (*it).second.at(i).getName() + "] - ";
//                cout << (*it).second.at(i).getcurrentlyOnInventory() << endl;
//            }
//        }
//    }
//}


bool Client::wishListContain(const string &p_name) {
    bool response = false;

    for (Book &b:*wishList)
        response |= (b.getName() == p_name);

    return response;
}

void Client::removeFromWishList(string p_name) {
    p_name = fixName(p_name);
    bool isF = false;
//    cout << "try to remove from wishList " + p_name << endl;

    for (auto it = wishList->begin(); !isF && it != wishList->end(); ++it) {
        if ((it)->getName() == p_name) {
            isF = true;
//            cout << "--------in the if man------" << endl;
            wishList->erase(it);
//            cout << "--------erase------" << endl;
        }
    }


}

void Client::addToWishList(Book &p_book) {
    wishList->push_back(p_book);
}
//
//Client::~Client() {
//    clearMe();
//}


//
//void Client::clearClient() {
//    for (auto it = booksByGenere->begin(); it != booksByGenere->end(); ++it) {
//        vector<Book *> *vec = it->second;
//        for (int i = 0; i < (it)->second->size(); ++i) {
//
//            delete ((*it).second->at(i));
//        }
//
//        delete ((*it).second);
//    }
//    for (int i = 0; i < wishList->size(); ++i) {
//        delete (wishList->at(i));
//    }
//}




