/*****************************************
Sameer Ashok Sondur
This is the file which gets the addresses and read/write data
 from the trace file.
******************************************
*/

#include <stdio.h>
#include <stdlib.h>

typedef long unsigned int luint;
 
void inputterms( luint *po, luint *rwa, luint *numaccess)
{
char array1[10],array2[2],*arr;
unsigned long int c,tmp=0,it=0,*pointers;
luint *rw,*addr;
luint i=0;
FILE *fp;
fp = fopen("tracefile", "r");
c= getc(fp);
while(c != EOF)
{
	if(c==10)
		{i++;}
	c= getc(fp);
}
printf("\nThe number of accesses is %lu\n",i );// This is the munber of accesses
*numaccess=i;
fclose(fp);
addr=(luint*)calloc(i,sizeof(luint));
rw=(luint*)calloc(i,sizeof(luint));
if(addr==0 || rw==0)	//If the dynamic Memory allocation fails.
{printf("could not allocate memory!!\n"); 	exit(1);}
fp = fopen("tracefile", "r");
i=0;
c= 10;
while(1)
{
	if(c==32)	// If the character is a space
	{
		c=getc(fp);
		it=0;
		while(c!=10)
		{
			array1[it]=	(unsigned char)c;
			c=getc(fp);
			it++;
		}
		array1[it]='\0';
		addr[i]=strtoul(array1, NULL, 16);	// Store the address as a decimal
		i++;
		
	}
	if( c==10)	// If the character is line feed
	{
		c=getc(fp);
		if(c==EOF)	break;	
		it=0;
		while(c!=32)
		{
			array2[it]=	(unsigned char)c;
			c=getc(fp);	
			it++;
		}	
		array2[it]='\0';
		if(array2[0]=='S')
			rw[i]=1;
		else
			rw[i]=0;
		tmp++;		
	}	
}
*po=addr;
*rwa=rw;
}
