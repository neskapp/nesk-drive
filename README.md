# Nesk Drive

Nesk Drive is the Windows desktop sync client for [Nesk](https://files.nesk.ch).
It synchronizes files and authenticates only with the fixed Nesk service at
`https://files.nesk.ch`.

## Pilot support

The pilot release supports Windows 10 x64 and Windows 11 x64. Its application
ID is `ch.nesk.drive`.

## Network and privacy

Nesk Drive does not include telemetry and does not automatically upload crash
reports. Nesk Drive network traffic is limited to:

- `files.nesk.ch` for authentication and synchronization;
- `downloads.nesk.ch` for user-requested and periodic update checks.

The update service is not contacted for other purposes. See
[PRIVACY.md](PRIVACY.md) for the data-handling policy and
[SECURITY.md](SECURITY.md) for reporting a security issue.

## Releases and source

This repository is a fork of the [ownCloud Desktop Client](https://github.com/owncloud/client),
based on upstream release `v7.1.0` at commit
`37837175c2f542086c52255dc42282d2687ad8a1`. Nesk changes are made on
`nesk/stable-7.1`; changes from upstream are reviewed and deliberately merged
instead of being automatically rebased or imported. The upstream project
remains the authoritative source for its own releases.

Nesk Drive is distributed under the GNU General Public License, version 2 or
(at your option) any later version (GPL-2.0-or-later). A copy is in
[COPYING](COPYING). When distributing binaries or modified versions, we will
provide the corresponding source code and retain the required license,
copyright, and warranty notices. The complete source for Nesk releases is
available from this repository and its release tags.

Release-signing controls are described in [CODE_SIGNING_POLICY.md](CODE_SIGNING_POLICY.md).
Product naming and identifiers are described in [BRANDING.md](BRANDING.md).

## Building from source

This project uses CMake and KDE Craft. General build information is available
from the [CMake documentation](https://cmake.org/documentation/) and the
[KDE Craft documentation](https://community.kde.org/Craft). The upstream
project also provides [ownbuild instructions](https://github.com/owncloud/ownbuild).

## Upstream notices and contribution history

Nesk Drive is derived from the ownCloud Desktop Client, which is developed by
the ownCloud community and [has received patches from many
authors](https://github.com/owncloud/client/graphs/contributors). Historical
upstream maintainers listed by the upstream README include Markus Goetz,
Olivier Goffart, Christian Kamm, Thomas Müller, Klaas Freitag, Daniel
Molkentin, and Andreas Schneider. Their copyright and license notices remain
in the source tree.

For Nesk-specific bugs or improvement proposals, open an issue in this
repository. For security-sensitive reports, follow [SECURITY.md](SECURITY.md)
instead of opening a public issue.
