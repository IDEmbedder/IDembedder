#include "gtest/gtest.h"
#include "../GridsOfGroups.hpp"

class TestGridsOfGroups : public ::testing::Test {
    protected:
        std::vector<double> dim1 = {-1, 0, 0.5, 1, 2};
        std::vector<double> dim2 = {2, 3, 4, 10};
        std::vector<std::vector<double> > dis_points = {dim1, dim2};
        std::vector<std::vector<size_t> > groups = {{0}, {1}, {0, 1}};
        GridsOfGroups grids;
        
        TestGridsOfGroups() : grids(dis_points, groups) {}
};

TEST_F(TestGridsOfGroups, ctor) {
    Grid g1(std::vector<std::vector<double> >(1, dim1));
    Grid g2(std::vector<std::vector<double> >(1, dim2));
    Grid g3(dis_points);
    std::vector<Grid> temp_grids;
    
    temp_grids.push_back(g1);
    temp_grids.push_back(g2);
    temp_grids.push_back(g3);
    
    EXPECT_EQ(temp_grids, grids._grids_of_groups);
    temp_grids.push_back(g3);
    
    EXPECT_NE(temp_grids, grids._grids_of_groups);
    
    EXPECT_EQ(dis_points.size(), grids._max_number_of_dimensions);
}

TEST_F(TestGridsOfGroups, get_vertex) {
    std::vector<double> vertex_expected = {0.5, 10};
    std::vector<double> vertex_actual(2);
    std::vector<double> vertex_not_equal = {0.5, 11};
    size_t index_of_vertex = 11;
    size_t index_of_vertex_fail = 30;
    
    grids.get_vertex(2, index_of_vertex, vertex_actual);
    EXPECT_EQ(vertex_expected, vertex_actual);
    
    EXPECT_NE(vertex_not_equal, vertex_actual);
    
    ASSERT_DEATH(grids.get_vertex(2, index_of_vertex_fail, vertex_actual), "");
}

TEST_F(TestGridsOfGroups, get_num_of_dims) {
    EXPECT_EQ(1, grids.get_num_of_dims(0));
    
    EXPECT_EQ(1, grids.get_num_of_dims(1));
    
    EXPECT_EQ(2, grids.get_num_of_dims(2));
}

TEST_F(TestGridsOfGroups, get_num_of_vertices) {
    EXPECT_EQ(5, grids.get_num_of_vertices(0));
    
    EXPECT_EQ(4, grids.get_num_of_vertices(1));
    
    EXPECT_EQ(20, grids.get_num_of_vertices(2));
}

TEST_F(TestGridsOfGroups, get_number_of_all_vertices) {
    EXPECT_EQ(29, grids.get_number_of_all_vertices());
}

TEST_F(TestGridsOfGroups, get_number_of_groups) {
     EXPECT_EQ(3, grids.get_number_of_groups());
}

TEST_F(TestGridsOfGroups, operator_brackets) {
    Grid g1(std::vector<std::vector<double> >(1, dim1));
    Grid g2(std::vector<std::vector<double> >(1, dim2));
    Grid g3(dis_points);
    
    EXPECT_EQ(grids[0], g1);

    EXPECT_EQ(grids[1], g2);

    EXPECT_EQ(grids[2], g3);
}

TEST_F(TestGridsOfGroups, get_max_number_of_dimensions) {
    EXPECT_EQ(dis_points.size(), grids.get_max_number_of_dimensions());
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
