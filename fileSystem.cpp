#include<iostream>
#include<string>
#include<vector>
#include<new>
#include <windows.h>

using namespace std;

typedef struct FCB
{
	string FileName; //文件名
	string FileType; //文件类型
	vector<FCB> * subdirectory; //下一级目录地址
	vector<FCB> * updirectory; //上一级目录地址
	string parent; //父文件夹名
	SYSTEMTIME creating_time; //创建时间
//	SYSTEMTIME last_modified_time; //最近访问时间
	string status; //文件状态
	string details;//文件内容
}FCB;

FCB fileCopy;//全局变量，用于保存复制或剪切的文件或文件夹

//登录
void Login()
{
	string name,password;
	cout <<"\n\n\n";
	cout <<"\t\t            操作系统大型试验-模拟文件系统"<<endl<<endl<<endl<<endl<<endl<<endl;
	cout <<"\t\t             作者：XXX"<<endl<<endl<<endl;
	system("pause");
    system("cls");
}

//显示时间
void show_time(SYSTEMTIME& sys)
{
	cout << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond;
}

//显示当前目录中的所有文件和文件夹
void dir(vector<FCB>& a)
{
	int length = a.size();
	int i = 0;
	for(i=1;i<length;i++)
	{
		if(a[i].FileType != "")
		{
			cout << "文件"<<a[i].FileName;
			cout << "." << a[i].FileType<<endl;
			cout << "    创建时间: ";
			show_time(a[i].creating_time);
			cout <<endl<< "    最近访问时间: ";
///			show_time(a[i].last_modified_time);
			cout <<endl<< "    状态: ";
			cout << a[i].status << endl<<endl;
		}
		else
		{
			cout << "文件夹"+a[i].FileName<<endl;
			cout << "      创建时间: ";
			show_time(a[i].creating_time);
			cout << endl<<"      最近访问时间: ";
//			show_time(a[i].last_modified_time);
			cout << endl<<"      状态: ";
			cout << a[i].status << endl<<endl;
		}
	}
	if(i==1) cout<<"当前目录下没有内容!"<<endl<<endl;
}

//显示帮助信息
void help()
{
	cout << "dir  " << "显示当前目录中的所有文件和文件夹" << endl;
	cout << "help  " << "显示帮助信息" << endl;
	cout << "命令+?  " << "显示该命令的说明文档，如：dir?" << endl;
	cout << "create xxx  " << "在当前目录中创建文件xxx.yyy或文件夹xxx" << endl;
	cout << "delete xxx  " << "在当前目录中删除文件xxx.yyy或文件夹xxx" << endl;
	cout << "open xxx  " << "打开当前目录中的文件xxx.yyy或文件夹xxx" << endl;
	cout << "close xxx  " << "关闭当前目录中的文件xxx.yyy或文件夹xxx" << endl;
	cout << "cd ..  " << "返回到上一级目录" << endl;
	cout << "cd xxx  " << "转到当前目录的xxx子目录" << endl;
	cout << "cd root  " << "转到根目录root" << endl;
	cout << "rename xxx yyy  " << "重命名" << endl;
	cout << "copy xxx  " << "复制" << endl;
	cout << "cut xxx  " << "剪切" << endl;
	cout << "paste  " << "粘贴" << endl;
	cout << "attrib xxx  " << "显示xxx文件或文件夹的属性" << endl;
	cout << "exit  " << "退出" << endl<<endl;
}

//判断是否是帮助请求命令
bool is_require(string& str)
{
	return (str[str.length()-1] == '?');
}

//显示命令的说明
void command_explan(string& command)
{
	if(command=="dir?") cout << "\"dir\"命令用于显示当前目录中的所有文件和文件夹" << endl<<endl;
	else if(command=="help?") cout << "\"help\"命令用于显示帮助信息" << endl<<endl;
	else if(command=="create?") cout << "\"create\"命令用于在当前目录中创建文件或文件夹，如：create hello.txt,create hello,注意：创建文件必须有扩展名，无扩展名则认为是文件夹,文件和文件不能同名，文件夹和文件夹不能同名，文件和文件夹可以同名" << endl<<endl;
	else if(command=="delete?") cout << "\"delete\"命令用于在当前目录中删除文件或文件夹，如：delete hello.txt,delete hello" << endl<<endl;
	else if(command=="open?") cout << "\"open\"命令用于在当前目录中打开文件或文件夹，如：open hello.txt,open hello" << endl<<endl;
	else if(command=="close?") cout << "\"close\"命令用于在当前目录中关闭文件或文件夹，如：close hello.txt,close hello" << endl<<endl;
	else if(command=="cd?") cout << "\"cd\"命令用于跳转目录，如：cd hello,\"cd ..\"用于跳转到当前目录的上一级目录，\"cd root\"用于直接转到根目录" << endl<<endl;
	else if(command=="rename?") cout << "\"rename\"命令用于给当前目录中的文件或文件夹重命名，如：rename hello.txt hi.cpp(可以修改扩展名),rename hello.txt hi(不加扩展名则默认为原扩展名),rename hello hi(修改文件夹名)" << endl<<endl;
	else if(command=="cut?") cout << "\"cut\"命令用于剪切当前目录中的文件或文件夹，如：cut hello.txt"<< endl<<endl;
	else if(command=="paste?") cout << "\"paste\"命令用于将文件或者文件夹粘贴到当前目录中，如：paste ，这样就直接把前面复制或者是剪切的文件粘贴到当前位置"<< endl<<endl;
	else if(command=="copy?") cout << "\"copy\"命令用于复制当前目录中的文件或文件夹，如：cut hello.txt"<< endl<<endl;
	else if(command=="attrib?") cout << "\"attrib\"命令用于显示文件或文件夹的属性，如：attrib hello.txt"<< endl<<endl;
	else if(command=="exit?") cout << "\"exit\"命令用于退出系统" << endl;
	else cout << "没有该命令!请检查后重新输入!" << endl<<endl;
}

//在当前目录中创建文件或文件夹
void create(vector<FCB> * & a)
{
	string file = "",name = "",type = "";
	FCB fcb,fcb2;
	int length = 0,i = 0;
	int size = (*a).size();

	cin >> 	file;
	length = file.length();

	for(i=0;i<length;i++)//如果输入的文件名中含有“.”，则说明是文件，将文件类型存入文件结构中的类型部分
	{
		if(file[i]=='.') break;
	}

	if(i<length-1)
	{
		name = file.substr(0,i);
		type = file.substr(i+1,length-i-1);
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==name && (*a)[i].FileType != "")
			{
				cout << "该文件名已存在!" << endl<<endl;
				return;
			}
		}
		fcb.FileName = name;
		fcb.FileType = type;
		fcb.subdirectory = NULL;
		fcb.updirectory = NULL;
		fcb.parent = "";
		GetLocalTime(&fcb.creating_time);
//		GetLocalTime(&fcb.last_modified_time);
		fcb.status = "closed";
		cout<<"请输入文件内容:"<<endl;
		cin>>fcb.details; 
		(*a).push_back(fcb);
		cout<<"文件"<<file<<"创建成功!"<<endl<<endl;
	}
	else//文件名循环到结束也没有找到“.”,则说明是文件夹
	{
		name = file.substr(0,i);
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==name && (*a)[i].FileType=="")
			{
				cout << "该文件夹已经存在!" << endl<<endl;
				return;
			}
		}

		vector<FCB> * sub_dir = new vector<FCB>();
		
		fcb2.FileName = "";
		fcb2.FileType = "";
		fcb2.subdirectory = NULL;
		fcb2.updirectory = a;
		fcb2.parent = name;
		GetLocalTime(&fcb2.creating_time);
//		GetLocalTime(&fcb2.last_modified_time);
		fcb2.status = "closed";

		fcb.FileName = name;
		fcb.FileType = "";
		fcb.subdirectory = sub_dir;
		fcb.updirectory = NULL;
		fcb.parent = "";
		GetLocalTime(&fcb.creating_time);
//		GetLocalTime(&fcb.last_modified_time);
		fcb.status = "closed";

		(*a).push_back(fcb);
		
		(*sub_dir).push_back(fcb2);	
		cout<<"文件夹"<<file<<"创建成功!"<<endl<<endl;
	}
}

//在当前目录中删除文件或文件夹
void del(vector<FCB>& a)
{
	string file = "",filename = "",filetype = "";
	cin >> file;
	int length = file.length();
	int size = a.size();
	int i = 0;
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	if(i<length-1)
	{
		filename = file.substr(0,i);
		filetype = file.substr(i+1,length-i-1);
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==filename && a[i].FileType != "")
			{
				if(a[i].FileType==filetype)
				{
					a.erase(&a[i]);
					cout <<"文件"<< filename << "." << filetype <<"已经删除成功!" << endl<<endl; 
				}
				else cout << "没有这个文件!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "没有这个文件!" <<endl<<endl;
	}
	else if(i==length)
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				a.erase(&a[i]);
				cout << "文件夹" << file<< "已经删除成功!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "没有这个文件夹!" <<endl<<endl;
	}
	else cout << "没有这个文件夹!" <<endl<<endl;
}




//用于剪切功能中，在当前目录中删除文件或文件夹
void del(vector<FCB>& a,string file)
{
	string filename = "",filetype = "";
	int length = file.length();
	int size = a.size();
	int i = 0;
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	if(i<length-1)
	{
		filename = file.substr(0,i);
		filetype = file.substr(i+1,length-i-1);
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==filename && a[i].FileType != "")
			{
				if(a[i].FileType==filetype)
				{
					a.erase(&a[i]);
				}
				return;
			}
		}
	}
    if(i==length)
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				a.erase(&a[i]);
				return;
			}
		}
	}
}



//打开当前目录中的文件
void open(vector<FCB> &a,string& directory)
{
	string file = "",filename = "",filetype = "";
	cin >> file;
	int length = file.length();
	int size = a.size();
	int i = 0;
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	if(i<length-1)
	{
		filename = file.substr(0,i);
		filetype = file.substr(i+1,length-i-1);
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==filename && a[i].FileType != "")
			{
				if(a[i].FileType==filetype)
				{
					if(a[i].status=="closed")
					{
						a[i].status = "opened";
//						GetLocalTime(&a[i].last_modified_time);
						cout << "文件"<<a[i].FileName << "." << a[i].FileType << "成功打开!" <<endl;
						cout<<"显示文件内容："<<a[i].details<<endl<<endl;
						return;
					}
					else
					{
						cout <<"文件"<<a[i].FileName << "." << a[i].FileType <<  "已经是打开状态!"<<endl;
						cout<<"显示文件内容："<<a[i].details<<endl<<endl;
						return;
					}
				}
				else cout << "当前目录下没有这个文件!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "当前目录下没有这个文件!" <<endl<<endl;
		return;
	}
	else if(i==length)////打开文件夹
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				if(a[i].status=="closed")
				{
					a[i].status = "opened";
//					GetLocalTime(&a[i].last_modified_time);
					cout << "文件夹"<<a[i].FileName << "成功打开!" <<endl;//////////////
				    cout <<"显示该文件夹内容：" <<endl;
					dir(*a[i].subdirectory);
					return;
				}
				else
				{
					cout <<"文件夹"<< a[i].FileName << "已经是打开状态!" << endl;
					cout <<"显示该文件夹内容：" <<endl;
				    dir(*a[i].subdirectory);
					return;
				}
			}
		}
		if(i==size) cout << "没有这个文件夹!" << endl<<endl;
		return;
	}
	else cout << "没有这个文件夹!" << endl<<endl;
	
}

//关闭当前目录中的文件
void close(vector<FCB>& a)
{
	string file = "",filename = "",filetype = "";
	cin >> file;
	int length = file.length();
	int size = a.size();
	int i = 0;
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	if(i<length-1)
	{
		filename = file.substr(0,i);
		filetype = file.substr(i+1,length-i-1);
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==filename && a[i].FileType != "")
			{
				if(a[i].FileType==filetype)
				{
					if(a[i].status=="opened")
					{
						a[i].status = "closed";
						cout <<"文件" <<a[i].FileName << "." << a[i].FileType << "成功关闭!" << endl<<endl;
						return;
					}
					else
					{
						cout <<"文件" << a[i].FileName << "." << a[i].FileType << "原来就是关闭状态!" << endl<<endl;
						return;
					}
				}
				else cout << "没有这个文件!" <<endl<< endl;
				return;
			}
		}
		if(i==size) cout <<  "没有这个文件!"  <<endl<<endl;
	}
	else if(i==length)
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				if(a[i].status=="opened")
				{
					a[i].status = "closed";
					cout <<"文件夹"<< a[i].FileName << " " << "成功关闭!" << endl<<endl;
					return;
				}
				else
				{
					cout <<"文件夹"<< a[i].FileName << " " << "原来就是关闭状态!" << endl<<endl;
					return;
				}
			}
		}
		if(i==size) cout << "没有这个文件夹!" << endl;
	}
	else cout << "没有这个文件夹!" << endl;
	
}

//显示出错信息
void error()
{
	cout << "命令错误!!" << endl<<endl;
}

//更改当前目录
void cd(vector<FCB> * & a,string& str,vector<FCB> * root)
{
	string dir;
	int i = 0;
	int size = (*a).size();
	int length1 = 0,length2 = 0;

	cin >> dir;

	if(dir=="root")
	{
		a = root;		
		str = "root:/";	
	}
	else if(dir=="..")
	{	
		if(a==root) return;
		string parent_name = (*a)[0].parent;
		length1 = str.length();
		length2 = parent_name.length();
		str = str.substr(0,length1-length2-1);
		a = (*a)[0].updirectory;
	}
	else
	{
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==dir && (*a)[i].FileType=="")
			{
				a = (*a)[i].subdirectory;
				str += dir;
				str += "/";
				return;
			}
		}
		if(i==size)
		{
			cout << "没有此目录!" << endl<<endl;
			return;
		}
	}

}

//判断当前目录中指定名称的文件是否存在
bool find_file(vector<FCB>& a,string& filename)
{
	int size = a.size();
	int i = 0;
	for(i=1;i<size;i++)
	{
		if(a[i].FileName==filename && a[i].FileType != "") return true;
	}
	return false;
}

//判断当前目录中指定名称的文件夹是否存在
bool find_file_server(vector<FCB>& a,string& filename)
{
	int size = a.size();
	int i = 0;
	for(i=1;i<size;i++)
	{
		if(a[i].FileName==filename && a[i].FileType=="") return true;
	}
	return false;
}

//给当前目录中的文件或文件夹重命名
void rename(vector<FCB>& a)
{
	string file1 = "",file1_name = "",file1_type = "",file2 = "",file2_name = "",file2_type = "";
	cin >> file1 >> file2;
	int length1 = file1.length();
	int length2 = file2.length();
	int size = a.size();
	int i = 0;

	for(i=0;i<length1;i++)
	{
		if(file1[i]=='.') break;
	}
	
	if(i<length1-1)
	{
		file1_name = file1.substr(0,i);
		file1_type = file1.substr(i+1,length1-i-1);

		for(i=0;i<length2;i++)
		{
			if(file2[i]=='.') break;
		}

		if(i<length2-1)
		{
			file2_name = file2.substr(0,i);
			if(find_file(a,file2_name))
			{
				cout << "该文件名已经存在!" << endl<<endl;
				return;
			}
			file2_type = file2.substr(i+1,length2-i-1);
			for(i=1;i<size;i++)
			{
				if(a[i].FileName==file1_name && a[i].FileType != "")//判断文件1是否存在（先判断文件名）
				{
					if(a[i].FileType==file1_type)//比较文件类型
					{	
						if(file1_type==file2_type)
						{
							a[i].FileName = file2_name;
							a[i].FileType = file2_type;
							cout<<"重命名成功!"<<endl<<endl;
							return;
						}
						else
						{
							cout<<"重命名的文件格式与原来的文件不一致,您已经改变了文件类型!"<<endl;
							cout<<"是否要继续这样命名？Y/N"<<endl;
							string s;
							cin>>s;
								if(s=="Y"||s=="y")
								{
									a[i].FileName = file2_name;
									a[i].FileType = file2_type;
									cout<<"重命名成功!"<<endl<<endl;
									return;
								}
								else
								{
									cout<<"重命名失败!"<<endl<<endl;
									return;
								}
						}
					}
					else cout << "没有此文件!" << endl<<endl;
					return;
				}
			}
			if(i==size) cout << "没有此文件!" << endl<<endl;//没有找到文件1的文件名，说明文件1不存在
			return;
		}
		else if(i==length2)//对于输入的文件名2的情况是没有文件类型的输入方式
		{
			file2_name = file2.substr(0,i);
			if(find_file(a,file2_name))
			{
				cout << "该文件已经存在!" << endl<<endl;
				return;
			}
			for(i=1;i<size;i++)
			{
				if(a[i].FileName==file1_name && a[i].FileType != "")
				{
					if(a[i].FileType==file1_type)
					{
						a[i].FileName = file2_name;
						return;
					}
					else cout << "没有此文件!" << endl<<endl;
					return;
				}
			}
			if(i==size) cout << "没有此文件!" << endl<<endl;
			return;
		}
		else
		{
			cout << "对此文件的命名不合法!" <<endl<<endl;
			return;
		}
	
	}

	else if(i==length1)
	{
		for(i=0;i<length2;i++)
		{
			if(file2[i]=='.') break;
		}

		if(i==length2)
		{
			if(find_file_server(a,file2))
			{
				cout << "该文件夹已经存在!" << endl<<endl;
				return;
			}
			for(i=1;i<size;i++)
			{
				if(a[i].FileName==file1 && a[i].FileType=="")
				{
					a[i].FileName = file2;
					cout<<"重命名成功!"<<endl<<endl;
					return;
				}
			}
			if(i==size) cout << "没有此文件夹!" << endl<<endl;
			return;
		}
		else cout << "对此文件夹的命名不合法!"<<endl<<endl;
	
	}

	else cout << "对此文件夹的命名不合法!"<<endl<<endl;
	
}


//复制当前目录中的文件或文件夹
void copy(vector<FCB>& a)
{
	string file= "",file_name="",file_type="";
	cin >> file;
	int length = file.length();
	int size = a.size();
	int i = 0;
	
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	
	if(i<length-1)
	{
		file_name = file.substr(0,i);
		file_type = file.substr(i+1,length-i-1);
		for(i=0;i<size;i++)
		{
			if(a[i].FileName==file_name&&a[i].FileType==file_type)
			{
				fileCopy.FileName =a[i].FileName;
				fileCopy.FileType =a[i].FileType;
				fileCopy.subdirectory =a[i].subdirectory;
				fileCopy.updirectory =a[i].updirectory;
				fileCopy.parent =a[i].parent;
				fileCopy.creating_time=a[i].creating_time;
//	            fileCopy.last_modified_time=a[i].last_modified_time;
				fileCopy.details=a[i].details;
				fileCopy.status=a[i].status;
				cout << "已成功将文件"+file+"复制!" << endl<<endl;
			    return;
			}
		
		}
		if(i==size)
		{
			cout << "该文件不存在!" << endl<<endl;
			return;
		}
	}
	else
	{
		file_name = file;
		file_type = "";
		for(i=0;i<size;i++)
		{
			if(a[i].FileName==file_name&&a[i].FileType==file_type)
			{
				fileCopy.FileName =a[i].FileName;
				fileCopy.FileType =a[i].FileType;
				fileCopy.subdirectory =a[i].subdirectory;
				fileCopy.updirectory =a[i].updirectory;
				fileCopy.parent =a[i].parent;
				fileCopy.creating_time=a[i].creating_time;
//	            fileCopy.last_modified_time=a[i].last_modified_time;
				fileCopy.details=a[i].details;
				fileCopy.status=a[i].status;
				cout << "已成功将文件夹"+file+"复制!" << endl<<endl;
				return;
			}
		}
		if(i==size)
		{
			cout << "该文件夹不存在!" << endl<<endl;
			return;
		}
	}
}


//剪切当前目录中的文件或文件夹
void cut(vector<FCB>& a)
{
	string file= "",file_name="",file_type="";
	cin >> file;
	int length = file.length();
	int size = a.size();
	int i = 0;
	
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	
	if(i<length-1)
	{
		file_name = file.substr(0,i);
		file_type = file.substr(i+1,length-i-1);
		for(i=0;i<size;i++)
		{
			if(a[i].FileName==file_name&&a[i].FileType==file_type)
			{
				fileCopy.FileName =a[i].FileName;
				fileCopy.FileType =a[i].FileType;
				fileCopy.subdirectory =a[i].subdirectory;
				fileCopy.updirectory =a[i].updirectory;
				fileCopy.parent =a[i].parent;
				fileCopy.creating_time=a[i].creating_time;
//	            fileCopy.last_modified_time=a[i].last_modified_time;
				fileCopy.details=a[i].details;
				fileCopy.status=a[i].status;
				del(a,file);
			    cout << "已成功将文件"+file+"剪切!" << endl<<endl;
				return;
			}
		}
		if(i==size)
		{
			cout << "该文件不存在!" << endl<<endl;
			return;
		}
	}
	else
	{
		file_name = file;
		file_type = "";
		for(i=0;i<size;i++)
		{
			if(a[i].FileName==file_name&&a[i].FileType==file_type)
			{
				fileCopy.FileName =a[i].FileName;
				fileCopy.FileType =a[i].FileType;
				fileCopy.subdirectory =a[i].subdirectory;
				fileCopy.updirectory =a[i].updirectory;
				fileCopy.parent =a[i].parent;
				fileCopy.creating_time=a[i].creating_time;
//	            fileCopy.last_modified_time=a[i].last_modified_time;
				fileCopy.details=a[i].details;
				fileCopy.status=a[i].status;
				del(a,file);
			    cout << "已成功将文件夹"+file+"剪切!" << endl<<endl;
			    return;
			}
		
		}
		if(i==size)
		{
			cout << "该文件夹不存在!" << endl<<endl;
			return;
		}
	}
}


//用于在粘贴中在当前目录中创建文件或文件夹
void create(vector<FCB> * & a,FCB fcb)
{
	string name,type;
	int i;
	int size=(*a).size();

	if(fcb.FileType!="")
	{
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==fcb.FileName && (*a)[i].FileType==fcb.FileType )
			{
				cout << "已存在同名文件，无法粘贴到此位置!" << endl<<endl;
				return;
			}
		}
		(*a).push_back(fcb);
		cout << "成功将文件"+fcb.FileName+"粘贴到此位置!" << endl<<endl;
		return;
	}
	else//文件夹
	{
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==fcb.FileName && (*a)[i].FileType==fcb.FileType)
			{
				cout << "已存在同名文件夹，无法粘贴到此位置!" << endl<<endl;
				return;
			}
		}

		(*a).push_back(fcb);
		cout << "成功将文件夹"+fcb.FileName+"粘贴到此位置!" << endl<<endl;
		return;
	}
}

//粘贴文件或文件夹到当前目录中
void paste(vector<FCB>* & a)
{
	create(a,fileCopy);
}

//显示文件或文件夹的属性
void attribute(vector<FCB>& a)
{
    string file = "",filename = "",filetype = "";
	cin >> file;
	int length = file.length();
	int size = a.size();
	int i = 0;
	for(i=0;i<length;i++)
	{
		if(file[i]=='.') break;
	}
	if(i<length-1)
	{
		filename = file.substr(0,i);
		filetype = file.substr(i+1,length-i-1);
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==filename && a[i].FileType != "")
			{
				if(a[i].FileType==filetype)
				{
					cout<<"***该文件属性***"<<endl;
					cout << "文件"<<a[i].FileName;
					cout << "." << a[i].FileType<<endl;
					cout<<"文件类型："<<a[i].FileType<<endl;
					cout << "创建时间: ";
					show_time(a[i].creating_time);
					cout <<endl<< "最近访问时间: ";
//					show_time(a[i].last_modified_time);
					cout <<endl<< "状态: ";
					cout << a[i].status << endl<<endl;
					return;
				}
				else cout << "当前目录下没有这个文件!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "当前目录下没有这个文件!" <<endl<<endl;
		return;
	}
	else if(i==length)////打开文件夹
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				    cout<<"***该文件夹属性***"<<endl;
					cout << "文件夹"<<a[i].FileName<<endl;
					cout << "创建时间: ";
					show_time(a[i].creating_time);
					cout <<endl<< "最近访问时间: ";
//					show_time(a[i].last_modified_time);
					cout <<endl<< "状态: ";
					cout << a[i].status << endl<<endl;
					return;
			}
		}
		if(i==size) cout << "没有这个文件夹!" << endl<<endl;
		return;
	}
	else cout << "没有这个文件夹!" << endl<<endl;
	
}




int main()
{
	vector<FCB> * root = new vector<FCB>();
	vector<FCB> * current = new vector<FCB>();
	string command;
	string directory = "root:/";
	FCB fcb;

	fcb.FileName = "";
	fcb.FileType = "";
	fcb.subdirectory = NULL;
	fcb.updirectory = NULL;
	fcb.parent = "";
	GetLocalTime(&fcb.creating_time);
//	GetLocalTime(&fcb.last_modified_time);

	(*root).push_back(fcb);
	current = root;
	
    Login();
    cout<<"******************  请认真查看帮助文档，以便操作此系统!  ******************"<<endl<<endl;
	help();

	cout<<"**************************  root表示根目录!  **************************"<<endl<<endl;

	while(1)
	{
		cout << directory << ">";
		cin >> command;
		if(command=="dir") dir(*current);
		else if(command=="help") help();
		else if(command=="create") create(current);
		else if(command=="delete") del(*current);
		else if(command=="open") open(*current,directory);
		else if(command=="close") close(*current);
		else if(command=="cd") cd(current,directory,root);
		else if(command=="rename") rename(*current);
		else if(command=="paste") paste(current);
		else if(command=="cut") cut(*current);
		else if(command=="copy") copy(*current);
		else if(command=="attrib") attribute(*current);
		else if(command=="exit") exit(0);
		else if(is_require(command)) command_explan(command);
		else error();

	}

	return 0;
}