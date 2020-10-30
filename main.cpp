// Authors:
//          Nelson Alvarez
//          Dean Vargas

#include <chrono>
#include <random>
#include <iostream>
#include <cstdint>
#include <cstring>
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


void generate_values(xmm_val_t**, xmm_val_t**, int, mt19937&);
void test_all(xmm_val_t**, xmm_val_t**, int, mt19937&);
void validate(xmm_val_t*, xmm_val_t*, int);
chrono::duration<double> do_test(xmm_val_t**, xmm_val_t**, int, function<float(xmm_val_t*, xmm_val_t*)>);
bool isNumber(char* s);

int main(int argc, char* argv[]){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);

    u_int trials = 10000;

    if(argc > 1){
        if(isNumber(argv[1]))
            trials = stoi(argv[1]);
        else{
            cout<<"Usage: "<<argv[0]<<" <number of tests>\n";
            return 0;    
        }
    }

    alignas(16) xmm_val_t* vector1 = new xmm_val_t[trials];
    alignas(16) xmm_val_t* vector2 = new xmm_val_t[trials];

    test_all(&vector1,&vector2, trials, generator);

    delete vector1;
    delete vector2;

    return 0;
}

float dot_prod_c(xmm_val_t* v1, xmm_val_t* v2){
    float result = 0.0;
    for(u_int i = 0; i < DATA_N; ++i)
        result += v1->m_f32[i] * v2->m_f32[i];
    return result;
}

void test_all(xmm_val_t** v1, xmm_val_t** v2, int n, mt19937& generator){
    generate_values(v1,v2,n,generator);
    cout<<"TIME TEST with "<<n<<" trials\n";

    cout<<"HIGH LEVEL: "<<do_test(v1, v2, n, dot_prod_c).count()<<"s\n";
    cout<<"AVX PACKED: "<<do_test(v1, v2, n, dot_prod_avx_p).count()<<"s\n";
    cout<<"AVX SCALAR: "<<do_test(v1, v2, n, dot_prod_avx_s).count()<<"s\n";
    cout<<"SSE PACKED: "<<do_test(v1, v2, n, dot_prod_sse_p).count()<<"s\n";
    cout<<"SSE SCALAR: "<<do_test(v1, v2, n, dot_prod_sse_s).count()<<"s\n";
}

void validate(xmm_val_t *v1, xmm_val_t *v2, int n){
    cout << "R1: "<<dot_prod_c(v1,v2)<<"\tR2: "<<dot_prod_avx_p(v1,v2)<<"\tR3: "<<dot_prod_avx_s(v1,v2)<<"\tR4: "<<dot_prod_sse_p(v1,v2)<<"\tR5: "<<dot_prod_sse_s(v1,v2)<<endl;
}

void generate_values(xmm_val_t** v1, xmm_val_t** v2, int n, mt19937& generator){
    for(u_int i = 0; i < n; ++i){
        (*v1+i)->m_f32[0] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        (*v1+i)->m_f32[1] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        (*v1+i)->m_f32[2] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        (*v1+i)->m_f32[3] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
            
        (*v2+i)->m_f32[0] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        (*v2+i)->m_f32[1] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        (*v2+i)->m_f32[2] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
        (*v2+i)->m_f32[3] = (float)(generator() % USHRT_MAX)/(float)USHRT_MAX;
    }
}


chrono::duration<double> do_test(xmm_val_t** v1, xmm_val_t** v2, int n, function<float(xmm_val_t*, xmm_val_t*)> test){
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    for(u_int i = 0; i < n; ++i){
        test((*v1+i),(*v2+i));
    }
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(t2 - t1);
}

bool isNumber(char* s){
    for (int i = 0; i < strlen(s); ++i)
        if (!isdigit(s[i]))
            return false;
    return true;
}
