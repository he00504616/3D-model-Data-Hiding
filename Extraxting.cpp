#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <time.h>
using namespace std;

struct Vertex  //頂點結構
{
double x,y,z;
public:
	Vertex( double px=0,double py=0,double pz=0 )
	    {
		x=px;y=py;z=pz;
		}
	void show()
	    {
		printf("(%.3f , %.3f , %.3f) ",x,y,z);
	    }
	bool operator==( const Vertex &k )
	    {
		if ( this->x == k.x && this->y == k.y && this->z == k.z )
		    return true;
		return false;
		}
}P0;

struct Edge  //邊結構
{
Vertex a,b,q;  // q為ab線段旁的點，組成另一三角
int index;  //set q 用
public:
	void show()
	    {
		cout << "a= ";a.show();
		cout << "b= ";b.show();
		cout << "q= ";q.show();
		cout << "index= " << index;
		}
	bool operator==( const Edge &k )
	    {
		if ( ( this->a == k.a && this->b == k.b) || ( this->a == k.b && this->b == k.a ) )
		    return true;
		return false;
		}
};

struct Face  //面結構
{
Vertex i,j,k;
Edge ij,ik,jk;
int vertex_index[3];
bool check;  //檢測是否尋訪過
//int index;
public:
	Face( Vertex a=P0,Vertex b=P0,Vertex c=P0 )
	    {
	    i=a;j=b;k=c;
	    check=0;
		}
	void show_p()
	    {
		cout << "i= ";i.show();
		cout << "j= ";j.show();
		cout << "k= ";k.show();
		}
	void show_e()
	    {
		cout << "ij-> ";ij.show();cout<<endl;
		cout << "ik-> ";ik.show();cout<<endl;
		cout << "jk-> ";jk.show();cout<<endl;
		}
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

vector <int> viTraversal;     //面的尋訪序列
vector <Vertex> vVertex(1);   //儲存obj中的頂點 (第0項為空點(0,0,0) )
vector <Face> vFace;          //儲存obj中的面
vector <Edge> vEdge;

void Traverse ( int iStart )
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

	while ( !qFace.empty() )
	    {
	    if ( iNow == sStart.size() )  //到底重頭
	        iNow = 0;

	    bOutward = 0;                 //設定
		bRotate = sStart[ iNow ] -'0';

		Face temp = qFace.front();
		qFace.pop();
		Face next_ij( temp.i ,temp.j , temp.ij.q );   //以該三角形之三邊  分別創建 外圍三角形
		Face next_ik( temp.i ,temp.k , temp.ik.q );
		Face next_jk( temp.j ,temp.k , temp.jk.q );

		for ( int x=0;x<iFaceSize;x++ )           // ij
		    if ( !vFace[x].check && vFace[x] == next_ij  )
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
		        if ( !vFace[x].check && vFace[x] == next_ik )
	                {
				    qFace.push( vFace[x] );
				    viTraversal.push_back(x);
				    vFace[x].check = 1;
				    bOutward = 1;
				    break;
				    }
		    for ( int x=0;x<iFaceSize;x++ )  //jk
		            if ( !vFace[x].check && vFace[x] == next_jk )
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
		            if ( !vFace[x].check && vFace[x] == next_jk )
	                {
				    qFace.push( vFace[x] );
				    viTraversal.push_back(x);
			    	vFace[x].check = 1;
				    bOutward = 1;
				    break;
				    }
			for ( int x=0;x<iFaceSize;x++ ) //ik
		        if ( !vFace[x].check && vFace[x] == next_ik )
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

int main ()
{
string sKeyWord;
int iMessages,p,i,j,k,iStart;

//讀取Stego.obj 
fstream fin;
fin.open("Stego.obj",ios::in);

//由第一個英文字母來決定後面吃測資的方式 
clock_t start = clock();
while ( fin >> sKeyWord )
    {
    //把頂點加入陣列 
    if ( sKeyWord == "v" )
	    {
	    Vertex temp;
	    fin >> temp.x >> temp.y >> temp.z;
	    vVertex.push_back( temp );
		}
	//把面加入陣列 
	else if ( sKeyWord == "f" )
	    {
	    Face temp;
	    fin >> temp.vertex_index[0] >> temp.vertex_index[1] >> temp.vertex_index[2];
	    temp.i = vVertex[temp.vertex_index[0]];
	    temp.j = vVertex[temp.vertex_index[1]];
	    temp.k = vVertex[temp.vertex_index[2]];
	    Edge Eij,Eik,Ejk;                 //設定三邊 
		Eij.a = Eik.a = temp.i;
		Eij.b = Ejk.a = temp.j;
		Eik.b = Ejk.b = temp.k;	
		temp.ij = Eij;
		temp.ik = Eik;
		temp.jk = Ejk;
		vFace.push_back( temp );
		}
	}
clock_t end = clock();
float costTime = (float)(end - start)/CLK_TCK;
printf("---fin : CPU Time: %f sec.\n",costTime );

start = clock();
for ( int x=0;x<vFace.size();x++ )     //設定q，q用來與線段組成另一三角形 
    {
	vFace[x].ij.index = vFace[x].ik.index = vFace[x].jk.index = x;
	Edge Eij,Eik,Ejk;   //取出該三角形之三邊 
	Eij = vFace[x].ij;
	Eik = vFace[x].ik;
	Ejk = vFace[x].jk;
	bool ij=1,ik=1,jk=1;
	bool temp;
	for ( int y=0;y<vEdge.size(); )
	    {
	    temp=1;
		if ( Eij == vEdge[y] )
		    {
		    if ( Eij == vFace[ vEdge[y].index ].ij )
		        {
				vFace[x].ij.q = vFace[ vEdge[y].index ].k;
				vFace[ vEdge[y].index ].ij.q = vFace[x].k;
				}
			else if ( Eij == vFace[ vEdge[y].index ].ik )
		        {
				vFace[x].ij.q = vFace[ vEdge[y].index ].j;
				vFace[ vEdge[y].index ].ik.q = vFace[x].k;
				}
			else if ( Eij == vFace[ vEdge[y].index ].jk )
		        {
				vFace[x].ij.q = vFace[ vEdge[y].index ].i;
				vFace[ vEdge[y].index ].jk.q = vFace[x].k;
				}
			ij=temp=0;		    
			}
		else if ( Eik == vEdge[y] )
		    {
		    if ( Eik == vFace[ vEdge[y].index ].ij )
		        {
				vFace[x].ik.q = vFace[ vEdge[y].index ].k;
				vFace[ vEdge[y].index ].ij.q = vFace[x].j;
				}
			else if ( Eik == vFace[ vEdge[y].index ].ik )
		        {
				vFace[x].ik.q = vFace[ vEdge[y].index ].j;
				vFace[ vEdge[y].index ].ik.q = vFace[x].j;
				}
			else if ( Eik == vFace[ vEdge[y].index ].jk )
		        {
				vFace[x].ik.q = vFace[ vEdge[y].index ].i;
				vFace[ vEdge[y].index ].jk.q = vFace[x].j;
				}
			ik=temp=0;
		    }
		else if ( Ejk == vEdge[y] )
		    {
		    if ( Ejk == vFace[ vEdge[y].index ].ij )
		        {
				vFace[x].jk.q = vFace[ vEdge[y].index ].k;
				vFace[ vEdge[y].index ].ij.q = vFace[x].i;
				}
			else if ( Ejk == vFace[ vEdge[y].index ].ik )
		        {
				vFace[x].jk.q = vFace[ vEdge[y].index ].j;
				vFace[ vEdge[y].index ].ik.q = vFace[x].i;
				}
			else if ( Ejk == vFace[ vEdge[y].index ].jk )
		        {
				vFace[x].jk.q = vFace[ vEdge[y].index ].i;
				vFace[ vEdge[y].index ].jk.q = vFace[x].i;
				}
			jk=temp=0;
		    }
		if ( temp )
		    y++;
		else
		    vEdge.erase(vEdge.begin()+y);
		}
	if ( ij )
	    vEdge.push_back(Eij);
	if ( ik )
	    vEdge.push_back(Eik);
	if ( jk )
		vEdge.push_back(Ejk);
	}
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---set q : CPU Time: %f sec.\n",costTime );

cout << "Load file OK.\nTotal " << vVertex.size() -1 << " Vertex, " << vFace.size() << " Faces." << endl << endl;
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
int iFactorial=1;

for ( i = 0 ; i < p ; i ++ )
    {
	iSeq[i] = i;
    if ( i )
        iFactorial *= i;
	}
cout << iFactorial << endl;

start = clock();
Traverse(iStart);
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---Traverse : CPU Time: %f sec.\n",costTime );

/*for ( i = 0 ; i < viTraversal.size() ; i ++ )
    printf("%d->",viTraversal[i]);
cout << endl;*/
for ( i = 0 ; i < p ; i ++ )
    iPer[i] = viTraversal[i+1] >= iStart ? viTraversal[i+1]-1 : viTraversal[i+1];
cout << "Permutation : ";
iMessages = 0;
for ( i = 0 ; i < p ; i ++ )
    {
	cout << iPer[i];
	iMessages += iSeq[iPer[i]]*iFactorial;
    for ( j = iPer[i] + 1 ; j < p ; j ++ )
        iSeq[j]--;
    if ( i < p-1 )
	    iFactorial /= p-1-i;
    }
cout << endl << "Message : " << iMessages << endl;
system("PAUSE");
} 
