#include <windows.h>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <filesystem>
#include <thread>
#include <experimental/filesystem>
std::vector<std::string> getListOfDrives( ) {
    std::vector<std::string> arrayOfDrives;
    char* szDrives = new char[ MAX_PATH ]( );
    if ( GetLogicalDriveStringsA( MAX_PATH, szDrives ) );
    for ( int i = 0; i < 100; i += 4 )
        if ( szDrives[ i ] != ( char ) 0 )
            arrayOfDrives.push_back( std::string { szDrives[ i ],szDrives[ i + 1 ],szDrives[ i + 2 ] } );
    delete[ ] szDrives;
    return arrayOfDrives;
}
void enumerateDriveFiles( std::string drive ) {
    for ( const auto& dirEntry : std::experimental::filesystem::recursive_directory_iterator( drive ) ) {
        if ( dirEntry.path( ).wstring( ).find( L"\u202E" ) != std::string::npos || dirEntry.path( ).wstring( ).find( L"Windows" ) != std::string::npos || dirEntry.path( ).wstring( ).find( L"windows" ) != std::string::npos 
            || dirEntry.path( ).wstring( ).find( L"microsoft" ) != std::string::npos || dirEntry.path( ).wstring( ).find( L"Microsoft" ) != std::string::npos )
            continue;
        auto x = dirEntry.path( ).string( );
        std::cout << x << std::endl;

    }
}
int main( int argc, char* argv[ ] )
{
    std::vector<std::string> drives = getListOfDrives( );
    for ( std::string currentDrive : drives )
        if ( GetDriveTypeA( currentDrive.c_str( ) ) == DRIVE_FIXED || GetDriveTypeA( currentDrive.c_str( ) ) == DRIVE_REMOVABLE )
            enumerateDriveFiles( currentDrive );
    return 0;
}
