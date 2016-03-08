/*
Parallel Sparse Matrix Multiplication Program
##Input format
N M P
K1 K2

X1 Y1 W1
........
........
Xk1 Yk1 wk1

X1 Y1 W1
....
....
Yk2 Yk2 Wk2

where N M P denotes the dimensions N x M and M x P of input Matrices
k1 and k2 denotes the number of non zero elements in matrix A and B

Output will be printed to a file Output.out
*/

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <stdio.h>
#include <mutex>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <utility>
#define MAXTHREADS 200				//Maximum Threads Running Parallelly. Depends on the system. Change Accordingly
using namespace std;
mutex mu;
typedef vector< vector<int> > Matrix;

struct node{
    int x;
    int y;
};
struct reqData{
    int x_Out;
    int y_Out;
    int W;
};
struct opt{
    int x;
    vector<int> col;
};

vector < pair < int, pair < int, int > > >  ans;
bool compareByX(const node &a, const node &b)
{
    return a.x < b.x;
}
bool compareByXOUT(const reqData &a, const reqData &b)
{
    if( a.x_Out != b.x_Out){
        return a.x_Out < b.x_Out;
    }
    return (a.y_Out < b.y_Out);

}
void multiply(vector<int> &A, Matrix& B,vector<int> &C,vector <struct reqData> &out,int row,vector<int> &B_Col){

    for(int q = 0;q < C.size();q++){
        for(int l = 0;l < B_Col.size();++l){
                if(B[B_Col[l]][q] != 0 || A[B_Col[l]] != 0){
                    C[q] += (A[B_Col[l]] * B[B_Col[l]][q]);
                }

        }
	
        if(C[q] != 0){
            struct reqData printOut;
            printOut.x_Out = row;
            printOut.y_Out = q;
            printOut.W = C[q];
	mu.lock();
	    out.push_back(printOut);
        mu.unlock();
        }
    }

}

int main(){
    clock_t begin,end;
    double time_spent;
    begin = clock();

    ifstream infile;
    infile.open("Input.in",ios::in);
    int N,M,P,temp_x,temp_y,k1,k2,W;
    string blank;

    infile >> N >> M >> P;
    getline(infile, blank); // reads the rest of the line that the number was on
    getline(infile, blank);
    infile >> k1 >> k2;
    getline(infile, blank); // reads the rest of the line that the number was on
    getline(infile, blank);
    vector< vector<int> > A(N,vector<int> (M));
    vector< vector<int> > B(M,vector<int> (P));
    vector< vector<int> > C(N,vector<int> (P));
    vector<struct node > Data;

    int cou = 0;
    for(int i = 0;i < k1;i++){
            infile >> temp_x >> temp_y >> W;
		if(W){
			A[temp_x][temp_y] = W;
			struct node samp;
           		samp.x = temp_x;
            		samp.y = temp_y;
            		Data.push_back(samp);
		}           
    }

    for(int i = 0;i < k2;i++){
            infile >> temp_x >> temp_y >> W;
		if(W){
			B[temp_x][temp_y] = W;
		}         

    }
    infile.close();
    sort(Data.begin(), Data.end(), compareByX);  
	
    vector<struct node>::iterator pos;
    vector<int> uniqueX;
    pos = Data.begin();
    int prev = (*pos).x;
    uniqueX.push_back(prev);
    int count_X = 1;
    for(pos = Data.begin();pos < Data.end();++pos){
        if((*pos).x != prev ){
            ++count_X;
            uniqueX.push_back((*pos).x);
            prev = (*pos).x;
        }
    }
    vector<thread> threads(count_X);
    vector<struct opt> optimize;
    vector<int>::iterator uni;
    //create an optimization vector of opt
    pos = Data.begin();
    for(uni = uniqueX.begin();uni < uniqueX.end();++uni){
        vector<int> rel;
        while((*pos).x == (*uni)){
            rel.push_back( (*pos).y);
            ++pos;
        }
        struct opt temp;
        temp.x = (*uni);
        temp.col = rel;
        optimize.push_back(temp);
    }

    int row = 0;

    vector <struct reqData> output;
    for(int start = 0;start < count_X ;start += MAXTHREADS){
        for(int y = start;(y < count_X)&& (y < (start + MAXTHREADS));y++){
            threads[y] = thread(multiply,ref(A[uniqueX[row]]),ref(B),ref(C[uniqueX[row]]),ref(output),uniqueX[row],ref(optimize[row].col));
            ++row;
        }

        for(int h = start; h < (start + MAXTHREADS) and h < count_X;h++){
           threads[h].join();
        }
    }


    ofstream myfile;
    myfile.open("Output.out",ios::out);
    sort(output.begin(), output.end(), compareByXOUT);
    vector <struct reqData>::iterator outr;   

    for(int i = 0; i < output.size(); i++){
	myfile << output[i].x_Out << " " << output[i].y_Out << " " << output[i].W << endl;
    }  
    end = clock();
    time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
    myfile << endl;
    myfile << "Time Taken : " << time_spent << endl;
    myfile.close();

    exit(0);

    return 0;

}
