//
// Created by magami@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_CLIENT_H
#define BOOST_ECHO_CLIENT_CLIENT_H

#include "Book.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>

using namespace std;


class Client {
private:
    Book emptyBook;
    string name;
    string passcode;
    vector<Book>* wishList;
    unordered_map<string,vector<Book>>* booksByGenere;
    vector<string>* subs;
    int receiptNum=0;
    unordered_map<int,string>* messsageByReceipt;
    unordered_map<string ,string>* topicsAndSubsId;

public:
    Client(string _name, string _passcode);
//    Client(Client&);
//    Client& operator=(const Client&);
    void clearMe();
    //~Client();
    string getByFirstKey(int first);
    void addMessage(int,string);
    void addBook(string,Book);
    void removeBook(const string&,Book&);
    int getReceiptNum();
    string getReceipt(int num);
    const string &getName() const;
    string fixName(string);
    void addToSubs(const string& topic);
    void addTotopicsAndSubsId(const string&,string);
    void removeFromTopicsAndSubsId(const string&);
    string getSubIdByTopic(string);
    void removeFromSubs(string topic);
    void returnToMe(string);
    void SetPrevOwner(string,string);
    Book& containesBook(string bookName);
    string getInventory(const string&);
    bool containedBeforeBook(const string& bookName);
    Book &getFromBooksByGenere(const string& gen, string bookName);
    void printInventory();
    bool wishListContain(const string& p_name);
    //void clearClient();
    void removeFromWishList(string p_name);
    void addToWishList(Book& p_book);
};


#endif //BOOST_ECHO_CLIENT_CLIENT_H
