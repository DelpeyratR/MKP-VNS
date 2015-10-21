#include "couple.h"
#include <iostream>
#include <math.h>
using namespace std;

Couple::Couple()
{
	nom = 0;
	note = -1;
}
Couple::Couple(int a)
{
	nom = 0;
	note = 0;
}
void Couple::setname(int n)
{
	nom = n;
}
void Couple::setmark(float n)
{
	note = n;
}
int Couple::getname()
{
	return nom;
}
float Couple::getmark()
{
	return note;
}
CompareNote::CompareNote()
{
	ptr = new int;
	*ptr = 0;
}

bool CompareNote::operator()(Couple a, Couple b)
{
	(*ptr)++;
	float i = a.getmark();
	float j = b.getmark();
	return i<j;
}
int CompareNote::getvalue()
{
	return *ptr;
}
CompareNom::CompareNom()
{
	pointe = new int;
	*pointe = 0;
}
bool CompareNom::operator()(Couple a, Couple b)
{
	(*pointe)++;
	int i = a.getname();
	int j = b.getname();
	return i<j;
}
int CompareNom::getvalue()
{
	return *pointe;
}