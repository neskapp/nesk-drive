# Security policy

## Reporting a vulnerability

Do not report a security vulnerability in a public issue. Use the
[private GitHub security advisory form](https://github.com/Neprena/nesk-drive/security/advisories/new)
for this repository. Include a clear description, affected release or commit,
reproduction steps, and any suggested mitigation.

If private reporting is unavailable, use the repository's GitHub contact path
and state that the report is security-sensitive. Do not include exploit details
in a public issue.

## Supported release line

The supported pilot line is `nesk/stable-7.1`, based on ownCloud Desktop Client
`v7.1.0`. Security fixes are evaluated for that line and are published as
signed Nesk Drive releases when applicable.

## Coordinated disclosure

We will acknowledge a valid private report, investigate it, and coordinate a
fix and disclosure with the reporter where practical. Releases follow
[CODE_SIGNING_POLICY.md](CODE_SIGNING_POLICY.md). Nesk Drive's service endpoint
is `https://files.nesk.ch`; update checks use `https://downloads.nesk.ch`.
