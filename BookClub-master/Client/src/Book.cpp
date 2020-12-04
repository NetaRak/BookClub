//
// Created by magami@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include <iostream>
#include <utility>
#include "Book.h"
using namespace std;

Book::Book(const Book & other)
        :name(other.name)
        ,currentlyOnInventory(other.currentlyOnInventory)
        ,genere(other.genere)
        ,previousOwner(other.previousOwner)
{
//    cout<<"Copy !!!!!"<<endl;
}
Book::Book(Book && other) noexcept
        :name(move(other.name))
        ,currentlyOnInventory(other.currentlyOnInventory)
        ,genere(move(other.genere))
        ,previousOwner(move(other.previousOwner))
{
//    cout<<"Move const"<<endl;
}
Book::Book(string _name,string _previousOwner,string gen):name(std::move(_name)),currentlyOnInventory(true),genere(std::move(gen)),previousOwner(std::move(_previousOwner)){}

void Book::wishToHave(){
    currentlyOnInventory=false;
}

const string &Book::getName() const {
    return name;
}


const string &Book::getpreviousOwner() const {
    return previousOwner;
}
void Book::setpreviousOwner(string owner)  {
    previousOwner=std::move(owner);
}

const string &Book::getGenere() const {
    return genere;
}

const bool &Book::getcurrentlyOnInventory() const {
    return currentlyOnInventory;
}
void Book::setcurrentlyOnInventory(bool set) {
//    cout<<"setting book "+name+"to - ";
//    cout<<set<<endl;
    currentlyOnInventory=set;
}

bool Book::empty() {
    bool answer = this->getName()=="";
    return answer;
}


