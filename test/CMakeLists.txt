include(owncloud_add_test.cmake)

add_subdirectory(testutils)

owncloud_add_test(JHash)

owncloud_add_test(OwncloudPropagator)
owncloud_add_test(OwnSql)
owncloud_add_test(SyncJournalDB)
owncloud_add_test(SyncFileItem)
owncloud_add_test(ConcatUrl)
owncloud_add_test(XmlParse)
owncloud_add_test(OAuthHtmlPage ../src/gui/creds/oauthhtmlpage.cpp)
owncloud_add_test(ChecksumValidator)
owncloud_add_test(ConnectionValidator)

# TODO: we need keychain access for this test
if (NOT APPLE OR NOT DEFINED ENV{GITHUB_ACTION})
    owncloud_add_test(CredentialManager)
endif()

owncloud_add_test(ExcludedFiles)

owncloud_add_test(Utility)
owncloud_add_test(AppConfig ../src/libsync/owncloudtheme.cpp)

# Nesk Drive branding and fixed-server tests: only meaningful when the Nesk OEM theme is compiled in.
if(APPLICATION_SHORTNAME STREQUAL "nesk-drive")
    owncloud_add_test(NeskTheme)
    # the wizard classes are not exported from owncloudGui, so the test compiles them in,
    # together with the network adapters and the oauth machinery they pull along
    owncloud_add_test(NewAccountWizard
        ../src/gui/newaccountwizard/advancedsettingspagecontroller.cpp
        ../src/gui/newaccountwizard/authsuccesspagecontroller.cpp
        ../src/gui/newaccountwizard/newaccountenums.h
        ../src/gui/newaccountwizard/newaccountmodel.cpp
        ../src/gui/newaccountwizard/newaccountwizard.cpp
        ../src/gui/newaccountwizard/newaccountwizardcontroller.cpp
        ../src/gui/newaccountwizard/oauthpagecontroller.cpp
        ../src/gui/newaccountwizard/urlpagecontroller.cpp
        ../src/gui/networkadapters/determineauthtypeadapter.cpp
        ../src/gui/networkadapters/discoverwebfingerserviceadapter.cpp
        ../src/gui/networkadapters/fetchcapabilitiesadapter.cpp
        ../src/gui/networkadapters/resolveurladapter.cpp
        ../src/gui/networkadapters/userinfoadapter.cpp
        ../src/gui/networkadapters/webfingerlookupadapter.cpp
        ../src/gui/creds/oauth.cpp
        ../src/gui/creds/oauthhtmlpage.cpp
        ../src/gui/tlserrordialog.cpp
        ../src/gui/tlserrordialog.ui
    )
    # tlserrordialog.cpp includes its generated ui header, which needs uic on this target too
    set_target_properties(NewAccountWizardTest PROPERTIES AUTOUIC ON)
endif()

owncloud_add_test(SyncEngine)
owncloud_add_test(SyncMove)
add_dependencies(SyncMoveTest test_helper)
owncloud_add_test(SyncDelete)
owncloud_add_test(SyncConflict)
owncloud_add_test(SyncFileStatusTracker)
owncloud_add_test(Download)
owncloud_add_test(UploadReset)
owncloud_add_test(Blacklist)
owncloud_add_test(LocalDiscovery)
owncloud_add_test(RemoteDiscovery)
owncloud_add_test(Permissions)
owncloud_add_test(DatabaseError)
owncloud_add_test(LockedFiles)
owncloud_add_test(Adapters
    ../src/gui/networkadapters/determineauthtypeadapter.cpp
    ../src/gui/networkadapters/discoverwebfingerserviceadapter.cpp
    ../src/gui/networkadapters/fetchcapabilitiesadapter.cpp
#    ../src/gui/networkadapters/resolveurladapter.cpp
#    ../src/gui/tlserrordialog.cpp
    ../src/gui/networkadapters/webfingerlookupadapter.cpp
)

if (WITH_AUTO_UPDATER)
    owncloud_add_test(Updater)
endif()

# Deliberately outside the WITH_AUTO_UPDATER guard: what protects the update channel is
# cryptography, and it has to be verified on every platform we build on, including those
# that ship without the updater.
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
owncloud_add_test(UpdateSignature ../src/gui/updater/updatesignature.cpp)
# plain signature, to match the one owncloud_add_test already used on this target
target_link_libraries(UpdateSignatureTest OpenSSL::Crypto)

owncloud_add_test(FolderWatcher)

if( UNIX AND NOT APPLE )
    owncloud_add_test(InotifyWatcher)
endif(UNIX AND NOT APPLE)

owncloud_add_test(FileSystem)

owncloud_add_test(FolderMan)

owncloud_add_test(OAuth ../src/gui/creds/oauth.cpp ../src/gui/networkadapters/userinfoadapter.cpp ../src/gui/creds/oauthhtmlpage.cpp)

configure_file(test_journal.db "${PROJECT_BINARY_DIR}/bin/test_journal.db" COPYONLY)

owncloud_add_test(JobQueue)

add_subdirectory(modeltests)

if(WIN32)
    find_file(cldapi_FOUND CldApi.dll)
    if (cldapi_FOUND)
        include("${CMAKE_SOURCE_DIR}/test/owncloud_add_test.cmake")
        owncloud_add_test(WinVfs)
        target_link_libraries(WinVfsTest vfs_plugin_win_static)
        add_dependencies(WinVfsTest vfs_wincfapi)
    else()
        message(WARNING "Failed to detect CldApi.dll: Skipping WinVfsTest")
    endif()
endif()
