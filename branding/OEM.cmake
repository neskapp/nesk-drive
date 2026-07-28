set( APPLICATION_NAME       "Nesk Drive" )
set( APPLICATION_SHORTNAME  "nesk-drive" )
set( APPLICATION_EXECUTABLE "nesk-drive" )
set( APPLICATION_DOMAIN     "nesk.ch" )
set( APPLICATION_VENDOR     "Neskapp" )
set( APPLICATION_UPDATE_URL "https://downloads.nesk.ch/drive/windows/stable.xml" CACHE STRING "URL for updater" )
set( APPLICATION_ICON_NAME  "nesk-drive" )
set( APPLICATION_VIRTUALFILE_SUFFIX "neskdrive" CACHE STRING "Virtual file suffix (not including the .)")

set( LINUX_PACKAGE_SHORTNAME "nesk-drive" )

set( THEME_CLASS            "NeskTheme" )
set( APPLICATION_REV_DOMAIN "ch.nesk.drive" )
set( WIN_SETUP_BITMAP_PATH  "${CMAKE_SOURCE_DIR}/admin/win/nsi" )

set( MAC_INSTALLER_BACKGROUND_FILE "${CMAKE_SOURCE_DIR}/admin/osx/installer-background.png" CACHE STRING "The MacOSX installer background image")

set( THEME_INCLUDE          "${OEM_THEME_DIR}/nesktheme.h" )

# Nesk Drive ships without any crash reporter or telemetry (see PRIVACY.md).
option( WITH_CRASHREPORTER "Build crashreporter" OFF )

# OIDC registration of Nesk Drive on the Nesk identity provider.
#
# A native application is a public client: these values are not a secret in the usual sense,
# and the security of the flow rests on PKCE, exact loopback redirects, short lived access
# tokens and server side revocation. They are still injected at build time rather than stored
# here, so that rotating the registration never requires a public commit and the history does
# not accumulate stale credentials. Release builds pass them on the CMake command line.
# The defaults below are development placeholders: the identity provider rejects them.
set( NESK_OAUTH_CLIENT_ID "nesk-drive-dev-placeholder" CACHE STRING "OIDC client id of the Nesk Drive registration" )
set( NESK_OAUTH_CLIENT_SECRET "nesk-drive-dev-placeholder-secret" CACHE STRING "OIDC client secret of the Nesk Drive registration" )
