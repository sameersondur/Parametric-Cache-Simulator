/*****************************************
Sameer Ashok Sondur
This file takes in the cache sizes and passes them to the appropriate functions in 
the file d1.c which calculate the misses and hits.

*/
#include <stdio.h>
#include <stdlib.h>



//function declarations
void binary(long unsigned int a, int m);
int m2convert(long unsigned int j);


//global memory declaration
long unsigned int mainm,l1size,vicsize,assot,blksize,wripy;//mainm=main mem size,l1size=l1 cache size,victim=victim cache blocks,assot=associtivity
//wripy=write policy,blksize=blocksize
long unsigned int *caind,*catag,*victag,*vicind,*valca,*valvic,*lruca;
unsigned long int rhitc=0,rmissc=0,rhitv=0,rmissv=0,lruvic=0,whitc=0,wmissc=0,whitv=0,wmissv=0;
long unsigned int *po,*rwa,ix=0,numaccess;//rwa::0=read and :1=write || numaccess: number of accesses
//////////////////////////////////////////
//Considereing the 64 bit machine

void main()
{
//variables
int j,i;
unsigned long int k=1,sizindex,noblk,index; 
float l1mr,vmr=0;
//end of variables
//taking input parameters
l1ss:;
printf("\nEnter the cache size in bytes:\n");
scanf("%lu",&l1size);
if(!(pow2(l1size)))
{
	printf("\n****Entered number is not in powers of 2.********\n");
	goto l1ss;
}
//////////////////////////////////////////////
l9ss:;
printf("\nEnter the associativity in the powers of 2: \n");
scanf("%lu",&assot);
if(!(pow2(assot)))
{
	printf("\n****Entered number is not in powers of 2.********\n");
	goto l9ss;
}
///////////////////////////////////////////////
l3ss:;
printf("\nEnter the block size in the powers of 2 : \n");
scanf("%lu",&blksize);
if(!(pow2(blksize)))
{
	printf("\n****Entered number is not in powers of 2.********\n");
	goto l3ss;
}
noblk=l1size/blksize;
index=noblk/assot;
sizindex=m2convert(noblk/assot);
blksize=m2convert(blksize);
/////////////////////////////////////////////////////////
printf("\nEnter:\n0 for Write Back with Write Allocate policy.\n1 for Write through with no Write Allocate policy.\n");
scanf("%lu",&wripy);
/////////////////////////////////////////////
printf("\nEnter the number of victim cache blocks.\n");
scanf("%lu",&vicsize);
/////////////////////////////////////////////
inputterms( &po, &rwa , &numaccess );
mainm=64;
caind=calloc(noblk,sizeof(long unsigned int));
catag=calloc(noblk,sizeof(long unsigned int));
victag=calloc(vicsize,sizeof(long unsigned int));
vicind=calloc(vicsize,sizeof(long unsigned int));
valca=calloc(noblk,sizeof(long unsigned int));
valvic=calloc(vicsize,sizeof(long unsigned int));
lruca=calloc(index,sizeof(long unsigned int));
for(i=0;i<noblk;i++)
{
caind[i]=i/assot;
}
for(i=0;i<numaccess;i++)
{ 
	if(rwa[i]==1)
	{whitmiss(catag,caind,victag,vicind,valca,valvic,po[i],(blksize+sizindex),sizindex,blksize,64,assot,vicsize,wripy,4,1);	
		
	}
	else
	{rhitmiss(catag,caind,victag,vicind,valca,valvic,po[i],(blksize+sizindex),sizindex,blksize,64,assot,vicsize,4,1);	
		
	}
}
l1mr=(float)(rmissc+wmissc)/(wmissc+whitc+rhitc+rmissc);
if(vicsize!=0)
vmr=(float)(rmissv+wmissv)/(wmissv+whitv+rhitv+rmissv);
printf("-----------------------\n------------------------\n-----------------------\n=====  Simulation Report ======\n------------------------\n-----------------------\n");
printf("\n\nNumber of L1 reads: %lu\nNumber of L1 read misses: %lu\nNumber of Victim Cache Reads: %lu\nNumber of victim cache read misses: %lu\nNumber of L1 writes: %lu\nNumber of L1 write misses %lu\nNumber of victim cache writes: %lu\nNumber of victim cache write misses: %lu\nL1 miss rate %f\nVictim cache miss rate %f\nTotal memory traffic to/from main memory: %lu\n-------------------\n-----------------\n",(rhitc+rmissc),rmissc,(rhitv+rmissv),rmissv,(whitc+wmissc),wmissc,(wmissv+whitv),wmissv,l1mr,vmr,numaccess);
////////freeing 
free(caind);
free(catag);
free(victag);
free(vicind);
free(valca);
free(valvic);
free(lruca);
free(po);
free(rwa);
//free(lruvic);
}


void binary(long unsigned int a,int m)
{long unsigned int i,j;
for(i=0;i<m;i++)
{
	j=((a>>(m-1-i))&0x01);
	printf("%lu",j);
}
}

int m2convert(long unsigned int j)
{int i;
for(i=0;i<32;i++)
{
	if(j==(1<<i))
		return i;
}
}

int pow2(long unsigned int x)
{
long unsigned int xx;
xx=x-1;
if((xx && x)==0)
return 0;
else
return 1;
}
