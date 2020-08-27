#include "kasperskyhook.hpp"
#include <iostream>

SC_HANDLE handle_kasperskyhook_svc = nullptr;

// Loads MasterHide.sys
//
bool kasperskyhook::load()
{
	// Get current directory
	//
	char buf[ MAX_PATH ]{ };
	GetCurrentDirectoryA( sizeof( buf ), buf );

	// Build MasterHide.sys path
	//
	const auto path = std::string( buf ) + "\\MasterHide.sys";

	// Check if file is there lol
    //
	if ( GetFileAttributesA( path.c_str() ) == INVALID_FILE_ATTRIBUTES )
	{
		std::cout << "MasterHide.sys not found on the current directory." << std::endl;
		return false;
	}

	// Create MasterHide service
	//
	handle_kasperskyhook_svc = loader::create_service( "MasterHide", "MasterHide", path );

	// Load MasterHide.sys
	//
	return handle_kasperskyhook_svc ? loader::start_service( handle_kasperskyhook_svc ) : false;
}

// Unloads MasterHide.sys
//
bool kasperskyhook::unload()
{
	SERVICE_STATUS svc_status { };

	// Unload MasterHide.sys
	//
	bool success = loader::stop_service( handle_kasperskyhook_svc, &svc_status );

	// Service not started
	//
	if ( !success && GetLastError() == ERROR_SERVICE_NOT_ACTIVE )
		success = true;

	// Delete MasterHide service
	//
	return success ? loader::delete_service( handle_kasperskyhook_svc ) : false;
}