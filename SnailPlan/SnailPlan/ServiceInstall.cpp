#include "stdafx.h"
#include "ServiceInstall.h"
#include <windows.h>
#include <winsvc.h>
ServiceInstall::ServiceInstall()
{
}


ServiceInstall::~ServiceInstall()
{
}


//
//   FUNCTION: InstallService
//
//   PURPOSE: Install the current application as a service to the local 
//   service control manager database.
//
//   PARAMETERS:
//   * pszServiceName - the name of the service to be installed
//   * pszDisplayName - the display name of the service
//   * dwStartType - the service start option. This parameter can be one of 
//     the following values: SERVICE_AUTO_START, SERVICE_BOOT_START, 
//     SERVICE_DEMAND_START, SERVICE_DISABLED, SERVICE_SYSTEM_START.
//   * pszDependencies - a pointer to a double null-terminated array of null-
//     separated names of services or load ordering groups that the system 
//     must start before this service.
//   * pszAccount - the name of the account under which the service runs.
//   * pszPassword - the password to the account name.
//
//   NOTE: If the function fails to install the service, it prints the error 
//   in the standard output stream for users to diagnose the problem.
//
int ServiceInstall::InstallService(CString szServiceName, CString szDisplayName, DWORD dwStartType, CString szDependencies, CString szAccount,CString szPath)
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT |SC_MANAGER_CREATE_SERVICE);
	if (schSCManager == NULL)
	{
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
		return -1;
	}

	schService = CreateService(
		schSCManager,                   // SCManager database
		szServiceName,                 // Name of service
		szDisplayName,                 // Name to display
		SERVICE_QUERY_STATUS,           // Desired access
		SERVICE_WIN32_OWN_PROCESS,      // Service type
		dwStartType,                    // Service start type
		SERVICE_ERROR_NORMAL,           // Error control type
		szPath,                         // Service's binary
		NULL,                           // No load ordering group
		NULL,                           // No tag identifier
		szDependencies,                // Dependencies
		NULL,                     // Service running account
		NULL                     // Password of the account
		);
	
	if (schService == NULL)
	{		
		DWORD error = GetLastError();
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
		CloseServiceHandle(schService);
		schService = NULL;
		return -2;
	}
	else
	{
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
		CloseServiceHandle(schService);
		schService = NULL;
		return 0;
	}

}

//
//   FUNCTION: UninstallService
//
//   PURPOSE: Stop and remove the service from the local service control 
//   manager database.
//
//   PARAMETERS: 
//   * pszServiceName - the name of the service to be removed.
//
//   NOTE: If the function fails to uninstall the service, it prints the 
//   error in the standard output stream for users to diagnose the problem.
//
int ServiceInstall::UninstallService(CString szServiceName)
{
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {};

	// Open the local default service control manager database
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (schSCManager == NULL)
	{
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
		return -1;
	}

	// Open the service with delete, stop, and query status permissions
	schService = OpenService(schSCManager, szServiceName, SERVICE_STOP |
		SERVICE_QUERY_STATUS | DELETE);
	if (schService == NULL)
	{
		DWORD error = GetLastError();
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
		CloseServiceHandle(schService);
		schService = NULL;
		return -2;
	}

	// Try to stop the service
	if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
	{
		Sleep(1000);

		while (QueryServiceStatus(schService, &ssSvcStatus))
		{
			if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
			{
				Sleep(1000);
			}
			else break;
		}
	}

	// Now remove the service by calling DeleteService.
	if (!DeleteService(schService))
	{
		return 0;
	}
	return 1;
}