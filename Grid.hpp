#ifndef _INTERPOLATED_DISCRETIZATION_GRID
#define _INTERPOLATED_DISCRETIZATION_GRID

#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <cassert>

#ifdef ID_GOOGLE_TEST
#include "gtest/gtest.h"
#endif

/**
 * Grid represents an n-dimensional discrete space
 */
class Grid {

#ifdef ID_GOOGLE_TEST
    FRIEND_TEST(TestGrid, ctor);
    FRIEND_TEST(TestGrid, find_lower_bound);
    FRIEND_TEST(TestGrid, norm_calc);
    FRIEND_TEST(TestGrid, double_grid);
    FRIEND_TEST(TestID, steps_between_dims);
#endif
    friend class ID;
    
    private:
        /** Discrete points */ 
        std::vector<std::vector<double> > _discrete_points;
        /** Number of the existed vertices in the grid */
        size_t _number_of_vertices;
        /** Vector of distance steps between each dimension */
        std::vector<size_t> _steps_between_dims;
        
        /**
         * The calculation of the steps between dimensions vector.
         * Example: Given a Cube with M x N x K dimensions.
         * The steps vector will equal to: {N*K, K, 1}.
         */
        void _steps_between_dims_calc() {
			assert(_steps_between_dims == std::vector<size_t>(get_num_of_dims(), 1));
			
            for (ptrdiff_t i = get_num_of_dims() - 2; i >= 0; i--) {
                _steps_between_dims[i] = _steps_between_dims[i + 1] * _discrete_points[i + 1].size();
            }
        }
        
        /** 
         * Corrects value of the feature in a case, if the value is out of range
         * @param feature_value the value of the feature which refers to the discrete points  
         * @param dim_discrete_points discrete points of a dimension
         */
        void _correction_of_feature_out_of_range(double& feature_value, const std::vector<double>& dim_discrete_points) const {
            if (feature_value < dim_discrete_points.front())
                feature_value = dim_discrete_points.front();
            else if (feature_value > dim_discrete_points.back())
                feature_value = dim_discrete_points.back();
        }

        /**
         * Returns the lower and upper discrete points for the given value
         * @param dim_discrete_points discrete points of a dimension
         * @param feature_value the search value
         * @return lower bound of feature_value
         */
        size_t _find_lower_bound(const std::vector<double>& dim_discrete_points, double& feature_value) const {
            _correction_of_feature_out_of_range(feature_value, dim_discrete_points);
            
            auto l_iterator = lower_bound(dim_discrete_points.cbegin(), dim_discrete_points.cend(), feature_value);
            size_t lower_bound = l_iterator - dim_discrete_points.begin();

            if ((lower_bound == (dim_discrete_points.size() - 1)) || (dim_discrete_points[lower_bound] != feature_value)) {
                lower_bound--;
            }
        
            return lower_bound;
        }
        
        /**
         * Calculus of norm with received lower and upper limits and a value in the range
         * @param lower_value lower limit
         * @param upper_value upper limit
         * @param feature_value value in the range
         * @return norm of value in the range
        */
        inline double _norm_calc(double lower_value, double upper_value, double feature_value) const {
            assert((feature_value >= lower_value) && (feature_value <= upper_value)
            && "The value should be in range [low, up]");
            
            double result = (feature_value - lower_value) / (upper_value - lower_value);
    
            return result;
        }
        
        /**
         * Validates that each dimension has at least 2 discrete points
         * @return true if each dimension has at leasta 2 discrete points, else false
         */ 
        inline bool _discrete_points_dim_assert() const {
            for (const auto& dim_discrete_points : _discrete_points) {
                if (dim_discrete_points.size() < 2)
                    return false;
            }
            
            return true;
        }

        /**
         * Validates a correct sorting of discrete partition of each dimension
         * @return true if discrete point are sorted, else false
         */
        inline bool _discrete_points_dim_sort_assert() const {
            for (const auto& dim_discrete_points : _discrete_points) {
                if (!std::is_sorted(dim_discrete_points.begin(), dim_discrete_points.end()))
                    return false;
            }
            
            return true;
        }
        
    public:
        /**
         * Constrctor
         * @param discrete_points_for_all_dims presentation of an n-dimensional space with discrete partition for each dimension
         */
        Grid(const std::vector<std::vector<double> >& discrete_points_for_all_dims)
            : _discrete_points(discrete_points_for_all_dims)
            , _number_of_vertices(_discrete_points[0].size())
            , _steps_between_dims(discrete_points_for_all_dims.size(), 1)
            {
                assert(_discrete_points_dim_assert() 
                && "Each dimension in the Grid should contain at least 2 discrete points");
                assert(_discrete_points_dim_sort_assert()
                && "The discrete points should be sorted per each dimension");
                
                _steps_between_dims_calc(); 
                _number_of_vertices *= _steps_between_dims[0];    
            }
        
        /**
         * Copy constrctor
         * @param grid grid to copy
         */
        Grid(const Grid& grid)
            : Grid(grid._discrete_points)
            {}
            
        /**
         * This function returns the corresponding simplex to the received object
         * @param vector_to_embed an object which corresponds to the simplex 
         * @param norms norms of each vertex in simplex
         * @param lower_bounds lower bounds of each vertex of the simplex in the grid
         * @param indices presentation of the simplex by order of indices
         */ 
        void find_simplex(const std::vector<double>& vector_to_embed, std::vector<double>& norms, std::vector<size_t>& lower_bounds, std::vector<size_t>& indices) const {
            for (size_t i = 0; i < get_num_of_dims(); i++) {
                const std::vector<double>& current_dim(_discrete_points[i]);
                double feature_value = vector_to_embed[i];
        
                _correction_of_feature_out_of_range(feature_value, current_dim);
                size_t bound_of_feature = _find_lower_bound(current_dim, feature_value);
                lower_bounds[i] = bound_of_feature;
                
                double lower_value = current_dim[bound_of_feature];
                double upper_value = current_dim[bound_of_feature + 1];
                norms[i] = _norm_calc(lower_value, upper_value, feature_value);
                
                indices[i] = i;
            }
        
            sort(indices.begin(), indices.end(), [&](const size_t& i, const size_t& j)
            { return norms[i] < norms[j]; });
        }
        
        /**
         * Getter of a vertex from the grid by index
         * @param index index of the desired vertex
         * @param vertex reference in which will be passed values of the desired vertex 
         */
        void get_vertex(size_t index, std::vector<double>& vertex) const {
            assert((vertex.size() == get_num_of_dims()) && 
            "The vertex size should be equals to the number of dimensions");
            assert((index < _number_of_vertices) && 
            "Index out of range");

            for (size_t i = 0; i < get_num_of_dims(); i++) {
                size_t step_skip_i = _steps_between_dims[i];
                
                size_t index_of_discrete_point = index / step_skip_i;
                index %= step_skip_i;

                vertex[i] = _discrete_points[i][index_of_discrete_point];
            }
        } 
        
        /**
         * Getter for number of the dimensions
         * @return number of dimensions of this grid
         */
        size_t get_num_of_dims() const {
            return _discrete_points.size();
        }

        /**
         * Getter for number of verices
         * @return number of vertices in this grid
         */
        size_t get_num_of_vertices() const {
            return _number_of_vertices;
        }
        
        /**
         * Operator [] returns a const reference to the discrete points of the requested dimension
         * @param index index of the requested dimension
         * @return const reference to the discrete points of requested dimension of this grid
         */ 
        const std::vector<double>& operator[](size_t index) const {
            return _discrete_points[index];
        }
        
        /**
         * Operator << sends a representation of discrete points of a grid to the standard output
         * @param os reference to ostream object
         * @param grid desired object
         * @return std ostream concatenated with grid
         */
        friend std::ostream& operator<<(std::ostream& os, const Grid& grid) {
            os << "Grid: \n_discrete_points_for_all_dims: ";
            
            for (const auto& dim_discrete_points : grid._discrete_points) {
                for (const  auto& discrete_point : dim_discrete_points) {
                    os << discrete_point << ", ";
                }
            }
            
            return os;
        }
        
         /**
		 * Calculation of index in ID vector for a weight of vertex
         * @param lower bounds of each vertex of the simplex in the grid
         * @return index in learning vector
         */
        size_t index_calc(const std::vector<size_t>& lower_bounds) const {
            size_t result = 0;
            
            for (size_t i = 0; i < lower_bounds.size(); i++) {
                result += (lower_bounds[i] * _steps_between_dims[i]);
            }
            
            return result;
        }
        
        /**
         * Double this grid
         */
        void double_grid() {
            _discrete_points.resize(2 * get_num_of_dims());
            std::copy_n(_discrete_points.begin(), get_num_of_dims()/2, _discrete_points.begin() + get_num_of_dims()/2);
            _number_of_vertices *= _number_of_vertices;
            _steps_between_dims = std::vector<size_t>(get_num_of_dims(), 1);
            _steps_between_dims_calc();
        }
        
        
        /**
         * Compares current grid to other
         * @param other grid to compare
         * @return true if and only if _discrete_points, get_num_of_dims(), _number_of_vertices and _steps_between_dims are equal. 
         */
        bool operator==(const Grid& other) const {
            bool result = false;
            
            if ((_discrete_points == other._discrete_points) &&
                (get_num_of_dims() == other.get_num_of_dims()) &&
                (_number_of_vertices == other._number_of_vertices) &&
                (_steps_between_dims == other._steps_between_dims))
                    result = true;
                    
            return result;
        }
        
        /**
         * Compares current grid to other
         * @param other grid to compare
         * @return true if grids are different
         */
        bool operator!=(const Grid& other) const {
            return !this->operator ==(other);
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
