#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <winsvc.h>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {                                                    //Main function has number of arguments and the actual arguments
	if (argc == 1) {                          //This line would trigger the next block to be presented to the user as help
		cout << "\nUSAGE: " << argv[0] << " [switch]" << " [options]" << endl;
		cout << "\nSWITCHES:" << endl;
		cout << "\n   -q: 'sc query', USAGE       : midterm.exe -q [ServiceName]" << endl;
		cout << "\n   -c: 'sc create', USAGE      : midterm.exe -c [ServiceName] [BinPath] [Start 02(auto), 03(demand) 04(disabled)]" << endl;
		cout << "\n   -n: 'sc qdescription', USAGE: midterm.exe -n [ServiceName]" << endl;
		cout << "\n   -s: 'sc start', USAGE       : midterm.exe -s [ServiceName]" << endl;
		cout << "\n   -p: 'sc stop', USAGE        : midterm.exe -p [ServiceName]" << endl;
		cout << "\n   -d: 'sc delete', USAGE      : midterm.exe -d [ServiceName]" << endl;
		cout << "\n   -g: 'sc config', USAGE : " << endl;
		cout << "\n         1- Change Display Name: midterm.exe -g [ServiceName] -y [NewName]" << endl;
		cout << "\n         2- Change Start Type  : midterm.exe -g [ServiceName] -r [Type 02(auto),03(demand),04(disabled)]" << endl;
		cout << "\n         3- Change Binary Path : midterm.exe -g [ServiceName] -b [BinPath]" << endl;
		cout << "\n         4- Change Service Type: midterm.exe -g [ServiceName] -t [Type 16(WIN32_OWN), 32(WIN32)]" << endl;
		cout << "\n         5- Change Dependencies: midterm.exe -g [ServiceName] -e [Dependency, service name]" << endl;
		cout << "\n   -f: 'sc failure', USAGE: " << endl;
		cout << "\n         1- Change Command Failure: midterm.exe -f [ServiceName] -m [Command]" << endl;
		cout << "\n         2- Change Failure Actions: midterm.exe -f [ServiceName] -h [Action] [MS] [Action] [MS] [Action] [MS] [Reset]" << endl;
		cout << "\n                                    MS = Milliseconds, Reset = reset time in seconds" << endl;
		cout << "\n                                    Example Command: midterm.exe -f midterm_test -h 1 50 1 50 0 50 200" << endl;
		cout << "\n                                    1 = SC_ACTION_RESTART , 0 = SC_ACTION_NONE" << endl;
		return 0;
	}
	char swtch = argv[1][1];              //This is the start of the switches that determine what block of code to execute based on the user's input
	switch (swtch)
	{
	case 'q': {                                              //This switch option executes block of code that replicates SC QUERY
		if (argv[2] == NULL) {                                            //Checking a service name was inputted as an argument
			cout << "\nYou did not input service name!" << endl;
			return 1;
		}
		SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //This line creates handle to the service manager
		if (h_sc_manager == NULL) {                       //Checking the service mananger handle was created successfully
			cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
			return 1;
		}
		LPCSTR service_name = argv[2];                                                  
		SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SC_MANAGER_ALL_ACCESS);   //This line creates a handle to the service to be queried
		if (h_service == NULL) {                                         //Checking the service handle was created successfully
			cout << "\nService handle creation failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		SERVICE_STATUS_PROCESS status_pointer{};   //This variable holds status information
		DWORD num_bytes;
		BOOL service_status = QueryServiceStatusEx(h_service, SC_STATUS_PROCESS_INFO, (LPBYTE)&status_pointer, sizeof(status_pointer), &num_bytes); //Querying the service status 
		if (service_status == NULL) {                                             //Checking the service query was successful
			cout << "\nService status failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_service);                                //Closing both service manager and service handles 
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		cout << "\nService Name: " << service_name << endl;
		if (status_pointer.dwServiceType == 48) {                          //Determining the type of the service based on the query results
			cout << "Service Type2: Service_Win32" << endl;
		}
		else if (status_pointer.dwServiceType == 32) {
			cout << "Service Type: SERVICE_WIN32_SHARE_PROCESS" << endl;
		}
		else if (status_pointer.dwServiceType == 16) {
			cout << "Service Type: SERVICE_WIN32_OWN_PROCESS" << endl;
		}
		else if (status_pointer.dwServiceType == 8) {
			cout << "Service Type: SERVICE_RECOGNIZER_DRIVER" << endl;
		}
		else if (status_pointer.dwServiceType == 1) {
			cout << "Service Type: SERVICE_KERNEL_DRIVER" << endl;
		}
		else if (status_pointer.dwServiceType == 2) {
			cout << "Service Type: SERVICE_FILE_SYSTEM_DRIVER" << endl;
		}
		else if (status_pointer.dwServiceType == 256) {
			cout << "Service Type2: SERVICE_INTERACTIVE_PROCESS" << endl;
		}
		else if (status_pointer.dwServiceType == 4) {
			cout << "Service Type: SERVICE_ADAPTER" << endl;
		}
		else if (status_pointer.dwServiceType == 272) {
			cout << "Service Type: WIN32_OWN_PROCESS  (interactive)" << endl;
		}
		else {
			cout << "Unknown serice type, " << "Service Type Code: " << std::hex << status_pointer.dwServiceType << endl;
		}
		int status_code = status_pointer.dwCurrentState;
		if (status_code == 1) {                                                  //Determining the service status based on the status code
			cout << "Service State: " << status_code << " Stopped" << endl;
		}
		else if (status_code == 2) {
			cout << "Service State: " << status_code << " Start Pending" << endl;
		}
		else if (status_code == 3) {
			cout << "Service State: " << status_code << " Stop Pending" << endl;
		}
		else if (status_code == 4) {
			cout << "Service State: " << status_code << " Runing" << endl;
		}
		else if (status_code == 5) {
			cout << "Service State: " << status_code << " Continue Pending" << endl;
		}
		else if (status_code == 6) {
			cout << "Service State: " << status_code << " Pause Pending" << endl;
		}
		else if (status_code == 7) {
			cout << "Service State: " << status_code << " Paused" << endl;
		}
		else {
			cout << "Unknown Service State." << endl;
		}
		cout << "Win32 Exit Code: " << status_pointer.dwWin32ExitCode << endl;
		cout << "Service Exit Code: " << status_pointer.dwServiceSpecificExitCode << endl;
		cout << "Checkpoint: " << status_pointer.dwCheckPoint << endl;
		cout << "Wait Hint: " << status_pointer.dwWaitHint << endl;
		CloseServiceHandle(h_service);                                //Closing handles 
		CloseServiceHandle(h_sc_manager);
		break;
	} 
	case 'c': {                                                  //This switch executes block of code that replicates the SC CREATE command
		if (argv[2] == NULL) {                                            //Checking all arguments
			cout << "\nYou did not input service name!" << endl;
			return 1;
		}
		if (argv[3] == NULL) {
			cout << "\nYou did not input binary path!" << endl;
			return 1;
		}
		if (argv[4] == NULL) {
			cout << "\nYou did not input start type!" << endl;
			return 1;
		}
		LPCSTR service_name = argv[2];
		LPCSTR binary_path = argv[3];
		SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);                   //Creating SM handle
		if (h_sc_manager == NULL) {
			cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
			return 1;
		}
		DWORD new_start = atoi(argv[4]);
		SC_HANDLE create_service = CreateServiceA(h_sc_manager, service_name, service_name, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, new_start,
			SERVICE_ERROR_NORMAL, binary_path, NULL, NULL, NULL, NULL, NULL);
		if (create_service == NULL) {                                                      //Checking service has been created
			cout << "\nService creation failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		else {
			cout << "\nService " << service_name << " was created successfully." << endl;
			return 0;
		}
		CloseServiceHandle(create_service);                                                    //Closing handles
		CloseServiceHandle(h_sc_manager);
		break;
	}	
	case 'n': {                                                    //This switch executes block of code that replicates SC QDESCRIPTION
		if (argv[2] == NULL) {                                            //Checking a service name was inputted as an argument
			cout << "\nYou did not input service name!" << endl;
			return 1;
		}
		SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);              //Creating SM handle
		if (h_sc_manager == NULL) {
			cout << "SCManger handle creation failed, " << GetLastError() << endl;
			return 1;
		}
		LPCSTR service_name = argv[2];
		SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_QUERY_CONFIG);                //Creating service handle
		if (h_service == NULL) {
			cout << "Service handle creation failed, " << GetLastError() << endl;
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		DWORD mem_bytes = 8000;           //Maximum memory size 
		LPQUERY_SERVICE_CONFIG buffer;                                //Creating buffer to hold the description 
		buffer = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, mem_bytes);   
		BOOL service_configuration = QueryServiceConfig2(h_service, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)buffer, mem_bytes, &mem_bytes);  //Obtaining service configuration information 
		if (service_configuration == NULL) {                            //Checking service configuration information obtained successfully
			cout << "Service configuration retrival wasn not successful, " << GetLastError() << endl;
			LocalFree(buffer);
			CloseServiceHandle(h_service);
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		LPSERVICE_DESCRIPTION description;  //Variable to store service description
		description = (LPSERVICE_DESCRIPTION)buffer;
		cout << "\nService Name: " << service_name << endl;
		wcout << "\nService description: " << description->lpDescription << endl;
		LocalFree(buffer);
		CloseServiceHandle(h_service);
		CloseServiceHandle(h_sc_manager);
		break;
	}		 
	case 's': {                                                                       //This switch executes block of code that replicates SC START
		if (argv[2] == NULL) {                                            //Checking a service name was inputted as an argument
			cout << "\nYou did not input service name!" << endl;
			return 1;
		}
		SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);    //Create SM handle
		if (h_sc_manager == NULL) {
			cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
			return 1;
		}
		LPCSTR service_name = argv[2];
		SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_START | SERVICE_QUERY_STATUS);   //Creating service handle
		if (h_service == NULL) {
			cout << "\nService handle creation failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		BOOL start_service = StartServiceA(h_service, NULL, NULL);                          //Starting the service
		if (start_service == NULL) {                                            
			cerr << "\nFailed to start the " << argv[2] << " service, error code " << GetLastError << endl;
			return 1;
		}
		else {
			cout << "\nStarted the " << argv[2] << " service successfully." << endl;
		}
		SERVICE_STATUS_PROCESS status{};                                     //Creating a pointer to the service status 
		DWORD mem_bytes;
		BOOL service_status = QueryServiceStatusEx(h_service, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(status), &mem_bytes);  //Retrieving service status
		if (service_status == NULL) {                                                 //Determining whether status retrieval failed
			cout << "\nService status failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_service);
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		cout << "\nService Name: " << service_name << endl;
		if (status.dwServiceType == 48) {                   //Determining service type based on type code
			cout << "Service Type: Service_Win32" << endl;
		}
		else if (status.dwServiceType == 32) {
			cout << "Service Type: SERVICE_WIN32_SHARE_PROCESS" << endl;
		}
		else if (status.dwServiceType == 16) {
			cout << "Service Type: SERVICE_WIN32_OWN_PROCESS" << endl;
		}
		else if (status.dwServiceType == 8) {
			cout << "Service Type: SERVICE_RECOGNIZER_DRIVER" << endl;
		}
		else if (status.dwServiceType == 1) {
			cout << "Service Type: SERVICE_KERNEL_DRIVER" << endl;
		}
		else if (status.dwServiceType == 2) {
			cout << "Service Type: SERVICE_FILE_SYSTEM_DRIVER" << endl;
		}
		else if (status.dwServiceType == 256) {
			cout << "Service Type: SERVICE_INTERACTIVE_PROCESS" << endl;
		}
		else if (status.dwServiceType == 4) {
			cout << "Service Type: SERVICE_ADAPTER" << endl;
		}
		else if (status.dwServiceType == 272) {
			cout << "Service Type: WIN32_OWN_PROCESS  (interactive)" << endl;
		}
		else {
			cout << "Unknown serice type, Service Type Code: " << std::hex << status.dwServiceType << endl;
		}
		int status_code = status.dwCurrentState;                      
		if (status_code == 1) {                                               //Determining service status based on status code 
			cout << "Service State: " << status_code << " Stopped" << endl;
		}
		if (status_code == 2) {
			cout << "Service State: " << status_code << " Start Pending" << endl;
		}
		if (status_code == 3) {
			cout << "Service State: " << status_code << " Stop Pending" << endl;
		}
		if (status_code == 4) {
			cout << "Service State: " << status_code << " Runing" << endl;
		}
		if (status_code == 5) {
			cout << "Service State: " << status_code << " Continue Pending" << endl;
		}
		if (status_code == 6) {
			cout << "Service State: " << status_code << " Pause Pending" << endl;
		}
		if (status_code == 7) {
			cout << "Service State: " << status_code << " Paused" << endl;
		}
		cout << "Win32 Exit Code: " << status.dwWin32ExitCode << endl;
		cout << "Service Exit Code: " << status.dwServiceSpecificExitCode << endl;
		cout << "Checkpoint: " << status.dwCheckPoint << endl;
		cout << "Wait Hint: " << status.dwWaitHint << endl;
		cout << "PID: " << status.dwProcessId << endl;
		cout << "Flags: " << status.dwServiceFlags << endl;
		CloseServiceHandle(h_service);                              //Closing both handles
		CloseServiceHandle(h_sc_manager);
		break;
	}
	case 'p': {                                                                       //This switch executes block of code that replicates SC STOP
		if (argv[2] == NULL) {                                            //checking a service name was inputted as an argument
			cout << "\nyou did not input service name!" << endl;
			return 1;
		}
		SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);                //Creating SM handle
		if (h_sc_manager == NULL) {
			cout << "SCManger handle creation failed, error code " << GetLastError() << endl;
			return 1;
		}
		LPCSTR service_name = argv[2];
		SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_STOP | SERVICE_QUERY_STATUS);  //Creating service handle
		if (h_service == NULL) {
			cout << "Service handle creation failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		SERVICE_STATUS service_status;
		BOOL servic_control = ControlService(h_service, SERVICE_CONTROL_STOP, &service_status);    //Stoping the service 
		if (servic_control == NULL) {                    //Checking the previous line was executed successfully 
			cout << "\nFailed to stop service, error code " << GetLastError() << endl;
		}
		else {
			cout << "\nService stopped successfully." << endl;
		}
		SERVICE_STATUS_PROCESS status{};                   //Creating a pointer to the service status
		DWORD num_bytes;
		BOOL service_status2 = QueryServiceStatusEx(h_service, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(status), &num_bytes);  //Query the service status
		if (service_status2 == NULL) {
			cout << "Service status failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_service);
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		cout << "\nService Name: " << service_name << endl;
		if (status.dwServiceType == 48) {                          //Determining service type using the type code
			cout << "Service Type: Service_Win32" << endl;
		}
		else if (status.dwServiceType == 32) {
			cout << "Service Type: SERVICE_WIN32_SHARE_PROCESS" << endl;
		}
		else if (status.dwServiceType == 16) {
			cout << "Service Type: SERVICE_WIN32_OWN_PROCESS" << endl;
		}
		else if (status.dwServiceType == 8) {
			cout << "Service Type: SERVICE_RECOGNIZER_DRIVER" << endl;
		}
		else if (status.dwServiceType == 1) {
			cout << "Service Type: SERVICE_KERNEL_DRIVER" << endl;
		}
		else if (status.dwServiceType == 2) {
			cout << "Service Type: SERVICE_FILE_SYSTEM_DRIVER" << endl;
		}
		else if (status.dwServiceType == 256) {
			cout << "Service Type: SERVICE_INTERACTIVE_PROCESS" << endl;
		}
		else if (status.dwServiceType == 4) {
			cout << "Service Type: SERVICE_ADAPTER" << endl;
		}
		else if (status.dwServiceType == 272) {
			cout << "Service Type: WIN32_OWN_PROCESS  (interactive)" << endl;
		}
		else {
			cout << "Unknown serice type, Service Type Code: " << std::hex << status.dwServiceType << endl;
		}
		int status_code = status.dwCurrentState;
		if (status_code == 1) {                                              //Determining service state using the status code
			cout << "Service State: " << status_code << " Stopped" << endl;
		}
		if (status_code == 2) {
			cout << "Service State: " << status_code << " Start Pending" << endl;
		}
		if (status_code == 3) {
			cout << "Service State: " << status_code << " Stop Pending" << endl;
		}
		if (status_code == 4) {
			cout << "Service State: " << status_code << " Runing" << endl;
		}
		if (status_code == 5) {
			cout << "Service State: " << status_code << " Continue Pending" << endl;
		}
		if (status_code == 6) {
			cout << "Service State: " << status_code << " Pause Pending" << endl;
		}
		if (status_code == 7) {
			cout << "Service State: " << status_code << " Paused" << endl;
		}
		cout << "Win32 Exit Code: " << status.dwWin32ExitCode << endl;
		cout << "Service Exit Code: " << status.dwServiceSpecificExitCode << endl;
		cout << "Checkpoint: " << status.dwCheckPoint << endl;
		cout << "Wait Hint: " << status.dwWaitHint << endl;
		cout << "PID: " << status.dwProcessId << endl;
		cout << "Flags: " << status.dwServiceFlags << endl;
		CloseServiceHandle(h_service);                                   //Closing both handles
		CloseServiceHandle(h_sc_manager);
		break;
	}			
	case 'd': {                                                                       //This switch executes block of code that replicates SC DELETE
		if (argv[2] == NULL) {                                            //Checking a service name was inputted as an argument
			cout << "\nYou did not input service name!" << endl;
			return 1;
		}
		SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);         //Creating SM handle
		if (h_sc_manager == NULL) {
			cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
			return 1;
		}
		LPCSTR service_name = argv[2];
		SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SC_MANAGER_ALL_ACCESS);  //Creating service handle
		if (h_service == NULL) {
			cout << "\nService handle creation failed, error code " << GetLastError() << endl;
			CloseServiceHandle(h_sc_manager);
			return 1;
		}
		BOOL delete_service = DeleteService(h_service);        //Deleting service
		if (delete_service == NULL) {                                                         //Determining service deleting was successful
			cout << "\nService deletion failed, error code " << GetLastError << endl;
		}
		else {
			cout << "\nService deletion completed succssfully.\n";
		}
		CloseServiceHandle(h_service);                           //Closing both handles
		CloseServiceHandle(h_sc_manager);
		break;
	}	
	case 'g': {                                                                       //This switch executes block of code that replicates SC CONFIG
		char swtch_config = argv[3][1];
		switch (swtch_config) {
		case 'y': {                                                             //Switch -y executes code to change service displayname
			if (argv[4] == NULL) {                                            //Checking a new service name was inputted as an argument
				cout << "\nYou did not input a new service name!" << endl;
				return 1;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); //Creating SM handle
			if (h_sc_manager == NULL) {
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			LPCSTR service_name = argv[2];
			SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_CHANGE_CONFIG);  //Creating service handle
			if (h_service == NULL) {
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			LPCSTR new_name = argv[4];
			BOOL change_name = ChangeServiceConfigA(h_service, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, NULL, NULL, NULL, //Changing service displayname
				NULL, NULL, NULL, new_name);
			if (change_name == NULL) {                                                                     //Checking service displayname was changed 
				cout << "\nDisplay name change failed, error code " << GetLastError() << endl;
				return 1;
			}
			else {
				cout << "\nDisplayname was changed successfully." << endl;
			}
			CloseServiceHandle(h_service);  //Closing both handles
			CloseServiceHandle(h_sc_manager);
			break;
		}
		case 'r': {                                                    //This switch executes code to change service start type
			if (argv[4] == NULL) {                                            //Checking a service start type was inputted as an argument
				cout << "\nYou did not input a new service start type!" << endl;
				return 1;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);  //Creating SM handle
			if (h_sc_manager == NULL) {
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			LPCSTR service_name = argv[2];                                                            //Creating service handle
			SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_CHANGE_CONFIG);
			if (h_service == NULL) {
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			DWORD new_start = atoi(argv[4]);            //Converting char* to DWORD
			BOOL change_start = ChangeServiceConfigA(h_service, SERVICE_NO_CHANGE, new_start, SERVICE_NO_CHANGE, NULL, NULL, NULL,  //Changing start type
				NULL, NULL, NULL, NULL);
			if (change_start == NULL) {                                                              //Checking start type was changed
				cout << "\nStart type change failed, error code " << GetLastError() << endl;
				return 1;
			}
			else {
				cout << "\nStart type changed successfully.\n";
			}
			CloseServiceHandle(h_service);                 //Closing both handles
			CloseServiceHandle(h_sc_manager);
			break;
		}
		case 'b': {                                                                             //Change binpath for a service 
			if (argv[4] == NULL) {                                            //Checking a new binary path was inputted as an argument
				cout << "\nYou did not input a new binary path!" << endl;
				return 1;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);              //Creating SM handle
			if (h_sc_manager == NULL) {
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			LPCSTR service_name = argv[2];
			SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_CHANGE_CONFIG);      //Creating service handle
			if (h_service == NULL) {
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			LPCSTR new_path = argv[4];          //Converting char* to LPCSTR
			BOOL change_path = ChangeServiceConfigA(h_service, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, new_path, NULL, NULL,  //Changing binpath
				NULL, NULL, NULL, NULL);
			if (change_path == NULL) {                       //Checking binpath has been changed
				cout << "\nBinary path change failed, error code " << GetLastError() << endl;
				return 1;
			}
			else {
				cout << "\nBinary path was changed successfully." << endl;
			}
			CloseServiceHandle(h_service);                   //Closing both handles
			CloseServiceHandle(h_sc_manager);
			break;
		}
		case 't': {                                                                                        //Change service type
			if (argv[4] == NULL) {                                            //Checking a new service type was inputted as an argument
				cout << "\nYou did not input a new service type!" << endl;
				return 1;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);           //Creating SM handle
			if (h_sc_manager == NULL) {
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			LPCSTR service_name = argv[2];
			SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_CHANGE_CONFIG);        //Creating service handle
			if (h_service == NULL) {
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			DWORD new_type = atoi(argv[4]);               //Converting char* to DWORD
			BOOL change_type = ChangeServiceConfigA(h_service, new_type, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, NULL, NULL, NULL,  //Changing service type
				NULL, NULL, NULL, NULL);
			if (change_type == NULL) {                                                                 //Checking service type was changed
				cout << "\nService type change failed, error code " << GetLastError() << endl;
				return 1;
			}
			else {
				cout << "\nService type change completed successfully.\n";
			}
			CloseServiceHandle(h_service);               //Closing both handles
			CloseServiceHandle(h_sc_manager);
			break;
		}
		case 'e': {                                                                                 //Change service dependencies
			if (argv[4] == NULL) {                                            //Checking a service name was inputted as a dependency argument
				cout << "\nYou did not input a service name as dependency!" << endl;
				return 1;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);  //Creating SM handle
			if (h_sc_manager == NULL) {
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			LPCSTR service_name = argv[2];
			SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_CHANGE_CONFIG);           //Creating service handle
			if (h_service == NULL) {
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			LPCSTR new_dependency = argv[4];  //Converting char* to LPCSTR
			BOOL change_dependency = ChangeServiceConfigA(h_service, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, NULL, NULL, NULL,  //Changing dependencies
				new_dependency, NULL, NULL, NULL);
			if (change_dependency == NULL) {                 //Checking dependencies were changed 
				cout << "\nDependencies change failed, error code " << GetLastError() << endl;
				return 1;
			}
			else {
				cout << "\nDependencies were changed successfully." << endl;
			}
			CloseServiceHandle(h_service);  //Closing both handles
			CloseServiceHandle(h_sc_manager);
			break;
		}
		}
		break;
	}
	case 'f': {                                                                       //This switch executes block of code that replicates SC FAILURE
		char swtch_config = argv[3][1];
		switch (swtch_config) {
		case 'h': {                                                                               //Change service failure actions settings
			if ((argv[4] == NULL) or (argv[5] == NULL) or (argv[6] == NULL) or (argv[7] == NULL) or 
				(argv[8] == NULL) or (argv[9] == NULL) or (argv[10] == NULL)) {                   //Checking command arguments
				cout << "\nYou did not input all required arguments!" << endl;
				return 1;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //Creating SM handle
			if (h_sc_manager == NULL) {
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			LPCSTR service_name = argv[2];
			SC_HANDLE h_service = OpenServiceA(h_sc_manager, service_name, SERVICE_ALL_ACCESS);        //Creating servcie handle
			if (h_service == NULL) {
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			SERVICE_FAILURE_ACTIONS fail_actions;         //Creating a variable to hold failure actions  
			SC_ACTION service_fail[3];                //Array created to hold three elements of the service failure actions
			int num1 = strtoul(argv[4], NULL, 10);     //Determining the 1st failure action based on the user's input
			if (num1 == 0) {
				service_fail[0].Type = SC_ACTION_NONE;
			}
			else if (num1 == 1) {
				service_fail[0].Type = SC_ACTION_RESTART;
			}
			else if (num1 == 2) {
				service_fail[0].Type = SC_ACTION_REBOOT;
			}
			else if (num1 == 3) {
				service_fail[0].Type = SC_ACTION_RUN_COMMAND;
			}
			else {
				cout << "\nYou did not input correct service failure action, please user numbers 0, 1, 2, 4";
			}
			service_fail[0].Delay = strtoul(argv[5], NULL, 10);   //Determining the delay for 1st action based on the user's input 
			int num2 = strtoul(argv[6], NULL, 10);   
			if (num2 == 0) {                                //Determining the 2nd failure action based on the user's input
				service_fail[1].Type = SC_ACTION_NONE;
			}
			else if (num2 == 1) {
				service_fail[1].Type = SC_ACTION_RESTART;
			}
			else if (num2 == 2) {
				service_fail[1].Type = SC_ACTION_REBOOT;
			}
			else if (num2 == 3) {
				service_fail[1].Type = SC_ACTION_RUN_COMMAND;
			}
			else {
				cout << "\nYou did not input correct service failure action, please user numbers 0, 1, 2, 4";
			}
			service_fail[1].Delay = strtoul(argv[7], NULL, 10);    //Determining the delay for 2nd action based on the user's input
			int num3 = strtoul(argv[8], NULL, 10);     //Determining the 3rd failure action based on the user's input
			if (num3 == 0) {
				service_fail[2].Type = SC_ACTION_NONE;
			}
			else if (num3 == 1) {
				service_fail[2].Type = SC_ACTION_RESTART;
			}
			else if (num3 == 2) {
				service_fail[2].Type = SC_ACTION_REBOOT;
			}
			else if (num3 == 3) {
				service_fail[2].Type = SC_ACTION_RUN_COMMAND;
			}
			else {
				cout << "\nYou did not input correct service failure action, please user numbers 0, 1, 2, 4";
			}
			service_fail[2].Delay = strtoul(argv[9], NULL, 10);   //Determining the delay for 3rd action based on the user's input
			fail_actions.dwResetPeriod = strtoul(argv[10], NULL, 10);  //Setting the reset period
			fail_actions.lpCommand = NULL;
			fail_actions.lpRebootMsg = NULL;
			fail_actions.cActions = 3;
			fail_actions.lpsaActions = service_fail;  //Updating the Failure_Actions variable with out array of failure actions
			BOOL change_failure = ChangeServiceConfig2(h_service, SERVICE_CONFIG_FAILURE_ACTIONS, &fail_actions);  //Changing failure actions
			if (change_failure == NULL) {     //Checking failure actions were changed 
				cout << "\nFailure settings change wasn't successful, error code " << GetLastError() << endl;
			}
			else {
				cout << "\nFailure settings were changed successfully." << endl;
			}
			CloseServiceHandle(h_service);                 //Closing handles 
			CloseServiceHandle(h_sc_manager);
			break;
		}
		case 'm': {                                                                               //Change service failure command
			if (argv[4] == NULL) {
				cout << "\nYou did not input a command as an argument!" << endl;
			}
			SC_HANDLE h_sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //Creating SM handle
			if (h_sc_manager == NULL)
			{
				cout << "\nSCManger handle creation failed, error code " << GetLastError() << endl;
				return 1;
			}
			int size = MultiByteToWideChar(CP_ACP, 0, argv[2], -1, NULL, 0);
			LPWSTR service_name = new wchar_t[size];
			MultiByteToWideChar(CP_ACP, 0, argv[2], -1, service_name, size);
			SC_HANDLE h_service = OpenServiceW(h_sc_manager, service_name, SERVICE_ALL_ACCESS);         //Creating service handle
			if (h_service == NULL)
			{
				cout << "\nService handle creation failed, error code " << GetLastError() << endl;
				CloseServiceHandle(h_sc_manager);
				return 1;
			}
			SERVICE_FAILURE_ACTIONS fail_actions = {0};    //Initializing a variable for the failure actions
			SC_ACTION service_fail[1];
			service_fail->Type = SC_ACTION_RUN_COMMAND;
			service_fail->Delay = 1000;
			int size1 = MultiByteToWideChar(CP_ACP, 0, argv[4], -1, NULL, 0); //Starting the casting process of the command inputted by the user
			LPWSTR Lservice_cmd = new wchar_t[size1];
			MultiByteToWideChar(CP_ACP, 0, argv[4], -1, Lservice_cmd, size1);
			fail_actions.lpCommand = const_cast<LPTSTR>(_T(service_cmd)); //Casting command to LPTSTR 
			fail_actions.cActions = 1;
			fail_actions.lpsaActions = service_fail;
			BOOL change_command = ChangeServiceConfig2(h_service, SERVICE_CONFIG_FAILURE_ACTIONS, &fail_actions);  //Changing service failure actions
			if (change_command == NULL) {         //Checking failure actions were changed 
				cout << "\nService failure command change wasn't successful, error code " << GetLastError() << endl;
			}
			else {
				cout << "\nService failure command was changed successfully." << endl;
			}
			CloseServiceHandle(h_service);     //Closing both handles 
			CloseServiceHandle(h_sc_manager);
			return 0;
		}
		}
		break;
	}
	default: {
		cout << "You did not input a correct switch, please run midterm.exe with no switches for more information." << endl;
		break;
	}
	}
	}
