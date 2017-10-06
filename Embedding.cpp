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
string sKeyWord;
int iMessages,p,i,j,k,iStart;

//Ū��cover.obj 
fstream fin;
fin.open("cover.obj",ios::in);

//�ѲĤ@�ӭ^��r���ӨM�w�᭱�Y���ꪺ�覡 
clock_t start = clock();
while ( fin >> sKeyWord )
    {
    //�⳻�I�[�J�}�C 
    if ( sKeyWord == "v" )
	    {
	    Vertex temp;
	    fin >> temp.x >> temp.y >> temp.z;
	    vVertex.push_back( temp );
		}
	//�⭱�[�J�}�C 
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
		vFace.push_back( temp );
		}
	}
clock_t end = clock();
float costTime = (float)(end - start)/CLK_TCK;
printf("---fin : CPU Time: %f sec.\n",costTime );

start = clock();	
for ( int x=0;x<vFace.size();x++ )     //�]�wq�Aq�ΨӻP�u�q�զ��t�@�T���� 
    {
	Edge Eij,Eik,Ejk;   //���X�ӤT���Τ��T�� 
	Eij = vFace[x].ij;
	Eik = vFace[x].ik;
	Ejk = vFace[x].jk;
	for ( int y=0;y<vFace.size();y++ )
	    {
		if ( x == y )  //�ۤv�N���X 
		    continue;
		else if ( vFace[y].ij == Eij )      //�M�䦳�ۦP�䪺�T���� �]�w q
		    vFace[x].ij.q = vFace[y].k;
		else if ( vFace[y].ik == Eij )
		    vFace[x].ij.q = vFace[y].j;
		else if ( vFace[y].jk == Eij )
		    vFace[x].ij.q = vFace[y].i;
		
		else if ( vFace[y].ij == Eik )
		    vFace[x].ik.q = vFace[y].k;
		else if ( vFace[y].ik == Eik )
		    vFace[x].ik.q = vFace[y].j;
		else if ( vFace[y].jk == Eik )
		    vFace[x].ik.q = vFace[y].i;
		
		else if ( vFace[y].ij == Ejk )
		    vFace[x].jk.q = vFace[y].k;
		else if ( vFace[y].ik == Ejk )
		    vFace[x].jk.q = vFace[y].j;
		else if ( vFace[y].jk == Ejk )
		    vFace[x].jk.q = vFace[y].i;		
		}
	}
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---set q : CPU Time: %f sec.\n",costTime );
	
//�ϥΪ̿�J�_�l�T���ί��ޭȡBp�B�P�ݭn���ê��T��(�ثe���Ʀr)	
cout << "Load file OK.\nTotal " << vVertex.size() -1 << " Vertex, " << vFace.size() << " Faces." << endl << endl;
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
	
	for ( j = 0 ; j < vFace.size() ; j ++ )
        cout << stego[j] << ' ';
    cout << endl;
    }
//��XStego.obj 
fout.open( "Stego.obj" , ios::out );
for ( i = 1 ; i < vVertex.size() ; i ++ )
    fout << "v " << vVertex[i].x << " " << vVertex[i].y << " " << vVertex[i].z << endl; 
for ( i = 0 ; i < vFace.size() ; i ++ )
    fout << "f " << vFace[stego[i]].vertex_index[0] << " " << vFace[stego[i]].vertex_index[1] << " " << vFace[stego[i]].vertex_index[2] << endl;
cout << "Stego.obj created.\n";
system("PAUSE");
} 
