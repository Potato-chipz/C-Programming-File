#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{

	//Fill in code below
	//When deciding which way to go for the next step, generate a random number as follows.
	//r = rand() % 4;
	//Treat r = 0, 1, 2, 3 as up, right, down and left respectively.

	//The random walk should stop once the x coordinate or y coordinate reaches $-n$ or $n$. 
	//The function should return the fraction of the visited $(x, y)$ coordinates inside (not including) the square.
    int xc = n; 
    int yc = n; 
    int s = 2*n+1; 

    int Tracking[s][s]; 
    
    //To generalize and initialize array 
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            Tracking[i][j] = 0; 
        }
    }

    while(xc > 0 && xc < 2*n && yc > 0 && yc < 2*n)
    {
        Tracking[xc][yc] = 1; //mark the path 
        int step = rand()%4; // to decide which way to go, randomized  
        if (step == 0)
        {
            yc++;//up
        }
        if(step == 1)
        {
            xc++;//right
        }
        if(step == 2)
        {
            yc--;//down
        }
        if(step == 3)
        {
            xc--;//left
        }
    }

    double c = 0.0;
    double total = (2*n-1)*(2*n-1);
    
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            if(Tracking[i][j])
            {
                c++; //step count incremented 
            }
        }
    }
    return c/total;


}

//Do not change the code below
int main(int argc, char* argv[])
{
	int trials = 1000;
	int i, n, seed;
	if (argc == 2) seed = atoi(argv[1]);
	else seed = 12345;

	srand(seed);
	for(n=1; n<=64; n*=2)
	{	
		double sum = 0.;
		for(i=0; i < trials; i++)
		{
			double p = two_d_random(n);
			sum += p;
		}
		printf("%d %.3lf\n", n, sum/trials);
	}
	return 0;
}
