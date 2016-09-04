#include "gtest/gtest.h"
#include "../IDpair.hpp"

class TestIDpair : public ::testing::Test {
    protected:
        std::vector<double> dim1 = {-1, 0, 0.5, 1, 2};
        std::vector<double> dim2 = {2, 3, 4, 10};
        std::vector<std::vector<double> > dis_points = {dim1, dim2};
        Grid grid;
        Grid double_grid;
        IDpair id;
        
	TestIDpair() :   grid(dis_points), double_grid(grid), id(grid) {
            double_grid.double_grid();
        };
};

TEST_F(TestIDpair, ctor) {
    EXPECT_EQ(double_grid, id._discrete_points_for_all_dims);
    
    EXPECT_EQ(double_grid.get_num_of_dims() + 1, id._embedded_vector.size());
}

TEST_F(TestIDpair, operator_parentheses) {
    std::vector<double> vector_to_cmp1 = {1.25, 7};
    std::vector<double> vector_to_cmp2 = {1.5, 1};
    std::vector<double> vector_to_embed_fail = {1.25, 7, 10};

    std::vector<Pair> expected_result = {{292, 0.5}, {296, 0}, {316, 0.25}, {396, 0.25}, {397, 0}};
    std::vector<Pair> actual_result = id(vector_to_cmp1, vector_to_cmp2);
    
    EXPECT_EQ(expected_result, actual_result);
    
    ASSERT_DEATH(id(vector_to_embed_fail, vector_to_cmp2), "");
    
    ASSERT_DEATH(id(vector_to_cmp1, vector_to_embed_fail), "");
}

TEST_F(TestIDpair, embed) {
    std::vector<double> vector_to_cmp1 = {1.25, 7};
    std::vector<double> vector_to_cmp2 = {1.5, 1};
    std::vector<double> vector_to_embed_fail = {1.25, 7, 10};

    std::vector<Pair> expected_result = {{292, 0.5}, {296, 0}, {316, 0.25}, {396, 0.25}, {397, 0}};
    std::vector<Pair> actual_result = id(vector_to_cmp1, vector_to_cmp2);
    
    EXPECT_EQ(expected_result, actual_result);
    
    ASSERT_DEATH(id(vector_to_embed_fail, vector_to_cmp2), "");
    
    ASSERT_DEATH(id(vector_to_cmp1, vector_to_embed_fail), "");
}

TEST_F(TestIDpair, less) {
    std::vector<double> v_a = {0.3, 0.5, 0.1};
    std::vector<double> v_b = {0.3, 0.1, 0.1};
    std::vector<double> v_c = {0.9, 0.5, 0.1};
    
    EXPECT_TRUE(id._less(v_a, v_c));
    
    EXPECT_FALSE(id._less(v_c, v_a));
    
    EXPECT_FALSE(id._less(v_a, v_b));
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
