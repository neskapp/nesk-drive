# Nesk Drive privacy

This policy sets requirements for Nesk Drive releases. Nesk releases must use
the fixed Nesk service at `https://files.nesk.ch`; the pilot target is Windows
10 x64 and Windows 11 x64.

## Data and destinations

To authenticate and synchronize, a Nesk release must communicate with
`files.nesk.ch`. Those requests may contain the account, authentication, file,
and synchronization data necessary to provide the service.

Nesk releases may contact `downloads.nesk.ch` only for user-requested and
periodic update checks. Update checks must request release/update information
only; they must not be used for analytics or advertising.

## No telemetry or automatic crash uploads

Nesk releases must not include telemetry or automatically upload crash reports.
They must not use a telemetry endpoint or a crash-report upload endpoint. If a
user chooses to seek support, they decide what diagnostic information to share
through the selected support channel.

## Changes to this policy

Any future Nesk Drive feature that adds a network destination, telemetry, or
crash-report upload requires a documented policy update and release review
before it is enabled. The source and release history for this GPL-2.0-or-later
client are available in this repository.
