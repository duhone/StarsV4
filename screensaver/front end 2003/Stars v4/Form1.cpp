#include "stdafx.h"
#include "Form1.h"
#include"preview.h"
#include <windows.h>

using namespace Starsv4;




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	int temp = 1;
    char hwndc[128];
    int* hwnd;
    HANDLE mutex;

    while((*lpCmdLine) != NULL)
	{
		switch (tolower(*lpCmdLine))
		{
		case 's': //run
			temp = 1;
			break;
		case 'p': //preview
			temp = 2;
                        strcpy(hwndc, lpCmdLine+1);
                        hwnd = (int*)atoi(hwndc);
			break;
                case 'c': //config
                        temp =0;
                        break;
		}
		lpCmdLine++;
	}
	if(temp == 0)
	{

		System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
		Application::Run(new Form1());

	}
	else if(temp == 1)
	{
		mutex = CreateMutex(NULL , FALSE, "stars4");
        if(WaitForSingleObject(mutex,0) != WAIT_OBJECT_0)
            return 0;

		unsigned char *apppath = new unsigned char[512];
        HKEY skey;
        unsigned long size;
        STARTUPINFO startup_info;

        size = sizeof(int);
		if((RegOpenKeyEx(HKEY_CURRENT_USER,"Software",0,KEY_ALL_ACCESS,&skey)) != ERROR_SUCCESS)
                return 0;
				 
      	RegOpenKeyEx(skey,"Eric Duhon",0,KEY_ALL_ACCESS,&skey);
        RegOpenKeyEx(skey,"Stars v4.5",0,KEY_ALL_ACCESS,&skey);
        size=512;
        RegQueryValueEx(skey,"install_folder",NULL,NULL,apppath,&size);


		strcat((char*)apppath,"\\Stars v4.exe");
        startup_info.cb = sizeof(STARTUPINFO);
        startup_info.lpReserved = NULL;
        startup_info.lpDesktop = NULL;
        startup_info.lpTitle = NULL;
        startup_info.dwFlags = 0;
        startup_info.cbReserved2 =0;
        startup_info.lpReserved2 = NULL;
		PROCESS_INFORMATION pro_info;
        
        CreateProcess(NULL, (char*)apppath, NULL, NULL, false, IDLE_PRIORITY_CLASS, NULL, NULL, &startup_info, &pro_info);
        ShowCursor(false);

		while(WaitForSingleObject(pro_info.hProcess, 10) != WAIT_OBJECT_0)
		{
			Sleep(500);
		}



	}
	else if(temp ==2)
	{
		System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
		Preview *preview = new Preview();
		IntPtr child = preview->get_Handle();
		SetParent((HWND)child.ToPointer(),(HWND)hwnd);
		Application::Run(preview);

	}
	else return 0;
	if(temp==1)
	{
		ReleaseMutex(mutex);
        CloseHandle(mutex);
        
	}
	return 0;
}
