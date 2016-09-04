#ifndef _INTERPOLATED_DISCRETIZATION_ID
#define _INTERPOLATED_DISCRETIZATION_ID
#include <cassert>
#include "Grid.hpp"
#include "Pair.hpp"

#ifdef ID_GOOGLE_TEST
#include "gtest/gtest.h"
#endif

/**
 * The ID embedding class.
 * Usage: Offline use ctor which gets Grid.
 *        Online use operator() which embeds a given vector and returns a sparse representation of it.
 */ 
class ID {

	friend class IDgroup;
	
#ifdef ID_GOOGLE_TEST
    FRIEND_TEST(TestID, ctor);
    FRIEND_TEST(TestID, num_of_dims);
    FRIEND_TEST(TestID, steps_between_dims);
    FRIEND_TEST(TestID, embedded_vector_lambdas_calc);
    FRIEND_TEST(TestID, embedded_vector_indices_calc);
#endif
    
    private:
        /**
         * Calculation of lambdas equation
         * @param lambdas vector where the values will be stored
         * @param normalized_features norms of each vertex in simplex
         * @param indices_order presentation of the simplex by order of indices
         */
        void _embedded_vector_lambdas_calc(const std::vector<double>& normalized_features, const std::vector<size_t>& indices_order) const {
            double sum_of_lambdas = 0;
            double current_lambda = 0;
            size_t size_of_lambdas = _embedded_vector.size();
            
            for (size_t i = 0; i < _num_of_dims(); i++) {
                current_lambda = normalized_features[indices_order[i]] - sum_of_lambdas;
                
                _embedded_vector[size_of_lambdas - i - 1]._weight = current_lambda;
                sum_of_lambdas += current_lambda;
            }
            _embedded_vector[0]._weight = 1 - sum_of_lambdas;
        }
        
        /**
         * Calculation of _embedded_vector
         * @param lower_bounds lower bounds of each vertex of simplex
         * @param lambdas weights of each vertex of simplex
         * @param indices_order presentation of the simplex
         */
        void _embedded_vector_indices_calc(const std::vector<size_t>& lower_bounds, const std::vector<size_t>& indices_order) const {
            size_t next_index;
            size_t prod_size_i;
            
            _embedded_vector[0]._index = _discrete_points_for_all_dims.index_calc(lower_bounds);
        
            for (size_t i = 1; i < _embedded_vector.size(); i++) {
                prod_size_i = _steps_between_dims()[indices_order[_steps_between_dims().size() - i]];
                next_index = _embedded_vector[i - 1]._index + prod_size_i;
                _embedded_vector[i]._index = next_index;    
            }
        }

    protected:
        /** Discrete points */ 
        Grid _discrete_points_for_all_dims;
        /** Vector pairs contans weights of vertices of the corresponding simplex in grid and indices of each weight in learning vector */
        
        mutable std::vector<Pair> _embedded_vector;
        /** Number of dimensions of discrete points */
        
        size_t _num_of_dims() const {
            return _discrete_points_for_all_dims.get_num_of_dims();
        }
        
        /** Vector of distance steps between each dimension */
        const std::vector<size_t>& _steps_between_dims() const {
            return _discrete_points_for_all_dims._steps_between_dims;
        }
        
    public:
        /**
         * Constrctor
         * @param discrete_points_for_all_dims presentation of an n-dimensional space as Grid
         */
        ID(const Grid& discrete_points_for_all_dims)
            : _discrete_points_for_all_dims(discrete_points_for_all_dims)
            , _embedded_vector(_num_of_dims() + 1)
            {}
        
        /**
         * Opearator () does interpolation of discrete points 
         * @param vector_to_embed object which represents a point in space
         * @return reference to embedded_vector
         */
        const std::vector<Pair>& operator()(const std::vector<double>& vector_to_embed) const {
            assert( _num_of_dims() == vector_to_embed.size()
            && "Number of features should be equal to number of dimensions of discrete points");
            
            std::vector<double> norms(vector_to_embed.size());
            std::vector<size_t> lower_bounds(vector_to_embed.size());
            std::vector<size_t> indices(vector_to_embed.size());
            
            _discrete_points_for_all_dims.find_simplex(vector_to_embed, norms, lower_bounds, indices);
            _embedded_vector_lambdas_calc(norms, indices);
            _embedded_vector_indices_calc(lower_bounds, indices);
            
            return _embedded_vector;
        }
        
        
        /**
         * Operator << sends a representation of discrete points of a grid to standard output
         * @param os reference to ostream object
         * @param id desired object
         * @return std ostream concatenated with id
         */
        friend std::ostream& operator<<(std::ostream& os, const ID& id) {
            os << "ID: \n_discrete_points_for_all_dims: ";
            
            os << id._discrete_points_for_all_dims;
            
            os << "\n_embedded_vector: ";
            for (auto& item : id._embedded_vector){
                os << item; 
            }
            
            os << "_prod_sizes: ";
            for (auto& item : id._steps_between_dims()) {
                os << item << ", ";
            }
            
            os << "\n_num_of_dims: " << id._num_of_dims() << "\n";

            return os;
        }
        
        /**
         * Compares current ID to other
         * @param other ID to compare
         * @return true if IDs are different
         */
        bool operator ==(const ID& other) const {
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
