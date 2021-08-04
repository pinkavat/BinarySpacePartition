#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* BSP.c
 *
 * A test of Binary Space Partitioning
 * 
 * Written 7 May 2020
*/

#define REGION_MINIMUM_SIZE 2   // The minimal edge length generated
#define REGION_MAXIMUM_AREA 64  // Any region larger must be split (TODO see if we can't factor this into the probability somehow instead)


struct grid {
    unsigned char *arr;
    unsigned int width, height;
};

struct region {
    unsigned int x, y, width, height;
};





// ========== BINARY SPACE PARTITION ==========

// Helper for labelling region (region fill becomes value)
void setRegionValue(struct grid g, struct region r, unsigned char value){
    for(unsigned int y = r.y; y < r.height + r.y; y++){
        for(unsigned int x = r.x; x < r.width + r.x; x++){
            //g.arr[(y*g.width)+x] = (y == r.y || x == r.x || y == r.y+r.height-1 || x == r.x+r.width-1) ? 1 : value;
            g.arr[(y*g.width)+x] = value;
        }
    }
}

// Compute the probability that the given region should be cut
// TODO: make this function more interesting (a distribution based on region area or something)
unsigned char breakUpProbability(struct region r){
    unsigned int area = r.width * r.height;
    if(area > REGION_MAXIMUM_AREA) return 1;    // Always split a region that is too large
    return rand() % 2;
    //return 1;
}

// Recursively execute Binary Space Partitioning on the given region of the given grid.
// Simplified from previous BSPForay.c, this algorithm bisects at random, perpendicular to the long axis.
void binarySpacePartition(struct grid g, struct region r){
    //printf("Operating on %u %u of dimensions %u %u: through[%u, %u]\n", r.x, r.y, r.width, r.height, r.x + r.width - 1, r.y+r.height - 1);
    // Region ID
    static unsigned char id = 1;
    
    // Compute breakup probability
    unsigned char breakUp = breakUpProbability(r);

    // Copy region data into potential subregions
    struct region alpha = r;
    struct region beta = r;

    if(breakUp && r.width > r.height && r.width >= REGION_MINIMUM_SIZE * 2){

        // Perform a vertical cut (bisect into two horizontally adjacent regions)
        unsigned int cutX = (rand() % (r.width - REGION_MINIMUM_SIZE - 1)) + REGION_MINIMUM_SIZE;
        //printf("CutX: %u\n", cutX);
        
        // alpha's x, y, height unchanged; beta's y, height unchanged
        // alpha's width is set to cutX
        alpha.width = cutX;
        // beta's x increases by cutX
        beta.x += cutX;
        // beta's width reduces by cutX
        beta.width -= cutX;

        binarySpacePartition(g, alpha);
        binarySpacePartition(g, beta);
        

    } else if(breakUp && r.height >= REGION_MINIMUM_SIZE * 2){

        // Perform a horizontal cut (bisect into two vertically adjacent regions)
        unsigned int cutY = (rand() % (r.height - REGION_MINIMUM_SIZE - 1)) + REGION_MINIMUM_SIZE;
        //printf("CutY: %u\n", cutY);

        // alpha's x, y, width unchanged; beta's x, width unchanged
        // alpha's height is set to cutY
        alpha.height = cutY;
        // beta's y increases by cutY
        beta.y += cutY;
        // beta's height reduces by cutY
        beta.height -= cutY;

        binarySpacePartition(g, alpha);
        binarySpacePartition(g, beta);

    } else {

        // Region is atomic (too small or we decided not to break it up)
        // Re-label region with label for demonstration
        setRegionValue(g, r, id++);
    }
} 





// ========== PRINTING AND DRIVER ==========

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"

#define ANSI_REVERSE        "\x1b[7m"
#define ANSI_BLINK          "\x1b[5m"

#define ANSI_RESET   "\x1b[0m"

char *colours[] = {ANSI_COLOR_RED, ANSI_COLOR_GREEN, ANSI_COLOR_YELLOW, ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, ANSI_COLOR_CYAN};

void printGrid(struct grid g){
    for(unsigned int y = 0; y < g.height; y++){
        for(unsigned int x = 0; x < g.width; x++){
            if(g.arr[(y * g.width) + x] == 0){
                printf("%s██", ANSI_COLOR_CYAN);
            } else {
                printf("%s%02u", colours[(g.arr[(y*g.width)+x]) % 6], (g.arr[(y * g.width) + x])%100);
            }
        }
        printf("%s\n", ANSI_RESET);
    }
}

int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "Usage: BSP <width> <height> [seed]\n");
        exit(2);
    }
    srand(time(0));
    if(argc > 3)srand(atoi(argv[3]));

    // Initialize grid
    struct grid g;
    g.width = atoi(argv[1]);
    g.height = atoi(argv[2]);
    g.arr = (unsigned char *)calloc(g.width * g.height, sizeof(unsigned char)); 

    // Begin with region spanning entire grid
    struct region r;
    r.x = 0;
    r.y = 0;
    r.width = g.width;
    r.height = g.height;

    // Recursively invoke the BSP algorithm
    binarySpacePartition(g, r);

    // Print the result
    printGrid(g);
}
