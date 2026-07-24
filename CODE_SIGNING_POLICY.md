# Code signing policy

Free code signing provided by SignPath.io, certificate by SignPath Foundation.

## Scope

This policy applies to every Nesk Drive Windows release artifact, including
installers, executables, and update payloads. A release may be signed only
from a reviewed commit after every gate in this policy is configured and
verified.

## Ownership and independent roles

The repository is owned by the [`neskapp`](https://github.com/neskapp)
organization. Release responsibilities are assigned through these stable
GitHub teams:

- [Nesk Drive maintainers](https://github.com/orgs/neskapp/teams/nesk-drive-maintainers)
- [Nesk Drive reviewers](https://github.com/orgs/neskapp/teams/nesk-drive-reviewers)
- [Nesk Drive signing approvers](https://github.com/orgs/neskapp/teams/nesk-drive-signing-approvers)

Every person assigned to a release role must use two-factor authentication.
The author of a release change cannot supply its independent review or approve
its signing request. Release and signing activity remains blocked until the
reviewer and signing-approver teams each contain a qualified independent
member and their access and two-factor authentication are verified.

## Review and signing requirements

- A change proposed by a non-maintainer must arrive by pull request and receive
  the independent reviewer's approval before merge.
- Every change to a release branch requires one approval. A change from a
  non-maintainer requires a pull request review from the independent reviewer
  before merge.
- Every signing request requires explicit manual approval by the independent,
  verified signing approver; no request is implicitly or automatically
  approved.
- The signing approver must verify the source commit, intended release version,
  generated artifact names, and release channel before approving the request.
- Signing credentials and the SignPath API token must be restricted to the
  signing environment and must never be committed to the repository or
  distributed to individual contributors.
- A signing approval and its resulting artifact identifiers must be retained in
  the release record for auditability.

## Mandatory repository and environment gates

Before any Nesk release, administrators must configure and verify these GitHub
branch-protection gates on `nesk/stable-7.1` and `main`: pull requests and one
approval required, stale approvals dismissed, required CI passing, and force
pushes and branch deletion disabled.

Before any signing request, administrators must configure and verify a
`signpath` environment that requires manual approval and restricts
`SIGNPATH_API_TOKEN` to that environment. Before a release is published,
administrators must configure and verify a `release` environment that requires
manual approval and restricts the manifest private key and storage credentials
to that environment. Missing, unverified, or bypassed controls block signing
and release; this policy grants no exception.
