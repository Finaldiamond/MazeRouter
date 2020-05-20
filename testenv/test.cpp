#include<iostream>
#include<list>
#include<queue>
#include<fstream>
#include<string>
using namespace std;
int ** grid;//全局变量
int n=0,m=0;//矩阵的行列数

class Position{
public:
	int row,//表示方格所在的行和列
		col;
};

bool FindPath(Position start,Position finish,int& PathLen,Position * & path)
{//计算从起始位置start到目标位置finish的最短布线路径
	//找到最短布线路径则返回false
	int i=0;
	if((start.row==finish.row)&&(start.col==finish.col))
	{PathLen=0;return true;}
	//设置方格阵列“围墙”
	for(i=0;i<=m+1;i++)
		grid[0][i]=grid[n+1][i]=1;//顶部和底部
	for(i=0;i<=n+1;i++)
		grid[i][0]=grid[i][m+1]=1;//左翼和右翼
	//初始化相对位移
	Position offset[4];
	offset[0].row=0;offset[0].col=1;//右
    offset[1].row=1;offset[1].col=0;//下
	offset[2].row=0;offset[2].col=-1;//左
	offset[3].row=-1;offset[3].col=0;//上
	int NumOfNbrs=4;//相邻方格数
	Position here,nbr;
	here.row=start.row;
	here.col=start.col;
	grid[start.row][start.col]=2;//起始位置的距离标记为2

    grid[finish.row][finish.col]=0;//终点标记为0

	queue<Position,list<Position> > Q;//先进先出队列
	do{//标记可达相邻方格
		for( i=0;i<NumOfNbrs;i++)
		{
		nbr.row=here.row+offset[i].row;//按右，下，左，上，移动
		nbr.col=here.col+offset[i].col;
		if(grid[nbr.row][nbr.col]==0){
			//该方格未标记
			grid[nbr.row][nbr.col]=grid[here.row][here.col]+1;//距离加1
				if((nbr.row==finish.row)&&(nbr.col==finish.col))break;//完成布线
				Q.push(nbr);}
		}
		//是否到达目标位置finish?
		if((nbr.row==finish.row)&&(nbr.col==finish.col))break;//完成布线
		//活结点队列是否非空
		if(Q.empty()) return false;//无解
		here=Q.front();//取下一扩展结点
		Q.pop();//删除第一个元素
   
		}while(true);
		
	//构造最短布线路径
	PathLen=grid[finish.row][finish.col]-2;//相对于start点的距离
		path=new Position[PathLen];
		//从目标位置finish开始向起始位置回朔
		here=finish;
		for(int j=PathLen-1;j>=0;j--){
			path[j]=here;
			//找前驱位置
			for(int i=0;i<NumOfNbrs;i++){
				nbr.row=here.row+offset[i].row;//按右，下，左，上，移动
				nbr.col=here.col+offset[i].col;
				if(grid[nbr.row][nbr.col]==j+2)break;//距离依次减小：7-6-5-4-3-2
			}
			here=nbr;//向前移动
		}
		return true;
}

//动态产生二维数组[n][m]
//并初始化为全0
int** new_arry(int n,int m)
{//所产生的二维数组第0行第0列不可用
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

//***************主函数**************
int main()
{
	int i=0,j=0,k=0;
	Position start,finish;
	int PathLen;
	Position * path;
	//打开输入文件，文件的第一行为矩阵的行列数：n,m
	string input;
	cout<<"enter input file name"<<endl;
	cin>>input;
	ifstream inputFile(input.c_str());
	if(!inputFile)
	{
		cout<<"can't open inupt file."<<endl;
		return 0;
	}
	//请输入输出文件名
	string output;
	ofstream outputFile;
	cout<<"enter output file name"<<endl;
	cin>>output;
    outputFile.open (output.c_str (),ios::app);
    if(!outputFile)
	{
		cout<<"can't open output file."<<endl;
		return 0;
	}

    inputFile>>n;//取得矩阵的行数n
	inputFile>>m;//取得矩阵的行数m

	 grid=new_arry( n+2, m+2);//直接创建一个动态二维数组，好象用起来很方便！
    //从输入文件中读取矩阵
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
	//输出结果
	cout<<endl<<"结果如下："<<endl<<endl;
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
	outputFile<<"路径长度为："<<PathLen<<endl;
	inputFile.close();
	outputFile.close();
	delete []grid;
	return 1;
} 