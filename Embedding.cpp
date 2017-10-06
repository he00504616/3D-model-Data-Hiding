#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <list>
#include <time.h>
using namespace std;

class Vertex	//���I���c
{
public:
	double x,y,z;
	int index;
    Vertex( double px=0,double py=0,double pz=0 ):x(px),y(py),z(pz),index(0){}
	bool operator==( const Vertex &k )
	    {
		return (this->x == k.x && this->y == k.y && this->z == k.z );
		}
}P0;
istream& operator>>( istream &is , Vertex &k )
    {
	return is >> k.x >> k.y >> k.z;
	}
ostream& operator<<( ostream &os , Vertex &k )
    {
	return os << "( " << k.x << " , " << k.y << " , " << k.z << " )";
	}

class Face	//�����c
{
public:
	Vertex i,j,k;		//�T�I
	int vicinity[3];	//���񪺭��s��
	int vicinity_count;
	int index;
    bool check;			//�˴��O�_�M�X�L
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
ostream& operator<<( ostream &os , Face &a )
    {
	return os << "i= " << a.i << " j= " << a.j << " k= " << a.k;
	}

string ToBinary ( int a )	//�Ʀr��8bit�G�i��
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

vector <int> viTraversal;	//�����M�X�ǦC
vector <Vertex> vVertex(1);	//�x�sobj�������I (��0�������I(0,0,0) )
vector <Face> vFace;		//�x�sobj������
vector <Face> vFace_sv;		//set vicinity

void Traverse ( int iStart )
    {
    queue <Face> qFace;            //BFS
    string sStart;                 //�_�l�����G�i��
    sStart = ToBinary( iStart );
    
    qFace.push( vFace[iStart] );   //��J�_�l��
    viTraversal.push_back(iStart); //�[�J�M�X�ǦC
    vFace[iStart].check = 1;

	bool bOutward = 0 , bRotate = 0; // �O�_���~�X�i /  ����or�f��
	int iNow = 0;
	int iFaceSize = vFace.size();

	while ( !qFace.empty() )
	    {
	    if ( iNow == sStart.size() )  //�쩳���Y
	        iNow = 0;

	    bOutward = 0;                 //�]�w
		bRotate = sStart[ iNow ] -'0';

		Face temp = qFace.front();
		qFace.pop();
		Face next_ij = vFace[ temp.vicinity[0] ];	//�H�ӤT���Τ��T��  ���O�Ы� �~��T����
		Face next_ik = vFace[ temp.vicinity[1] ];
		Face next_jk = vFace[ temp.vicinity[2] ];

		for ( int x=0;x<iFaceSize;x++ )           // ij
		    if ( !vFace[x].check && vFace[x] == next_ij  )
	            {
				qFace.push( vFace[x] );
				viTraversal.push_back(x);         //��J�M�X�ǦC
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
string sKeyWord;
int iMessages;
int iStart;
int p;
int i,j,k;

fstream fin;
fin.open("cover4.obj",ios::in);	//Ū��cover.obj

clock_t start = clock();
while ( fin >> sKeyWord )	//�ѲĤ@�ӭ^��r���ӨM�w�᭱�Y���ꪺ�覡
    {
    if ( sKeyWord == "v" )	//�⳻�I�[�J�}�C
	    {
	    Vertex temp;
	    fin >> temp;
	    temp.index = vVertex.size();
	    vVertex.push_back( temp );
		}
	else if ( sKeyWord == "f" )	//�⭱�[�J�}�C
	    {
	    Face temp;
	    fin >> i >> j >> k;
		temp.i = vVertex[i];
	    temp.j = vVertex[j];
	    temp.k = vVertex[k];
		temp.index = vFace.size();	    
		vFace.push_back( temp );
		}
	}
clock_t end = clock();
float costTime = (float)(end - start)/CLK_TCK;
printf("---fin : CPU Time: %f sec.\n",costTime );

int vFace_size=vFace.size();
start = clock();
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
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---set vicinity : CPU Time: %f sec.\n",costTime );

//�ϥΪ̿�J�_�l�T���ί��ޭȡBp�B�P�ݭn���ê��T��(�ثe���Ʀr)	
cout << "Load file OK.\nTotal " << vVertex.size() -1 << " Vertex, " << vFace_size << " Faces." << endl << endl;
printf("Type Start_Face ( number < %d )\n",vFace.size());
cin >> iStart;

start = clock();
Traverse(iStart);
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---Traverse : CPU Time: %f sec.\n",costTime );

cout << "Type p ( < 13 )" << endl;
cin >> p;
//�ھ�p����iPer(�s��ƦC�զX)
//iSeq(�s��Ӧ�m�����ޭ�)
//iFactorial(p-1!) 
int* iPer = new int [p];
int* iSeq = new int [p];
int iFactorial=1;
//iSeq�]��(0,1,2,3,4....,p-1)�A�P�ɭp��p-1! 
for ( i = 0 ; i < p ; i ++ )
    {
	iSeq[i] = i;
    if ( i )
        iFactorial *= i;
	}
//�N�ϥΪ̿�J���_�l�T���ί��ޭȻPp�զX�b�@�_�A���ͱK�_ 
fstream fout;
fout.open( "Secret Key.txt" , ios::out );
fout << iStart*1000+p << endl;
fout.close();

cout << "Type your message( interger number < " << iFactorial*p << " ) :" << endl;
cin >> iMessages;

//cout << "Your message in binary : " << ToBinary( iMessages ) << endl;

vector<int> stego(vFace.size());    //�s���X�������� 

//�p��X�K��������ƦC�զX 
for ( i = 0 ; i < p ; i ++ )
    {
    iPer[i] = iSeq[iMessages / iFactorial];
    for ( j = iMessages / iFactorial ; j < p - 1; j ++ )
        iSeq[j] = iSeq[j+1];
    iMessages %= iFactorial;
    if ( i < p-1 )
	    iFactorial /= p-1-i;
    }
    
for ( i = 0 ; i < p ; i ++ )
    cout << iPer[i];
cout << endl;
for ( i = 0 ; i < viTraversal.size() ; i ++ )
    printf("%d->",viTraversal[i]);
cout << endl;

//��l�ƿ�X���� 
for ( i = 0 ; i < vFace.size() ; i ++ )
    stego[i] = i;

//�p��X�M�X�ǦC�P�ƦC�զX��A���X�����ǧאּ�M�X��Y�i�o�X�ƦC�զX 
for ( i = 0 ; i < p ; i ++ )
    {
    //����ʨ�_�l�T���ΡA�ҥH��_�l�T���Ϋ᪺���������1��A�P�ɧP�_�O�_�S���N�w�g�b�w�� 
	if ( iPer[i] >= iStart && stego[iPer[i]+1] != viTraversal[i+1] )
	    //��swap�O�Ҥ��|�⭱�л\�έ���  
		swap ( stego[iPer[i]+1] , stego[find( stego.begin() , stego.end() , viTraversal[i+1] )-stego.begin()] );
	//�p�G���δ��N�w�g�Ʀn�A�hcontinue 
	else if ( iPer[i] >= iStart && stego[iPer[i]+1] == viTraversal[i+1] )
	    continue;
	//��_�l�T���ί��ޭȤp�����p 
	else if ( stego[iPer[i]] != viTraversal[i+1] )
	    swap ( stego[iPer[i]] , stego[find( stego.begin() , stego.end() , viTraversal[i+1] )-stego.begin()] );
	
	/*for ( j = 0 ; j < vFace.size() ; j ++ )
        cout << stego[j] << ' ';
    cout << endl;*/
    }
//��XStego.obj 
fout.open( "Stego.obj" , ios::out );
for ( i = 1 ; i < vVertex.size() ; i ++ )
    fout << "v " << vVertex[i].x << " " << vVertex[i].y << " " << vVertex[i].z << endl; 
for ( i = 0 ; i < vFace.size() ; i ++ )
    fout << "f " << vFace[stego[i]].i.index << " " << vFace[stego[i]].j.index << " " << vFace[stego[i]].k.index << endl;
cout << "Stego.obj created.\n";
system("PAUSE");
} 
