#ifndef _INTERPOLATED_DISCRETIZATION_GRIDS_OF_GROUPS
#define _INTERPOLATED_DISCRETIZATION_GRIDS_OF_GROUPS

#include <vector>
#include "Grid.hpp"

#ifdef ID_GOOGLE_TEST
#include "gtest/gtest.h"
#endif

class GridsOfGroups {

#ifdef ID_GOOGLE_TEST
    FRIEND_TEST(TestGridsOfGroups, ctor);
    FRIEND_TEST(TestGridsOfGroups, double_grids);
#endif

    private:
        /** vector of grids */
        std::vector<Grid> _grids_of_groups;
        /** number of dimensions of discrete_points_for_all_dims */
        size_t _max_number_of_dimensions;
        
    public:
        /**
         * Constrctor
         * @param discrete_points_for_all_dims presentation of an n-dimensional space with discrete partition for each dimension
         * @param indices_of_groups groups where each group presents k-dimensions
         */
        GridsOfGroups(const std::vector<std::vector<double> >& discrete_points_for_all_dims, const std::vector<std::vector<size_t> >& indices_of_groups) 
            : _max_number_of_dimensions(discrete_points_for_all_dims.size())
            {
                for (size_t i = 0; i < indices_of_groups.size(); i++) {
                    size_t group_size = indices_of_groups[i].size();
                    std::vector<std::vector<double> > temp_discrete_points(group_size);
                        
                    for (size_t j = 0; j < group_size; j++) {
                        size_t dim = indices_of_groups[i][j];
                        temp_discrete_points[j] = discrete_points_for_all_dims[dim];
                    }
                    
                    Grid grid(temp_discrete_points);
                    _grids_of_groups.push_back(grid);
                }
            }
        
        /**
         * Getter of a vertex from the grid by index from a specific group
         * @param group_index index of group
         * @param index index of the desired vertex
         * @param vertex reference in which will be passed values of the desired vertex 
         */
        void get_vertex(size_t group_index, size_t vertex_index, std::vector<double>& vertex) const {
            _grids_of_groups[group_index].get_vertex(vertex_index, vertex);
        }
        
        /**
         * Returns number dimensions of group
         * @param group_index index of group
         * @return number of dimensions of a group
         */
        size_t get_num_of_dims(size_t group_index) const {
            return _grids_of_groups[group_index].get_num_of_dims();
        }
        
       /**
         * Returns number vertices for a group
         * @param group_index index of group
         * @return number of vertices if a group
         */
        size_t get_num_of_vertices(size_t group_index) const {
            return _grids_of_groups[group_index].get_num_of_vertices();
        }
        
        /**
         * Returns number of all vertices
         * @return number of vertices
         */
        size_t get_number_of_all_vertices() const {
            size_t result = 0;
            
            for (const auto& group : _grids_of_groups) {
                result += group.get_num_of_vertices();
            }
            
            return result;
        }

        /**
         * Returns number of all grid groups
         * @return number of groups
         */
        size_t get_number_of_groups() const {
            return _grids_of_groups.size();
        }
        
        /**
         * Operator [] helps iterate over the groups like a vector
         * @param index the index of desired group
         * @return reference to group
         */
        const Grid& operator[](size_t index) const {
            return _grids_of_groups[index];
        }
        
        /**
         * Compares current grids of groups to other
         * @param other grids of groups to compare
         * @return true if and only if _grids_of_groups are equal. 
         */
        bool operator==(const GridsOfGroups& other) const {
            bool result = false;
            
            if (_grids_of_groups.size() == other._grids_of_groups.size()){
                bool result = true;
                
                for (size_t i = 0; result && i < _grids_of_groups.size(); i++) {
                    if (_grids_of_groups[i] != other._grids_of_groups[i])
                        result = false;
                }
            }
            
            return result;
        }
        
        /**
         * @return number of dimensions of discrete_points_for_all_dims
         */
        size_t get_max_number_of_dimensions() const {
            return _max_number_of_dimensions;
        }
};
#endif


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
