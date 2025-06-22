#include<stdio.h>
#include<stdint.h>

// funckja zapisująca czasy obliczeń do plików .csv w folderze /data
void save_times(double times[5][3][2][100], uint16_t ammount){

    // definicja nazw wykorzystywanych w plikach
    const char *alg_names[] = {
        "prim", "kruskal", "dijkstra", "ford_bellman", "ford_fulkerson"
    };

    const int densities[] = {25, 50, 99};

    const char *rep_names[] = {"matrix", "list"};

    // główna pętla funkcji, zapisuje wyniki każdego algorytmu do osobnego pliku
    for(int a = 0; a < 5; a++){

        // tworzenie plików
        char filename[256];
        snprintf(filename, sizeof(filename), "data/%s_%d.csv", alg_names[a], ammount);

        FILE *file = fopen(filename, "w");
        if(file == NULL){

            fprintf(stderr, "Error opening file %s\n", filename);
            continue;

        }

        // zapisanie nagłówka
        fprintf(file, "run");

        for (int d = 0; d < 3; d++)
            for(int r = 0; r < 2; r++) 
                fprintf(file, ";%d_%s", densities[d], rep_names[r]);
            
        
        fprintf(file, "\n");

        // zapisanie czasów
        for(int run = 0; run < 100; run++){

            fprintf(file, "%d", run);

            for (int d = 0; d < 3; d++) 
                for (int r = 0; r < 2; r++) 
                    fprintf(file, ";%.9f", times[a][d][r][run]);
                
            
            fprintf(file, "\n");

        }

        fclose(file);

    }
    
}
