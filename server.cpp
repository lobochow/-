#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdlib.h>
#include <utility>
#include <stdlib.h>
#include <algorithm>
#include <conio.h>
#include <Bitset>
#include "block.h"
#include "view.h"

#pragma comment(lib,"ws2_32.lib")

using namespace std;

const int w = 16, h = 20;
const int viewlen = 20 + w*h, buflen = 64;
unsigned int N = 7;

view v1(w,h), v2(w,h);

vector<block> kinds;

void tras(view &tmp, bitset<viewlen> &data)
{
	int loc = 0;
	data[loc++] = tmp.S;
	if(tmp.S == 1) tmp.S = 0;
	data[loc++] = tmp.over;
	for(int i = 0; i != w; ++i)
	{
		for(int j = 0; j != h; ++j)
		{
			data[loc++] = tmp.solu[j][i];
		}
	}
	for(int i = 0; i != 4; ++i)
	{
		for(int j = 0; j != 4; ++j)
		{
			data[loc++] = tmp.next.solu[j][i];
		}
	}
}
				
DWORD WINAPI Control1(LPVOID lp)
{
	SOCKET *fd = (SOCKET*) lp;
	char a[1];

	while(!v1.over)
	{
		if( recv(*fd,a,1,0) == 1)
		switch(a[0])
		{
		case 'f':
			if(v1.scores >= 1)
			{
				v1.S = 1;
				v2.attack();
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
	return 0;
}

DWORD WINAPI Control2(LPVOID lp)
{
	SOCKET *fd = (SOCKET*) lp;
	char a[1];

	while(!v2.over)
	{
		if(recv(*fd,a,1,0) == 1)
		{
			switch(a[0])
			{
			case 'f':
				if(v2.scores >= 1)
				{
					v2.S = 1;
					v1.attack();
					v2.scores -=1;
				}
				break;
			case 'a':
				v2.left();
				break;
			case 'd':
				v2.right();
				break;
			case 's':
				v2.down();
				break;
			case 'w':
				v2.change();
				break;
			case 'q':
				v2.over = true;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI Screen(LPVOID lp)
{
	SOCKET *fd = (SOCKET*) lp;

	bitset<viewlen> bit_buf;
	char *buf = (char*)&bit_buf;

	while(!v1.over && !v2.over)
	{
		tras(v2,bit_buf);
		send(*fd,buf,buflen,0);
		Sleep(100);
	}
	tras(v2,bit_buf);
	send(*fd,buf,buflen,0);
	Sleep(10);
	return 0;
}

DWORD WINAPI Screen2(LPVOID lp)
{
	SOCKET *fd = (SOCKET*) lp;

	bitset<viewlen> bit_buf;
	char *buf = (char*)&bit_buf;

	while(!v1.over && !v2.over)
	{
		tras(v1,bit_buf);
		send(*fd,buf,buflen,0);
		Sleep(100);
	}
	tras(v1,bit_buf);
	send(*fd,buf,buflen,0);
	Sleep(10);
	return 0;
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

DWORD WINAPI GAME2(LPVOID lp)
{
	v2.next.ini();
	v2.next.clear_buf();
	v2.ini();
	v2.clear_buf();
	int now2;
	int next2 = N*rand()/(RAND_MAX+1);

	while(!v1.over && !v2.over)
	{
		now2 = next2;
		next2 = N*rand()/(RAND_MAX+1);
		v2.load_block(kinds[now2]);
		v2.next.load_block(kinds[next2]);

		if(!v2.down())
		{
			v2.over = true;
			break;
		}

		while(v2.down() && !v1.over && !v2.over)
		{
			Sleep(500);
		}

		v2.load_has();
		v2.check();
		v2.clear_buf();
		v2.next.ini();
	}
	return 0;
}

void game()
{
	sockaddr_in servaddr,cliaddr1,cliaddr2;

	int clilen1,clilen2;
	clilen1 = clilen2 = sizeof(sockaddr);

	SOCKET listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP), connfd1,connfd2;
		   
	connfd1 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	connfd2 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.S_un.S_addr = htonl(0);
	servaddr.sin_port = htons(9877);

	bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr));
	listen(listenfd,5);
	
	cout << "等待玩家连接。" << endl;
	connfd1 = accept(listenfd,(sockaddr*)&cliaddr1,&clilen1);
	cout << "一位玩家连上。" << endl;
	connfd2 = accept(listenfd,(sockaddr*)&cliaddr2,&clilen2);

	cout << "游戏开始。" << endl;
	system("Cls");

	char S[1] = {'8'};
	Sleep(1000);
	send(connfd1,S,1,0);
	send(connfd2,S,1,0);

	HANDLE HC1,HC2,HScreen,HScreen2,HGAME1,HGAME2;
	HC1=CreateThread(NULL,0,Control1,(LPVOID)&connfd1,0,NULL);
	HC2=CreateThread(NULL,0,Control2,(LPVOID)&connfd2,0,NULL);
	HScreen=CreateThread(NULL,0,Screen,(LPVOID)&connfd1,0,NULL);
	HScreen2=CreateThread(NULL,0,Screen2,(LPVOID)&connfd2,0,NULL);
	HGAME1=CreateThread(NULL,0,GAME1,NULL,0,NULL);
	HGAME2=CreateThread(NULL,0,GAME2,NULL,0,NULL);


	while(WaitForSingleObject(HScreen,50) || WaitForSingleObject(HScreen2,50) || WaitForSingleObject(HGAME1,50) || WaitForSingleObject(HGAME2,50))
	{
		v1.outputvl();
		v2.outputvr(v2.next.r);
	}
	CloseHandle(HC1);
	CloseHandle(HC2);
	CloseHandle(HScreen);
	CloseHandle(HScreen2);
	CloseHandle(HGAME1);
	CloseHandle(HGAME2);

	closesocket(listenfd);
	closesocket(connfd1);
	closesocket(connfd2);
}

void main()
{
	//socket
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

	while(!v1.over && !v2.over)
	{
		while(1)
		{
			system("Cls");
			v1.cls(0,0);
			game();
			system("Cls");
			v1.cls(0,0);
			cout << "3秒后重新开始";
			v1.over = false;
			v2.over = false;
		}
	}
}
