#include <stdio.h>
#include <stdlib.h>

#define MAX 8 
int array[MAX] = {1,4,12,18,26,31,40,42};
int rekursionstiefe = 0;

int binarysearch(int zahl, int links, int rechts) {
    rekursionstiefe++;
    int mitteright = ((links + rechts) / 2)+1;
    int mitteleft = (links + rechts) / 2;
    printf("\nRekursionstiefe: %d", rekursionstiefe);
    if (array[mitteleft] == zahl ){
        return mitteleft; }
    if (array[mitteright] == zahl){
    	return mitteright;}
    if (links == rechts){
        return -1; }
    if (array[mitteleft] > zahl){
        return binarysearch(zahl, links, mitteleft);} 
    if (array[mitteright] < zahl){
        return binarysearch(zahl, mitteright, rechts);}
    return -1;
}

int main(int argc, char *argv[]) {
    int zahl, position, i;
    if(argc < 2) {
        printf("Benutzung: %s <zu suchende Zahl>\n", argv[0]);
        return 1;
    }
    zahl = atoi(argv[1]);

    for (i = 0; i < MAX; i++) {
        printf("%4d", array[i]);
    }
    position = binarysearch(zahl, 0, 7); 
    if (position >= 0) {
        printf("\nGesuchte Zahl %d an Arrayposition %d\n", zahl, position);
    }
    else {
        printf("\nZahl %d nicht gefunden\n", zahl);
    }
    return 0;
}
