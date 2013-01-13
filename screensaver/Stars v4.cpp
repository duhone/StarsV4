// Stars v4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "hpt graphics.h"
#include "starobject.h"
#include "planet.h"
#include <list>
#include<stdlib.h>
#include"music.h"

using namespace std;

HPTGraphicsEngine *gengine;
HWND hwndm;
int width;
int height;
bool done;
HPT3DObject *tempobject;
HPT3DObjectP *tempobjectp;
HPT3DObjectP *tempobjectp2;
list<Star*> stars;
Planet* planet;

LARGE_INTEGER timerfreq;
LARGE_INTEGER starttime;
LARGE_INTEGER currenttime;	
float speed = 20000.0f;
float full_speed = 20000.0f;
int state = 0;
float time_to_next_state;
float acceleration;

float speedx = 20000.0f;
float full_speedx = 20000.0f;
float speedy = 20000.0f;
float full_speedy = 20000.0f;

char path[255];
int starfielddensity;
int planetsyn;
int savecpu = 1;
   
int nframes = 0;
float time_elapsed = 0;
float frame_rate = 0;
int framecounteryn;
int texturequality = 0;
int quality = 0;
bool mouse_first = true;
unsigned int mousexi;
unsigned int mouseyi;

bool check_input = false;
bool widescreen = false;

bool music = false;

char *hptstrcat(char *x,char *y)
{
	static char *temp_string = NULL;
	if(temp_string != NULL) delete[] temp_string;
	temp_string = new TCHAR[300];
	strcpy(temp_string,x);
	strcat(temp_string,y);
	return temp_string;
}


void LoadRegistry()
{
        int speedpos;
  
        HKEY skey;
	unsigned long size,type;

	size = sizeof(int);

	if((RegOpenKeyEx(HKEY_CURRENT_USER,"Software",0,KEY_ALL_ACCESS,&skey)) != ERROR_SUCCESS)
		return;

	RegOpenKeyEx(skey,"Eric Duhon",0,KEY_ALL_ACCESS,&skey);

	if((RegOpenKeyEx(skey,"Stars v4.5",0,KEY_ALL_ACCESS,&skey)) != ERROR_SUCCESS)
		return;

	RegQueryValueEx(skey,"planets",NULL,NULL,(unsigned char*)(&planetsyn),&size);
	RegQueryValueEx(skey,"frame_counter",NULL,NULL,(unsigned char*)(&framecounteryn),&size);
	RegQueryValueEx(skey,"star_density",NULL,NULL,(unsigned char*)(&starfielddensity),&size);
	RegQueryValueEx(skey,"texture_quality",NULL,&type,(unsigned char*)(&texturequality),&size);
	RegQueryValueEx(skey,"quality",NULL,&type,(unsigned char*)(&quality),&size);
    RegQueryValueEx(skey,"speed",NULL,&type,(unsigned char*)(&speedpos),&size);
    //RegQueryValueEx(skey,"save_cpu",NULL,&type,(unsigned char*)(&savecpu),&size);
	RegQueryValueEx(skey,"widescreen",NULL,&type,(unsigned char*)(&widescreen),&size);
	RegQueryValueEx(skey,"music",NULL,&type,(unsigned char*)(&music),&size);
	type = REG_SZ; size = 11;

	RegCloseKey(skey);
	if(starfielddensity == 0) starfielddensity = 1500;
	else if(starfielddensity == 1) starfielddensity = 3000;
	else starfielddensity = 6000;


	speed = speedpos * 1000.0f;
	full_speed = speedpos * 1000.0f;

#if defined DEMO
	texturequality = 0;
#endif


};

LRESULT CALLBACK WndProcsaver(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	  case WM_SYSCOMMAND:
		 if (wParam==SC_SCREENSAVE)
        {
          return FALSE;
        }
		 break;
		case WM_DESTROY:
//			MessageBox(NULL,"wm destroy","wm destroy",MB_OK);
			PostQuitMessage(0);
			done = true;
			return false;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN: 
		case WM_RBUTTONDOWN: 
/*		if(check_input)
		{*/
			case WM_KEYDOWN:
//				MessageBox(NULL,"key destroy","key destroy",MB_OK);

				PostQuitMessage(0);
				done = true;
				return false;
			/*case WM_TIMER:
				//iterate();
				return false;
			*/case WM_MOUSEMOVE:
				if(mouse_first)
				{
					mouse_first = false;
					mousexi = LOWORD(lParam);
					mouseyi = HIWORD(lParam);
				}
				else
				{
					if(abs((long)((LOWORD(lParam) - mousexi) > 150) || (abs((long)(HIWORD(lParam) - mouseyi)) > 150)))			
					{
//						MessageBox(NULL,"mouse destroy","mouse destroy",MB_OK);

						PostQuitMessage(0);
						done = true;
					}
				}
				//else nummoves++;
				//return false;
//		}

	};
	return DefWindowProc(hwnd,message,wParam,lParam);
}


void Iterate()
{
	list<Star*>::iterator tempsi;
	float timepassed;
	QueryPerformanceCounter(&currenttime);
	timepassed = (currenttime.QuadPart - starttime.QuadPart)/(float)timerfreq.QuadPart;
	starttime.QuadPart = currenttime.QuadPart;

	time_to_next_state -= timepassed;
	if(savecpu != 0)
		if(timepassed < 0.025f)
			Sleep((int)((0.025f-timepassed)*1000));


	Star* tempstar;
	if((state < 5) || (state > 7))
	{
		float sptp = speed*timepassed;
		for(tempsi= stars.begin();tempsi != stars.end();)
		{
			if((*tempsi)->movestar(sptp))
			{
				tempstar = (*tempsi);
				stars.push_back(tempstar);
				tempsi = stars.erase(tempsi);
			}
			else
			{
				tempsi++;
			}
		}
	}
//	for(int count = 0;count < 10000;count++)
//		stars[count].movestarturn(speed*timepassed,-speed*timepassed*0.5f,speed*timepassed*0.5f);
	if((state > 0) && (state < 4)) planet->movestar(speed*timepassed);
	if((state > 0) && (state < 4)) planet->MoveSky(timepassed);

	switch(state)
	{
	case 0:
		if(planetsyn != 0)
		{
			if(time_to_next_state < 0)
			{
				state = 1;
				planet->createnewstar();
			}
		}
		break;
	case 1:
		if(planet->GetZ() < 50000)
		{
			state = 2;
			acceleration = ((300*300) - (speed*speed))*0.0000125f/*/(float)(2*(50000-10000))*/;
		}
		break;
	case 2:
		speed += acceleration*timepassed;
		if(speed <= 300)
		{
			speed = 300;
			state = 3;
		}
		break;
	case 3:
		if(planet->IsDone())
		{
			acceleration = ((full_speed*full_speed) - (speed*speed))/(float)(2*(50000-10000));
			state = 4;
		}
		break;
	case 4:
		speed += acceleration*timepassed;
		if(speed >= full_speed)
		{
			speed = full_speed;
			float angle = (rand() % 6283) / 1000.0f;
			full_speedx = cos(angle)*20000;
			full_speedy = sin(angle)*20000;
			speedx = 0;
			speedy = 0;
			state = 5;
		}
		break;
	case 5:
		speedx += full_speedx *0.2f * timepassed;
		speedy += full_speedy *0.2f * timepassed;
		for(tempsi= stars.begin();tempsi != stars.end();)
		{
			if((*tempsi)->movestarturn(speed*timepassed,speedx*timepassed,speedy*timepassed))
			{
				tempstar = (*tempsi);
				tempsi = stars.erase(tempsi);
				stars.push_back(tempstar);
			}
			else
			{
				tempsi++;
			}
		}
		if(fabs(speedx) >= fabs(full_speedx))
		{
			time_to_next_state = ((rand()%6000)+2000)/1000.0f;

			state = 6;
		}
		break;
	case 6:
		for(tempsi= stars.begin();tempsi != stars.end();)
		{
			if((*tempsi)->movestarturn(speed*timepassed,speedx*timepassed,speedy*timepassed))
			{
				tempstar = (*tempsi);
				tempsi = stars.erase(tempsi);
				stars.push_back(tempstar);
			}
			else
			{
				tempsi++;
			}
		}
		if(time_to_next_state <= 0)
		{
			time_to_next_state = 5.0f;
			state = 7;
		}
		break;
	case 7:
		speedx -= full_speedx *0.2f * timepassed;
		speedy -= full_speedy *0.2f * timepassed;
		for(tempsi= stars.begin();tempsi != stars.end();)
		{
			if((*tempsi)->movestarturn(speed*timepassed,speedx*timepassed,speedy*timepassed))
			{
				tempstar = (*tempsi);
				tempsi = stars.erase(tempsi);
				stars.push_back(tempstar);
			}
			else
			{
				tempsi++;
			}
		}
		if(time_to_next_state <= 0)
		{
			time_to_next_state = ((rand()%30000)+5000)/1000.0f;

			state = 0;
		}
		break;

	};
	gengine->BeginFrame();
	for(list<Star*>::reverse_iterator temprsi= stars.rbegin();temprsi != stars.rend();temprsi++)
		(*temprsi)->drawstar();
	if((state > 0) && (state < 4)) planet->drawstar();
/*	tempobject->Render();*/
//	tempobjectp->Render();
//	tempobjectp2->Render();

	nframes++;
	time_elapsed += timepassed;
	if(nframes > 100)
	{
		if(time_elapsed <= 0) time_elapsed = 0.000001f;
		frame_rate = nframes/time_elapsed;
		nframes = 0;
		time_elapsed = 0;

	}
	char tempstring[30];
	sprintf(tempstring,"Frame rate is %f",frame_rate);
/*	int pos = 0;
	for(unsigned int count = 0;count < strlen(temp);count++)
	{
		if(temp[count] != '0') pos = count;
	}
	temp[pos+1] = 0;
*/
	gengine->Position(0,0);
	if(framecounteryn != 0) (*gengine) << tempstring;

#if defined DEMO
	if(check_input)
	{
		gengine->Position(0.05f*width,0.2f*height);
		(*gengine) << "This is a DEMO version, Full version does not have these messages.";

		gengine->Position(0.08f*width,0.4f*height);
		(*gengine) << "Full version supports high resolution textures.";
		
		gengine->Position(0.11f*width,0.6f*height);
		(*gengine) << "Visit http://www.hpt-interactive.com to get your copy today!!!";

		gengine->Position(0.14f*width,0.8f*height);
		(*gengine) << "Only $6.95!!!";
	}
#endif
	
	gengine->EndFrame();

}


			
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	//MessageBox(NULL,"start","start",MB_OK);
	OutputDebugString("start stars");
/*	HANDLE mutex;
	char name[9] = "stars v4";
	name[8] = 0;
	mutex = CreateMutex(NULL,TRUE,name);
	if(GetLastError() == ERROR_ALREADY_EXISTS) return 0;
	//MessageBox(NULL,"mutex","mutex",MB_OK);
	OutputDebugString("created mutex");*/

	
	GetModuleFileName(NULL,path,256);
	path[_tcslen(path) - 12] = 0;
		done = false;
	srand(GetTickCount());
	MSG msg;

	if(strlen(lpCmdLine) == 0)
	{
		check_input = true;
		SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,TRUE,0,0);
		width = GetSystemMetrics(SM_CXSCREEN); 
		height = GetSystemMetrics(SM_CYSCREEN);

 	// TODO: Place code here.
		WNDCLASS wc;
		wc.style = 0;
		wc.lpfnWndProc = WndProcsaver;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance,NULL/*MAKEINTRESOURCE(IDI_ICON1)*/);
		wc.hCursor = LoadCursor(NULL,IDC_ARROW);
		wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = _T("STARS V4EXE");
		RegisterClass(&wc);
	
		hwndm = CreateWindowEx(0/*WS_EX_TOPMOST*/,_T("STARS V4EXE"),NULL,WS_POPUP,0,0,width,height,NULL,NULL,hInstance,NULL);
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		ShowWindow(hwndm,SW_SHOW);
		SetFocus(hwndm);
		
		ShowCursor(false);
		
		HPTSetWindow(hInstance,hwndm);
		LoadRegistry();
		
		gengine = GetHPTGraphicsEngine(quality,widescreen);
		gengine->BeginFrame();
		gengine->Position(0.4f*width,0.4f*height);
		(*gengine) << "Loading Stars Version 4!!!";
		gengine->EndFrame();

		if(texturequality == 0) gengine->LoadHGF(hptstrcat(path,"low texture pack.hgf"));
		if(texturequality == 1) gengine->LoadHGF(hptstrcat(path,"med texture pack.hgf"));
		if(texturequality == 2) gengine->LoadHGF(hptstrcat(path,"high texture pack.hgf"));
	}
	else
	{
		if(lpCmdLine[0] != 'f')
		{
			check_input = false;
			HWND hwndt;
			sscanf(lpCmdLine,"%d",&hwndt);
			RECT rc; GetWindowRect(hwndt,&rc);
			int cx=rc.right-rc.left, cy=rc.bottom-rc.top;
			width = cx;
			width = cy;
			WNDCLASS wc;
			wc.style = 0;
			wc.lpfnWndProc = WndProcsaver;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(hInstance,NULL/*MAKEINTRESOURCE(IDI_ICON1)*/);
			wc.hCursor = LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = _T("STARS V4EXE");
			RegisterClass(&wc);

			hwndm=CreateWindowEx(0,_T("STARS V4EXE"),NULL,WS_CHILD|WS_VISIBLE,
								  0,0,cx,cy,hwndt,NULL,hInstance,NULL);
			
			HPTSetWindow(hInstance,hwndm);
 
  
			starfielddensity = 100;

			planetsyn = false;
			framecounteryn = false;
			speed = 50 * 1000.0f;
			full_speed = 70 * 1000.0f;
			savecpu = true;
			quality = 0;

			gengine = GetHPTGraphicsEngine(quality,false);
			gengine->BeginFrame();
			gengine->Position(0.4f*width,0.4f*height);
			(*gengine) << "Loading Stars Version 4!!!";
			gengine->EndFrame();
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			Sleep(0);
			gengine->LoadHGF(hptstrcat(path,"preview texture pack.hgf"));
		}
		else
		{
			check_input = true;
			HWND hwndt;
			sscanf(lpCmdLine+1,"%d",&hwndt);
			width = GetSystemMetrics(SM_CXSCREEN); 
			height = GetSystemMetrics(SM_CYSCREEN);
			//RECT rc; GetWindowRect(hwndt,&rc);
			//int cx=rc.right-rc.left, cy=rc.bottom-rc.top;  
			WNDCLASS wc;
			wc.style = 0;
			wc.lpfnWndProc = WndProcsaver;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(hInstance,NULL/*MAKEINTRESOURCE(IDI_ICON1)*/);
			wc.hCursor = LoadCursor(NULL,IDC_ARROW);
			wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = _T("STARS V4EXE");
			RegisterClass(&wc);

			hwndm=CreateWindowEx(WS_EX_TOPMOST,_T("STARS V4EXE"),NULL,WS_CHILD|WS_VISIBLE|WS_POPUP,
								  0,0,width,height,hwndt,NULL,hInstance,NULL);
			ShowCursor(false);
			
			HPTSetWindow(hInstance,hwndm);
 
  
			//starfielddensity = 100;

			/*planetsyn = false;
			framecounteryn = false;
			speed = 50 * 1000.0f;
			full_speed = 70 * 1000.0f;
			savecpu = true;
			quality = 0;*/
			LoadRegistry();
			
			gengine = GetHPTGraphicsEngine(quality,widescreen);
			gengine->BeginFrame();
			gengine->Position(0.4f*width,0.4f*height);
			(*gengine) << "Loading Stars Version 4!!!";
			gengine->EndFrame();

			if(texturequality == 0) gengine->LoadHGF(hptstrcat(path,"low texture pack.hgf"));
			if(texturequality == 1) gengine->LoadHGF(hptstrcat(path,"med texture pack.hgf"));
			if(texturequality == 2) gengine->LoadHGF(hptstrcat(path,"high texture pack.hgf"));

		}
	}
		//	MessageBox(NULL,"init","init",MB_OK);
	OutputDebugString("init window");

		for(int count1 = 0;count1 < starfielddensity;count1++)
			stars.push_back(new Star);
		planet = new Planet;
		for(int count1 = 0;count1 < 200;count1++)
			for(list<Star*>::iterator tempsi= stars.begin();tempsi != stars.end();)
			{
				Star* tempstar;
				if((*tempsi)->movestarsetup(5000.0f))
				{
					tempstar = (*tempsi);
					tempsi = stars.erase(tempsi);
					stars.push_back(tempstar);
				}
				else
				{
					tempsi++;
				}
			}

		if(starfielddensity == 100)
			for(list<Star*>::iterator tempsi= stars.begin();tempsi != stars.end();tempsi++)
			{
				(*tempsi)->SetSizeMult(8);
			}
		time_to_next_state = ((rand()%5000)+1000)/1000.0f;
	/*	tempobject = gengine->Create3DObject();
		tempobject->Scale(60);
		tempobject->Position(-300,-150,800);
		tempobject->SetImage(0,1);
		tempobject->SetColor(1.1f,1.05f,0.70f,1);
		tempobject->Rotate(1.5f,0.5f);
*/
/*		tempobjectp = gengine->Create3DObjectP();
		tempobjectp->Scale(100);
		tempobjectp->Position(150,-100,500);
		tempobjectp->SkyPosition(0.1f);
		tempobjectp->SetImage(2,3);
		tempobjectp->SetColor(0.01f,0.01f,0.01f,0.0f);
		tempobjectp->SetLightVec(-1,0,-0.1f);
		tempobjectp->Rotate(2.4f,0.0f);
	*/	
	/*	tempobjectp2 = gengine->Create3DObjectP();
		tempobjectp2->Scale(100);
		tempobjectp2->Position(0,250,2000);
		tempobjectp2->SkyPosition(0.1f);
		tempobjectp2->SetImage(4,5);
		tempobjectp2->SetColor(0.01f,0.01f,0.01f,0.0f);
		tempobjectp2->SetLightVec(-0.2f,-1,-0.4f);
		tempobjectp2->Rotate(0,0.0f);
		tempobjectp2->AddRing(6);*/


		Music *musicobject;
		if(music)
		{
			musicobject = new Music(path);
		}
		QueryPerformanceFrequency(&timerfreq);
		QueryPerformanceCounter(&starttime);

		/*while(PeekMessage(&msg,NULL,WM_KEYDOWN,WM_KEYDOWN,PM_REMOVE) != 0)
		{
		};*/
	//MessageBox(NULL,"loop","loop",MB_OK);
		OutputDebugString("begin loop");
//		try
//		{
			while(!done)
			{
				//OutputDebugString("iterate");
				Iterate();
				if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
//		}
//		catch(...)
//		{
//			MessageBox(NULL,"unkown error","error",MB_OK);
//			exit(0);
//		}
	//	MessageBox(NULL,"error","error",MB_OK);
	//	tempobject->Release();
			if(music)
			{
				delete musicobject;
			}
	gengine->Release();
	delete planet;
	for(list<Star*>::iterator tempsi= stars.begin();tempsi != stars.end();)
	{
		Star* tempstar;
		tempstar = (*tempsi);
		tempsi = stars.erase(tempsi);
		delete tempstar;
	}
	ShowCursor(true);
	//ReleaseMutex(mutex);
	//CloseHandle(mutex);
	SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,FALSE,0,0);

	return 0;
}



