#pragma once



class ServiceInstall
{
public:
	ServiceInstall();
	~ServiceInstall();

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

	int InstallService(CString szServiceName,
		CString szDisplayName,
		DWORD dwStartType,
		CString szDependencies,
		CString szAccount,
		CString szPath);

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
	int UninstallService(CString szServiceName);
};

