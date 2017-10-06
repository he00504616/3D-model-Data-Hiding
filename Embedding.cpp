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
Vertex a,b,q;// q為ab線段旁的點，組成另一三角
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

struct  Face   //面結構 
{
Vertex i,j,k;
int vertex_index[3];
Edge ij,ik,jk;
bool check;    //檢測是否尋訪過 
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

string ToBinary ( int a ) //數字轉8bit二進位 
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
				viTraversal.push_back(x);            //放入尋訪序列 
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

//讀取cover.obj 
fstream fin;
fin.open("cover.obj",ios::in);

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
	Edge Eij,Eik,Ejk;   //取出該三角形之三邊 
	Eij = vFace[x].ij;
	Eik = vFace[x].ik;
	Ejk = vFace[x].jk;
	for ( int y=0;y<vFace.size();y++ )
	    {
		if ( x == y )  //自己就跳出 
		    continue;
		else if ( vFace[y].ij == Eij )      //尋找有相同邊的三角形 設定 q
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
	
//使用者輸入起始三角形索引值、p、與需要隱藏的訊息(目前為數字)	
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
//根據p產生iPer(存放排列組合)
//iSeq(存放該位置之索引值)
//iFactorial(p-1!) 
int* iPer = new int [p];
int* iSeq = new int [p];
int iFactorial=1;
//iSeq設為(0,1,2,3,4....,p-1)，同時計算p-1! 
for ( i = 0 ; i < p ; i ++ )
    {
	iSeq[i] = i;
    if ( i )
        iFactorial *= i;
	}
//將使用者輸入的起始三角形索引值與p組合在一起，產生密鑰 
fstream fout;
fout.open( "Secret Key.txt" , ios::out );
fout << iStart*1000+p << endl;
fout.close();

cout << "Type your message( interger number < " << iFactorial*p << " ) :" << endl;
cin >> iMessages;

//cout << "Your message in binary : " << ToBinary( iMessages ) << endl;

vector<int> stego(vFace.size());    //存放輸出的面順序 

//計算出密文對應的排列組合 
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

//初始化輸出順序 
for ( i = 0 ; i < vFace.size() ; i ++ )
    stego[i] = i;

//計算出尋訪序列與排列組合後，把輸出的順序改為尋訪後即可得出排列組合 
for ( i = 0 ; i < p ; i ++ )
    {
    //不能動到起始三角形，所以把起始三角形後的全部往後推1格，同時判斷是否沒換就已經在定位 
	if ( iPer[i] >= iStart && stego[iPer[i]+1] != viTraversal[i+1] )
	    //用swap保證不會把面覆蓋或重複  
		swap ( stego[iPer[i]+1] , stego[find( stego.begin() , stego.end() , viTraversal[i+1] )-stego.begin()] );
	//如果不用換就已經排好，則continue 
	else if ( iPer[i] >= iStart && stego[iPer[i]+1] == viTraversal[i+1] )
	    continue;
	//比起始三角形索引值小的情況 
	else if ( stego[iPer[i]] != viTraversal[i+1] )
	    swap ( stego[iPer[i]] , stego[find( stego.begin() , stego.end() , viTraversal[i+1] )-stego.begin()] );
	
	for ( j = 0 ; j < vFace.size() ; j ++ )
        cout << stego[j] << ' ';
    cout << endl;
    }
//輸出Stego.obj 
fout.open( "Stego.obj" , ios::out );
for ( i = 1 ; i < vVertex.size() ; i ++ )
    fout << "v " << vVertex[i].x << " " << vVertex[i].y << " " << vVertex[i].z << endl; 
for ( i = 0 ; i < vFace.size() ; i ++ )
    fout << "f " << vFace[stego[i]].vertex_index[0] << " " << vFace[stego[i]].vertex_index[1] << " " << vFace[stego[i]].vertex_index[2] << endl;
cout << "Stego.obj created.\n";
system("PAUSE");
} 
