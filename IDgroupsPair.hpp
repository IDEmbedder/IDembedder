#ifndef _INTERPOLATED_DISCRETIZATION_ID_GROUPS_PAIR
#define _INTERPOLATED_DISCRETIZATION_ID_GROUPS_PAIR

#include "IDgroupsBase.hpp"

#ifdef ID_GOOGLE_TEST
#include "gtest/gtest.h"
#endif

class IDgroupsPair : public IDgroupsBase {

#ifdef ID_GOOGLE_TEST
    FRIEND_TEST(TestIDgroupsPair, ctor);
    FRIEND_TEST(TestIDgroupsPair, embedded_vector_size_calc);
#endif    

    private:
        /**  vector of ids */
        std::vector<IDpair> _ids_of_groups;
        
        /**
         * Calculatiing size for number_of_vertices_of_groups object
         * @param indices_of_groups desired groups 
         * @return calculated size
         */
        size_t _embedded_vector_size_calc(const std::vector<std::vector<size_t> >& indices_of_groups) const {
            size_t result = 0;
            
            for (const auto& group : indices_of_groups) {
                result += ((group.size() * 2) + 1);
            }
            
            return result;
        }
    
    public:
        /**
         * Constrctor
         * @param grids_of_groups group of grids, each grid presentated as k-dimensional space
         * @param indices_of_groups desired groups 
         */
        IDgroupsPair(const GridsOfGroups& grids_of_groups, const std::vector<std::vector<size_t> >& indices_of_groups) 
            : IDgroupsBase(  indices_of_groups, 
                            _embedded_vector_size_calc(indices_of_groups), 
                            grids_of_groups.get_max_number_of_dimensions())
                            {
                                for (size_t i = 0; i < grids_of_groups.get_number_of_groups(); i++) {
                                    _ids_of_groups.push_back(grids_of_groups[i]);
                                                                        
                                    size_t number_of_vertices = grids_of_groups[i].get_num_of_vertices();
                                    number_of_vertices *= number_of_vertices;
                                    
                                    _number_of_vertices_of_groups.push_back(number_of_vertices);
                                }
                            }
            
        /**
         * Operator () calculate a distence between two objects in each group space 
         * @param vector_to_cmp1 first object which represents a point in space
         * @param vector_to_cmp2 second object which represents a point in space
         * @return reference to number_of_vertices_of_groups
         */
        const std::vector<Pair>& operator()(const std::vector<double>& vector_to_cmp1, const std::vector<double>& vector_to_cmp2) const {
            assert(_max_number_of_dimensions == vector_to_cmp1.size()
            && _max_number_of_dimensions == vector_to_cmp2.size()
            && "Number of features should be equal to number of dimensions of discrete points");
            
            size_t shift_index = 0;
            size_t next_place_index = 0;
            
            for (size_t i = 0; i < _num_of_groups(); i++) {
                std::vector<double> temp_vector_to_cmp1;
                std::vector<double> temp_vector_to_cmp2;
                _init_temp_vector_to_embed(i, vector_to_cmp1, temp_vector_to_cmp1);
                _init_temp_vector_to_embed(i, vector_to_cmp2, temp_vector_to_cmp2);

                const std::vector<Pair>& _part_results(_ids_of_groups[i](temp_vector_to_cmp1, temp_vector_to_cmp2));
                
                _copy_part_results(_part_results, shift_index, next_place_index);
                shift_index += _number_of_vertices_of_groups[i];
            }
            
            return _embedded_vector_groups;
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
