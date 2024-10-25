#include "functions_for_2_5.h"


int main() {
    char buff[BUFSIZ];
    int count;
    printf("\t\tTest 1\n");
    count = oversprintf(buff, "%d %d %Ro %.2f", 3888, -120, 0, 3.22);
    printf("string: '%s'; count symbols: %d\n\n", buff, count);
    FILE *output = fopen("out_2_5.txt", "w");
    if (output == NULL){
        return FILE_OPEN_ERROR;
    }
    overfprintf(output, "%d %d %Ro %.2f\n", 3888, -120, 0, 3.22);
}
