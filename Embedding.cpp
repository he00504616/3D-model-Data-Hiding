#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <vector>
#include <time.h>
#include <queue>
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
	
void shift( Face &x , int times )
    {
    Vertex temp;
    while ( times-- )
		{
		temp = x.i;
	    x.i = x.j;
	    x.j = x.k;
	    x.k = temp;
	    }
    }

int main ()
{
string sKeyWord,sMessages;
int iMessages;
int iStart;
int p;
int i,j,k;

fstream fin;
fin.open("cover4.obj",ios::in);	//讀取cover.obj

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
		
		if ( i > j )
		    swap( i , j );
		if ( i > k )
		    swap( i , k );
		if ( j > k )
		    swap( j , k );
		
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
cout << "Load file OK.\nTotal " << vVertex.size() -1 << " Vertex, " << vFace_size << " Faces." << endl;

start = clock();
Set_Vicinity();
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---set vicinity : CPU Time: %f sec.\n",costTime );

printf("Type Start_Face ( number < %d )\n",vFace_size);
cin >> iStart;	//使用者輸入起始三角形索引值、p、與需要隱藏的訊息

cout << "Type p( < 13 ),this will decide how much character you can type.\n";
cin >> p;

//將使用者輸入的起始三角形索引值與p組合在一起，產生密鑰 
fstream fout;
fout.open( "Secret Key.txt" , ios::out );
fout << iStart*1000+p << endl;
fout.close();

//根據p產生iPer(存放排列組合)
//iSeq(存放該位置之索引值)
//iFactorial(p-1!) 
int* iPer = new int [p];
int* iSeq = new int [p];
int iFactorial=1;
int iThree_pow = 1;
int sM_size;

for ( i = 0 ; i < p ; i ++ )
	    {
		iThree_pow *= 3;
	    if ( i )
	        iFactorial *= i;
		}
cout << "Type your message( string length < " << min( vFace_size/p , iFactorial*p*iThree_pow ) << " ) :" << endl;
cin >> ws;
getline( cin , sMessages );
sM_size = sMessages.size();
//cout << "Your message in binary : " << ToBinary( iMessages ) << endl;

start = clock();
Traverse(iStart,sM_size,p);
end = clock();
costTime = (float)(end - start)/CLK_TCK;
printf("---Traverse : CPU Time: %f sec.\n",costTime );

vector<int> stego(vFace_size);    //存放輸出的面順序 
vector<int> Rotate(vFace_size);
//初始化輸出順序 
for ( i = 0 ; i < vFace.size() ; i ++ )
    stego[i] = i;
/*  
for ( i = 0 ; i < 20 ; i ++ )
	    printf("%d->",viTraversal[i]);
	cout << endl;*/ 
iFactorial = 1;
iThree_pow = 1;
//計算出密文對應的排列組合 
for ( k = 0 ; k < sM_size+1 ; k ++ )
    {
    if ( k )
        iMessages = sMessages[k-1] + 128;
    else
        iMessages = sM_size;
    cout << "int M = " << iMessages << endl;
	//iSeq設為(0,1,2,3,4....,p-1)，同時計算p-1! 
	for ( i = 0 ; i < p ; i ++ )
	    {
		iSeq[i] = i+k*p;
	    if ( i )
	        {
			iFactorial *= i;
	        iThree_pow *= 3;
	        }
		}
	for ( i = 0 ; i < p ; i ++ )
	    {
	    int iQuotient = iMessages / ( iFactorial * iThree_pow );
	    //cout << iQuotient << endl;
	    iPer[i] = iSeq[iQuotient / 3];
	    Rotate[i+p*k] = iQuotient % 3;
	    for ( j = iQuotient / 3 ; j < p - 1; j ++ )
	        iSeq[j] = iSeq[j+1];
	    iMessages %= ( iFactorial * iThree_pow );
	    if ( i < p-1 )
		    {
			iFactorial /= p-1-i;
		    iThree_pow /= 3;
	        }
	    }
	cout << "Per : " ;    
    for ( i = 0 ; i < p ; i ++ )
	    cout << iPer[i]-p*k << ' ';
	cout << endl;
    cout << "Rotate : ";
	for ( i = p*k ; i < p*k+p ; i ++ )
	    cout << Rotate[i] << ' ';
	cout << endl;
	
	//計算出尋訪序列與排列組合後，把輸出的順序改為尋訪後即可得出排列組合 
	for ( i = 0 ; i < p ; i ++ )
	    {
	    //不能動到起始三角形，所以把起始三角形後的全部往後推1格，同時判斷是否沒換就已經在定位 
		if ( iPer[i] >= iStart )
		    {
			//用swap保證不會把面覆蓋或重複  
			if ( stego[iPer[i]+1] != viTraversal[i+1+p*k] )
			    swap ( stego[iPer[i]+1] , stego[find( stego.begin() , stego.end() , viTraversal[i+1+p*k] )-stego.begin()] );
			else
			    //如果不用換就已經排好，則continue 
		        continue;
			}
		//比起始三角形索引值小的情況 
		else 
		    {
			if ( stego[iPer[i]] != viTraversal[i+1+p*k] )
		        swap ( stego[iPer[i]] , stego[find( stego.begin() , stego.end() , viTraversal[i+1+p*k] )-stego.begin()] );
		    else
		        continue;
		    }
		
	    /*for ( j = 0 ; j < 30 ; j ++ )
	        cout << stego[j] << ' ';
	    cout << endl;  */
		}
	}


//輸出Stego.obj 
fout.open( "Stego.obj" , ios::out );
for ( i = 1 ; i < vVertex.size() ; i ++ )
    fout << setprecision(10) << "v " << vVertex[i].x << " " << vVertex[i].y << " " << vVertex[i].z << endl; 
for ( i = 0 ; i < vFace.size() ; i ++ )
    {
	Face fOutput = vFace[stego[i]];
	
	if ( Rotate[i] )
	    shift( fOutput , Rotate[i] );
	fout << "f " << fOutput.i.index << " " << fOutput.j.index << " " << fOutput.k.index << endl;
	}
cout << "Stego.obj created.\n";
system("PAUSE");
} 
