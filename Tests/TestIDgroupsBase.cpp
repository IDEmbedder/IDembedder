#include "gtest/gtest.h"
#include "../IDgroupsBase.hpp"

class TestIDgroupsBase : public ::testing::Test {
    protected:
        std::vector<std::vector<size_t> > groups = {{0}, {1}, {0, 1}};
        IDgroupsBase id_group;
        TestIDgroupsBase() : id_group(groups, 4, 2) {} 
};

TEST_F(TestIDgroupsBase, ctor) {
    EXPECT_EQ(groups, id_group._indices_of_groups);
    
    EXPECT_EQ(4, id_group._embedded_vector_groups.size());
    
    EXPECT_EQ(2, id_group._max_number_of_dimensions);
}

TEST_F(TestIDgroupsBase, num_of_groups) {
    EXPECT_EQ(groups.size(), id_group._num_of_groups());
}

TEST_F(TestIDgroupsBase, init_temp_vector_to_embed) {
    size_t group_index1 = 1;
    size_t group_index2 = 2;
    std::vector<double> vector_to_embed = {0.5, 0.1};
    std::vector<double> actual;
    
    std::vector<double> expected1 = {0.1};
    std::vector<double> expected2 = {0.5, 0.1};
    
    id_group._init_temp_vector_to_embed(group_index1, vector_to_embed, actual);
    EXPECT_EQ(expected1, actual);
    
    id_group._init_temp_vector_to_embed(group_index2, vector_to_embed, actual);
    EXPECT_EQ(expected2, actual);
}

TEST_F(TestIDgroupsBase, copy_part_results) {
    std::vector<Pair> expected = {{0, 0}, {31, 0.5}, {33, 0.1}, {0, 0}};
    std::vector<Pair> partial = {{1, 0.5}, {3, 0.1}};
    size_t next_place_index = 1;
    size_t shift_index = 30;
    
    id_group._copy_part_results(partial, shift_index, next_place_index);
    EXPECT_EQ(expected, id_group._embedded_vector_groups);
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
