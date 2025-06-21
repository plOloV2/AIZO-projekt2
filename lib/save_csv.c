#include<stdio.h>
#include<stdint.h>

void save_times(double times[5][3][2][100], uint16_t ammount){

    const char *alg_names[] = {
        "prim", "kruskal", "dijkstra", "ford_bellman", "ford_fulkerson"
    };

    const int densities[] = {25, 50, 99};

    const char *rep_names[] = {"matrix", "list"};

    for(int a = 0; a < 5; a++){

        char filename[256];
        snprintf(filename, sizeof(filename), "data/%s_%d.csv", alg_names[a], ammount);

        FILE *file = fopen(filename, "w");
        if(file == NULL){

            fprintf(stderr, "Error opening file %s\n", filename);
            continue;

        }

        // Write header
        fprintf(file, "run");

        for (int d = 0; d < 3; d++)
            for(int r = 0; r < 2; r++) 
                fprintf(file, ";%d_%s", densities[d], rep_names[r]);
            
        
        fprintf(file, "\n");

        // Write data
        for(int run = 0; run < 100; run++){

            fprintf(file, "%d", run);

            for (int d = 0; d < 3; d++) 
                for (int r = 0; r < 2; r++) 
                    fprintf(file, ";%.6f", times[a][d][r][run]);
                
            
            fprintf(file, "\n");

        }

        fclose(file);

    }
    
}
