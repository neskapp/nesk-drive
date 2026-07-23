# Code signing policy

Free code signing provided by SignPath.io, certificate by SignPath Foundation.

## Scope

This policy applies to every Nesk Drive Windows release artifact, including
installers, executables, and update payloads. Signed artifacts are produced
only from reviewed commits on protected Nesk release branches.

## Roles and access

The repository uses role-based GitHub teams rather than personal accounts:

- Maintainers: [Nesk Drive maintainers](https://github.com/orgs/Neprena/teams/nesk-drive-maintainers)
- Reviewers: [Nesk Drive reviewers](https://github.com/orgs/Neprena/teams/nesk-drive-reviewers)
- Signing approvers: [Nesk Drive signing approvers](https://github.com/orgs/Neprena/teams/nesk-drive-signing-approvers)

Membership must require two-factor authentication. Until the corresponding
Neprena GitHub teams are provisioned and access is verified, these role URLs
are the operational ownership record; release signing must not proceed on the
strength of an unverified or personal-account assignment.

## Review and signing requirements

- Changes proposed by a non-maintainer require a pull request review from a
  maintainer or reviewer before merge.
- Every signing request requires explicit approval by a member of the signing
  approvers team; no request is implicitly or automatically approved.
- The approver verifies the source commit, intended release version, generated
  artifact names, and release channel before approving the request.
- Signing credentials and the SignPath API token are restricted to the signing
  environment and are never committed to the repository or distributed to
  individual contributors.
- A signing approval and its resulting artifact identifiers must be retained in
  the release record for auditability.

## Repository and environment controls

The `nesk/stable-7.1` and `main` branches are release-controlled branches:
pull requests and one approval are required, stale approvals are dismissed,
required CI must pass, and force pushes and branch deletion are disabled.

The `signpath` environment requires manual approval and holds
`SIGNPATH_API_TOKEN`. The `release` environment requires manual approval and
holds the manifest private key and storage credentials. Repository
administrators configure these controls in GitHub; this policy does not grant
an exception when a control is missing.
