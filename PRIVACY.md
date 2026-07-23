# Nesk Drive privacy

Nesk Drive is a Windows client for the fixed Nesk service at
`https://files.nesk.ch`. The pilot supports Windows 10 x64 and Windows 11 x64.

## Data and destinations

To authenticate and synchronize, Nesk Drive communicates with
`files.nesk.ch`. Those requests contain the account, authentication, file, and
synchronization data necessary to provide the service.

Nesk Drive contacts `downloads.nesk.ch` only for user-requested and periodic
update checks. Update checks request release/update information; they are not
used for analytics or advertising.

## No telemetry or automatic crash uploads

Nesk Drive does not include telemetry. It does not automatically upload crash
reports. The client has no telemetry endpoint and no crash-report upload
endpoint. If a user chooses to seek support, they decide what diagnostic
information to share through the selected support channel.

## Changes to this policy

Any future Nesk Drive feature that adds a network destination, telemetry, or
crash-report upload requires a documented policy update and release review
before it is enabled. The source and release history for this GPL-2.0-or-later
client are available in this repository.
