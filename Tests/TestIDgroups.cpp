#include "gtest/gtest.h"
#include "../IDgroups.hpp"

class TestIDgroups : public ::testing::Test {
    protected:
        std::vector<double> dim1 = {-1, 0, 0.5, 1, 2};
        std::vector<double> dim2 = {2, 3, 4, 10};
        std::vector<std::vector<double> > dis_points = {dim1, dim2};
        std::vector<std::vector<size_t> > groups = {{0}, {1}, {0, 1}};
        GridsOfGroups grids;
        IDgroups id_group;
        
        TestIDgroups() : grids(dis_points, groups), id_group(grids, groups) {}
};

TEST_F(TestIDgroups, ctor) {
    std::vector<size_t> number_of_vertices_of_groups_expected = {5, 4, 20};
    std::vector<ID> ids_of_groups_expected;
    ids_of_groups_expected.push_back(grids[0]);
    ids_of_groups_expected.push_back(grids[1]);
    ids_of_groups_expected.push_back(grids[2]);
    
    
    EXPECT_EQ(groups, id_group._indices_of_groups);
    
    EXPECT_EQ(7, id_group._embedded_vector_groups.size());
    
    EXPECT_EQ(number_of_vertices_of_groups_expected, id_group._number_of_vertices_of_groups);
    
    EXPECT_EQ(ids_of_groups_expected, id_group._ids_of_groups);
    
    ids_of_groups_expected[2] = ID(grids[1]);
    EXPECT_NE(ids_of_groups_expected, id_group._ids_of_groups);
}

TEST_F(TestIDgroups, embedded_vector_size_calc) {
    EXPECT_EQ(7 , id_group._embedded_vector_size_calc(groups));
    
    EXPECT_NE(0 , id_group._embedded_vector_size_calc(groups));
}

TEST_F(TestIDgroups, operator_parentheses) {
    std::vector<double> vector_to_embed = {1.25, 7};
    std::vector<double> vector_to_embed_fail = {1.25, 7, 10};

    std::vector<Pair> expected_result = {{3, 0.75}, {4, 0.25}, 
                                         {7, 0.5}, {8, 0.5}, 
                                         {23, 0.5}, {24, 0.25}, {28, 0.25}};
    std::vector<Pair> actual_result = id_group(vector_to_embed);
    
    EXPECT_EQ(expected_result, actual_result);
    
    ASSERT_DEATH(id_group(vector_to_embed_fail), "");
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
