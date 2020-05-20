#include<iostream>
#include<list>
#include<queue>
#include <fstream>
#include <string>
using namespace std;
int ** grid;//ȫ�ֱ���
int n=0,m=0;//�����������

class Position{
public:
	int row,//��ʾ�������ڵ��к���
		col;
};

bool FindPath(Position start,Position finish,int& PathLen,Position * & path)
{//�������ʼλ��start��Ŀ��λ��finish����̲���·��
	//�ҵ���̲���·���򷵻�false
	int i=0;
	if((start.row==finish.row)&&(start.col==finish.col))
	{PathLen=0;return true;}
	//���÷������С�Χǽ��
	for(i=0;i<=m+1;i++)
		grid[0][i]=grid[n+1][i]=1;//�����͵ײ�
	for(i=0;i<=n+1;i++)
		grid[i][0]=grid[i][m+1]=1;//���������
	//��ʼ�����λ��
	Position offset[4];
	offset[0].row=0;offset[0].col=1;//��
    offset[1].row=1;offset[1].col=0;//��
	offset[2].row=0;offset[2].col=-1;//��
	offset[3].row=-1;offset[3].col=0;//��
	int NumOfNbrs=4;//���ڷ�����
	Position here,nbr;
	here.row=start.row;
	here.col=start.col;
	grid[start.row][start.col]=2;//��ʼλ�õľ�����Ϊ2

    grid[finish.row][finish.col]=0;//�յ���Ϊ0

	queue<Position,list<Position> > Q;//�Ƚ��ȳ�����
	do{//��ǿɴ����ڷ���
		for( i=0;i<NumOfNbrs;i++)
		{
		nbr.row=here.row+offset[i].row;//���ң��£����ϣ��ƶ�
		nbr.col=here.col+offset[i].col;
		if(grid[nbr.row][nbr.col]==0){
			//�÷���δ���
			grid[nbr.row][nbr.col]=grid[here.row][here.col]+1;//�����1
				if((nbr.row==finish.row)&&(nbr.col==finish.col))break;//��ɲ���
				Q.push(nbr);}
		}
		//�Ƿ񵽴�Ŀ��λ��finish?
		if((nbr.row==finish.row)&&(nbr.col==finish.col))break;//��ɲ���
		//��������Ƿ�ǿ�
		if(Q.empty()) return false;//�޽�
		here=Q.front();//ȡ��һ��չ���
		Q.pop();//ɾ����һ��Ԫ��
   
		}while(true);
		
	//������̲���·��
	PathLen=grid[finish.row][finish.col]-2;//�����start��ľ���
		path=new Position[PathLen];
		//��Ŀ��λ��finish��ʼ����ʼλ�û�˷
		here=finish;
		for(int j=PathLen-1;j>=0;j--){
			path[j]=here;
			//��ǰ��λ��
			for(int i=0;i<NumOfNbrs;i++){
				nbr.row=here.row+offset[i].row;//���ң��£����ϣ��ƶ�
				nbr.col=here.col+offset[i].col;
				if(grid[nbr.row][nbr.col]==j+2)break;//�������μ�С��7-6-5-4-3-2
			}
			here=nbr;//��ǰ�ƶ�
		}
		return true;
}

//��̬������ά����[n][m]
//����ʼ��Ϊȫ0
int** new_arry(int n,int m)
{//�������Ķ�ά�����0�е�0�в�����
int** aa;
aa=new int*[n];
int i=0,j=0;
//aa[0]=NULL;
for(i=0;i<n;i++)
  aa[i]=new int[m];

for(i=0;i<n;i++)
for(j=0;j<m;j++)
  aa[i][j]=0;

return aa;
}

//***************������**************
void main()
{
int i=0,j=0,k=0;
Position start,finish;
int PathLen;
Position * path;
//�������ļ����ļ��ĵ�һ��Ϊ�������������n,m
	string input;
	cout<<"enter input file name"<<endl;
	cin>>input;
	ifstream inputFile(input.c_str());
	if(!inputFile){
	cout<<"can't open inupt file."<<endl;
	return;
	}
	//����������ļ���
	string output;
	ofstream outputFile;
	cout<<"enter output file name"<<endl;
	cin>>output;
    outputFile.open (output.c_str (),ios::app);
    if(!outputFile){
	cout<<"can't open output file."<<endl;
	return;
	}

    inputFile>>n;//ȡ�þ��������n
	inputFile>>m;//ȡ�þ��������m

	 grid=new_arry( n+2, m+2);//ֱ�Ӵ���һ����̬��ά���飬�����������ܷ��㣡
    //�������ļ��ж�ȡ����
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=m;j++)
		{
			inputFile>>grid[i][j];
		if(grid[i][j]==-1){start.row=i;start.col=j;}
		if(grid[i][j]==-2){finish.row=i;finish.col=j;}
        cout<<grid[i][j]<<" ";
		}
		cout<<endl;
	}
FindPath(start,finish,PathLen,path);
//������
cout<<endl<<"������£�"<<endl<<endl;
for(i=0;i<n+2;i++)
{
for(j=0;j<m+2;j++){
	for(k=0;k<PathLen;k++)
		if(i==path[k].row&&j==path[k].col)
		{cout<<"* ";outputFile<<"* ";break;}
		if(k==PathLen){cout<<grid[i][j]<<" ";outputFile<<grid[i][j]<<" ";}
}
cout<<endl;
outputFile<<endl;
}
outputFile<<"·������Ϊ��"<<PathLen<<endl;
inputFile.close();
outputFile.close();
delete []grid;

} 