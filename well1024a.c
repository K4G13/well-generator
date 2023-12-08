#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define R 32
#define M1 3
#define M2 24
#define M3 10

static unsigned int z0, z1, z2, state_i;
static unsigned int STATE[R];

#define MAT1(v)         (v)
#define MAT3POS(t,v)    (v^(v>>t))
#define MAT3NEG(t,v)    (v^(v << (-t)))

#define V0      STATE[ state_i                          ]
#define VM1     STATE[ (state_i+M1)     & 0x0000001fUL  ]  
#define VM2     STATE[ (state_i+M2)     & 0x0000001fUL  ]
#define VM3     STATE[ (state_i+M3)     & 0x0000001fUL  ]
#define VRm1    STATE[ (state_i+R-1)    & 0x0000001fUL  ] 
#define newV0   STATE[ (state_i+R-1)    & 0x0000001fUL  ]
#define newV1   STATE[ state_i                          ]   

void InitWELL1024a (unsigned int *init){
    int j;
    state_i = 0;
    for (j=0; j < R; j++) STATE[j] = init[j];
}

double WELL1024a(){
    z0 = VRm1;
    z1 = MAT1(V0) ^ MAT3POS(8,VM1);
    z2 = MAT3NEG (-19, VM2) ^ MAT3NEG(-14,VM3);
    newV1 = z1 ^ z2;
    newV0 = MAT3NEG(-11,z0) ^ MAT3NEG(-7,z1) ^ MAT3NEG(-13,z2);
    state_i = (state_i + 31) & 0x0000001fUL;
    return ((double) STATE[state_i] * 2.32830643653869628906e-10);
}

void generateRandomInitState(unsigned int * init){
    srand(time(NULL));
    for(int i=0;i<R;i++)
        init[i] = rand();
}

int main(int argc, char **argv){

    unsigned int outc = 1;
    unsigned int init[R];

    if(argc>1){
        // 1 arg
        if(argc-1==1){
            outc = atoi(argv[1]);
            generateRandomInitState(init);
        }
        // R args
        else if(argc-1==R){ 
            for(int i=0;i<R;i++)
                init[i] = atoi(argv[i + 1]);
        }
        // R+1 args (outputs cout + init array)
        else if(argc-1==R+1){
            outc = atoi(argv[1]);
            for(int i=0;i<R;i++)
                init[i] = atoi(argv[i + 1]);
        }
        // error
        else{
            printf("Expected %d or %d args, got %d.",R+1,R,argc-1);
            return 1;
        }
    }
    // 0 args
    else{
        generateRandomInitState(init);
    }

    InitWELL1024a(init);

    for (int i=0;i<outc;i++) {
        double random_number = WELL1024a();
        printf("%.10lf\n", random_number);
    }

    return 0;
}