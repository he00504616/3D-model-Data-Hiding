#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <time.h>
using namespace std;

struct Vertex  //���I���c
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

struct Edge  //�䵲�c
{
Vertex a,b,q;// q��ab�u�q�Ǫ��I�A�զ��t�@�T��
public:
	void show()
	    {
		/*cout << "a= ";a.show();
		cout << "b= ";b.show();*/
		cout << "q= ";q.show();
		}
	bool operator==( const Edge &k )
	    {
		if ( ( this->a == k.a && this->b == k.b) || ( this->a == k.b && this->b == k.a ) )
		    return true;
		return false;
		}
};

struct  Face   //�����c
{
Vertex i,j,k;
int vertex_index[3];
Edge ij,ik,jk;
bool check;    //�˴��O�_�M�X�L
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
		cout << "ij-> ";ij.show();
		cout << "ik-> ";ik.show();
		cout << "jk-> ";jk.show();
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

string ToBinary ( int a ) //�Ʀr��8bit�G�i��
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

vector <int> viTraversal;     //�����M�X�ǦC
vector <Vertex> vVertex(1);   //�x�sobj�������I (��0�������I(0,0,0) )
vector <Face> vFace;          //�x�sobj������

void Traverse ( int iStart )
    {
    queue <Face> qFace;            //BFS
    string sStart;                 //�_�l�����G�i��
    sStart = ToBinary( iStart );
    qFace.push( vFace[iStart] );   //��J�_�l��
    vFace[iStart].check = 1;
    viTraversal.push_back(iStart); //�[�J�M�X�ǦC

	bool bOutward = 0 , bRotate = 0; // �O�_���~�X�i /  ����or�f��
	int iNow = 0;
	int iFaceSize = vFace.size();

	while ( !qFace.empty() )
	    {
	    if ( iNow == sStart.size() )  //�쩳���Y
	        iNow = 0;

	    bOutward = 0;                 //�]�w
		bRotate = sStart[ iNow ] -'0';

		Face temp=qFace.front();
		qFace.pop();
		Face next_ij( temp.i ,temp.j , temp.ij.q );   //�H�ӤT���Τ��T��  ���O�Ы� �~��T����
		Face next_ik( temp.i ,temp.k , temp.ik.q );
		Face next_jk( temp.j ,temp.k , temp.jk.q );

		for ( int x=0;x<iFaceSize;x++ )           // ij
		    if ( !vFace[x].check && vFace[x] == next_ij  )
	            {
				qFace.push( vFace[x] );
				viTraversal.push_back(x);            //��J�M�X�ǦC
				vFace[x].check = 1;
				bOutward = 1;
				break;
				}
		if ( bRotate == 0 )  //����
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
		else if ( bRotate == 1 ) //�f��
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
		iNow += bOutward;  //�����~+1
		}
    }
int main ()
{
fstream fin;
fin.open("Stego.obj",ios::in);
string sKeyWord;
int iMessages,p,i,j,k,iStart;

while ( fin >> sKeyWord )
    {
    if ( sKeyWord == "v" )
	    {
	    Vertex temp;
	    fin >> temp.x >> temp.y >> temp.z;
	    vVertex.push_back( temp );
		}
	else if ( sKeyWord == "f" )
	    {
	    Face temp;
	    fin >> temp.vertex_index[0] >> temp.vertex_index[1] >> temp.vertex_index[2];
	    temp.i = vVertex[temp.vertex_index[0]];
	    temp.j = vVertex[temp.vertex_index[1]];
	    temp.k = vVertex[temp.vertex_index[2]];
	    Edge Eij,Eik,Ejk;                 //�]�w�T�� 
		Eij.a = Eik.a = temp.i;
		Eij.b = Ejk.a = temp.j;
		Eik.b = Ejk.b = temp.k;	
		temp.ij = Eij;
		temp.ik = Eik;
		temp.jk = Ejk;
		
	    //temp.index = vFace.size();
		vFace.push_back( temp );
		}
	}
for ( int x=0;x<vFace.size();x++ )     //�]�wq�Aq�ΨӻP�u�q�զ��t�@�T���� 
    {
	Edge Eij,Eik,Ejk;
	Eij = vFace[x].ij;
	Eik = vFace[x].ik;
	Ejk = vFace[x].jk;
	for ( int y=0;y<vFace.size();y++ )
	    {
		if ( x == y )
		    continue;
		if ( vFace[y].ij == Eij )
		    vFace[x].ij.q = vFace[y].k;
		if ( vFace[y].ik == Eij )
		    vFace[x].ij.q = vFace[y].j;
		if ( vFace[y].jk == Eij )
		    vFace[x].ij.q = vFace[y].i;
		
		if ( vFace[y].ij == Eik )
		    vFace[x].ik.q = vFace[y].k;
		if ( vFace[y].ik == Eik )
		    vFace[x].ik.q = vFace[y].j;
		if ( vFace[y].jk == Eik )
		    vFace[x].ik.q = vFace[y].i;
		
		if ( vFace[y].ij == Ejk )
		    vFace[x].jk.q = vFace[y].k;
		if ( vFace[y].ik == Ejk )
		    vFace[x].jk.q = vFace[y].j;
		if ( vFace[y].jk == Ejk )
		    vFace[x].jk.q = vFace[y].i;		
		}
	}
cout << "Load file OK.\nTotal " << vVertex.size() << " Vertex, " << vFace.size() << " Faces." << endl << endl;
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
Traverse(iStart);
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
