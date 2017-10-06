#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <list>
#include <time.h>
using namespace std;

class Vertex	//頂點結構
{
public:
	double x,y,z;
	int index;
    Vertex( double px=0,double py=0,double pz=0 ):x(px),y(py),z(pz),index(0){}
	bool operator==( const Vertex &k )	{ return (this->x == k.x && this->y == k.y && this->z == k.z ); }
}P0;
istream& operator>>( istream &is , Vertex &k )	{ return is >> k.x >> k.y >> k.z; }
ostream& operator<<( ostream &os , Vertex k )	{ return os << "( " << k.x << " , " << k.y << " , " << k.z << " )"; }

class Face	//面結構
{
public:
	Vertex i,j,k;		//三點
	int vicinity[3];	//附近的面編號
	int vicinity_count;
	int index;
    bool check;			//檢測是否尋訪過
	Face( Vertex a=P0 , Vertex b=P0 , Vertex c=P0 ):i(a),j(b),k(c),check(0),vicinity_count(0){}
	bool operator==( const Face &k )
	    {
		if ( (this->i == k.i && this->j == k.j && this->k == k.k ) ||
		     (this->i == k.i && this->j == k.k && this->k == k.j ) ||
		     (this->i == k.j && this->j == k.i && this->k == k.k ) ||
		     (this->i == k.j && this->j == k.k && this->k == k.i ) ||
		     (this->i == k.k && this->j == k.i && this->k == k.j ) ||
		     (this->i == k.k && this->j == k.j && this->k == k.i )  )
		    return true;
		return false;
		}
};
ostream& operator<<( ostream &os , Face a )	{ return os << "i= " << a.i << " j= " << a.j << " k= " << a.k; }

string ToBinary ( int a )  //數字轉8bit二進位
    {
    string temp="";
	while ( a != 0 )
	    {
		temp += a%2+'0';
		a /= 2;
		}
	reverse ( temp.begin() , temp.end() );
	while ( temp.size() < 8 )
	    temp = "0" + temp;
	return temp;
	}

vector <int> viTraversal;	//面的尋訪序列
vector <Vertex> vVertex(1);	//儲存obj中的頂點 (第0項為空點(0,0,0) )
vector <Face> vFace;		//儲存obj中的面
vector <Face> vFace_sv;		//set vicinity

void Traverse ( int iStart , int sM_size , int p )
    {
    queue <Face> qFace;            //BFS
    string sStart;                 //起始面之二進制
    sStart = ToBinary( iStart );
    
    qFace.push( vFace[iStart] );   //放入起始面
    viTraversal.push_back(iStart); //加入尋訪序列
    vFace[iStart].check = 1;

	bool bOutward = 0 , bRotate = 0; // 是否往外擴張 /  順時or逆時
	int iNow = 0;
	int iFaceSize = vFace.size();
    ++sM_size;
	while ( !qFace.empty() )
	    {    
	    if ( iNow == sStart.size() )  //到底重頭
	        iNow = 0;

	    bOutward = 0;                 //設定
		bRotate = sStart[ iNow ] -'0';

		Face temp = qFace.front();
		qFace.pop();
		Face next_ij = vFace[ temp.vicinity[0] ];	//以該三角形之三邊  分別創建 外圍三角形
		Face next_ik = vFace[ temp.vicinity[1] ];
		Face next_jk = vFace[ temp.vicinity[2] ];

		for ( int x=0;x<iFaceSize;x++ )           // ij
		    if ( viTraversal.size() <= sM_size*p && !vFace[x].check && vFace[x] == next_ij  )
	            {
				qFace.push( vFace[x] );
				viTraversal.push_back(x);         //放入尋訪序列
				vFace[x].check = 1;
				bOutward = 1;
				break;
				}
		if ( bRotate == 0 )  //順時
		    {
			for ( int x=0;x<iFaceSize;x++ )  // ik
		        if ( viTraversal.size() <= sM_size*p && !vFace[x].check && vFace[x] == next_ik )
	                {
				    qFace.push( vFace[x] );
				    viTraversal.push_back(x);
				    vFace[x].check = 1;
				    bOutward = 1;
				    break;
				    }
		    for ( int x=0;x<iFaceSize;x++ )  //jk
		            if ( viTraversal.size() <= sM_size*p && !vFace[x].check && vFace[x] == next_jk )
	                {
				    qFace.push( vFace[x] );
				    viTraversal.push_back(x);
			    	vFace[x].check = 1;
				    bOutward = 1;
				    break;
				    }
			}
		else if ( bRotate == 1 ) //逆時
		    {
		    for ( int x=0;x<iFaceSize;x++ ) //jk
		            if ( viTraversal.size() <= sM_size*p && !vFace[x].check && vFace[x] == next_jk )
	                {
				    qFace.push( vFace[x] );
				    viTraversal.push_back(x);
			    	vFace[x].check = 1;
				    bOutward = 1;
				    break;
				    }
			for ( int x=0;x<iFaceSize;x++ ) //ik
		        if ( viTraversal.size() <= sM_size*p && !vFace[x].check && vFace[x] == next_ik )
	                {
				    qFace.push( vFace[x] );
				    viTraversal.push_back(x);
				    vFace[x].check = 1;
				    bOutward = 1;
				    break;
				    }
			}
		iNow += bOutward;  //有往外+1
		}
    }

void Set_Vicinity ()
    {
    int vFace_size=vFace.size();
	for ( int x=0;x<vFace_size;x++ )
        {
        bool temp;
        for ( int y=0;y<vFace_sv.size(); )
            {
		    Face& a = vFace[x];
		    Face& b = vFace[ vFace_sv[y].index ];
		    int ai,aj,ak,bi,bj,bk;
		    ai=aj=ak=bi=bj=bk=0;
		    if ( a.i == b.i || a.i == b.j || a.i == b.k )
		        ++ai;
		    if ( a.j == b.i || a.j == b.j || a.j == b.k )
		        ++aj;
		    if ( a.k == b.i || a.k == b.j || a.k == b.k )
		        ++ak;
		    
		    if ( b.i == a.i || b.i == a.j || b.i == a.k )
		        ++bi;
		    if ( b.j == a.i || b.j == a.j || b.j == a.k )
		        ++bj;
		    if ( b.k == a.i || b.k == a.j || b.k == a.k )
		        ++bk;		
		    if ( ai+aj+ak == 2 )
		        {
		        if ( ai && aj )
		            a.vicinity[0] = b.index;
		        else if ( ai && ak )
		            a.vicinity[1] = b.index;
		        else if ( aj && ak )
		            a.vicinity[2] = b.index;				
			    ++a.vicinity_count;
			    }
		    if ( bi+bj+bk == 2 )
		        {
		    	if ( bi && bj )
	    			b.vicinity[0] = a.index;
	    		else if ( bi && bk )
	    			b.vicinity[1] = a.index;
		    	else if ( bj && bk )
		    		b.vicinity[2] = a.index;
		    	++b.vicinity_count;
		    	}
	    	if ( b.vicinity_count == 3 )
	            vFace_sv.erase( vFace_sv.begin()+y );
	    	else
	    	    y++;
	    	}
    	vFace_sv.push_back(vFace[x]);	    
	    }	
	}
	
int main ()
{
string sKeyWord,sMessages;
int iMessages;
int iStart;
int p;
int i,j,k;
int rotation_times;

vector<int> Rotate;

fstream fin;
fin.open("Stego.obj",ios::in);	//讀取Stego.obj 

clock_t start = clock();
while ( fin >> sKeyWord )	//由第一個英文字母來決定後面吃測資的方式
    {
    if ( sKeyWord == "v" )	//把頂點加入陣列
	    {
	    Vertex temp;
	    fin >> temp;
	    temp.index = vVertex.size();
	    vVertex.push_back( temp );
		}
	else if ( sKeyWord == "f" )	//把面加入陣列
	    {
	    Face temp;
	    fin >> i >> j >> k;
	    
		temp.index = vFace.size();	
		    
		if ( i > j )
		    rotation_times = 2;
		else if ( i > k )
		    rotation_times = 1;
		else
		    rotation_times = 0;
		Rotate.push_back( rotation_times );
		
		if ( i > j )
		    swap( i , j );
		if ( i > k )
		    swap( i , k );
		if ( j > k )
		    swap( j , k );
		    
		temp.i = vVertex[i];
	    temp.j = vVertex[j];
	    temp.k = vVertex[k];
	    
		vFace.push_back( temp );
		}
	}
clock_t end = clock();
float costTime = (float)(end - start)/CLK_TCK;
printf("---fin : CPU Time: %f sec.\n",costTime );

cout << "Load file OK.\nTotal " << vVertex.size() -1 << " Vertex, " << vFace.size() << " Faces." << endl;

start = clock();
Set_Vicinity();
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---set vicinity : CPU Time: %f sec.\n",costTime );


fin.close();
fin.open("Secret Key.txt",ios::in);
printf("Secret Key.txt loading success.\n");
int iKey;
fin >> iKey;
printf("Secret Key : %d\n",iKey);
iStart = iKey/1000;
p = iKey%1000;
int* iSeq = new int [p];
int* iPer = new int [p];
int iFactorial = 1;
int iThree_pow = 1;
int sM_size = 1;


sMessages = "";
cout<<endl;
Traverse(iStart,sM_size,p);


for ( i = 0 ; i < p ; i ++ )
    {
	iSeq[i] = i;
    if ( i )
        {
		iFactorial *= i;
        iThree_pow *= 3;
        }
	}
for ( i = 0 ; i < p ; i ++ )
    {
    int iTargetFace = viTraversal[i+1] >= iStart ? viTraversal[i+1]-1 : viTraversal[i+1];
    iPer[i] = iTargetFace;
	
	}
cout << "Permutation : ";
iMessages = 0;
for ( i = 0 ; i < p ; i ++ )
    {
	cout << iPer[i];
	iMessages += ( 3*iSeq[iPer[i]]+Rotate[i] )*iFactorial*iThree_pow;
    for ( j = iPer[i] + 1 ; j < p ; j ++ )
        iSeq[j]--;
    if ( i < p-1 )
	    {
		iFactorial /= p-1-i;
	    iThree_pow /= 3;
	    }
    }
cout << endl;
sM_size = iMessages; 
cout << "sM_size = " << sM_size << endl;
for ( int x=0;x<viTraversal.size();x++ )
    vFace[ viTraversal[x] ].check=0;
viTraversal.clear();
/*
for ( i = 0 ; i < viTraversal.size() ; i ++ )
    printf("%d->",viTraversal[i]);
cout << endl;
cout << p <<endl; */

start = clock();
Traverse(iStart,sM_size,p);
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---Traverse : CPU Time: %f sec.\n",costTime );

/*for ( i = 0 ; i < viTraversal.size() ; i ++ )
    printf("%d->",viTraversal[i]);
cout << endl;*/

for ( k = 1 ; k <= sM_size ; k ++ )
	{
	for ( i = 0 ; i < p ; i ++ )
	    {
		iSeq[i] = i;
	    if ( i )
	        {
			iFactorial *= i;
	        iThree_pow *= 3;
	        }
		}
	for ( i = p*k ; i < p*k+p ; i ++ )
	    {
	    int iTargetFace = ( viTraversal[i+1] >= iStart ? viTraversal[i+1]-1 : viTraversal[i+1] );
	    iPer[i-p*k] = iTargetFace-p*k;
		}
	cout << "Permutation : ";
	iMessages = 0;
	for ( i = 0 ; i < p ; i ++ )
	    {
		cout << iPer[i];
		iMessages += ( 3*iSeq[iPer[i]]+Rotate[i+p*k] ) * iFactorial * iThree_pow;
	    for ( j = iPer[i] + 1 ; j < p ; j ++ )
	        iSeq[j]--;
	    if ( i < p-1 )
		    {
			iFactorial /= p-1-i;
		    iThree_pow /= 3;
		    }
	    }
	cout << endl;
    cout << "Rotate : ";
    for ( i = p*k ; i < p*k+p ; i++ )
        cout << Rotate[i] << ' ';
    cout << endl;
	cout << iMessages-128 << endl; 
	sMessages.push_back(iMessages-128);
	}
	
cout << endl << "Message : " << sMessages << endl;
system("PAUSE");
} 
