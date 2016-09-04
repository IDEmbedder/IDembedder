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

void testClassificationPairs() {
    const size_t numOfSamples = 2000;
    std::vector<double> gridForX1 = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::vector<double> gridForX2 = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200};
   
    std::vector<std::vector<double> > descrete_points = {gridForX1, gridForX2};
    std::vector<std::vector<double> > examples;
    
    arma_rng::set_seed_random();

    vec A = randi<vec>(numOfSamples/2, distr_param(0, 100));
    vec B = randi<vec>(numOfSamples/2, distr_param(0, 200));
    
    for (size_t i = 0; i < A.size(); ++i) {
        vector<double> p1 = {A(i), B(i)};
        examples.push_back(p1);
    }
    
    A = randi<vec>(numOfSamples/2, distr_param(0, 100));
    B = randi<vec>(numOfSamples/2, distr_param(0, 200));
    
    for (size_t i = 0; i < B.size(); ++i) {
        vector<double> p1 = {A(i), B(i)};
        examples.push_back(p1);
    }
    
/*    int temp = 0;
    for (auto i : examples) {
        std::cout << temp++ << " : ";
        for (auto j : i) {
            std::cout << j << ", ";
        }
        std::cout << std::endl;
    }cout<<"\nexamples.size():"<<examples.size() <<endl;*/
    
    
    std::vector<std::vector <size_t> > indecies_of_pairs(numOfSamples/2);

    for (size_t i = 0; i < numOfSamples/2; i++) {
        vector<size_t> p1 = {i, i + numOfSamples/2};
        indecies_of_pairs[i] = p1;
    }
    
    /*int*/ /*temp = 0;
    for (auto i : indecies_of_pairs) {
        std::cout << temp++ << " : ";
            for (auto j : i) 
                std::cout << j << ", ";
        std::cout << std::endl;
    }cout<<"\nindecies_of_pairs.size():"<<indecies_of_pairs.size() <<endl;*/    
    
    vector<short> tags(numOfSamples/2,0);
    // generate tags.
    int counter = 0;
    for (size_t i = 0; i < indecies_of_pairs.size(); i++) {
        // cout<< "examples[" << i << "]: " << examples[indecies_of_pairs[i][0]][0] << " examples[" << i << "]: " << examples[indecies_of_pairs[i][1]][0] << " ";
        double dist = L1DistanceScalar( examples[indecies_of_pairs[i][0]][0], examples[indecies_of_pairs[i][1]][0] );

        tags[i] = dist < 90 ? -1 : 1;
        // cout<< "tag: " << tags[i] << endl;
        if(tags[i] < 0) counter++;
        
    }
    // cout<<"counter: "<< counter << endl;
    
    Learning l;
    double C = 0.0000000001;
    double thold = 20;
    // std::vector<std::vector<double> > gridpair(descrete_points);
    // gridpair.insert(gridpair.end(), descrete_points.begin(), descrete_points.end());
    Grid grid(descrete_points);
    IDpair id_pair(grid);
    size_t numOfErrors = 0;
    std::vector<double> W = l.init(examples, indecies_of_pairs, tags, descrete_points, C, thold);
    
    for (size_t i = 0; i < indecies_of_pairs.size(); i++) {
        std::vector<Pair> vol = id_pair( examples[ indecies_of_pairs[i][0] ], examples[ indecies_of_pairs[i][1] ] );
        short s = l.classification(W, vol, thold);
        if(s != tags[i]) numOfErrors++;
        // cout<< s << " , "  << tags[i] << endl; 
    }
    
    cout << "num of errors: " << numOfErrors <<endl;
    cout << "errors precent: " << (double)numOfErrors/tags.size() <<endl; /*success*/
    cout << "thold: " << thold << endl;
    
    // cout<< "W.size(): "<< W.size() << " \nW:" << endl;
    // for (auto i : W) {
    //     cout << i << " , ";
    // }
    // cout<< endl;
}

void sanityTest1(){
    std::vector<std::vector<double> > dis_points = {{0, 2, 3, 4, 5, 6}, {0, 2, 3, 4, 5, 6}};

    std::vector<std::vector<double> > for_grid_pair(dis_points);
    // for_grid_pair.insert(for_grid_pair.end(), dis_points.begin(), dis_points.end());
    
    Grid gp(for_grid_pair);
    IDpair idp(gp);
    gp.double_grid();
    
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
    
    std::vector<std::vector<double> > grid = {{0, 1, 2}, {0, 1, 2}};
    std::vector<std::vector<double> > gridpair(grid);
    gridpair.insert(gridpair.end(), grid.begin(), grid.end());
    
    std::vector<double> vec1 = {0, 0};
    std::vector<double> vec2 = {2, 0};
    std::vector<double> vec3 = {0, 2};
    std::vector<double> vec4 = {1, 1};
    
    std::vector<double> tags = {2, 1, 1, 2, 2, 1};

    tags.push_back(L1Distance(vec1, vec2));
    tags.push_back(L1Distance(vec1, vec3));
    tags.push_back(L1Distance(vec1, vec4));
    tags.push_back(L1Distance(vec2, vec3));
    tags.push_back(L1Distance(vec2, vec4));
    tags.push_back(L1Distance(vec3, vec4));
    
    for (auto i : tags) {
        std::cout << i << " , ";
    }cout<<endl;
    
    /*GridGroup gridGroup(gridpair, indecies, true);
    IDgroupBase idGroup(gridGroup, indecies);
    
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