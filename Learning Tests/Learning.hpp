#ifndef _LEARNING_ID
#define _LEARNING_ID

#include "/home/ubuntu/workspace/New/IDpair.hpp"
#include<math.h>
#include "armadillo"

#define ALPHA 1.0
#define EPOCH_TIMES 3
#define NUMBER_OF_ITERATIONS 5

using namespace arma;
using namespace std;
typedef std::vector<double> stdvec;
typedef std::vector< std::vector<double> > stdvecvec;

// g++ -Wall -g -std=c++11 -o T test_Learning.cpp

class Learning{
private:
    bool _if_equal_dist_zero;
    bool _if_non_equal_dist_non_zero;
    const double _NON_EQUAL_EPSILON;
    bool _symmetry;

public:

    Learning() 
        : _if_equal_dist_zero(true)
        , _if_non_equal_dist_non_zero(true)
        , _symmetry(true)
        , _NON_EQUAL_EPSILON(std::numeric_limits<double>::epsilon())
        {}

    // ariel functions start
    //get filename return armadilo matrix
    mat load_file_to_matrix(std::string filename){
        mat data;
        data.load(filename,csv_ascii);
        return data;
    }

    // convert armadillo matrix to std vector of vector by rows
    std::vector< std::vector<double> > mat_to_std_vec(arma::mat &A) {
        std::vector< std::vector<double> > V(A.n_rows);

        for (size_t i = 0; i < A.n_rows; ++i) {
            V[i] = arma::conv_to< std::vector<double> >::from(A.row(i));
        };
        return V;
    }

    // convert armadillo matrix to std vector of vector convert by colmns
    std::vector< std::vector<double> > mat_to_std_vec_2(arma::mat &A) {
        std::vector< std::vector<double> > V(A.n_cols);

        for (size_t i = 0; i < A.n_cols; ++i) {
            V[i] = arma::conv_to< std::vector<double> >::from(A.col(i));
        };
        return V;
    }


    //create matrix of discrete points , each dim to K points (K = number of chunks)
    mat find_discrit_points(const mat & data, const size_t & number_of_chunks){
        mat disc_point(number_of_chunks, data.n_cols, fill::zeros);

        int chunk_size = data.n_rows/(number_of_chunks-1); //-1 becuase for to get K parts we need to do K-1 "Cuts"


        for (size_t i = 0; i < data.n_cols; ++i) {
            //extract vector and sort it.
            vec z = sort(data.col(i));
            //the ignore last step .
            for (size_t j = 0; j < number_of_chunks-1; ++j) {
                disc_point.col(i)[j]= z[j*chunk_size];
            }

            disc_point.col(i)[number_of_chunks-1]= z[data.n_rows-1];

        }
        //  now ittreate over each column , sort it and break into chunks.

        return disc_point;
    }

    // generate n choose k combination of indexes
    std::vector< std::vector<size_t> > create_combination_2(size_t indexes_length){
        //generate vector
        std::vector<size_t> indexes(indexes_length);
        //fill with indexes
        std::iota(indexes.begin(),indexes.end(),0);

        std::vector< std::vector<size_t> > _combinations;

        while(indexes.size() >= 1){

            for (size_t j = 1; j < indexes.size(); ++j) {
                std::vector<size_t> _tempVector{};
                _tempVector.push_back(indexes.at(0));
                _tempVector.push_back(indexes.at(j));
                _combinations.push_back(_tempVector);
            }

            indexes.erase(indexes.begin());

        }

        return _combinations;
    }

    //get the range index of a num
    size_t rangeIndex(size_t num, vec & ranges){
        if(num==0)
            return 0;
        else{
            for (size_t i = 0; i < ranges.size(); ++i) {
                if(ranges[i] > num)
                    return i-1;
            }
        }

        return ranges.size()-1;
    }

    // check if in the same range
    short isSameRange(size_t a, size_t b, vec & ranges){
        if(rangeIndex(a, ranges)==rangeIndex(b, ranges))
            return 1;

        return -1;
    }

    /* ariel functions end
    ********************************************************************************************************************************************
    ********************************************************************************************************************************************
    ********************************************************************************************************************************************
    ********************************************************************************************************************************************
    */

    
/*    void check_step(const std::vector<std::vector <double> >& examples, const std::vector<std::vector <size_t> >& indecies_of_pairs,
                             const std::vector<short>& tags, const std::vector<std::vector <double> >& discrete_points, const double C, double& thold){
        std::vector<double> Wreg;
        Grid grid_pair(discrete_points);
        IDpair idpair(grid_pair);
        Wreg = construct_Wreg(grid_pair);
        size_t W_size = idpair._discrete_points_for_all_dims.get_num_of_vertices();

        std::vector<double> W(W_size, 0);
        assert(Wreg.size() == W.size());
        size_t num_of_pairs = indecies_of_pairs.size();
        
        double sub = L2Distance(W, Wreg);
        sub *= 0.5;
        double dotProd = 0;
        
        for (size_t i = 0; i < num_of_pairs; i++) {
            const std::vector<Pair>& volume = idpair(examples[ indecies_of_pairs[i][0] ], examples[ indecies_of_pairs[i][1] ]);
             for (size_t i = 0; i < volume.size(); i++){
                dotProd += volume[i]._weight * W[ volume[i]._index ];
             }
        }

        
    }*/
    
    std::vector<double> init(const std::vector<std::vector <double> >& examples, const std::vector<std::vector <size_t> >& indecies_of_pairs,
                             const std::vector<short>& tags, const std::vector<std::vector <double> >& discrete_points, const double C, double& thold){
        std::vector<double> Wreg;
        std::vector<double> W;

        cout<< "discrete_points.size(): " << discrete_points.size() << endl;

        // cout << "create grid_pair" << endl;
        Grid grid_pair(discrete_points);

        IDpair id_pair(grid_pair);
        grid_pair.double_grid();
        cout << "create id pair" << endl;

        Wreg = construct_Wreg(grid_pair);
        cout << "create w reg" << endl;
        W = learn_similar(examples, id_pair, indecies_of_pairs, tags, Wreg, C, thold);

        return W;
    }

    /*
    W and Wreg size equal to the number of dimention in the power of the number of descrete points for each dimention .
    examples - is a matrix, each row contains a point in the grid.
    indecies_of_pairs - is a matrix, each row contains two indices related to example matrix, such that each index is a vector of points in the grid.
    tags - is a vector contain the labels for each row in indecies_of_pairs, such that -1(similar) , 1(non-similar).
    Wreg - is a vector of distances, euclidean distance between points (L2 distance).
    IDpair - INTERPOLATED DISCRETIZATION pair-object.
    C - const double
    */
    std::vector<double> learn_similar(const std::vector<std::vector <double> >& examples , const IDpair& idpair , const std::vector<std::vector <size_t> >& indecies_of_pairs ,
                                      const std::vector<short>& tags, const std::vector<double>& Wreg, const double C, double& thold){
        assert(tags.size() == indecies_of_pairs.size());

        size_t W_size = idpair.get_num_of_vertices();

        std::vector<double> W(Wreg.size(), 0);
        assert(Wreg.size() == W.size());

        size_t num_of_pairs = indecies_of_pairs.size();


        for (int j = 0; j < EPOCH_TIMES; ++j) {

            std::vector<int> random_indexes(num_of_pairs) ;
            std::iota (std::begin(random_indexes), std::end(random_indexes), 0); // Fill with 0, 1, ..., n.
            std::random_shuffle ( random_indexes.begin(), random_indexes.end() );

            for (size_t i = 0; i < num_of_pairs; i++) {

                //get random index
                size_t random_index = random_indexes.back();
                random_indexes.pop_back();

                const std::vector<Pair>& volume = idpair(examples[ indecies_of_pairs[random_index][0] ], examples[ indecies_of_pairs[random_index][1] ]);

                SGD_similar( W, Wreg, volume, tags[random_index], thold, C , i+1);
            }
        }

        cout<< "W.size(): "<< W.size() << " \nW:" << endl;
        for (auto i : W) {
            cout << i << " , ";
        }
        cout<< endl;

        return W;
    }
    
        void SGD_similar(std::vector<double>& W, const std::vector<double>& Wreg, const std::vector<Pair>& volume,
                     short tag, double & thold, const double C, size_t etha ){

        std::vector<double> W_old(W);
        size_t size = W.size();
        size_t sparse_size = volume.size();
        double dotProd = 0;

        //ID(X_pi_1, X_pi_2) * W
        for (size_t i = 0; i < sparse_size; i++)
            dotProd += volume[i]._weight * W[ volume[i]._index ];

        // 1 - { (ID(X_pi_1, X_pi_2) * W) - threshold } * y_i
        if(  ( 1 - ( (dotProd - thold) * tag) ) > 0 ) {
            for (auto& simplex_point : volume) {
                W[simplex_point._index] -= (1.0/(double)etha) *  (C * tag * simplex_point._weight);  //////*************************************
                W[simplex_point._index] = W[simplex_point._index] < 0 ? 0 : W[simplex_point._index];
            }
            thold -= ( (1.0 / (double)etha) * (C * tag) );
        }

        for (size_t i = 0; i < size; i++){
            W[i] -= (1.0/(double)etha) * ((W_old[i] - Wreg[i]));
            W[i] = W[i]<0 ? 0 : W[i];
        }
    }

    /*
    construct Wreg for learning
    Wreg is a regulizer distance for the vertexes (using L2 distances)
    */
    std::vector<double> construct_Wreg(const Grid& points_pair){
        size_t num_of_dims = points_pair.get_num_of_dims();
        size_t num_of_ver = points_pair.get_num_of_vertices();

        // cout << num_of_ver << endl;
        std::vector<double> Wreg(num_of_ver, 0);
        std::vector<double> ver(num_of_dims, 0);
        std::vector<double> ver0(num_of_dims/2, 0);
        std::vector<double> ver1(num_of_dims/2, 0);


        for (size_t i = 0; i < num_of_ver - 1; i++) {
            points_pair.get_vertex(i, ver);
            std::vector<double>::iterator it = ver.begin();

            ver0.assign(it , it + ver.size()/2);
            it += ver.size()/2;
            ver1.assign(it , it + ver.size()/2);

            Wreg[i] = L1Distance(ver0, ver1);
        }

/*        cout << "Wreg:" << endl;
        for (auto& obj : Wreg) {
            std::cout << obj << " ";
        }
        std::cout << std::endl;
        std::cout << "num_of_ver = " << num_of_ver << std::endl;*/

        return Wreg;
    }

    /*
    called from construct_Wreg
    compute euclidean distance L2 on vertexes
    */
    double L2Distance(const std::vector <double>& p1, const std::vector <double>& p2){
        assert(p1.size() == p2.size());
        size_t num_of_points = p1.size();
        double dist = 0;
        std::vector <double> difference(p1.size(),0);

        for (size_t i = 0; i < num_of_points; i++) {
            double x = p2[i] - p1[i];
            difference[i] = std::pow(x,2);
            dist += difference[i];
        }

        dist = std::sqrt(dist);
        return dist;
    }

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

    // -1(similar) , 1(non-similar)
    short classification(const std::vector<double>& W, const std::vector<Pair>& non_zero, double thold){
        double dotProd = 0;
        short ans = 0;

        for (size_t i = 0; i < non_zero.size() ; i++)
            dotProd += non_zero[i]._weight * W[ non_zero[i]._index ];

        if( (dotProd - thold) < 0) ans = 1;
        else ans = -1;

        return ans;
    }

    // c_vec_intialization - initialize vector c
    void c_vec_intialization(std::vector<double> &c_vec) {
        for (int n = -5; n <= 15; n += 2)
            c_vec.push_back(pow(2, n));
    }

};

#endif
