#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cassert>
#include <exception>
#include <time.h>
#include "armadillo"
#include "Learning.hpp"
#include "/home/ubuntu/workspace/New/GridsOfGroups.hpp"
#include <ctime>

using namespace std;
using namespace arma;

//g++ -g -std=c++11 euclideanDis.cpp -o T

/*
    called from construct_Wreg
    compute L1 distance on vertexes (|p2|-|p1|)
    */
double L1Distance(const std::vector <double>& p1, const std::vector <double>& p2){
    assert(p1.size() == p2.size());
    size_t num_of_points = p1.size();
    double dist = 0;

    for (size_t i = 0; i < num_of_points; i++) {
        dist += fabs( p2[i] - p1[i] );  //include math.h
    }
    
    return dist;
}
    
/*
    called from construct_Wreg
    compute L1 distance on vertexes (|p2|-|p1|)
    */
double L1DistanceScalar(double p1, double p2){
    return fabs(p1-p2);
}
    
double L2Distance(const std::vector <double>& p1, const std::vector <double>& p2){
        assert(p1.size() == p2.size());
        size_t num_of_points = p1.size();
        double dist = 0;
        std::vector <double> difference(p1.size(), 0);
        
        for (size_t i = 0; i < num_of_points; i++) {
            double x = p2[i] - p1[i];
            difference[i] = pow(x, 2);
            dist += difference[i];        
        }
    
        dist = sqrt(dist);
        return dist;
}

//c_vec_intialization -	initialize vector c 
void c_vec_intialization(vector<double> &c_vec) {
	for (int n = -15; n <= 5; n += 2) 
		c_vec.push_back(pow(2, n));
}

void testClassificationPairs() {
    const size_t numOfSamples = 250000;
    std::vector<double> gridForX1 = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::vector<double> gridForX2 = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200};
   
    std::vector<std::vector<double> > descrete_points = {gridForX1, gridForX2};
    std::vector<std::vector<double> > examples;
    
    arma_rng::set_seed_random();
    
    // loop for creating pairs
    for (int i = 0; i < 2; i++) {
        vec A = randi<vec>(numOfSamples/2, distr_param(0, 100));
        vec B = randi<vec>(numOfSamples/2, distr_param(0, 200));
        
        for (size_t j = 0; j < A.size(); j++) {
            vector<double> p1 = {A(j), B(j)};
            examples.push_back(p1);
        }    
    }
    
    std::vector<std::vector <size_t> > indecies_of_pairs;
    
    for (size_t i = 0; i < numOfSamples/2; i++) {
        vector<size_t> p1 = {i, i + numOfSamples/2};
        indecies_of_pairs.push_back(p1);
    }
    
    vector<short> tags(numOfSamples/2,0);
    // generate tags.
    int counter = 0;
    for (size_t i = 0; i < indecies_of_pairs.size(); i++) {
        // cout<< "examples[" << i << "]: " << examples[indecies_of_pairs[i][0]][0] << " examples[" << i << "]: " << examples[indecies_of_pairs[i][1]][0] << " ";
        double dist = L1DistanceScalar( examples[indecies_of_pairs[i][0]][0], examples[indecies_of_pairs[i][1]][0] );

        tags[i] = dist < 20 ? -1 : 1;
        // cout<< "tag: " << tags[i] << endl;
        if(tags[i] < 0) counter++;
        
    }
    cout<<"num of good examples: "<< counter << " out of: " << numOfSamples/2 << " examples" << endl;
    
    Learning l;
    
    vector<double> c_vec;
    c_vec_intialization(c_vec);
    // double C = pow(10, -4);
    // std::cout << C << std::endl;
    double thold = 1;
    
    std::vector<std::vector<double> > gridpair(descrete_points);
    gridpair.insert(gridpair.end(), descrete_points.begin(), descrete_points.end());
    
    Grid grid(gridpair);
    IDpair id_pair(grid);
    // std::vector<double> Wreg = l.construct_Wreg(grid);
    
    time_t tstart, tend; 
    
    // size_t temp = 0;
    for (size_t j = 0; j < c_vec.size() ; j++) {
        tstart = time(0);
        size_t numOfErrors = 0;
        // std::cout << temp << std::endl;
        std::vector<double> W = l.init(examples, indecies_of_pairs, tags, gridpair, c_vec[j], thold);
        
        for (size_t i = 0; i < indecies_of_pairs.size(); i++) {
            std::vector<Pair> vol = id_pair( examples[ indecies_of_pairs[i][0] ], examples[ indecies_of_pairs[i][1] ] );
            short s = l.classification(W, vol, thold);
            
            // cout<< s << " , "  << tags[i] << endl; 
            
            if(s != tags[i]) numOfErrors++;
        }
        
        tend = time(0); 
        cout << "c_vec[j]:" << c_vec[j] << endl;
        cout << "num of errors: " << numOfErrors <<endl;
        cout << "error precent: " << (double)numOfErrors/tags.size() <<endl;
        cout << "thold: " << thold << ".\n" << endl;
        cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;
        
        // cout<< "W.size(): "<< W.size() << " \nW:" << endl;
        // for (auto i : W) {
        //     cout << i << " , ";
        // }
        // cout<< endl;
    }
}

void sanityTest1(){
    std::vector<std::vector<double> > dis_points = {{0, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 6}};

    std::vector<std::vector<double> > for_grid_pair(dis_points);
    for_grid_pair.insert(for_grid_pair.end(), dis_points.begin(), dis_points.end());
    
    Grid gp(for_grid_pair);
    IDpair idp(gp);
    
    std::vector<double> vec1 = {1.5, 3.5};
    std::vector<double> vec2 = {4, 2};
    
    std::vector<Pair> result = idp(vec1, vec2);
    
    Learning l2;
    
    std::vector<double> w = l2.construct_Wreg(gp);
    
    double dotProd = 0;
    
    for (size_t i = 0; i < result.size() ; i++)
        dotProd += result[i]._weight * w[ result[i]._index ];

    std::cout << "dot: " << dotProd << std::endl;
}

void sanityTest2(){
    // implement sanity tests (metric but not eaclidean), page 26 on ID document.
    
    std::vector<std::vector<double> > grid = {{-1, 0.5, 3.0}, {-1, 0.5, 3.0}};
    std::vector<std::vector<double> > discrete_points(grid);
    discrete_points.insert(discrete_points.end(), grid.begin(), grid.end());
    
    std::vector<double> vec1 = {0, 0};
    std::vector<double> vec2 = {2, 0};
    std::vector<double> vec3 = {1, 1};
    std::vector<double> vec4 = {1, 2};
    
    std::vector<std::vector<double> > examples = { vec1, vec2, vec3, vec4 };
    std::vector<std::vector<size_t> > indecies_of_pairs = { {0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3} };
    // std::vector<double> tags = {2, 1, 2, 1, 2, 1};
    
    std::vector<double> tags;
    for (size_t i = 0; i < indecies_of_pairs.size(); i++)
        tags.push_back( L1Distance( examples[ indecies_of_pairs[i][0] ], examples[ indecies_of_pairs[i][1] ] ) );
        
    for (auto i : tags) {
        std::cout << i << " , ";
    }cout<<endl;
    
    Grid grid_pair(discrete_points);
    IDpair id_pair(grid_pair);
    
    Learning l;
    std::vector<double> C;
    c_vec_intialization(C);
    
    for (int i = 0; i < C.size(); i++) {
        
        // std::vector<double> W = l.init_Distance(examples, indecies_of_pairs, tags, discrete_points, C[i]);
    std::vector<double> W;
        int counter = 0;
        for (size_t i = 0; i < indecies_of_pairs.size(); i++) {
            std::vector<Pair> vol = id_pair( examples[ indecies_of_pairs[i][0] ], examples[ indecies_of_pairs[i][1] ] );
            
            double dotProd = 0;
            for (size_t i = 0; i < vol.size() ; i++){
                dotProd += vol[i]._weight * W[ vol[i]._index ];
            
                std::cout << vol[i]._weight << " " << W[ vol[i]._index ] << std::endl;
            }
            std::cout << dotProd << std::endl;
            // short s = l.classification(W, vol, 0);
    
            // if(s==tags[i])
            //     counter++;
        }
        
        // cout << "C[" << i << "] = " << C[i] << " success precent: " << (double)counter/indecies_of_pairs.size() << endl;
        
        cout << "W[" << i << "] = " << endl;
        for (auto& obj : W) {
            std::cout << obj << " ";
        } std::cout << std::endl;
    }

    /*GridGroup gridGroup(gridpair, indecies, true);
    //IDgroupBase idGroup(gridGroup, indecies);
    std::vector<Pair> result1 = idGroup(vec1, vec2);
    std::vector<Pair> result2 = idGroup(vec1, vec3);
    std::vector<Pair> result3 = idGroup(vec1, vec4);
    std::vector<Pair> result4 = idGroup(vec2, vec3);
    std::vector<Pair> result5 = idGroup(vec2, vec4);
    std::vector<Pair> result6 = idGroup(vec3, vec4);*/
}

int main(){
    
    testClassificationPairs();
    
    // sanityTest1();

    // sanityTest2();
    
    return 0;
}