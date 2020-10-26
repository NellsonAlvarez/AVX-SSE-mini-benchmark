#include <chrono>
#include <random>
#include <iostream>
#include <cstdint>
#include <climits>
#include <functional>

using namespace std;

const int DATA_N = 4;

struct xmm_val
{
    int32_t m_i32[DATA_N];
};


typedef unsigned int u_int;
typedef struct xmm_val xmm_val_t;


int dot_prod_c(xmm_val_t*, xmm_val_t*);
extern "C" int dot_prod_avx_p(xmm_val_t*, xmm_val_t*);
extern "C" int dot_prod_avx_s(xmm_val_t*, xmm_val_t*);
extern "C" int dot_prod_sse_p(xmm_val_t*, xmm_val_t*);
extern "C" int dot_prod_sse_s(xmm_val_t*, xmm_val_t*);


chrono::duration<double> do_test(xmm_val_t*, xmm_val_t*, int, mt19937&,function<int(xmm_val_t*, xmm_val_t*)>);

int main(int argc, char* argv[]){

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);

    alignas(16) xmm_val_t vector1;
    alignas(16) xmm_val_t vector2;

    unsigned int trials = 10;

    cout<<"HIGH LEVEL TEST: "<<do_test(&vector1, &vector2, trials, generator, dot_prod_c).count()<<"s\n";
}

int dot_prod_c(xmm_val* v1, xmm_val* v2){
    int result = 0;
    for(u_int i = 0; i < DATA_N; ++i)
        result += v1->m_i32[i] * v2->m_i32[i];
    return result;
}

chrono::duration<double> do_test(xmm_val_t* v1, xmm_val_t* v2, int n, mt19937& generator,function<int(xmm_val_t*, xmm_val_t*)> test){
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    for(u_int i = 0; i < n; ++i){
        v1->m_i32[0] = (generator() % USHRT_MAX) - SHRT_MAX;
        v1->m_i32[1] = (generator() % USHRT_MAX) - SHRT_MAX;
        v1->m_i32[2] = (generator() % USHRT_MAX) - SHRT_MAX;
        v1->m_i32[3] = (generator() % USHRT_MAX) - SHRT_MAX;
        
        v2->m_i32[0] = (generator() % USHRT_MAX) - SHRT_MAX;
        v2->m_i32[1] = (generator() % USHRT_MAX) - SHRT_MAX;
        v2->m_i32[2] = (generator() % USHRT_MAX) - SHRT_MAX;
        v2->m_i32[3] = (generator() % USHRT_MAX) - SHRT_MAX;
        cout << "R1: "<<test(v1,v2)<<"\tR2: "<<dot_prod_avx_p(v1,v2)<<endl;
    }
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(t2 - t1);
}
