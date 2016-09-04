#include <vector>
#include <iostream>
#include "Grid.hpp"
#include "GridsOfGroups.hpp"
#include "ID.hpp"
#include "IDpair.hpp"
#include "IDgroups.hpp"
#include "IDgroupsPair.hpp"

int main(int argc, char *argv[]) {
    std::vector<Pair> vector_id;
    
    std::vector<double> vector_to_embed1 = {1.3, 1.4, -1.8};
    std::vector<double> vector_to_embed2 = {2.2, -1.4, 8.7};
    
    //dimensions
    std::vector<double> dim1 = {0, 2, 4, 6, 10};
    std::vector<double> dim2 = {-3, 0, 3};
    std::vector<double> dim3 = {-6.4, -1.3, 0.1, 13, 17.3};
    
    //discrete points of dimensions
    std::vector<std::vector<double> > discrete_points = {dim1, dim2, dim3};
    
    //grid wrapper for discrete points
    Grid grid(discrete_points);
    
    //groups indices - in the following example combinations of two dimensions
    std::vector<std::vector<size_t> > groups = {{0, 1}, {0, 2}, {1, 2}};
    
    //grids of groups makes grid for each group
    GridsOfGroups grids_of_groups(discrete_points, groups);
    
    //interpolated discretization
    ID id(grid);
    
    //embedder
    vector_id = id(vector_to_embed1);
    
    std::cout << "-----------------------ID-----------------------" << std::endl;
    for (const auto& pair : vector_id) {
        std::cout << pair << std::endl;
    }
    std::cout << "\n\n";
    
    
    //interpolated discretization of pairs
    IDpair id_pair(grid);
    
    //embedder
    vector_id = id_pair(vector_to_embed1, vector_to_embed2);
    
    std::cout << "--------------------ID  PAIR--------------------" << std::endl;
    for (const auto& pair : vector_id) {
        std::cout << pair << std::endl;
    }
    std::cout << "\n\n";
    
    
    //interpolated discretization for all ID groups
    IDgroups id_groups(grids_of_groups, groups);
    
    //embedder
    vector_id = id_groups(vector_to_embed1);
    
    std::cout << "-------------------ID  GROUPS-------------------" << std::endl;
    for (const auto& pair : vector_id) {
        std::cout << pair << std::endl;
    }
    
    
    //interpolated discretization for all IDpair groups
    IDgroupsPair id_groups_pair(grids_of_groups, groups);
    
    //embedder
    vector_id = id_groups_pair(vector_to_embed1, vector_to_embed2);
    
    std::cout << "-----------------ID GROUPS PAIR-----------------" << std::endl;
    for (const auto& pair : vector_id) {
        std::cout << pair << std::endl;
    }

    return 0;
}


/**
IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

This license is for all code in this distribution. 

By downloading, copying, installing or using the software you agree to this license. If you do not
agree to this license, do not download, install, copy or use the software.

LICENSE CONDITIONS

Copyright (c) 2016, Ofir Pele, Alexey Kurbatsky
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met: 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * The names of its contributors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
