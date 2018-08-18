#include <stdio.h>
#include <omp.h>
#include <time.h>


#define MAX 10000
#define NOT_CONNECTED -1
#define FILE_PATH "Testing-n-5000.txt"

int distance[MAX][MAX];

int nodesCount;

int threadNum;

void Initialize(){
    int i,j;
    if(threadNum > 4){
    #pragma omp parallel for private(j) collapse(2) schedule(dynamic)
        for (i=0;i<MAX;++i){
            for (j=0;j<MAX;++j){
                distance[i][j]=NOT_CONNECTED;

            }
        }
    }
    else{
        #pragma omp parallel for private(j) schedule(static)
        for (i=0;i<MAX;++i){
            for (j=0;j<MAX;++j){
                distance[i][j]=NOT_CONNECTED;

            }
        }

    }
    for(i=0;i<MAX;++i){
        distance[i][i]=0;
    }

}


int main(void){

    threadNum = omp_get_num_threads();

   FILE *in_file  = fopen(FILE_PATH, "r");
    if (in_file  == NULL)
    {
        printf("Can't open file for reading.\n");
        return -1;
    }



    Initialize();

    fscanf(in_file,"%d", &nodesCount);

    int a, b, c;
    while(fscanf(in_file,"%d %d %d", &a, &b, &c)!= EOF){
        if ( a > nodesCount || b > nodesCount){
            printf("Vertex index out of boundary.");
            return -1;
        }
        distance[a][b]=c;
    }

    //Floyd-Warshall
    int k;
    for (k=1;k<=nodesCount;++k){
        int i,j;
        // if else
        if(threadNum > 4){
            #pragma omp parallel for private(j) collapse(2) schedule(dynamic)
            for (i=1;i<=nodesCount;++i){
                {
                    for (j=1;j<=nodesCount;++j){
                        if ((distance[i][k]!=NOT_CONNECTED)&&(distance[k][j]!=NOT_CONNECTED && (distance[i][j]==NOT_CONNECTED || distance[i][k]+distance[k][j]<distance[i][j]))){
                            distance[i][j]=distance[i][k]+distance[k][j];
                        }
                    }
                }
            }
        }
        else{
            #pragma omp parallel for private(j)schedule(static)
            for (i=1;i<=nodesCount;++i){
                {
                    for (j=1;j<=nodesCount;++j){
                        if ((distance[i][k]!=NOT_CONNECTED)&&(distance[k][j]!=NOT_CONNECTED && (distance[i][j]==NOT_CONNECTED || distance[i][k]+distance[k][j]<distance[i][j]))){
                            distance[i][j]=distance[i][k]+distance[k][j];
                        }
                    }
                }
            }

        }

    }

    int diameter=-1;

    //look for the most distant pair
    int i,j;
    if(threadNum > 4){
        #pragma omp parallel for private(j) collapse(2) schedule(dynamic)
        for (i=1;i<=nodesCount;++i){
            for (j=1;j<=nodesCount;++j){
                if (diameter<distance[i][j]){
                    diameter=distance[i][j];
                    //printf("%d-%d-%d\n", i, diameter, j);
                }
            }
        }
    }
    else{
        #pragma omp parallel for private(j) schedule(static)
        for (i=1;i<=nodesCount;++i){
            for (j=1;j<=nodesCount;++j){
                if (diameter<distance[i][j]){
                    diameter=distance[i][j];
                    //printf("%d-%d-%d\n", i, diameter, j);
                }
            }
        }

    }

    printf("%d\n", diameter);

    return 0;

}
