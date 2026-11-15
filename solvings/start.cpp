#include <iostream>
#include <omp.h>
#include <random>
#include <vector>
std::vector <int> randomARR(int n,int low,int high){
    std::vector<int> v(n);
    #pragma omp parallel
    {
        std::mt19937 gen(42 + omp_get_thread_num());
        std::uniform_int_distribution<int> dist(low,high);
        #pragma omp for
        for (int i = 0; i < n; i++){
            v[i] = dist(gen);
        }
        
    }
     return v;
}
long long tSum(int n,std::vector <int> &v){
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++){
        sum += v[i];
    }
    return sum;
}
long long serial_tSum(int n,std::vector <int> &v){
    long long sum = 0;
    
    for(int i = 0; i < n; i++){
        sum += v[i];
    }
    return sum;
}


int tMax(int n,std::vector <int>&v){
    int largest = v[0];
    #pragma omp parallel for reduction(max:largest)
    for(int i = 0; i < n; i++){
        if(v[i] > largest){
            largest = v[i];
        }
    }
    return largest;
}

int serial_tMax(int n,std::vector <int>&v){
    int largest = v[0];
    
    for(int i = 0; i < n; i++){
        if(v[i] > largest){
            largest = v[i];
        }
    }
    return largest;
}

int tMin(int n,std::vector <int> &v){
    int smallest = v[0];
    #pragma omp parallel for reduction(min:smallest)
    for(int i =0; i < n; i++){
        if(v[i] < smallest) smallest = v[i];
    }
    return smallest;
}

int serial_tMin(int n,std::vector <int> &v){
    int smallest = v[0];
    #pragma omp parallel for reduction(min:smallest)
    for(int i =0; i < n; i++){
        if(v[i] < smallest) smallest = v[i];
    }
    return smallest;
}
template<typename f>
double getTime(f func){
    double t1 = omp_get_wtime();
    func;
    double t2 = omp_get_wtime();

    double totalT = t2 - t1;
    return totalT;
}
double speedup(double t_serial ,double t_parallel){
    double s = t_serial / t_parallel;

    return s;
    
}

double efficiency(double t_serial,double t_parallel ,int threads){
    double S = speedup(t_serial,t_parallel);

    double e = 100 * (S / threads);

    return e;
}

void printResults(int resultP,int resultS,double Ptime,double serial_time){
    std::cout << "Serial results: " << resultS << "\n";
    std::cout << "Parallel results: " << resultP << "\n";
    std::cout << "Time of parallel: " << Ptime << "\n";
    std::cout << "Time of serial:  " << serial_time << "\n"; 
    
}
int main(){
long long sum = 0;
int n = 10000000;

std::vector <int> v(randomARR(n,1,100));


std::cout << "Serial runs: " << "\n";
int result_smax = serial_tMax(n,v);
double time_smax = getTime(serial_tMax(n,v));

int result_smin = serial_tMin(n,v);
double time_smin = getTime(serial_tMin(n,v));

long long result_ssum = serial_tSum(n,v);
double time_ssum = getTime(serial_tSum(n,v));

std::cout << "\n" << "parallel runs ";
int result_pmax = tMax(n,v);
double time_pmax = getTime(tMax(n,v));

int result_pmin = tMin(n,v);
double time_pmin = getTime(tMin(n,v));

long long result_psum = tSum(n,v);
double time_psum = getTime(tSum(n,v));
std::cout << "max" << "\n";
printResults(result_pmax,result_smax,time_pmax,time_smax);

std::cout << "Min" << "\n";
printResults(result_pmin,result_smin,time_pmin,time_smin);

std::cout << "Sum" << "\n";
printResults(result_psum, result_ssum, time_ssum, time_psum);


int threads = omp_get_max_threads();

long long sS, sP;
double tS = getTime([&]{ sS = serial_tSum(n, v); });
double tP = getTime([&]{ sP = tSum(n, v); });
std::cout << "Sum: serial=" << tS << " parallel=" << tP
          << " speedup=" << speedup(tS, tP)
          << " efficiency=" << efficiency(tS, tP, threads) << "%"
          << (sS == sP ? " match\n" : " MISMATCH\n");


    return 0;
}