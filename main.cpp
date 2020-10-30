// Nelson Alvarez
// Dean Vargas

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
  float m_f32[DATA_N];
};


typedef unsigned int u_int;
typedef struct xmm_val xmm_val_t;


float dot_prod_c(xmm_val_t*, xmm_val_t*);
extern "C" float dot_prod_avx_p(xmm_val_t*, xmm_val_t*);
extern "C" float dot_prod_avx_s(xmm_val_t*, xmm_val_t*);
extern "C" float dot_prod_sse_p(xmm_val_t*, xmm_val_t*);
extern "C" float dot_prod_sse_s(xmm_val_t*, xmm_val_t*);


chrono::duration<double> do_test(xmm_val_t*, xmm_val_t*, int, mt19937&,function<float(xmm_val_t*, xmm_val_t*)>);
void valid_test(xmm_val_t*, xmm_val_t*, int);

int main(int argc, char* argv[]){

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);

    alignas(16) xmm_val_t vector1;
    alignas(16) xmm_val_t vector2;

    u_int trials = 10000;


    //cout<<"RESULT TEST\n";

    //valid_test(&vector1,&vector2, 10);

    cout<<"TIME TEST\n";

    cout<<"HIGH LEVEL TEST: "<<do_test(&vector1, &vector2, trials, generator, dot_prod_c).count()<<"s\n";
    cout<<"AVX PACKED TEST: "<<do_test(&vector1, &vector2, trials, generator, dot_prod_avx_p).count()<<"s\n";
    cout<<"AVX SCALAR TEST: "<<do_test(&vector1, &vector2, trials, generator, dot_prod_avx_s).count()<<"s\n";
    cout<<"SSE PACKED TEST: "<<do_test(&vector1, &vector2, trials, generator, dot_prod_sse_p).count()<<"s\n";
    cout<<"SSE SCALAR TEST: "<<do_test(&vector1, &vector2, trials, generator, dot_prod_sse_s).count()<<"s\n";
}

float dot_prod_c(xmm_val_t* v1, xmm_val_t* v2){
    float result = 0.0;
    for(u_int i = 0; i < DATA_N; ++i)
        result += v1->m_f32[i] * v2->m_f32[i];
    return result;
}

void valid_test(xmm_val_t *v1, xmm_val_t *v2, int n){
    for(u_int i = 0; i < n; ++i){
        v1->m_f32[0] = 2.5;  
        v1->m_f32[1] = 3.4;  
        v1->m_f32[2] = 18.9; 
        v1->m_f32[3] = 24.7; 
        
        v2->m_f32[0] = 120.4;
        v2->m_f32[1] = 1.333;
        v2->m_f32[2] = 6.1;  
        v2->m_f32[3] = 3.2;  
        cout << "R1: "<<dot_prod_c(v1,v2)<<"\tR2: "<<dot_prod_avx_p(v1,v2)<<"\tR3: "<<dot_prod_avx_s(v1,v2)<<"\tR4: "<<dot_prod_sse_p(v1,v2)<<"\tR5: "<<dot_prod_sse_s(v1,v2)<<endl;
    }
}

chrono::duration<double> do_test(xmm_val_t* v1, xmm_val_t* v2, int n, mt19937& generator,function<float(xmm_val_t*, xmm_val_t*)> test){
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    for(u_int i = 0; i < n; ++i){
        v1->m_f32[0] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        v1->m_f32[1] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        v1->m_f32[2] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        v1->m_f32[3] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        
        v2->m_f32[0] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        v2->m_f32[1] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        v2->m_f32[2] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        v2->m_f32[3] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;

        test(v1,v2);
    }
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(t2 - t1);
}
