#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

int N, N4;
signed char a[25480], b[25480], c[25480];
int memo_q5[50];
int memo_r5[50];

int memo_q25[250];
int memo_r25[250];

int memo_q239[2390];
int memo_r239[2390];

#define BODY_FOR_CALCULATE(j) {\
        SET( c, 1 );\
        LONGDIV( c, j );\
\
        SUBTRACT( a, c, a );\
        DIVIDE_25( a );\
\
        SUBTRACT( b, c, b );\
        DIVIDE_239( b );\
        DIVIDE_239( b );\
\
        progress();\
}

#define BODY_FOR_DIVIDE(k) {\
        u = r * 10 + x[k];\
        q = u/n;\
        r = u - q * n; \
        x[k] = q;}
#define BODY_FOR_DIVIDE239(k) {\
        u = r * 10 + x[k]; \
        x[k] = memo_q239[u];\
        r = memo_r239[u];}
#define BODY_FOR_DIVIDE25(k) {\
        u = r * 10 + x[k];\
        x[k] = memo_q25[u];\
        r = memo_r25[u];} 
#define BODY_FOR_DIVIDE5(k) {\
        u = r * 10 + x[k];\
        x[k] = memo_q5[u];\
        r = memo_r5[u];} 


void init_memo(int *q, int *r, int num){
    int i,j,div;
    i=0;
    div=0;
    while (i<num*10){
        for (j=0; j<num; j++){
            q[i] = div;
            r[i] = j;
            i++;
        }
        div++;
    }
}


// TODO: MEMOIZATION per q i r, necessitem 250 valors per cada una (el rang de u) 
void DIVIDE( signed char *x, int n )                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for( k = 0; k <= N4; k+=4 )                  
    {                                            
			BODY_FOR_DIVIDE(k)
			BODY_FOR_DIVIDE(k+1)
			BODY_FOR_DIVIDE(k+2)
			BODY_FOR_DIVIDE(k+3)
    }                                           
		for(;k <= N4; k++) BODY_FOR_DIVIDE(k);
}


void DIVIDE_239( signed char *x)                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for( k = 0; k <= N4; k+=4 )                  
    {                                            
       BODY_FOR_DIVIDE239(k); 
       BODY_FOR_DIVIDE239(k+1); 
       BODY_FOR_DIVIDE239(k+2); 
       BODY_FOR_DIVIDE239(k+3); 
    }                                           
		for(;k <= N4; k++) BODY_FOR_DIVIDE239(k);
}

void DIVIDE_25( signed char *x)                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for( k = 0; k <= N4; k+=4 )                  
    {                                            
       BODY_FOR_DIVIDE25(k); 
       BODY_FOR_DIVIDE25(k+1); 
       BODY_FOR_DIVIDE25(k+2); 
       BODY_FOR_DIVIDE25(k+3); 
    }                                           
		for(;k <= N4; k++) BODY_FOR_DIVIDE25(k);
}
//Dividir entre 25 es dividir entre 5 dos cops
void DIVIDE_5( signed char *x)                           
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    r = 0;                                       
    for( k = 0; k <= N4; k+=4 )                  
    {                                            
       BODY_FOR_DIVIDE5(k); 
       BODY_FOR_DIVIDE5(k+1); 
       BODY_FOR_DIVIDE5(k+2); 
       BODY_FOR_DIVIDE5(k+3); 
    }                                           
		for(;k <= N4; k++) BODY_FOR_DIVIDE5(k);
}

void LONGDIV( signed char *x, int n )                          
{                                                
    int j, k;
    unsigned q, r, u;
    long v;

    if( n < 6553 )                               
    {                                            
        r = 0;                                   
        for( k = 0; k <= N4; k++ )               
        {                                        
            u = r * 10 + x[k];                   
            q = u / n;                           
            r = u - q * n;                       
            x[k] = q;                            
        }                                       
    }                                            
    else                                         
    {                                            
        r = 0;                                   
        for( k = 0; k <= N4; k++ )              
        {                                       
            if( r < 6553 )                      
            {                                   
                u = r * 10 + x[k];              
                q = u / n;                      
                r = u - q * n;                  
            }                                   
            else                                
            {                                   
                v = (long) r * 10 + x[k];       
                q = v / n;                      
                r = v - q * n;                  
            }                                   
            x[k] = q;                           
        }                                       
    }                                           
}

void MULTIPLY( signed char *x, int n )                        
{                                            
    int j, k;
    unsigned q, r, u;
    long v;
    r = 0;                                   
    for( k = N4; k >= 0; k-- )               
    {                                        
        q = n * x[k] + r;                    
        r = q / 10;                          
        x[k] = q - r * 10;                   
    }                                        
}

void SET( signed char *x, int n )                              
{                                                
    memset( x, 0, N4 + 1 );                      
    x[0] = n;                                    
}


void SUBTRACT( signed char *x, signed char *y, signed char *z )                      
{                                                
    int j, k;
    unsigned q, r, u;
    long v;
    for( k = N4; k >= 1; k-- )                   
    {                                            
        if( (x[k] = y[k] - z[k]) < 0 )           
        {                                        
            x[k] += 10;                          
            z[k-1]++;                            
        }                                        
    }                                            
    if( (x[k] = y[k] - z[k]) < 0 )           
    {                                        
        x[k] += 10;                          
    }                                        
}


void calculate( void );
void progress( void );
void epilog( void );


int main( int argc, char *argv[] )
{
    N = 10000;
    
    if( argc > 1 )
        N = atoi(argv[1]);
    init_memo(memo_q5,memo_r5,5);
    init_memo(memo_q25,memo_r25,25);
    init_memo(memo_q239,memo_r239,239);
    setbuf(stdout, NULL);

    calculate();

    epilog();

    return 0;
}

void calculate( void )
{
    int j;

    N4 = N + 4;

    SET( a, 0 );
    SET( b, 0 );

    for( j = 2 * N4 + 1; j >= 3; j -= 8 )
    {
			BODY_FOR_CALCULATE(j)
			BODY_FOR_CALCULATE(j-2)
			BODY_FOR_CALCULATE(j-4)
			BODY_FOR_CALCULATE(j-6)
    }

    SET( c, 1 );

    SUBTRACT( a, c, a );
    //DIVIDE( a, 5 );
    DIVIDE_5( a );

    SUBTRACT( b, c, b );
    //DIVIDE( b, 239 );
    DIVIDE_239( b );

    MULTIPLY( a, 4 );
    SUBTRACT( a, a, b );
    MULTIPLY( a, 4 );

    progress();
}

/*

 N = 10000                      
 A = 0                          
 B = 0                          
 J = 2 * (N + 4) + 1            
 FOR J = J TO 3 STEP -2         
     A = (1 / J - A) / 5 ^ 2    
     B = (1 / J - B) / 239 ^ 2  
 NEXT J                         
 A = (1 - A) / 5                
 B = (1 - B) / 239              
 PI = (A * 4 - B) * 4           

*/

void progress( void )
{
    printf(".");
}

void epilog( void )
{
    int j;

    {
        fprintf( stdout, " \n3.");
        for( j = 1; j <= N; j++ )
        {
            fprintf( stdout, "%d", a[j]);
            if( j % 5  == 0 )
                if( j % 50 == 0 )
                    if( j % 250  == 0 )
                        fprintf( stdout, "    <%d>\n\n   ", j );
                    else
                        fprintf( stdout, "\n   " );
                else
                    fprintf( stdout, " " );
        }
    }
}

