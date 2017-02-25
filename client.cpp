#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <bitset>
#include "view.h"

#pragma comment (lib,"ws2_32.lib")

using namespace std;

const int w = 16, h = 20, viewlen = 19 + w*h , buflen = 64;
const int N = 7;

vector<block> kinds;
view v1(w,h), v2(w,h);



void update(view &tmp, bitset<viewlen>& buf)
{
	int loc = 1;
	tmp.over = buf[loc++];
	for(int i = 0; i != w; ++i)
	{
		for(int j = 0; j != h; ++j)
		{
			tmp.solu[j][i] = buf[loc++];
		}
	}
	for(int i = 0; i != 4; ++i)
	{
		for(int j = 0; j != 4; ++j)
		{
			tmp.next.solu[j][i] = buf[loc++];
		}
	}
}

DWORD WINAPI GAME1(LPVOID lp)
{
	v1.next.ini();
	v1.next.clear_buf();
	v1.ini();
	v1.clear_buf();
	int now1;
	int next1 = N*rand()/(RAND_MAX+1);

	while(!v1.over && !v2.over)
	{
		now1 = next1;
		next1 = N*rand()/(RAND_MAX+1);
		v1.load_block(kinds[now1]);
		v1.next.load_block(kinds[next1]);

		if(!v1.down())
		{
			v1.over = true;
			break;
		}

		while(v1.down() && !v1.over && !v2.over)
		{
			Sleep(500);
		}
		v1.load_has();
		v1.check();
		v1.clear_buf();
		v1.next.ini();
	}
	return 0;
}

DWORD WINAPI Control(LPVOID lp)
{
	SOCKET *fd = (SOCKET*)lp;
	char a[1];
	while(!v1.over && !v2.over)
	{
		if(kbhit())
		{
			a[0] = getch();
			send(*fd,a,1,0);
			switch(a[0])
			{
			case 'f':
				if(v1.scores >= 1)
				{
					v1.scores -= 1;
				}
				break;
			case 'a':
				v1.left();
				break;
			case 'd':
				v1.right();
				break;
			case 's':
				v1.down();
				break;
			case 'w':
				v1.change();
				break;
			case 'q':
				v1.over = true;
				break;
			}
		}
		Sleep(50);
	}
	return 0;
}

DWORD WINAPI JS(LPVOID lp)
{
	SOCKET *fd = (SOCKET*) lp;

	bitset<viewlen> bit_buf;
	char buf[buflen];

	while(!v1.over && !v2.over)
	{
		if( recv(*fd,buf,buflen,0) == buflen )
		{
			bit_buf = *(bitset<viewlen>*)&buf;
			if(bit_buf[0] == 1)
			{
				v1.attack();
			}
			update(v2,bit_buf);
		}
	}
	return 0;
}

DWORD WINAPI Screen(LPVOID lp)
{
	while(!v1.over && !v2.over)
	{
		v1.outputvl();
		v2.outputvr(v2.next.r);
		Sleep(1.0/100.0);
	}
	return 0;
}

void game()
{
	Sleep(3000);
	SOCKET connfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	sockaddr_in servaddr;
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.S_un.S_addr = inet_addr("103.44.145.245");
	servaddr.sin_port = htons(34366);
	//103.44.145.245
	cout << "等待连接。" << endl;
	connect(connfd,(sockaddr*)&servaddr,sizeof(servaddr));
	char K[1] = {'0'};
	while(1)
		{
		recv(connfd,K,1,0);
	if(K[0] == '8') break;
	}
	
	HANDLE HC,HScreen,HJS,HGAME1;
	HC = CreateThread(NULL,0,Control,(LPVOID)&connfd,0,NULL);
	HJS = CreateThread(NULL,0,JS,(LPVOID)&connfd,0,NULL);
	HScreen = CreateThread(NULL,0,Screen,NULL,0,NULL);
	HGAME1=CreateThread(NULL,0,GAME1,NULL,0,NULL);

	while(WaitForSingleObject(HC,50) || WaitForSingleObject(HJS,50) || WaitForSingleObject(HScreen,50) || WaitForSingleObject(HGAME1,50) ) {}

	v2.ini();

	CloseHandle(HC);
	CloseHandle(HJS);
	CloseHandle(HScreen);
	CloseHandle(HGAME1);
	closesocket(connfd);
}

void main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,0),&wsa);

	//隐藏光标
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle,&CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle,&CursorInfo);

	block ZFX(0,1,4,5);
	block LICK(0,1,2,3);
	block L(0,1,2,4);
	block L2(0,1,2,6);
	block T(0,1,2,5);
	block Z(0,1,5,6);
	block Z2(1,2,4,5);

	kinds.push_back(ZFX);
	kinds.push_back(LICK);
	kinds.push_back(L);
	kinds.push_back(L2);
	kinds.push_back(T);
	kinds.push_back(Z);
	kinds.push_back(Z2);

	while(1)
	{
		game();
		system("Cls");
		cout << "2s后重新开始。" << endl;
		v1.ini();
		v1.over = false;
		v2.over = false;
	}
}
