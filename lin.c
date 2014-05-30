/*****************************************
Sameer Ashok Sondur
This is the file whose trace file is to be generated.
******************************************
*/

#include <stdio.h>
#include <stdlib.h>

void determinant(int a[3][3],int *deter)//15reads and 1 write
{
*deter=a[0][0]*((a[1][1]*a[2][2])-(a[2][1]*a[1][2]))+(-1)*(a[0][1]*((a[1][0]*a[2][2])-(a[1][2]*a[2][0])))+a[0][2]*((a[1][0]*a[2][1])-(a[1][1]*a[2][0]));
}
///////////////////////////////////////////////////////
void main()
{
int a[3][3],tmp[3][3],b[3],deter,deterx,detery,deterz,i,j,x,y,z,aaa;
b[0]=15;
b[1]=7;	
b[2]=-9;	
a[0][0]=8;	
a[0][1]=-7;	
a[0][2]=10;	
a[1][0]=2;	
a[1][1]=3;	
a[1][2]=8;	
a[2][0]=-4;	
a[2][1]=5;	
a[2][2]=-2;	
determinant(a,&deter);	
for(i=0;i<3;i++)
	{
	for(j=0;j<3;j++)
	{	
		tmp[i][j]=a[i][j];	
	}
	}
for(i=0;i<3;i++)
	{
		tmp[i][0]=b[i];		
	}
determinant(tmp,&deterx);
////////////////////////////////////////////////////////////////////////////////////////
for(i=0;i<3;i++)
	{
	for(j=0;j<3;j++)
	{
		tmp[i][j]=a[i][j];
	}
	}
for(i=0;i<3;i++)
	{
		tmp[i][1]=b[i];
	}
determinant(tmp,&detery);
/////////////////////////////////////////////////////////////////////
for(i=0;i<3;i++)
	{
	for(j=0;j<3;j++)
	{
		tmp[i][j]=a[i][j];
	}
	}
for(i=0;i<3;i++)
	{
		tmp[i][2]=b[i];
	}
determinant(tmp,&deterz);
x=deterx/deter;		
y=detery/deter;		
z=deterz/deter;		
}
