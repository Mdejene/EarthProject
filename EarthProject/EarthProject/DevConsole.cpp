#include <conio.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <string>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <tchar.h>

#include "DevConsole.hpp"

using namespace std;

DevConsole* DevConsole::singletonInstance = NULL; //Ensure we have a reference to our singleton.

void DevConsole::create()
{
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  int consoleHandleR, consoleHandleW;
  long stdioHandle;
  FILE *fptr;

  if( singletonInstance == NULL  )
    singletonInstance = new DevConsole;
  else
  {
    cerr << __FILE__ << " " << __LINE__ << " " << "There can only be one DevConsole." << endl;
    return; // Console is already initialized.
  }

  //See more about consoles at http://msdn.microsoft.com/en-us/library/ms681944(VS.85).aspx

  AllocConsole();
  std::wstring strW = L"Init Direct3D Console";
  SetConsoleTitle( strW.c_str() );
  
  GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo );

  // stdin - standard in is file descriptor 0
  stdioHandle = (long)GetStdHandle( STD_INPUT_HANDLE );
  consoleHandleR = _open_osfhandle( stdioHandle, _O_TEXT );
  fptr = _fdopen( consoleHandleR, "r" );
  *stdin = *fptr;
  setvbuf( stdin, NULL, _IONBF, 0 ); 

  // stdout - standard out is file descriptor 1
  stdioHandle = (long) GetStdHandle( STD_OUTPUT_HANDLE );
  consoleHandleW = _open_osfhandle( stdioHandle, _O_TEXT );
  fptr = _fdopen( consoleHandleW, "w" );
  *stdout = *fptr;
  setvbuf( stdout, NULL, _IONBF, 0 );

  // stderr - standard error is file descriptor 2
  stdioHandle = (long)GetStdHandle( STD_ERROR_HANDLE );
  *stderr = *fptr;
  setvbuf( stderr, NULL, _IONBF, 0 );

  //ios::sync_with_stdio(); // Make sure that the I/O layer sees our file descriptor changes to ensue B/W compatibility.
  cout << "DevConsole Initialized..." << endl;
}