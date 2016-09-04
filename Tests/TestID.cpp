#include "gtest/gtest.h"
#include "../ID.hpp"

class TestID : public ::testing::Test {
    protected:
        std::vector<double> dim1 = {-1, 0, 0.5, 1, 2};
        std::vector<double> dim2 = {2, 3, 4, 10};
        std::vector<std::vector<double> > dis_points = {dim1, dim2};
        Grid grid;
        ID id;
        
        TestID() : grid(dis_points), id(grid) {};
};

TEST_F(TestID, ctor) {
    EXPECT_EQ(grid, id._discrete_points_for_all_dims);
    
    EXPECT_EQ(grid.get_num_of_dims() + 1, id._embedded_vector.size());
}

TEST_F(TestID, num_of_dims) {
    EXPECT_EQ(grid.get_num_of_dims(), id._num_of_dims());
    
    EXPECT_NE(grid.get_num_of_dims(), -1);
}

TEST_F(TestID, steps_between_dims) {
    EXPECT_EQ(grid._steps_between_dims, id._steps_between_dims());
}

TEST_F(TestID, embedded_vector_lambdas_calc) {
    std::vector<double> normalized_features = {0.5, 0.25};
    std::vector<size_t> indices_order = {1, 0};
    std::vector<Pair> expected_result = {{0, 0.5}, {0, 0.25}, {0, 0.25}};
    
    id._embedded_vector_lambdas_calc(normalized_features, indices_order);
    EXPECT_EQ(expected_result, id._embedded_vector);
}

TEST_F(TestID, embedded_vector_indices_calc) {
    std::vector<size_t> lower_bounds = {1, 1};
    std::vector<size_t> indices_order = {1, 0};
    std::vector<Pair> expected_result = {{5, 0}, {9, 0}, {10, 0}};
    
    id._embedded_vector_indices_calc(lower_bounds, indices_order);
    EXPECT_EQ(expected_result, id._embedded_vector);
}

TEST_F(TestID, operator_parentheses) {
    std::vector<double> vector_to_embed = {1.25, 7};
    std::vector<double> vector_to_embed_fail = {1.25, 7, 10};

    std::vector<Pair> expected_result = {{14, 0.5}, {15, 0.25}, {19, 0.25}};
    std::vector<Pair> actual_result = id(vector_to_embed);
    
    EXPECT_EQ(expected_result, actual_result);
    
    ASSERT_DEATH(id(vector_to_embed_fail), "");
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
