#pragma once
#include <iostream>
#include <math.h>
using namespace std;

class Couple {
	int nom;
	float note;
public:
	//ce constructeur cr�e un �l�ve sans nom ayant pour note -1
	Couple();
	//ce constructeur cr�e un �l�ve de nom a de note al�atoire entre 0 et 20
	Couple(int a);
	void setname(int n);
	void setmark(float n);
	int getname();
	float getmark();
};

class CompareNote {
public:
	CompareNote();
	bool operator()(Couple a, Couple b);
	int getvalue();
private:
	int* ptr;
};
class CompareNom {
public:
	CompareNom();
	bool operator()(Couple a, Couple b);
	int getvalue();
private:
	int* pointe;
};




