//
// Created by Ariel Shadkhan on 13/04/2016.
//

#include <iostream>
#include <armadillo>
#include "Learning.hpp"

using namespace std;
using namespace arma;
typedef std::vector< std::vector<double> > stdvecvec;
typedef std::vector<double> stdvec;

//get filename return matrix of data
mat load_file_to_matrix(string filename){
    mat data;
    data.load(filename,csv_ascii);
    return data;
}

//get matrix of data and number of chunks
//return matrix of discrit point for each dim.
mat find_discrit_points(const mat & data, const size_t & number_of_chunks){
    mat disc_point(number_of_chunks, data.n_cols, fill::zeros);

    int chunk_size = data.n_rows/(number_of_chunks-1); //-1 becuase for to get K parts we need to do K-1 "Cuts"


    for (int i = 0; i < data.n_cols; ++i) {
        //extract vector and sort it.
        vec z = sort(data.col(i));
        //ignore last step .
        for (int j = 0; j < number_of_chunks-1; ++j) {
            disc_point.col(i)[j]= z[j*chunk_size];
        }

        disc_point.col(i)[number_of_chunks-1]= z[data.n_rows-1];

    }

    return disc_point;
}


std::vector< std::vector<size_t> > create_combination_2(size_t num_rows){
    //generate vector
    std::vector<size_t> indexes(num_rows);
    //fill with indexes

    std::vector< std::vector<size_t> > _combinations;

    for (size_t j = 0; j < num_rows; ++j) {
        std::vector<size_t> _tempVector{j, j+num_rows};

        _combinations.push_back(_tempVector);
    }

    return _combinations;
}


void c_vec_intialization(std::vector<double> &c_vec) {
	    for (int n = -15; n <= 1; n += 2) 
	    	c_vec.push_back(pow(2, n));
}

int main() {
/*
    vec A = randi<vec>(100, distr_param(0, 100));
    vec B = randi<vec>(100, distr_param(0, 200));
    vector<vector<double> > pairs;
    for (size_t j = 0; j < A.size(); ++j) {
        vector<double> p1 = {A(j), B(j)};
        pairs.push_back(p1);
    }

    cout << pairs.size() << " " << pairs[0].size() << endl;


*/

    Learning l;

    //read original data to matrix
    mat data = load_file_to_matrix("./colors_v1.csv");


    mat x1(data.n_rows, 3);

    for (int i = 0; i < 3; ++i) {
        x1.col(i) = data.col(i);
    }


    mat x2(data.n_rows, 3);

    for (int i = 5; i >= 3; --i) {
        x2.col(i-3) = data.col(i);
    }

    mat y_tag(data.n_rows, 1);
    y_tag.col(0) = data.col(6);

    mat data_for_disc(2*data.n_rows, 3);

    for (int j = 0; j < x1.n_rows; ++j) {
        data_for_disc.row(j) = x1.row(j);
    }


    for (int j = x2.n_rows; j < 2*(x2.n_rows); ++j) {
        data_for_disc.row(j) = x2.row(j-x2.n_rows);
    }


    //find discrit points
    mat dis_points = find_discrit_points(data_for_disc,5);

    //generate pairs
    std::vector< std::vector<size_t> > pairs = create_combination_2(data.n_rows);

    int counter_minus = 0;
    int counter_plus = 0;

    std::vector<short> tags(pairs.size());
    for (int i = 0; i < tags.size(); ++i) {
        short label = y_tag[i] ==1 ? -1 : 1;
        tags[i] = label;

        if(label == -1){
            counter_minus++;
        }else{
            counter_plus++;
        }
    }

    cout << "ratio plus: " << (double)counter_plus/(double)pairs.size() << endl;
    cout << "ratio minus: " << (double)counter_minus/(double)pairs.size() << endl;





    double thold = 0;
    // double C = pow(10,-7);
    std::vector<double> C;
    c_vec_intialization(C);
    
    
    stdvecvec dis_points2= l.mat_to_std_vec_2(dis_points);

    int dis_size = dis_points2.size();
    for (int k = 0; k < dis_size; ++k) {
        dis_points2.emplace_back(dis_points2[k]);
    }


    stdvecvec examples= l.mat_to_std_vec(data_for_disc);


    Grid grid_pair(dis_points2);
    IDpair id_pair(grid_pair);

    for (int i = 0; i < C.size(); i++) {
        
        std::vector<double> W = l.init(examples, pairs, tags, dis_points2, C[i], thold);
    
        int counter = 0;
        for (size_t i = 0; i < pairs.size(); i++) {
            std::vector<Pair> vol = id_pair( examples[ pairs[i][0] ], examples[ pairs[i][1] ] );
            short s = l.classification(W, vol, thold);
    
            if(s==tags[i])
                counter++;
        }
        
        
        cout << "C[" << i << "] = " << C[i] << " success precent: " << (double)counter/pairs.size() << endl;
    }
    
    
    
    
    return  0;
}