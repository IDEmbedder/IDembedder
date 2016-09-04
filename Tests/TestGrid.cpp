#include "gtest/gtest.h"
#include "../Grid.hpp"

class TestGrid : public ::testing::Test {
    protected:
        std::vector<double> dim1 = {-1, 0, 0.5, 1, 2};
        std::vector<double> dim2 = {2, 3, 4, 10};
        std::vector<std::vector<double> > dis_points = {dim1, dim2};
        Grid grid;
        
        TestGrid() : grid(dis_points) {}
};

TEST_F(TestGrid, ctor) {
    std::vector<size_t> steps_between_dims = {4, 1};
    
    EXPECT_EQ(grid._discrete_points, dis_points);
    
    EXPECT_EQ(grid.get_num_of_dims(), 2);
    
    EXPECT_EQ(grid._number_of_vertices, (dim1.size() * dim2.size()));
    
    EXPECT_EQ(grid._steps_between_dims, steps_between_dims);
}

TEST_F(TestGrid, find_lower_bound) {
    double x0 = -3, x1 = -1, x2 = 0.25, x3 = 0.5, x4 = 2, x5 = 10;
	int actual_result;

	actual_result = grid._find_lower_bound(dim1, x0);
	EXPECT_EQ(0 , actual_result);
	
	actual_result = grid._find_lower_bound(dim1, x1);
	EXPECT_EQ(0 , actual_result);
	
	actual_result = grid._find_lower_bound(dim1, x2);
	EXPECT_EQ(1 , actual_result);
	
	actual_result = grid._find_lower_bound(dim1, x3);
	EXPECT_EQ(2 , actual_result);
	
	actual_result = grid._find_lower_bound(dim1, x4);
	EXPECT_EQ(3 , actual_result);
		
	actual_result = grid._find_lower_bound(dim1, x5);
	EXPECT_EQ(3 , actual_result);
}

TEST_F(TestGrid, norm_calc) {
    ASSERT_DEATH(grid._norm_calc(2, 3, 1), "");
    
    ASSERT_DEATH(grid._norm_calc(2, 3, 4), "");

    EXPECT_EQ(grid._norm_calc(1, 3, 1), 0);
    
    EXPECT_EQ(grid._norm_calc(1, 3, 3), 1);
    
    EXPECT_EQ(grid._norm_calc(1, 3, 1.5), 0.25);
}

TEST_F(TestGrid, find_simplex) {
    std::vector<double> vector_to_embed = {-0.5, 3.25};
    std::vector<double> norms_expected = {0.5, 0.25};
    std::vector<double> norms_actual(2);
    std::vector<size_t> lower_bounds_expected = {0, 1};
    std::vector<size_t> lower_bounds_actual(2);
    std::vector<size_t> indices_expected = {1, 0};
    std::vector<size_t> indices_actual(2);
    
    grid.find_simplex(vector_to_embed, norms_actual, lower_bounds_actual, indices_actual);
    EXPECT_EQ(norms_expected, norms_actual);
    
    EXPECT_NE(vector_to_embed, norms_actual);
    
    EXPECT_EQ(lower_bounds_expected, lower_bounds_actual);
    
    EXPECT_NE(indices_expected, lower_bounds_actual);
    
    EXPECT_EQ(indices_expected, indices_actual);
    
    EXPECT_NE(lower_bounds_expected, indices_actual);
}

TEST_F(TestGrid, get_vertex) {
    std::vector<double> vertex_expected = {0.5, 10};
    std::vector<double> vertex_actual(2);
    std::vector<double> vertex_not_equal = {0.5, 11};
    size_t index_of_vertex = 11;
    size_t index_of_vertex_fail = 30;
    
    grid.get_vertex(index_of_vertex, vertex_actual);
    EXPECT_EQ(vertex_expected, vertex_actual);
    
    EXPECT_NE(vertex_not_equal, vertex_actual);
    
    ASSERT_DEATH(grid.get_vertex(index_of_vertex_fail, vertex_actual), "");
}

TEST_F(TestGrid, get_num_of_dims) {
    EXPECT_EQ(dis_points.size(), grid.get_num_of_dims());
    
    EXPECT_NE(3, grid.get_num_of_dims());
}

TEST_F(TestGrid, get_num_of_vertices) {
    EXPECT_EQ(dim1.size() * dim2.size(), grid.get_num_of_vertices());
    
    EXPECT_NE(3, grid.get_num_of_vertices());
}

TEST_F(TestGrid, operator_brackets) {
    EXPECT_EQ(dim1, grid[0]);
    
    EXPECT_EQ(dim2, grid[1]);
}

TEST_F(TestGrid, index_calc) {
    size_t expected_index = 7;
    size_t actual_index;
    std::vector<size_t> lower_bounds = {1, 3};
    
    actual_index = grid.index_calc(lower_bounds);
    EXPECT_EQ(expected_index, actual_index);
    EXPECT_NE(-1, actual_index);
}

TEST_F(TestGrid, double_grid) {
    std::vector<std::vector<double> > double_dis_points(dis_points);
    double_dis_points.insert(double_dis_points.begin(), dis_points.begin(), dis_points.end());
    Grid double_grid(double_dis_points);
    
    grid.double_grid();  

    EXPECT_EQ(double_grid._discrete_points, grid._discrete_points);
    
    EXPECT_EQ(double_grid.get_num_of_dims() ,grid.get_num_of_dims());
    
    EXPECT_EQ(double_grid._number_of_vertices ,grid._number_of_vertices);
    
    EXPECT_EQ(double_grid._steps_between_dims ,grid._steps_between_dims);
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
