#ifndef _INTERPOLATED_DISCRETIZATION_ID_GROUPS_BASE
#define _INTERPOLATED_DISCRETIZATION_ID_GROUPS_BASE

#include <vector>
#include "ID.hpp"
#include "IDpair.hpp"
#include "GridsOfGroups.hpp"

#ifdef ID_GOOGLE_TEST
#include "gtest/gtest.h"
#endif

class IDgroupsBase {

#ifdef ID_GOOGLE_TEST
    friend class TestIDgroupsBase;
    FRIEND_TEST(TestIDgroupsBase, ctor);
    FRIEND_TEST(TestIDgroupsBase, num_of_groups);
    FRIEND_TEST(TestIDgroupsBase, init_temp_vector_to_embed);
    FRIEND_TEST(TestIDgroupsBase, copy_part_results);
#endif

    protected:
        /** vector of indices of groups */
        std::vector<std::vector<size_t> > _indices_of_groups;
    	/** Vector pairs contans weights of vertices and indices of each weight in embedded vector */
        mutable std::vector<Pair> _embedded_vector_groups;
        /** vector of number of vertices for each group */
        std::vector<size_t> _number_of_vertices_of_groups;
        /** number of dimensions of discrete_points_for_all_dims from GridsOfGroups */
        size_t _max_number_of_dimensions;
        
        /**
         * @return number of groups
         */
        size_t _num_of_groups() const {
            return _indices_of_groups.size();
        }

        /**
         * Copy of partial results from ID/IDpair to _embedded_vector_size_calc
         * @param part_results partial results
         * @param shift_index number of index to be shifted
         * @param next_place_index index of next free cell in number_of_vertices_of_groups
         */
        void _copy_part_results(const std::vector<Pair>& part_results, 
                                const size_t& shift_index, 
                                size_t& next_place_index) const 
                                {
                                    for (const auto& pair : part_results) {
                                        _embedded_vector_groups[next_place_index] = pair;
                                        _embedded_vector_groups[next_place_index]._index += shift_index;
                                        next_place_index++;
                                    }
                                }
        
        /**
         * Creation of new object which matches a group 
         * @param group_index index of desired group
         * @param vector_to_embed object which represents a point in space
         * @param temp_vector_to_embed object matches a group dimensions
         */
        void _init_temp_vector_to_embed(size_t group_index,
                                   const std::vector<double>& vector_to_embed,
                                   std::vector<double>& temp_vector_to_embed) const 
                                   {
                                        const std::vector<size_t>& group(_indices_of_groups[group_index]);
                                        size_t group_size = group.size();
                                        temp_vector_to_embed.resize(group_size);
                                        
                                        for (size_t i = 0; i < group_size; i++) {
                                            size_t dim = group[i];
                                            temp_vector_to_embed[i] = vector_to_embed[dim];
                                        }
                                    }
        
        /**
         * Constructor
         * @param indices_of_groups desired groups 
         */
        IDgroupsBase(const std::vector<std::vector<size_t> >& indices_of_groups, 
                    size_t embedded_vector_groups_size,
                    size_t max_number_of_dimensions) 
                        : _indices_of_groups(indices_of_groups)
                        , _embedded_vector_groups(embedded_vector_groups_size)
                        , _max_number_of_dimensions(max_number_of_dimensions)
                        {}
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
