#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <float.h>


double getDis(int i,int j,int N,double* dis){
    double distance =  dis[i*N+j];
    return distance;
}

int condition(int a,int b,int c,int d, double* dis,int N, double th){
    if (a==DBL_MAX||b==DBL_MAX||c==DBL_MAX||d==DBL_MAX){
        return 0; // false
    }
    int res =  fabs(getDis(a,b,N,dis)-getDis(c,d,N,dis))<th
            && fabs(getDis(a,d,N,dis)-getDis(b,c,N,dis))<th;
    return res;
}

long random_at_most(long max) {
    unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
            num_bins = (unsigned long) max + 1,
            num_rand = (unsigned long) RAND_MAX + 1,
            bin_size = num_rand / num_bins,
            defect   = num_rand % num_bins;

    long x;
    do {
        x = rand();
    }
        // This is carefully written not to overflow
    while (num_rand - defect <= (unsigned long)x);

    // Truncated division is intentional
    return x/bin_size;
}

int main() {

    printf("dataSource : ./disMat\n");

    int samples;
    int subSamples;
    double th;

    printf("samples = ");
    scanf("%d",&samples);
    printf("subSamples = ");
    scanf("%d",&subSamples);
    printf("th = ");
    scanf("%lf",&th);


    int N;

    FILE *disFp = fopen("./disMat","r");
    fscanf(disFp,"%d",&N);

    printf("N : %d \n",N);

    double * dis;
    int *votes;
    dis = malloc(N*N*sizeof(double));
    votes = calloc(N*N, sizeof(int));
    if (!dis || !votes)//如果malloc失败，可以得到一些log
    {
        perror("malloc");
        return-1;
    }

    for (int l = 0; l < N*N; ++l) {
        double i;
        fscanf(disFp,"%lf",&i);
        dis[l] = i<0?DBL_MAX:i;
    }

    fclose(disFp);

    int a,b,c,d,e,f;
    for (int i = 0; i < samples; ++i) {
        do {
            a = random_at_most(N-1);
            b = random_at_most(N-1);
            c = random_at_most(N-1);
            d = random_at_most(N-1);
        } while (a==b||a==c||a==d||b==c||b==d||c==d);
        if(condition(a,b,c,d,dis,N,th)){
            for (int j = 0; j < subSamples; ++j) {
                e = random_at_most(N-1);
                f = random_at_most(N-1);
                if (condition(a,b,e,f,dis,N,th)
                    &&condition(c,d,e,f,dis,N,th)){
                    votes[a*N+b]++;
                    votes[c*N+d]++;
                }
            }
        }

    }

    FILE *fp = fopen("./out.txt","w");
    for (int k = 0; k < N; ++k) {
        for (int i = 0; i < N; ++i) {
            fprintf(fp,"%d ",votes[k*N+i]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    return 0;
}