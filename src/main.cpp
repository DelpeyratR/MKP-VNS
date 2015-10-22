#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<algorithm>
#include <cstdlib> 
#include <ctime>
#include "couple.h"
using namespace std;
struct instance
{
    int nb_projects;
    int nb_resources;
    int optimal_value;
    std::vector<int> c; // objective function
    // a[i,j]: amount of resource i used by project j
    std::vector<std::vector<int> > a;
    std::vector<int> b; // available resources
};
struct solution
{
	int valeur;
	std::vector<int> projets;
	std::vector<int> reste;
	};

int somme(vector<vector<int>> v, int j)
{
	int result;
	result = 0;
	for (int i = 0; i < v.size();i++)
	{
		result = result + v[i][j];
	}
	return result;
}
vector<int> ajoutg(instance inst, vector<int> v,int j)
{
	vector<int> w=v;
	w[0]=1;
	for (int i = 0; i < w.size()-1;i++)
	{
		w[i+1] = w[i+1] - inst.a[i][j];
		if (w[i+1] < 0)
		{
			v[0] = 0;
			return v;
		}
	}
	return w;
}
vector<int> ajout(instance inst, vector<int> v, int j, int signe)
{
	vector<int> w = v;
	w[0] = 1;
	for (int i = 0; i < w.size() - 1; i++)
	{
		w[i + 1] = w[i + 1] - signe*inst.a[i][j];
		if (w[i + 1] < 0)
		{
			w[0] = 0;
		}
	}
	return w;
}
solution glouton(instance inst)
{
	//classement des projets par prix: c(j)/ somme(a(i,j), i entre 1 et M
	std::vector<Couple> prix;//vecteur des prix défini ci-dessus
	int sum;
	Couple prix_j;//couple est un class qui permet de manipuler un couplede données (nom,valeur) et de les trier par nom ou par valeur
	CompareNote cnote;
	std::vector<int> reste= inst.b;// quantités de marchandises restantes en stock
	reste.insert(reste.begin(), 0);
	solution sol;
	for (int j = 0; j < inst.c.size();j++)
	{
		sum = somme(inst.a,j);
		prix_j.setname(j);
		if (sum > 0)
			prix_j.setmark(inst.c[j] / float(sum));
		else
			prix_j.setmark(10000000);
		prix.push_back(prix_j);
	}
	std::sort(prix.begin(), prix.end(), cnote);
	//Du prix leplus grand au prix le plus petit:on essaie d'ajouter le projet et si on réussit on actualise la valeur et lesquantités de marchandises restantes
	sol.projets.resize(inst.c.size());
	sol.projets.assign(inst.c.size(), 0);
	sol.valeur = 0;
	while (prix.size()>0)
	{
		reste = ajoutg(inst, reste, prix.back().getname());
		if (reste[0]==1)
		{
			sol.projets.at(prix.back().getname()) = 1;
			sol.valeur = sol.valeur+inst.c[prix.back().getname()];
		}
		prix.pop_back();
	}
	sol.reste = reste;
	return sol;
}
int evaluer(instance inst, solution sol)
{
	int valeur = 0;
	for (int i = 0; i < sol.projets.size(); i++)
	{
		valeur = valeur + sol.projets[i] * inst.c[i];
	}
	return valeur;
}

solution monteeV1(instance inst, solution init)
{
	solution sol_max=init;//meilleure solution obtenue jusqu'à présent
	solution sol;//solution courante
	solution sol_prime;
	int valeur_prime;
	bool end = false;
	int ajout_j;
	int compteur = 0;
	//on verifie que la solution est réalisable, sinon on lui assigne la valeur 0
	
	while (!end)
	{
		end = true;
		sol = sol_max;
		//on construit un voisinage de x de distance de Hamming 1
		//pour chaque solution de ce voisinage on évalue la valeur et on la compare à la valeur de la solution maximale
		for (int j= 0; j < sol.projets.size(); j++)
		{
			sol_prime = sol;
			sol_prime.projets[j] = sol.projets[j] + 1;
			ajout_j = 1;
			if (sol_prime.projets[j] == 2)
			{
				sol_prime.projets[j] = 0;
				ajout_j = -1;
			}
			valeur_prime=evaluer(inst, sol_prime);
			sol_prime.reste = ajout(inst, sol_prime.reste, j, ajout_j);
			if (valeur_prime > sol.valeur)//cela veut forcementdire qu'on a ajouté le projet j
			{
				if (sol_prime.reste[0]==1)
				{
					end = false;
					sol_prime.valeur = valeur_prime;
					sol_max = sol_prime;
				}
			}
			//Si on trouve un meilleure solution on recommence
			//sinon on s'arrête
		}
		compteur++;
		if (compteur > 100)
			end = true;
	}
	
	return sol_max;
}
solution monteeV2(instance inst, solution init)
{
	solution sol_max = init;//meilleure solution obtenue jusqu'à présent
	solution sol;//solution courante
	solution sol_prime;
	int valeur_prime;
	bool end = false;
	int compteur = 0;
	signed int ajout_j;
	signed int ajout_k;
	
	while (!end)
	{
		end = true;
		sol = sol_max;
		//on construit un voisinage de x de distance de Hamming 2
		//pour chaque solution de ce voisinage on évalue la valeur et on la compare à la valeur de la solution maximale
		for (int j = 0; j < sol.projets.size(); j++)
		{
			
			
			for (int k = j + 1; k < sol.projets.size(); k++)
			{
				sol_prime = sol;
				sol_prime.projets[j] = sol.projets[j] + 1;
				ajout_j = 1;
				if (sol_prime.projets[j] == 2)
				{
					sol_prime.projets[j] = 0;
					ajout_j = -1;
				}
				sol_prime.projets[k] = sol.projets[k] + 1;
				ajout_k = 1;
				if (sol_prime.projets[k] == 2)
				{
					sol_prime.projets[k] = 0;
					ajout_k = -1;
				}
				valeur_prime = evaluer(inst, sol_prime);
				sol_prime.reste = ajout(inst, sol_prime.reste, j, ajout_j);
				sol_prime.reste = ajout(inst, sol_prime.reste, k, ajout_k);
				if (valeur_prime > sol.valeur)
				{
					
					if (sol_prime.reste[0]==1)
					{
						end = false;
						sol_prime.valeur = valeur_prime;
						sol_max = sol_prime;
						if (sol_max.valeur > inst.optimal_value)
							sol_max.valeur = 0;
					}
				}
			}
			//Si on trouve un meilleure solution on recommence
			//sinon on s'arrête

		}
		compteur++;
		if (compteur > 100)
			end = true;
	}
	
	return sol_max;
}
solution pertubationV1(instance inst, solution sol)
{
	int j = rand() % inst.c.size();
	sol.projets[j] = sol.projets[j] + 1;
	signed int ajout_j = 1;
	if (sol.projets[j] == 2)
	{
		sol.projets[j] = 0;
		ajout_j = -1;
	}
	sol.valeur = evaluer(inst, sol);
	sol.reste[0] = 1;
	for (int i = 0; i < inst.b.size(); i++)
	{
		sol.reste[i+1] = sol.reste[i+1] - ajout_j * inst.a[i][j];
		if (sol.reste[i + 1] < 0)
			sol.reste[0] = 0;
	}
	return sol;
}
solution pertubationV2(instance inst, solution sol)
{
	int j = rand() % inst.c.size();
	int k = rand() % inst.c.size();
	if (k == j && k != 0)
		k--;
	else if (k==j && k==0)
		k++;
	sol.projets[j] = sol.projets[j] + 1;
	 int ajout_j = 1;
	if (sol.projets[j] == 2)
	{
		sol.projets[j] = 0;
		ajout_j = -1;
	}
	sol.projets[k] = sol.projets[k] + 1;
	int ajout_k = 1;
	if (sol.projets[k] == 2)
	{
		sol.projets[k] = 0;
		ajout_k = -1;
	}
	sol.valeur=evaluer(inst, sol);
	sol.reste[0] = 1;
	for (int i = 0; i < inst.b.size(); i++)
	{
		sol.reste[i+1] = sol.reste[i+1] - ajout_j * inst.a[i][j] - ajout_k * inst.a[i][k];
		if (sol.reste[i + 1] < 0)
		{
			sol.reste[0] = 0;
		}
	}
	return sol;
}
solution heuristique(instance inst, solution init)
{
	int compteur = 0;
	int k = 0;
	int kmax = 3;
	int valeur;
	solution sol_max = init;
	solution sol_un;
	solution sol_deux;
	while (compteur < 100)
	{
		k = 1;
		while (k != kmax && compteur<1000)
		{
			//on pertube la solution maximale
			//on effectue une montée dans le k voisinage
			if (k == 1)
			{
				sol_un = pertubationV1(inst, sol_max);
				sol_deux = monteeV1(inst, sol_un);
				compteur++;
			}
			else
			{
				sol_un = pertubationV2(inst, sol_max);
				sol_deux = monteeV2(inst, sol_un);
				compteur++;
			}
			//valeur = evaluer(inst, sol_deux);
			
			//si on arrive à une meilleure solution on se déplace et on recommence à k=1
			if (sol_deux.valeur > sol_max.valeur)
			{
				if (sol_deux.reste[0] == 1)
				{
					sol_max = sol_deux;
					k = 1;
				}

			}
			//sinon on passe à un voisinage plus grand
			else
				k++;
		}
		compteur++;

	}
	return sol_max;
}
int main(int argc, char** argv)
{
	ifstream fichier("mknap1.txt", ios::in);
	srand(time(NULL));
	int nb_instances;

		fichier >> nb_instances;
	

std::vector<instance> instances(nb_instances);
    for (auto& inst : instances)
    {
        fichier>> inst.nb_projects >> inst.nb_resources >> inst.optimal_value;
        inst.c.resize(inst.nb_projects);
        for (int& cc : inst.c)
            fichier>> cc;

        inst.a.resize(inst.nb_resources);
        for (int i = 0; i < inst.nb_resources; i++)
        {
            inst.a[i].resize(inst.nb_projects);
            for (int j = 0; j < inst.nb_projects; j++)
            {
                fichier>> inst.a[i][j];
            }
        }

        inst.b.resize(inst.nb_resources);
        for (int& bb : inst.b)
            fichier>> bb;
    }

    for (const auto& inst : instances)
    {
		//appel de la fonction glouton pour produire une première solution admissible
		solution sol= glouton(inst);
		
        std::cout << inst.nb_projects << " variables" << std::endl;
        std::cout << inst.nb_resources << " constraints" << std::endl;
		std::cout << "OPT = " << inst.optimal_value << endl;
		cout<<"Valeur trouvee par glouton = "<<sol.valeur << std::endl << std::endl;
		//montée depuis la solution nulle avec un voisinage V(1)
		//sol.valeur = 0;
		//sol.projets.assign(sol.projets.size(), 0);
		//sol.reste = inst.b;
		//sol = monteeV1(inst, sol);
		//cout << "Valeur trouvee après une montée simple = " << sol.valeur << endl;
		//heuristique
		sol = heuristique(inst, sol);
		cout << "Valeur trouvee par heuristique = " << sol.valeur << endl;
		for (int i = 0; i < sol.projets.size(); i++)
		{
			std::cout << "Projet numero = " << i;
			if (sol.projets[i] == 0)
				cout << " rejete" << endl;
			else
				cout << " retenu" << endl;
		}
		cout << endl;

    }

    return 0;
}
