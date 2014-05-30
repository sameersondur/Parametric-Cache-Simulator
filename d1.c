/*****************************************
Sameer Ashok Sondur
This File is the heart of all the functions which are used to calculate the hits and
misses.
******************************************
*/
#include <stdio.h>

/////variables
extern unsigned long int rhitc,rmissc,rhitv,rmissv,whitc,wmissc,whitv,wmissv;
extern long unsigned int *lruca;
extern unsigned long int lruvic;


///////////////

//////function declarations
long unsigned int checkindexvalid(long unsigned int addr,long unsigned int *valid,long unsigned int sizindex,long unsigned int linesiz,long unsigned int assot);
long unsigned int retindex(long unsigned int addr,long unsigned int sizindex,long unsigned int linesiz);
long unsigned int rettag(long unsigned int addr,long unsigned int caln,int mem);
long unsigned int checktag(long unsigned int index,long unsigned int tag,long unsigned int *catag,long unsigned int assot);
int checktagvic(long unsigned int tag,long unsigned int index,long unsigned int *victag,long unsigned int *vicind,int vicsize);
int replal1vic(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int cind,long unsigned int assot,long unsigned int vicsize,long unsigned int tag);
long unsigned int replacementl1(long unsigned int *valca,long unsigned int index,int assot);
long unsigned int replacementvi(long unsigned int *valvi,long unsigned int vicsize);
int loadblock(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int vicsize,long unsigned int addr,long unsigned int sizindex,long unsigned int linesiz,long unsigned int caln,int mem,int assot);

int checkindvall1(long unsigned int *valca,long unsigned int index,long unsigned int assot);
int checkvicemp(long unsigned int *valvic,long unsigned int vicsize);
int rhitmiss(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int bytes,long unsigned int rn);
int checktagvicn(long unsigned int tag,long unsigned int index,long unsigned int *victag,long unsigned int *vicind,long unsigned int vicsize,long unsigned int n);
int whichline(long unsigned int index,long unsigned int tag,long unsigned int *catag,long unsigned int assot);
long unsigned int checktagindex(long unsigned int index,long unsigned int tag,long unsigned int *catag,long unsigned int assot);
int searchvic(long unsigned int ii,long unsigned int *valvic,long unsigned int vicsize);
void doproperlruvic(long unsigned int caught,long unsigned int vicsize,long unsigned int *valvic );
int whitmiss(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int wripy,long unsigned int bytes,long unsigned int rn);
int loadblk0(long unsigned int *catag,long unsigned int *valca,long unsigned int vicsize,long unsigned int addr,long unsigned int sizindex,long unsigned int linesiz,long unsigned int caln,int mem,int assot);
void apprptlru0(long unsigned int tmp1,long unsigned int *valca,long unsigned int assot,long unsigned int index );

void checknobytes(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int bytes,long unsigned int rn,long unsigned int index,long unsigned int tag);

void checknobytesw(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int wripy,long unsigned int bytes,long unsigned int rn,long unsigned int index,long unsigned int tag);

//////////////

////////check whether the index has valid bits and returns the number of valid bits in an associative mapping
long unsigned int checkindexvalid(long unsigned int addr,long unsigned int *valid ,long unsigned int sizindex,long unsigned int linesiz,long unsigned int assot)
{
int ii,i,j=0;
ii=retindex(addr,sizindex,linesiz);
for(i=0;i<assot;i++)
{
	if((valid[ii*assot+i]&0x80)==0x80)
		j++;
}
return j;
}

long unsigned int retindex(long unsigned int addr,long unsigned int sizindex,long unsigned int linesiz)
{
long unsigned int i,j=1,k=1;
for(i=0;i<sizindex+linesiz-1;i++)
{
	j=(j<<1)+1;
}

for(i=0;i<linesiz-1;i++)
{
	k=(k<<1)+1;
}
k=k^0xFFFFFFFF;

j=j&k;
j=addr & j;
j>>=linesiz;

return j;

}

//////////return tag
long unsigned int rettag(long unsigned int addr,long unsigned int caln,int mem)//mem=size of the memory bits,caln=totalcache size in powrs of 2
{	long unsigned int a;
	int j=1,mask;
	mask=(j<<(mem-1))>>(mem-caln-1);
	
	a=addr & mask;
	
	a>>=(caln);
	
	return a;
}

////////check whether "tag" is there in the cache with index "index" return 1=tag there :0=not there
long unsigned int checktag(long unsigned int index,long unsigned int tag,long unsigned int *catag,long unsigned int assot)
{
	int i;
	
	for(i=0;i<assot;i++)
		{
			if(tag==catag[index*assot+i])
				return 1;
		}
	return 0;
}

//////check tag in victim
int checktagvic(long unsigned int tag,long unsigned int index,long unsigned int *victag,long unsigned int *vicind,int vicsize)
{
	long unsigned int tgin=0,i;
	for (i=0;i<vicsize;i++)
		{
			if((tag==victag[i])&&(index==vicind[i]))
				return 1;		
		}
	return 0;
}

int replal1vic(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int cind,long unsigned int assot,long unsigned int vicsize,long unsigned int tag)//replace/swap the cind index from cache to an appropriate place in victim tag =the tag of address which is not found in the l1 but found in vic --cind=index of that address sobefore ruunig dis function run the above function
{
	int rl1,rvi,i;
	long unsigned int tmp1,tmp2;
	rl1=replacementl1(valca,cind,assot);
	
	for (i=0;i<vicsize;i++)
	{
		if((victag[i]==tag)&&(vicind[i]==cind))
		{
			break;		
		}
	}
	tmp1=catag[cind*assot+rl1];
	catag[cind*assot+rl1]=victag[i];
	victag[i]=tmp1;
	tmp1=caind[cind*assot+rl1];
	caind[cind*assot+rl1]=vicind[i];
	vicind[i]=tmp1;
	return i;
}


long unsigned int replacementl1(long unsigned int *valca,long unsigned int index,int assot)//to check and return the appropriate value in the indices of l1
{
	long unsigned int cinr,i;
	if(assot==1)
	{
		return 0;
	}
	for(i=0;i<assot;i++)
			{
				if(((valca[index*assot+i]&0x80)==0))
					return i;
			}
	for(i=0;i<assot;i++)
		{
			if((valca[index*assot+i]&0x7F)==0)
				return i;
		}	
	 
}

long unsigned int replacementvi(long unsigned int *valvi,long unsigned int vicsize)//to check and return the appropriate value in the indices of victim
{
	int i;
	for(i=0;i<vicsize;i++)
		{
			if((valvi[i]&0x80)==0)
				return i;
		}
	for(i=0;i<vicsize;i++)
	{
		if((valvi[i]&0x7F)==0)
		{
			return i;
		}
	}


}

int loadblock(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int vicsize,long unsigned int addr,long unsigned int sizindex,long unsigned int linesiz,long unsigned int caln,int mem,int assot)//load a block from main mem and replace appropriate place in l1 with the replaced line in the in the victim /addr=address in main mem to be loaded in cache
{
	long unsigned int i,j,cind,tag;
	cind=retindex(addr,sizindex,linesiz);
	
	i= replacementvi(valvic,vicsize);
	j=replacementl1(valca,cind,assot);
	tag=rettag(addr,caln,mem);
	
	victag[i]=catag[cind*assot+j];
	vicind[i]=caind[cind*assot+j];
	catag[cind*assot+j]=tag;
	caind[cind*assot+j]=cind;
	return i;
		
}

int loadblk0(long unsigned int *catag,long unsigned int *valca,long unsigned int vicsize,long unsigned int addr,long unsigned int sizindex,long unsigned int linesiz,long unsigned int caln,int mem,int assot)
{
	long unsigned int i,j,cind,tag;
	cind=retindex(addr,sizindex,linesiz);
	j=replacementl1(valca,cind,assot);
	catag[cind*assot+j]=tag;
	return j;

}


int checkindvall1(long unsigned int *valca,long unsigned int index,long unsigned int assot)//return the invalid lines return 999:if there are no invalid(full) :::::(0,1,2,3) if that line is invalid not invalid
{
	int i;
	for(i=0;i<assot;i++)
	{
		if((valca[assot*index+i]&0x80)==0x00)
			return i;
	}
	return 999;
}

int checkvicemp(long unsigned int *valvic,long unsigned int vicsize)//return the invalid lines return 999:if there are no invalid :::::(0,1,2,3) if that line is invalid not invalid
{
	int i;
	for(i=0;i<vicsize;i++)
	{
		if((valvic[i]&0x80)==0x00)
			return i;
	}
	return 999;
}

int rhitmiss(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int bytes,long unsigned int rn)
{
	long unsigned int index,tag,vl1,n,tmp1,tmp2,ii;
	index=retindex(addr,sizindex,linesiz);
	tag=rettag(addr,caln,mem);
	if(rn==1)
	checknobytes(catag, caind, victag, vicind, valca, valvic, addr, caln, sizindex, linesiz, mem, assot, vicsize,1,rn,index,tag);
	vl1=checkindvall1(valca,index,assot);
	if(vl1==999)//if cacahe block is full with all valid
		{	
			if(checktag(index,tag,catag,assot))//hit in l1
				{
					rhitc++;
					tmp1=whichline(index,tag,catag,assot);
					apprptlru(tmp1,valca,assot,index);					

					
					
					
				}
			else					//miss in l1
				{	
					rmissc++;
					if(vicsize!=0)
					{
					if(checkvicemp(valvic,vicsize)==999)//full
						{
							if(checktagvic(tag,index,victag,vicind,vicsize))//is that line in victim:yes
								{
									rhitv++;
									tmp2=replal1vic(catag,caind,victag,vicind,valca,valvic,index,assot,vicsize,tag);
									tmp1=whichline(index,tag,catag,assot);
									
									apprptlru(tmp1,valca,assot,index);
									doproperlruvic(tmp2,vicsize,valvic);

								}
							else	//not dere in victim
								{
									rmissv++;
									tmp2=loadblock(catag,caind,victag,vicind,valca,valvic,vicsize,addr,sizindex,linesiz,caln,mem,assot);
									tmp1=whichline(index,tag,catag,assot);
									
									apprptlru(tmp1,valca,assot,index);
									doproperlruvic(tmp2,vicsize,valvic);
								}							
						
						}
					else	//not full
						{
							n=checkvicemp(valvic,vicsize);
							if(checktagvicn(tag,index,victag,vicind,vicsize,n)==1)//yes present
								{
									rhitv++;
									tmp2=replal1vic(catag,caind,victag,vicind,valca,valvic,index,assot,vicsize,tag);
									tmp1=whichline(index,tag,catag,assot);
									apprptlru(tmp1,valca,assot,index);
									
									doproperlruvic0(tmp2,vicsize,valvic);
								}
							else							//not present
								{
									rmissv++;
									tmp2=loadblock(catag,caind,victag,vicind,valca,valvic,vicsize,addr,sizindex,linesiz,caln,mem,assot);
									tmp1=whichline(index,tag,catag,assot);
									valca[index*assot+tmp1]=(lruca[index]%assot)+0x80;
									lruca[index]++;
									if(lruca[index]==assot)
										lruca[index]==0;
									valvic[tmp2]=(lruvic % vicsize)+0x80;
									lruvic++;
									if(lruvic==vicsize)
										lruvic=0;
									apprptlru(tmp1,valca,assot,index);
									
								}
						}
					}
				else
					{
					tmp1=loadblk0(catag,valca,vicsize,addr,sizindex,linesiz,caln,mem,assot);
					apprptlru(tmp1,valca,assot,index);
					}		
				}
		}
	
	else
	{
		if(checktag(index,tag,catag,assot))//hit in l1
		{
			rhitc++;
			
//make the valid bits perfect
			tmp1=whichline(index,tag,catag,assot);
			apprptlru0(tmp1,valca,assot,index);		
		}
		else//load the 
		{	
			rmissc++;
			if(vicsize!=0)
			rmissv++;
			catag[index*assot+vl1]=tag;
			caind[index*assot+vl1]=index;
			tmp1=whichline(index,tag,catag,assot);
			
			valca[index*assot+tmp1]=(lruca[index]%assot)+0x80;
			lruca[index]++;
			if(lruca[index]==assot)
				lruca[index]=0;
			
					
		}
	}


}



//if some lines atr not valid then check till when it is valid i.e suffix n
int checktagvicn(long unsigned int tag,long unsigned int index,long unsigned int *victag,long unsigned int *vicind,long unsigned int vicsize,long unsigned int n)
{
	long unsigned int tgin=0,i;
	for (i=0;i<n;i++)
		{
			if((tag==victag[i])&&(index==vicind[i]))
				return 1;		
		}
	return 0;
}

int whichline(long unsigned int index,long unsigned int tag,long unsigned int *catag,long unsigned int assot)//which line is replaced in the block in cache
{
	int i;
	
	for(i=0;i<assot;i++)
		{
			if(tag==catag[index*assot+i])
				return i;
		}
	
}

int searchval(long unsigned int ii,long unsigned int *valca,long unsigned int assot,long unsigned int index)
{
	 int i;
	for(i=0;i<assot;i++)
	{
		if((valca[index*assot+i]&0x7F)==ii)
			{return i;}
			
	}

}

void apprptlru(long unsigned int tmp1,long unsigned int *valca,long unsigned int assot,long unsigned int index )//adjust the lru when all cache lines are full
{
	long unsigned int i,ddd;
	
	for(i=assot-1;i>tmp1;i--)
	{	
		ddd=searchval(i,valca,assot,index);
		
		(valca[index*assot+ddd])=(valca[index*assot+ddd])--;
	}
	valca[index*assot+tmp1]=0x83;
}


void apprptlru0(long unsigned int tmp1,long unsigned int *valca,long unsigned int assot,long unsigned int index )//same as above except wen all lines are not filled
{
	long unsigned int i,ddd,j=0,tmp2;
	tmp2=(valca[index*assot+tmp1]&0x7F);
	for(i=assot-1;i>tmp2;i--)
	{	
		ddd=searchval(i,valca,assot,index);
		if(ddd!=assot)
		{
			j++;
		(valca[index*assot+ddd])=(valca[index*assot+ddd])--;}
	}
	if((valca[index*assot+tmp1]&0x7F)!=(assot-1))
	valca[index*assot+tmp1]+=j;
}



long unsigned int checktagindex(long unsigned int index,long unsigned int tag,long unsigned int *catag,long unsigned int assot)//check the index of the tag in the associtivity
{
	int i;
	
	for(i=0;i<assot;i++)
		{
			if(tag==(catag[index*assot+i]))
				return i;
		}
	return 0;
}


int searchvic(long unsigned int ii,long unsigned int *valvic,long unsigned int vicsize)//search which index in victim has the lru number as ii
{
	long unsigned int i;
	for(i=0;i<vicsize;i++)
	{
		if((valvic[i]&0x7F)==ii)
			return i;	
	}

}


void doproperlruvic(long unsigned int caught,long unsigned int vicsize,long unsigned int *valvic )//fix the number valid bits in proper manner
{
	int i,tmp;
	for(i=vicsize-1;i>caught;i--)
		{
			tmp=searchvic(i,valvic,vicsize);
			(valvic[tmp])--;	
		}
	valvic[caught]=0x83;
}

void doproperlruvic0(long unsigned int caught,long unsigned int vicsize,long unsigned int *valvic )//fix the number valid bits in proper manner
{
	
	long unsigned int i,ddd,j=0,tmp2;
	tmp2=(valvic[caught]&0x7F);
	for(i=vicsize-1;i>tmp2;i--)
	{	
		ddd=searchvic(i,valvic,vicsize);
		if(ddd!=vicsize)
		{
			j++;
		(valvic[ddd])=(valvic[ddd])--;}
	}
	if((valvic[caught]&0x7F)!=(vicsize-1))
	valvic[tmp2]+=j;
}

//////////////////////////////writing////////////////////////////////////////////////////////////////
int whitmiss(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int wripy,long unsigned int bytes,long unsigned int rn)
{
	long unsigned int index,tag,vl1,n,tmp1,tmp2,ii;
	index=retindex(addr,sizindex,linesiz);
	tag=rettag(addr,caln,mem);
	if(rn==1)
	checknobytesw(catag, caind, victag, vicind, valca, valvic, addr, caln, sizindex, linesiz, mem, assot, vicsize,wripy,bytes,0,index,tag);
	vl1=checkindvall1(valca,index,assot);
	if(wripy==0)//WB with allocate
	{
		if(vl1==999)//if cacahe block is full with all valid
		{
			if(checktag(index,tag,catag,assot))//hit in l1
			{
				whitc++;
				tmp1=whichline(index,tag,catag,assot);
				apprptlru(tmp1,valca,assot,index);
			}
			else
			{
				wmissc++;
				if(vicsize!=0)
				{if(checkvicemp(valvic,vicsize)==999)//full
				{
					if(checktagvic(tag,index,victag,vicind,vicsize))//is that line in victim:yes
								{
									whitv++;
									tmp2=replal1vic(catag,caind,victag,vicind,valca,valvic,index,assot,vicsize,tag);
									tmp1=whichline(index,tag,catag,assot);
									apprptlru(tmp1,valca,assot,index);
									doproperlruvic(tmp2,vicsize,valvic);

								}
					else	//not dere in victim
								{
									wmissv++;
									tmp2=loadblock(catag,caind,victag,vicind,valca,valvic,vicsize,addr,sizindex,linesiz,caln,mem,assot);
									tmp1=whichline(index,tag,catag,assot);
									apprptlru(tmp1,valca,assot,index);
									doproperlruvic(tmp2,vicsize,valvic);
								}	
				}
				else
				{
					n=checkvicemp(valvic,vicsize);
							if(checktagvicn(tag,index,victag,vicind,vicsize,n)==1)//yes present
								{
									whitv++;
									tmp2=replal1vic(catag,caind,victag,vicind,valca,valvic,index,assot,vicsize,tag);
									tmp1=whichline(index,tag,catag,assot);
									apprptlru(tmp1,valca,assot,index);
									doproperlruvic0(tmp2,vicsize,valvic);
								}
							else							//not present
								{
									wmissv++;
									tmp2=loadblock(catag,caind,victag,vicind,valca,valvic,vicsize,addr,sizindex,linesiz,caln,mem,assot);
									tmp1=whichline(index,tag,catag,assot);
									//apprptlru(tmp1,valca,assot,index);
									valca[index*assot+tmp1]=(lruca[index]%assot)+0x80;
									lruca[index]++;
									if(lruca[index]==assot)
										lruca[index]==0;
									valvic[tmp2]=(lruvic % vicsize)+0x80;
									lruvic++;
									if(lruvic==vicsize)
										lruvic=0;
									apprptlru(tmp1,valca,assot,index);									
									//doproperlruvic0(tmp2,vicsize,valvic);
								}	
				}
			}
			else
			{
			tmp1=loadblk0(catag,valca,vicsize,addr,sizindex,linesiz,caln,mem,assot);
					apprptlru(tmp1,valca,assot,index);	
			}	
			}	
		}
		

		else
		{
		if(checktag(index,tag,catag,assot))//hit in l1
			{
			whitc++;
			
//make the valid bits perfect
			tmp1=whichline(index,tag,catag,assot);
			apprptlru0(tmp1,valca,assot,index);		
			}
		else//load the 
			{	
			wmissc++;
			if(vicsize!=0)
			wmissv++;
			catag[index*assot+vl1]=tag;
			caind[index*assot+vl1]=index;
			tmp1=whichline(index,tag,catag,assot);
			
			valca[index*assot+tmp1]=(lruca[index]%assot)+0x80;//doubtful
			lruca[index]++;
			if(lruca[index]==assot)
				lruca[index]=0;
			
					
			}
		}


	
	}
	else
	{
		if(checktag(index,tag,catag,assot))//hit in l1
			{
				whitc++;
				
				if(vl1==999)
				{tmp1=whichline(index,tag,catag,assot);
				apprptlru(tmp1,valca,assot,index);}
				else
				{
				tmp1=whichline(index,tag,catag,assot);
				apprptlru0(tmp1,valca,assot,index);
				}
			}
		else
		{
			wmissc++;///
		}

	}


}



void checknobytes(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int bytes,long unsigned int rn,long unsigned int index,long unsigned int tag)
{
	long unsigned int ind,tagg;
	addr+=(bytes-1);
	ind=retindex(addr,sizindex,linesiz);
	tagg=rettag(addr,caln,mem);
	if((index!=ind)&&(rn=1))
		rhitmiss( catag, caind, victag, vicind, valca, valvic, addr, caln, sizindex, linesiz, mem, assot, vicsize,1,0);
		


}



void checknobytesw(long unsigned int *catag,long unsigned int *caind,long unsigned int *victag,long unsigned int *vicind,long unsigned int *valca,long unsigned int *valvic,long unsigned int addr,long unsigned int caln,long unsigned int sizindex,long unsigned int linesiz,long unsigned int mem,long unsigned int assot,long unsigned int vicsize,long unsigned int wripy,long unsigned int bytes,long unsigned int rn,long unsigned int index,long unsigned int tag)
{
	long unsigned int ind,tagg;
	addr+=(bytes-1);
	ind=retindex(addr,sizindex,linesiz);
	tagg=rettag(addr,caln,mem);
	if((index!=ind)&&(rn=1))
		whitmiss( catag, caind, victag, vicind, valca, valvic, addr, caln, sizindex, linesiz, mem, assot, vicsize,wripy,1,0);
		


}

