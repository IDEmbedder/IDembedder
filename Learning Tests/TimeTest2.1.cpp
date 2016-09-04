#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
// #include "ID.hpp"
// #include "IDpair.hpp"
#include "GridGroup.hpp"
#include "IDgroup.hpp"
// #include "IDgroupPair.hpp"
// #include "Grid.hpp"
 
int main(int argc, char** argv){
    
    std::vector<double> gridForX1 = {-30, -20, 1, 2, 4, 10, 10.5, 10.8, 11, 11.1, 11.5, 11.55};
    std::vector<double> gridForX2 = {4, 5, 6, 8, 10, 20, 30};
    std::vector<double> gridForX3 = {gridForX1};
    std::vector<double> gridForX4 = {gridForX1};

    std::vector<double> point = {-11.4, 3, 10, 10.99};
    std::vector<double> point2 = {3, 25, 3, 10};

    std::vector<std::vector<double> > dis_points = {gridForX1, gridForX2, gridForX3, gridForX4};
    std::vector<std::vector<double> > di2(dis_points);
    std::vector<std::vector<double> > for_grid_pair(di2);
    for_grid_pair.insert(for_grid_pair.end(), di2.begin(), di2.end());
    
    
    Grid grid(di2);
    Grid grid_pair2(for_grid_pair);
    
    std::vector<double> vex(4);
    std::vector<double> vex2(8);

    
    std::vector<std::vector<size_t> > groups = {{0}, {1}, {2}, {3}, 
                                                {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}, 
                                                {0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3},
                                                {0, 1, 2, 3}};        
    
    size_t num_of_ver = grid.get_number_of_vertices();
    std::cout << num_of_ver << std::endl;
    
    GridGroup grid_groups(dis_points, groups);
    IDgroup id(grid_groups, groups);
    // Grid g(foo());
    // std::cout << g.get_number_of_vertices() << std::endl;
    
    // Grid grid(dis_points);
    IDpair id_pai2r(grid_pair2);
    const std::vector<Pair>& result3(id_pai2r(point, point2));
    
    for (const auto& item : result3) {
        std::cout << item << std::endl;
    }
    

    // const std::vector<Pair>& result(id(point));

    // for (size_t i = 0; i < 1000000; i++) {
    //     const std::vector<Pair>& result(id(point));
    // }
    
    // for (size_t i = 0; i < num_of_ver; i++) {
    //     grid.get_vertex(i, vex);
    // }
    
    
    // num_of_ver = grid_pair.get_number_of_vertices();
    // std::cout << num_of_ver << std::endl;
    
    
    GridGroup grid_groups_pair(dis_points, groups, true);

    IDgroupPair id_pair(grid_groups_pair, groups);
    
    // for (size_t i = 0; i < 100000; i++) {
    //     const std::vector<Pair>& result2(id_pair(point, point2));
    // }
    
    const std::vector<Pair>& result2(id_pair(point, point2));
    
    for (const auto& item : result2) {
        std::cout << item << std::endl;
    }
    // for (size_t i = 0; i < num_of_ver; i++) {
    //     grid_pair.get_vertex(i, vex2);
    // }
    
    return 0;
}
