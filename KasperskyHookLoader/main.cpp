#include <iostream>
#include "klhk.hpp"
#include "kasperskyhook.hpp"
#include <TlHelp32.h>

int main()
{
	SetConsoleTitleA( "KasperskyHookLdr" );

	// Open handle to SCM
	//
	if ( !loader::open_scm() )
	{
		std::cout << "[-] Failed to open handle to SCM!\n";
		return EXIT_FAILURE;
	}

	// Load klhk.sys
	//
	if ( !klhk::load() )
	{
		std::cout << "[-] Failed to load klhk.sys!\n";

		// Do cleanup related to klhk
		//
		klhk::cleanup( true );

		// Close handle to SCM
		//
		loader::close_scm();

		return EXIT_FAILURE;
	}

	// Make sure klhk.sys gets Shadow SSDT information by converting the current thread to a GUI thread
	//
	MessageBoxA( 0, "klhk.sys loaded!", 0, 0 );

	// Load MasterHide.sys
	//
	if ( !kasperskyhook::load() )
	{
		std::cout << "[-] Failed to load MasterHide.sys\n";

		// Do cleanup for MasterHide
		//
		kasperskyhook::unload();

		// Do cleanup for klhk
		//
		klhk::cleanup( true );

		// Close handle to SCM
		//
		loader::close_scm();

		return EXIT_FAILURE;
	}

	// Wait for user input
	//
	while ( !( GetAsyncKeyState( VK_END ) & 1 ) )
		Sleep( 1 );

	// Unload MasterHide.sys
	//
	if ( !kasperskyhook::unload() )
		std::cout << "[-] Failed to unload MasterHide.sys!\n";

	// Do cleanup for klhk
	//
	klhk::cleanup( false );

	// Close handle to SCM
	//
	loader::close_scm();

	return EXIT_SUCCESS;
}