#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

typedef struct{
    float czas,h,v;
    bool silnik;
} Stan;


int main()
{
    
    float m = 2000;
    float H_0 = 500;
    float v_0 = 10;
    float F_c = 4500;
    float g = 1.6f;
    bool silnik = false;
    printf("podaj mase: "); scanf("%f",&m);
    printf("podaj wysokosc startowa: "); scanf("%f",&H_0);
    printf("podaj predkosc poczatkowa: "); scanf("%f",&v_0);
    printf("podaj sile silnika: "); scanf("%f",&F_c);
    float h = H_0;
    float v = v_0;
    float a;
    float t = 0.2f;
    int size = 1;
    Stan *tab = malloc(size*sizeof(Stan));
    float czas = 0.0f;
    clock_t end;
    clock_t start = clock();
    while(true)
    {
        if(kbhit())
        {
            char przycisk = getch();
            if(przycisk == ' ') silnik = !silnik;
        }
        end = clock();
        if((float)(end-start)/CLOCKS_PER_SEC>=t)
        {
            tab = realloc(tab,size*sizeof(Stan));
            tab[size-1] = (Stan){czas,h,v,silnik};
            size++;
            printf("H=%.1fm, V= %.1fm/s, ",h,v);
            if(silnik) printf("ON\n");
            else printf("OFF\n");

            a = silnik ? g-F_c/m : g;
            h -= (v*t+a*t*t/2);
            v += a*t;
            czas += t;
            start = end;
            if(h<=0.0f)
            {
                czas -=t;
                v -= a*t;
                h += (v*t+a*t*t/2);
                double koncoweV = sqrt(pow(v,2)+2*a*h);
                if(koncoweV-v!=0) czas += (koncoweV-v)/a;
                v = koncoweV;
                h = 0.0f;
                printf("H=%.1fm, V= %.1fm/s, ",h,v);
                if(silnik) printf("ON\n");
                else printf("OFF\n");
                tab = realloc(tab,size*sizeof(Stan));
                tab[size-1] = (Stan){czas,h,v,silnik};
                break;
            }
        }
        Sleep(1);
    }
    if(v<1.0f)
    {
        printf("Udalo sie wyladowac\n");
        char nazwa_pliku[55];
        int n = snprintf(nazwa_pliku,sizeof(nazwa_pliku), "%.0fKG-%.0fm-%.0fm_s-%.0fN.csv", m,H_0,v_0,F_c);
        FILE *plik = fopen(nazwa_pliku, "w");
        if(plik !=NULL)
        {
            fprintf(plik,"czas;wysokosc;predkosc;stan silnika\n");
            for(int i=0; i<size; i++)
            {
                fprintf(plik,"%.2f;%.2f;%.2f;%i\n",tab[i].czas,tab[i].h,tab[i].v,tab[i].silnik);
            }
            fclose(plik);
        }
    }
    else printf("Nie udalo sie wyladowac\n");
    
    
    free(tab);
    return 0;
}