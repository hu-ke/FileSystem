#include<iostream>
#include<string>
#include<vector>
#include<new>
#include <windows.h>

using namespace std;

typedef struct FCB
{
	string FileName; //�ļ���
	string FileType; //�ļ�����
	vector<FCB> * subdirectory; //��һ��Ŀ¼��ַ
	vector<FCB> * updirectory; //��һ��Ŀ¼��ַ
	string parent; //���ļ�����
	SYSTEMTIME creating_time; //����ʱ��
//	SYSTEMTIME last_modified_time; //�������ʱ��
	string status; //�ļ�״̬
	string details;//�ļ�����
}FCB;

FCB fileCopy;//ȫ�ֱ��������ڱ��渴�ƻ���е��ļ����ļ���

//��¼
void Login()
{
	string name,password;
	cout <<"\n\n\n";
	cout <<"\t\t            ����ϵͳ��������-ģ���ļ�ϵͳ"<<endl<<endl<<endl<<endl<<endl<<endl;
	cout <<"\t\t             ���ߣ�XXX"<<endl<<endl<<endl;
	system("pause");
    system("cls");
}

//��ʾʱ��
void show_time(SYSTEMTIME& sys)
{
	cout << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << " " << sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond;
}

//��ʾ��ǰĿ¼�е������ļ����ļ���
void dir(vector<FCB>& a)
{
	int length = a.size();
	int i = 0;
	for(i=1;i<length;i++)
	{
		if(a[i].FileType != "")
		{
			cout << "�ļ�"<<a[i].FileName;
			cout << "." << a[i].FileType<<endl;
			cout << "    ����ʱ��: ";
			show_time(a[i].creating_time);
			cout <<endl<< "    �������ʱ��: ";
///			show_time(a[i].last_modified_time);
			cout <<endl<< "    ״̬: ";
			cout << a[i].status << endl<<endl;
		}
		else
		{
			cout << "�ļ���"+a[i].FileName<<endl;
			cout << "      ����ʱ��: ";
			show_time(a[i].creating_time);
			cout << endl<<"      �������ʱ��: ";
//			show_time(a[i].last_modified_time);
			cout << endl<<"      ״̬: ";
			cout << a[i].status << endl<<endl;
		}
	}
	if(i==1) cout<<"��ǰĿ¼��û������!"<<endl<<endl;
}

//��ʾ������Ϣ
void help()
{
	cout << "dir  " << "��ʾ��ǰĿ¼�е������ļ����ļ���" << endl;
	cout << "help  " << "��ʾ������Ϣ" << endl;
	cout << "����+?  " << "��ʾ�������˵���ĵ����磺dir?" << endl;
	cout << "create xxx  " << "�ڵ�ǰĿ¼�д����ļ�xxx.yyy���ļ���xxx" << endl;
	cout << "delete xxx  " << "�ڵ�ǰĿ¼��ɾ���ļ�xxx.yyy���ļ���xxx" << endl;
	cout << "open xxx  " << "�򿪵�ǰĿ¼�е��ļ�xxx.yyy���ļ���xxx" << endl;
	cout << "close xxx  " << "�رյ�ǰĿ¼�е��ļ�xxx.yyy���ļ���xxx" << endl;
	cout << "cd ..  " << "���ص���һ��Ŀ¼" << endl;
	cout << "cd xxx  " << "ת����ǰĿ¼��xxx��Ŀ¼" << endl;
	cout << "cd root  " << "ת����Ŀ¼root" << endl;
	cout << "rename xxx yyy  " << "������" << endl;
	cout << "copy xxx  " << "����" << endl;
	cout << "cut xxx  " << "����" << endl;
	cout << "paste  " << "ճ��" << endl;
	cout << "attrib xxx  " << "��ʾxxx�ļ����ļ��е�����" << endl;
	cout << "exit  " << "�˳�" << endl<<endl;
}

//�ж��Ƿ��ǰ�����������
bool is_require(string& str)
{
	return (str[str.length()-1] == '?');
}

//��ʾ�����˵��
void command_explan(string& command)
{
	if(command=="dir?") cout << "\"dir\"����������ʾ��ǰĿ¼�е������ļ����ļ���" << endl<<endl;
	else if(command=="help?") cout << "\"help\"����������ʾ������Ϣ" << endl<<endl;
	else if(command=="create?") cout << "\"create\"���������ڵ�ǰĿ¼�д����ļ����ļ��У��磺create hello.txt,create hello,ע�⣺�����ļ���������չ��������չ������Ϊ���ļ���,�ļ����ļ�����ͬ�����ļ��к��ļ��в���ͬ�����ļ����ļ��п���ͬ��" << endl<<endl;
	else if(command=="delete?") cout << "\"delete\"���������ڵ�ǰĿ¼��ɾ���ļ����ļ��У��磺delete hello.txt,delete hello" << endl<<endl;
	else if(command=="open?") cout << "\"open\"���������ڵ�ǰĿ¼�д��ļ����ļ��У��磺open hello.txt,open hello" << endl<<endl;
	else if(command=="close?") cout << "\"close\"���������ڵ�ǰĿ¼�йر��ļ����ļ��У��磺close hello.txt,close hello" << endl<<endl;
	else if(command=="cd?") cout << "\"cd\"����������תĿ¼���磺cd hello,\"cd ..\"������ת����ǰĿ¼����һ��Ŀ¼��\"cd root\"����ֱ��ת����Ŀ¼" << endl<<endl;
	else if(command=="rename?") cout << "\"rename\"�������ڸ���ǰĿ¼�е��ļ����ļ������������磺rename hello.txt hi.cpp(�����޸���չ��),rename hello.txt hi(������չ����Ĭ��Ϊԭ��չ��),rename hello hi(�޸��ļ�����)" << endl<<endl;
	else if(command=="cut?") cout << "\"cut\"�������ڼ��е�ǰĿ¼�е��ļ����ļ��У��磺cut hello.txt"<< endl<<endl;
	else if(command=="paste?") cout << "\"paste\"�������ڽ��ļ������ļ���ճ������ǰĿ¼�У��磺paste ��������ֱ�Ӱ�ǰ�渴�ƻ����Ǽ��е��ļ�ճ������ǰλ��"<< endl<<endl;
	else if(command=="copy?") cout << "\"copy\"�������ڸ��Ƶ�ǰĿ¼�е��ļ����ļ��У��磺cut hello.txt"<< endl<<endl;
	else if(command=="attrib?") cout << "\"attrib\"����������ʾ�ļ����ļ��е����ԣ��磺attrib hello.txt"<< endl<<endl;
	else if(command=="exit?") cout << "\"exit\"���������˳�ϵͳ" << endl;
	else cout << "û�и�����!�������������!" << endl<<endl;
}

//�ڵ�ǰĿ¼�д����ļ����ļ���
void create(vector<FCB> * & a)
{
	string file = "",name = "",type = "";
	FCB fcb,fcb2;
	int length = 0,i = 0;
	int size = (*a).size();

	cin >> 	file;
	length = file.length();

	for(i=0;i<length;i++)//���������ļ����к��С�.������˵�����ļ������ļ����ʹ����ļ��ṹ�е����Ͳ���
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
				cout << "���ļ����Ѵ���!" << endl<<endl;
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
		cout<<"�������ļ�����:"<<endl;
		cin>>fcb.details; 
		(*a).push_back(fcb);
		cout<<"�ļ�"<<file<<"�����ɹ�!"<<endl<<endl;
	}
	else//�ļ���ѭ��������Ҳû���ҵ���.��,��˵�����ļ���
	{
		name = file.substr(0,i);
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==name && (*a)[i].FileType=="")
			{
				cout << "���ļ����Ѿ�����!" << endl<<endl;
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
		cout<<"�ļ���"<<file<<"�����ɹ�!"<<endl<<endl;
	}
}

//�ڵ�ǰĿ¼��ɾ���ļ����ļ���
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
					cout <<"�ļ�"<< filename << "." << filetype <<"�Ѿ�ɾ���ɹ�!" << endl<<endl; 
				}
				else cout << "û������ļ�!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "û������ļ�!" <<endl<<endl;
	}
	else if(i==length)
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				a.erase(&a[i]);
				cout << "�ļ���" << file<< "�Ѿ�ɾ���ɹ�!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "û������ļ���!" <<endl<<endl;
	}
	else cout << "û������ļ���!" <<endl<<endl;
}




//���ڼ��й����У��ڵ�ǰĿ¼��ɾ���ļ����ļ���
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



//�򿪵�ǰĿ¼�е��ļ�
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
						cout << "�ļ�"<<a[i].FileName << "." << a[i].FileType << "�ɹ���!" <<endl;
						cout<<"��ʾ�ļ����ݣ�"<<a[i].details<<endl<<endl;
						return;
					}
					else
					{
						cout <<"�ļ�"<<a[i].FileName << "." << a[i].FileType <<  "�Ѿ��Ǵ�״̬!"<<endl;
						cout<<"��ʾ�ļ����ݣ�"<<a[i].details<<endl<<endl;
						return;
					}
				}
				else cout << "��ǰĿ¼��û������ļ�!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "��ǰĿ¼��û������ļ�!" <<endl<<endl;
		return;
	}
	else if(i==length)////���ļ���
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				if(a[i].status=="closed")
				{
					a[i].status = "opened";
//					GetLocalTime(&a[i].last_modified_time);
					cout << "�ļ���"<<a[i].FileName << "�ɹ���!" <<endl;//////////////
				    cout <<"��ʾ���ļ������ݣ�" <<endl;
					dir(*a[i].subdirectory);
					return;
				}
				else
				{
					cout <<"�ļ���"<< a[i].FileName << "�Ѿ��Ǵ�״̬!" << endl;
					cout <<"��ʾ���ļ������ݣ�" <<endl;
				    dir(*a[i].subdirectory);
					return;
				}
			}
		}
		if(i==size) cout << "û������ļ���!" << endl<<endl;
		return;
	}
	else cout << "û������ļ���!" << endl<<endl;
	
}

//�رյ�ǰĿ¼�е��ļ�
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
						cout <<"�ļ�" <<a[i].FileName << "." << a[i].FileType << "�ɹ��ر�!" << endl<<endl;
						return;
					}
					else
					{
						cout <<"�ļ�" << a[i].FileName << "." << a[i].FileType << "ԭ�����ǹر�״̬!" << endl<<endl;
						return;
					}
				}
				else cout << "û������ļ�!" <<endl<< endl;
				return;
			}
		}
		if(i==size) cout <<  "û������ļ�!"  <<endl<<endl;
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
					cout <<"�ļ���"<< a[i].FileName << " " << "�ɹ��ر�!" << endl<<endl;
					return;
				}
				else
				{
					cout <<"�ļ���"<< a[i].FileName << " " << "ԭ�����ǹر�״̬!" << endl<<endl;
					return;
				}
			}
		}
		if(i==size) cout << "û������ļ���!" << endl;
	}
	else cout << "û������ļ���!" << endl;
	
}

//��ʾ������Ϣ
void error()
{
	cout << "�������!!" << endl<<endl;
}

//���ĵ�ǰĿ¼
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
			cout << "û�д�Ŀ¼!" << endl<<endl;
			return;
		}
	}

}

//�жϵ�ǰĿ¼��ָ�����Ƶ��ļ��Ƿ����
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

//�жϵ�ǰĿ¼��ָ�����Ƶ��ļ����Ƿ����
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

//����ǰĿ¼�е��ļ����ļ���������
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
				cout << "���ļ����Ѿ�����!" << endl<<endl;
				return;
			}
			file2_type = file2.substr(i+1,length2-i-1);
			for(i=1;i<size;i++)
			{
				if(a[i].FileName==file1_name && a[i].FileType != "")//�ж��ļ�1�Ƿ���ڣ����ж��ļ�����
				{
					if(a[i].FileType==file1_type)//�Ƚ��ļ�����
					{	
						if(file1_type==file2_type)
						{
							a[i].FileName = file2_name;
							a[i].FileType = file2_type;
							cout<<"�������ɹ�!"<<endl<<endl;
							return;
						}
						else
						{
							cout<<"���������ļ���ʽ��ԭ�����ļ���һ��,���Ѿ��ı����ļ�����!"<<endl;
							cout<<"�Ƿ�Ҫ��������������Y/N"<<endl;
							string s;
							cin>>s;
								if(s=="Y"||s=="y")
								{
									a[i].FileName = file2_name;
									a[i].FileType = file2_type;
									cout<<"�������ɹ�!"<<endl<<endl;
									return;
								}
								else
								{
									cout<<"������ʧ��!"<<endl<<endl;
									return;
								}
						}
					}
					else cout << "û�д��ļ�!" << endl<<endl;
					return;
				}
			}
			if(i==size) cout << "û�д��ļ�!" << endl<<endl;//û���ҵ��ļ�1���ļ�����˵���ļ�1������
			return;
		}
		else if(i==length2)//����������ļ���2�������û���ļ����͵����뷽ʽ
		{
			file2_name = file2.substr(0,i);
			if(find_file(a,file2_name))
			{
				cout << "���ļ��Ѿ�����!" << endl<<endl;
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
					else cout << "û�д��ļ�!" << endl<<endl;
					return;
				}
			}
			if(i==size) cout << "û�д��ļ�!" << endl<<endl;
			return;
		}
		else
		{
			cout << "�Դ��ļ����������Ϸ�!" <<endl<<endl;
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
				cout << "���ļ����Ѿ�����!" << endl<<endl;
				return;
			}
			for(i=1;i<size;i++)
			{
				if(a[i].FileName==file1 && a[i].FileType=="")
				{
					a[i].FileName = file2;
					cout<<"�������ɹ�!"<<endl<<endl;
					return;
				}
			}
			if(i==size) cout << "û�д��ļ���!" << endl<<endl;
			return;
		}
		else cout << "�Դ��ļ��е��������Ϸ�!"<<endl<<endl;
	
	}

	else cout << "�Դ��ļ��е��������Ϸ�!"<<endl<<endl;
	
}


//���Ƶ�ǰĿ¼�е��ļ����ļ���
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
				cout << "�ѳɹ����ļ�"+file+"����!" << endl<<endl;
			    return;
			}
		
		}
		if(i==size)
		{
			cout << "���ļ�������!" << endl<<endl;
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
				cout << "�ѳɹ����ļ���"+file+"����!" << endl<<endl;
				return;
			}
		}
		if(i==size)
		{
			cout << "���ļ��в�����!" << endl<<endl;
			return;
		}
	}
}


//���е�ǰĿ¼�е��ļ����ļ���
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
			    cout << "�ѳɹ����ļ�"+file+"����!" << endl<<endl;
				return;
			}
		}
		if(i==size)
		{
			cout << "���ļ�������!" << endl<<endl;
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
			    cout << "�ѳɹ����ļ���"+file+"����!" << endl<<endl;
			    return;
			}
		
		}
		if(i==size)
		{
			cout << "���ļ��в�����!" << endl<<endl;
			return;
		}
	}
}


//������ճ�����ڵ�ǰĿ¼�д����ļ����ļ���
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
				cout << "�Ѵ���ͬ���ļ����޷�ճ������λ��!" << endl<<endl;
				return;
			}
		}
		(*a).push_back(fcb);
		cout << "�ɹ����ļ�"+fcb.FileName+"ճ������λ��!" << endl<<endl;
		return;
	}
	else//�ļ���
	{
		for(i=1;i<size;i++)
		{
			if((*a)[i].FileName==fcb.FileName && (*a)[i].FileType==fcb.FileType)
			{
				cout << "�Ѵ���ͬ���ļ��У��޷�ճ������λ��!" << endl<<endl;
				return;
			}
		}

		(*a).push_back(fcb);
		cout << "�ɹ����ļ���"+fcb.FileName+"ճ������λ��!" << endl<<endl;
		return;
	}
}

//ճ���ļ����ļ��е���ǰĿ¼��
void paste(vector<FCB>* & a)
{
	create(a,fileCopy);
}

//��ʾ�ļ����ļ��е�����
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
					cout<<"***���ļ�����***"<<endl;
					cout << "�ļ�"<<a[i].FileName;
					cout << "." << a[i].FileType<<endl;
					cout<<"�ļ����ͣ�"<<a[i].FileType<<endl;
					cout << "����ʱ��: ";
					show_time(a[i].creating_time);
					cout <<endl<< "�������ʱ��: ";
//					show_time(a[i].last_modified_time);
					cout <<endl<< "״̬: ";
					cout << a[i].status << endl<<endl;
					return;
				}
				else cout << "��ǰĿ¼��û������ļ�!" <<endl<<endl;
				return;
			}
		}
		if(i==size) cout << "��ǰĿ¼��û������ļ�!" <<endl<<endl;
		return;
	}
	else if(i==length)////���ļ���
	{
		for(i=1;i<size;i++)
		{
			if(a[i].FileName==file && a[i].FileType=="")
			{
				    cout<<"***���ļ�������***"<<endl;
					cout << "�ļ���"<<a[i].FileName<<endl;
					cout << "����ʱ��: ";
					show_time(a[i].creating_time);
					cout <<endl<< "�������ʱ��: ";
//					show_time(a[i].last_modified_time);
					cout <<endl<< "״̬: ";
					cout << a[i].status << endl<<endl;
					return;
			}
		}
		if(i==size) cout << "û������ļ���!" << endl<<endl;
		return;
	}
	else cout << "û������ļ���!" << endl<<endl;
	
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
    cout<<"******************  ������鿴�����ĵ����Ա������ϵͳ!  ******************"<<endl<<endl;
	help();

	cout<<"**************************  root��ʾ��Ŀ¼!  **************************"<<endl<<endl;

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