#include "gtest/gtest.h" // google test
#include <cassert>
using namespace std;

#include "ID.hpp"

#include <limits>
#include <cmath>

//TODO test: _sort_vectors , operator() ,struct SortedVectors

//compile: g++ -g -std=c++11 -o T3 gtest.cpp ID.cpp /usr/lib/libgtest.a /usr/lib/libgtest_main.a -lpthread
//debug: g++ -D_GLIBCXX_DEBUG ..

//test inital values in struct pair
TEST(structPair, initial_values){
	Pair p;
	EXPECT_EQ(0, p._index);
	EXPECT_EQ(0., p._weight);
	// cout<<p;
}

//test inital values in struct bounds
TEST(structBounds, initial_values){
	Bounds b;
	EXPECT_EQ(0, b._low);
	EXPECT_EQ(0, b._up);
}

/*TEST(structSortedVectors, initial_values){
	SortedVectors s;
	EXPECT_EQ(0, s._sorted_norms_of_obj_features);
	EXPECT_EQ(0, s);
}*/

// create test object for gTest 2d
class IDTest : public ::testing::Test {
protected:
	// 
	vector<double> x , v0 , v1;
	vector< vector<double> > v;

virtual void SetUp() { 
	x =  { 0.25, 20.0 };
	v0 = { 0.0, 1.0 };
	v1 = { 10.0, 30.0, 50.0 };
	v = vector< vector<double> >(x.size());
	v[0] = vector<double>(v0);
	v[1] = vector<double>(v1);
}
	//virtual void TearDown() {}
};

// TODO fix lambda calc
TEST_F(IDTest, _lambdas_calc){
	// generate ID using virtual data
	ID id(v);
	id(x);
	EXPECT_EQ(id._non_zero_indices_in_grid[0]._index, 0);
	EXPECT_EQ(id._non_zero_indices_in_grid[0]._weight, 0.5);
	
	EXPECT_EQ(id._non_zero_indices_in_grid[1]._index, 1);
	EXPECT_EQ(id._non_zero_indices_in_grid[1]._weight, 0.25);
	
	EXPECT_EQ(id._non_zero_indices_in_grid[2]._index, 4);
	EXPECT_EQ(id._non_zero_indices_in_grid[2]._weight, 0.25);
}


TEST_F(IDTest, ctor_ID_2d){
	ID id(v);
	
	EXPECT_EQ(id._num_of_features, 2);
	EXPECT_EQ(id._discrete_points_for_all_dim, v);
	
	std::vector<Pair> non_zero = std::vector<Pair>(3);
	ASSERT_EQ(id._non_zero_indices_in_grid.size(), non_zero.size());
	for (int i = 0; i < non_zero.size() ; i++){ 
		EXPECT_EQ(id._non_zero_indices_in_grid[i]._index, non_zero[i]._index);
		EXPECT_EQ(id._non_zero_indices_in_grid[i]._weight, non_zero[i]._weight);
	}
	
	std::vector<size_t> prod_sizes = {3,1};
	ASSERT_EQ(id._prod_sizes.size(), prod_sizes.size());
	for (int i = 0; i < prod_sizes.size() ; i++) 
		EXPECT_EQ(id._prod_sizes[i], prod_sizes[i]);
}

TEST_F(IDTest , _find_bounds){
    double x0 = 0.25, x1 = 7, x2 = -2.3, x3 = 0.0, x4 = 1.0;
	ID id(v);
	
	//x0 is between 
	Bounds b = id._find_bounds(v0, x0);
	EXPECT_EQ(b._low , 0);
	EXPECT_EQ(b._up , 1);

	//x3 is between - equals to v0[0]
	b = id._find_bounds(v0, x3);
	EXPECT_EQ(b._low , 0);	
	EXPECT_EQ(b._up , 1);

	//x4 is between - equals to v0[1]
	b = id._find_bounds(v0, x4);
	EXPECT_EQ(b._low , 0);
	EXPECT_EQ(b._up , 1);
}

TEST_F(IDTest , _find_bounds_3d){
    double x0 = 10, x1 = 30, x2 = 50, x3 = 12, x4 = 48;
	ID id(v);
	
	Bounds b = id._find_bounds(v1, x3);
	EXPECT_EQ(b._low , 0);
	EXPECT_EQ(b._up , 1);

	b = id._find_bounds(v1, x0);
	EXPECT_EQ(b._low , 0);	
	EXPECT_EQ(b._up , 1);

	b = id._find_bounds(v1, x1);
	EXPECT_EQ(b._low , 1);
	EXPECT_EQ(b._up , 2);
	
	b = id._find_bounds(v1, x2);
	EXPECT_EQ(b._low , 1);
	EXPECT_EQ(b._up , 2);
	
	b = id._find_bounds(v1, x4);
	EXPECT_EQ(b._low , 1);
	EXPECT_EQ(b._up , 2);
}

TEST_F(IDTest , Prod_Size){
    ID id(v);
	ASSERT_EQ(id._prod_sizes[0], 3);
	ASSERT_EQ(id._prod_sizes[1], 1);
	
	v.clear();
	v = vector< vector<double> >(3);
	v[0] = vector<double>(2, 0.0);
	v[1] = vector<double>(3, 1.0);
	v[2] = vector<double>(4, 2.0);
	ID id2(v);
	
	ASSERT_EQ(id2._prod_sizes[0], 12);
	ASSERT_EQ(id2._prod_sizes[1], 4);
	ASSERT_EQ(id2._prod_sizes[2], 1);
	
	v.clear();
	v = vector< vector<double> >(4);
	v[0] = vector<double>(2, 0.0);
	v[1] = vector<double>(3, 1.0);
	v[2] = vector<double>(4, 2.0);
	v[3] = vector<double>(5, 3.0);
	ID id3(v);
	
	ASSERT_EQ(id3._prod_sizes[0], 60);
	ASSERT_EQ(id3._prod_sizes[1], 20);
	ASSERT_EQ(id3._prod_sizes[2], 5);
	ASSERT_EQ(id3._prod_sizes[3], 1);
}

// check norm for 2d grid
TEST_F(IDTest , Calc_Norm_in_bounds){
	double low,up,val,norm,ans;
	ID id(v);
	
	low = v0[0];  
	up = v0[1];
	val = 0.1;
	norm = id._norm_calc(low, up, val);
	ans = 0.1;
	ASSERT_EQ(ans,norm);			

	low = v1[0];
	up = v1[1];
	val = 20;
	norm = id._norm_calc(low, up, val);
	ans = 0.5;
	ASSERT_EQ(ans,norm);
}

TEST_F(IDTest , Calc_Norm_left_bound){
	double low,up,val,norm,ans;
	ID id(v);
	
	low = v0[0];
	up = v0[1];
	val = -1.1;
	norm = id._norm_calc(low, up, val);
	ans = 0;
	ASSERT_EQ(ans,norm);			
	
	low = v1[1];
	up = v1[2];
	val = 20;
	norm = id._norm_calc(low, up, val);
	ans = 0;
	ASSERT_EQ(ans,norm);
}

TEST_F(IDTest , Calc_Norm_right_bound){
	double low,up,val,norm,ans;
	ID id(v);
	
	low = v0[0];
	up = v0[1];
	val = 1.1;
	norm = id._norm_calc(low, up, val);
	ans = 1;
	ASSERT_DEATH(ans==norm, "expected");			
	
	low = v1[1];
	up = v1[2];
	val = 100;
	norm = id._norm_calc(low, up, val);
	ans = 1;
	ASSERT_FALSE(ans==norm); 	
}

//The art of computer programming by Knuth
bool approximatelyEqual(double a, double b){
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());	}

bool essentiallyEqual(double a, double b){
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());	}

bool definitelyGreaterThan(double a, double b){
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());	}

bool definitelyLessThan(double a, double b){
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());	}


// check with ofir proximity floating-point numbers line 267	***************************************************************************************************************************************************************
TEST_F(IDTest , _index_calc){
	ID id(v);
	
	vector<Pair> result = id(x);
	double expected_w0 = 0.5, expected_w1 = 0.25, expected_w2 = 0.25;

//test volume , indecies of buttom triangle 
	EXPECT_EQ( result[0]._index , 	0);
	EXPECT_TRUE ( approximatelyEqual(result[0]._weight, expected_w0) 	);
	EXPECT_TRUE ( essentiallyEqual(result[0]._weight, expected_w0)	 	);
	EXPECT_FALSE( definitelyGreaterThan(result[0]._weight, expected_w0) );
	EXPECT_FALSE( definitelyLessThan(result[0]._weight, expected_w0) 	);

	EXPECT_EQ( result[1]._index , 	1);
	EXPECT_TRUE ( approximatelyEqual(result[1]._weight, expected_w1) 	);
	EXPECT_TRUE ( essentiallyEqual(result[1]._weight, expected_w1)	 	);
	EXPECT_FALSE( definitelyGreaterThan(result[1]._weight, expected_w1) );
	EXPECT_FALSE( definitelyLessThan(result[1]._weight, expected_w1) 	);

	EXPECT_EQ( result[2]._index , 	4);
	EXPECT_TRUE ( approximatelyEqual(result[2]._weight, expected_w2) 	);
	EXPECT_TRUE ( essentiallyEqual(result[2]._weight, expected_w2)	 	);
	EXPECT_FALSE( definitelyGreaterThan(result[2]._weight, expected_w2) );
	EXPECT_FALSE( definitelyLessThan(result[2]._weight, expected_w2) 	);
}

TEST_F(IDTest , test_sograim_operator){
	ID id(v);
	vector<Pair> result = id(x);
	double expected_w0 = 0.5, expected_w1 = 0.25, expected_w2 = 0.25;
	
	EXPECT_EQ(result[0]._index, 0);
	EXPECT_EQ(result[0]._weight, 0.5);
	
	EXPECT_EQ(result[1]._index, 1);
	EXPECT_EQ(result[1]._weight, 0.25);
	
	EXPECT_EQ(result[2]._index, 4);
	EXPECT_EQ(result[2]._weight, 0.25);
}

/*
TEST_F(IDTest , Huge_Prod_Sizes){
    v.clear();
	
	int size = 100;
	v = vector< vector<double> >(size);
	double mult=1, mid=1, p35=1, p97=1;
	for (int i = 0; i < v.size(); i++)
		v[i] = vector<double>(2, i);		//{0}{1,1}{2,2,2}...{99,99,...,99}
	
	for (int i = v.size(); i > 0; i--){
		mult *= i;
		if(i>=52) mid *= i;
		if(i>=37) p35 *= i;
		if(i>=99) p97 *= i;
	}
	ID id(v);
	
	EXPECT_EQ(id._prod_sizes[0], mult);
	EXPECT_EQ(id._prod_sizes[98], size);
	EXPECT_EQ(id._prod_sizes[size/2], mid);
	EXPECT_EQ(id._prod_sizes[35], p35);
	EXPECT_EQ(id._prod_sizes[97], p97);
	EXPECT_EQ(id._prod_sizes[size-1], 1);

	for (int i = 0; i < id._prod_sizes.size(); i++){
        cout << "id._prod_sizes[" << i << "] : "<< id._prod_sizes[i]<< endl;
    }	cout<<endl;
    
    	for (int i = 0; i < v.size(); i++){
        cout << "v[" << i << "].size : "<< v[i].size()<< endl;
    }	cout<<endl;
	
	cout<< id._prod_sizes[0]<<endl;
	cout<< mult<<" " <<mid<< " " << p97<< " " << p35<<endl;
	
	cout<< sizeof(size_t)<<endl;
}
*/

/*
// create test object for gTest 3d
// TODO
class IDTest3D : public ::testing::Test {
protected:
	// 
	vector<double> x , v0 , v1;
	vector< vector<double> > v;

virtual void SetUp() { 
	x =  { 0.25, 20.0 };
	v0 = { 0.0, 1.0 };
	v1 = { 10.0, 30.0, 50.0 };
	v = vector< vector<double> >(x.size());
	v[0] = vector<double>(v0);
	v[1] = vector<double>(v1);
}
	//virtual void TearDown() {}
};
*/
/*
TEST(ctor, ID_3d){
	double x1 = 7, x2 = 2, x3 = 2.3;
	vector<double> gridForX1 = {1, 2, 4, 8, 10};
	vector<double> gridForX2 = {3, 4, 5, 6};
	vector<double> gridForX3 = {4, 5, 6, 8, 10, 20, 30};
	vector<double> point = {x1, x2, x3};
	vector<vector<double> > dp = {gridForX1, gridForX2, gridForX3};
	    
	ID i3d(dp);

	EXPECT_EQ(i3d._num_of_features, 3);
	EXPECT_EQ(i3d._discrete_points_for_all_dim, dp);
	
	std::vector<Pair> non_zero = std::vector<Pair>(4);
	ASSERT_EQ(i3d._non_zero_indices_in_grid.size(), non_zero.size());
	for (int i = 0; i < non_zero.size() ; i++){ 
		EXPECT_EQ(i3d._non_zero_indices_in_grid[i]._index, non_zero[i]._index);
		EXPECT_EQ(i3d._non_zero_indices_in_grid[i]._weight, non_zero[i]._weight);
	}
	
	std::vector<size_t> prod_sizes = {28,7,1};
	ASSERT_EQ(i3d._prod_sizes.size(), prod_sizes.size());
	for (int i = 0; i < prod_sizes.size() ; i++) 
		EXPECT_EQ(i3d._prod_sizes[i], prod_sizes[i]);

	for (auto& item : i3d._prod_sizes){
        cout << item << " ";
    }	cout<<endl;
    
}	*/

int main(int argc, char **argv){ 
	::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}

/*
Fatal assertion					Nonfatal assertion					Verifies
ASSERT_TRUE(condition);			EXPECT_TRUE(condition);				condition is true
ASSERT_FALSE(condition);		EXPECT_FALSE(condition);			condition is false
ASSERT_EQ(val1,val2);			EXPECT_EQ(val1,val2);				val1 == val2
ASSERT_NE(val1,val2);			EXPECT_NE(val1,val2);				val1 != val2
ASSERT_LT(val1,val2);			EXPECT_LT(val1,val2);				val1 < val2
ASSERT_LE(val1,val2);			EXPECT_LE(val1,val2);				val1 <= val2
ASSERT_GT(val1,val2);			EXPECT_GT(val1,val2);				val1 > val2
ASSERT_GE(val1,val2);			EXPECT_GE(val1,val2);				val1 >= val2
ASSERT_STREQ(str1,str2);		EXPECT_STREQ(str1,_str_2);			the two C strings have the same content
ASSERT_STRNE(str1,str2);		EXPECT_STRNE(str1,str2);			the two C strings have different content
ASSERT_STRCASEEQ(str1,str2);	EXPECT_STRCASEEQ(str1,str2);		the two C strings have the same content, ignoring case
ASSERT_STRCASENE(str1,str2);	EXPECT_STRCASENE(str1,str2);		the two C strings have different content, ignoring case

True								ASSERT_TRUE(condition)							EXPECT_TRUE(condition)
False								ASSERT_FALSE(condition)							EXPECT_FALSE(condition)
Equal								ASSERT_EQ(arg1,arg2)							EXPECT_EQ(arg1,arg2)
Not Equal							ASSERT_NE(arg1,arg2)							EXPECT_NE(arg1,arg2)
Less Than							ASSERT_LT(arg1,arg2)							EXPECT_LT(arg1,arg2)
Less Than or Equal					ASSERT_LE(arg1,arg2)							EXPECT_LE(arg1,arg2)
Greater Than						ASSERT_GT(arg1,arg2)							EXPECT_GT(arg1,arg2)
Greater Than or Equal				ASSERT_GE(arg1,arg2)							EXPECT_GE(arg1,arg2)
									ASSERT_FLOAT_EQ (expected, actual)				EXPECT_FLOAT_EQ (expected, actual)
									ASSERT_DOUBLE_EQ (expected, actual)				EXPECT_DOUBLE_EQ (expected, actual)
									ASSERT_NEAR (expected, actual, absolute_range)	EXPECT_NEAR (expected, actual, absolute_range)
catch assertion failure				ASSERT_DEATH(statement, expected_message)
									ASSERT_EXIT(statement, predicate, expected_message)
C String Equal						ASSERT_STREQ(str1,str2)							EXPECT_STREQ(str1,str2)
C String Not Equal					ASSERT_STRNE(str1,str2)							EXPECT_STRNE(str1,str2)
C String Case Equal					ASSERT_STRCASEEQ(str1,str2)						EXPECT_STRCASEEQ(str1,str2)
C String Case Not Equal				ASSERT_STRCASENE(str1,str2)						EXPECT_STRCASENE(str1,str2)
Verify that exception is thrown		ASSERT_THROW(statement,exception_type)			EXPECT_THROW(statement,exception_type)
Verify that exception is thrown		ASSERT_ANY_THROW(statement)						EXPECT_ANY_THROW(statement)
Verify that exception is NOT thrown	ASSERT_NO_THROW(statement)						EXPECT_NO_THROW(statement)
*/	