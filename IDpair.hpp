#ifndef _INTERPOLATED_DISCRETIZATION_ID_PAIR
#define _INTERPOLATED_DISCRETIZATION_ID_PAIR

#include "ID.hpp"
#include <cassert>
#include <vector>

#ifdef ID_GOOGLE_TEST
#include "gtest/gtest.h"
#endif


class IDpair : ID {

#ifdef ID_GOOGLE_TEST
    FRIEND_TEST(TestIDpair, ctor);
    FRIEND_TEST(TestIDpair, embed);
    FRIEND_TEST(TestIDpair, less);
#endif

    private:
        /**
        * _embed calculate a distence between two objects in space
        * @param vector_to_cmp1 first object which represents a point in space
        * @param vector_to_cmp2 second object which represents a point in space
        * @return reference to _embedded_vector
        */
        const std::vector<Pair>& _embed(const std::vector<double>& vector_to_cmp1, const std::vector<double>& vector_to_cmp2) const {
/*            std::cout << _num_of_dims() << " " << vector_to_cmp1.size() << " " << vector_to_cmp2.size() << std::endl;
            
            for (auto i : vector_to_cmp1) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
            for (auto i : vector_to_cmp2) {
                std::cout << i << " ";
            }
            std::cout << std::endl;*/
            
            assert((_num_of_dims() / 2) == vector_to_cmp1.size()
            && (_num_of_dims() / 2) == vector_to_cmp2.size()
            && "Number of features should be equal to number of dimensions of discrete points");
            
            std::vector<double> concat_vector_to_map(vector_to_cmp1);
            
            concat_vector_to_map.insert(concat_vector_to_map.end(), vector_to_cmp2.begin(), vector_to_cmp2.end());
        
            return ID::operator()(concat_vector_to_map);
        }
        
        /**
        * _less check which object has a less distance
        * @param vector_to_cmp1 first object which represents a point in space
        * @param vector_to_cmp2 second object which represents a point in space
        * @return true if vector_to_cmp1 has a less distance than vector_to_cmp2, false otherwise
        */
        bool _less(const std::vector<double>& vector_to_cmp1, const std::vector<double>& vector_to_cmp2) const {
            for (size_t i = 0; i < vector_to_cmp1.size(); i++) {
                if (vector_to_cmp1[i] < vector_to_cmp2[i])
                    return true;
                else if (vector_to_cmp1[i] > vector_to_cmp2[i])
                    return false;
            }
            
            return false;
        }
        
    public:
    
        /**
		 * TODO: get one dimensional and do the concatenation here 
        * Constrctor
        * @param discrete_points_for_all_dims presentation of an n-dimensional space as Grid
        */
        IDpair(const Grid& discrete_points_for_all_dims)
            : ID(discrete_points_for_all_dims)
            { 
                _discrete_points_for_all_dims.double_grid();
                _embedded_vector.resize(_num_of_dims() + 1);
            }
        
         /**
         * Getter for number of verices
         * @return number of vertices in grid
         */
        size_t get_num_of_vertices() const {
            return _discrete_points_for_all_dims[0].size();
        }
        
        /**
        * Operator () calculate a distence between two objects in space
        * @param vector_to_cmp1 first object which represents a point in space
        * @param vector_to_cmp2 second object which represents a point in space
        * @return reference to _embedded_vector
        */
        const std::vector<Pair>& operator()(const std::vector<double>& vector_to_cmp1, const std::vector<double>& vector_to_cmp2) const {
            if (_less(vector_to_cmp1, vector_to_cmp2))
                return _embed(vector_to_cmp1, vector_to_cmp2);
            
            return _embed(vector_to_cmp2, vector_to_cmp1);
        }
        
        /**
         * Compares current IDpair to other
         * @param other IDpair to compare
         * @return true if IDs are different
         */
        bool operator ==(const IDpair& other) const {
            return _discrete_points_for_all_dims == other._discrete_points_for_all_dims;
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
