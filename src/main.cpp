#include <iostream>
#include <vector>

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

int main(int argc, char** argv)
{
    int nb_instances;
    std::cin >> nb_instances;
    std::vector<instance> instances(nb_instances);
    for (auto& inst : instances)
    {
        std::cin >> inst.nb_projects >> inst.nb_resources >> inst.optimal_value;
        inst.c.resize(inst.nb_projects);
        for (int& cc : inst.c)
            std::cin >> cc;

        inst.a.resize(inst.nb_resources);
        for (int i = 0; i < inst.nb_resources; i++)
        {
            inst.a[i].resize(inst.nb_projects);
            for (int j = 0; j < inst.nb_projects; j++)
            {
                std::cin >> inst.a[i][j];
            }
        }

        inst.b.resize(inst.nb_resources);
        for (int& bb : inst.b)
            std::cin >> bb;
    }

    for (const auto& inst : instances)
    {
        std::cout << inst.nb_projects << " variables" << std::endl;
        std::cout << inst.nb_resources << " constraints" << std::endl;
        std::cout << "OPT = " << inst.optimal_value << std::endl << std::endl;
    }

    return 0;
}
