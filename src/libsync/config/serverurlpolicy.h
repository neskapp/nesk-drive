/*
 * Copyright (C) by the Nesk Drive maintainers <https://github.com/neskapp/nesk-drive>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#pragma once

#include "owncloudlib.h"

#include <QString>
#include <QUrl>

namespace OCC {

/**
 * @brief Single-service constraint for branded builds.
 * @ingroup libsync
 *
 * A branded client such as Nesk Drive talks to exactly one service and
 * authenticates against exactly one identity provider. This class is the single
 * place where that constraint is expressed, so that every entry point of a
 * server URL (wizard field, resolved redirect, webfinger answer, restored
 * account, OIDC discovery document) can be checked the same way.
 *
 * The policy is inert when no fixed server URL is configured: unbranded builds
 * keep the upstream behaviour of accepting any server the user types in.
 */
class OWNCLOUDSYNC_EXPORT ServerUrlPolicy
{
public:
    /**
     * Build the policy from the compiled-in theme.
     */
    static ServerUrlPolicy fromTheme();

    /**
     * @param fixedServerUrl the only service this build may connect to, empty to disable the policy
     * @param pinnedIssuerUrl the only OIDC issuer this build may trust, empty to accept the issuer of the service
     */
    ServerUrlPolicy(const QUrl &fixedServerUrl, const QUrl &pinnedIssuerUrl);

    /**
     * @return true when this build is locked to a single service.
     */
    bool isEnforced() const;

    QUrl fixedServerUrl() const;
    QUrl pinnedIssuerUrl() const;

    /**
     * Accept only the branded service: https, the exact host, the default port,
     * no user information, no query, no fragment and the exact path.
     */
    bool isServerUrlAllowed(const QUrl &url) const;

    /**
     * Accept a URL used to start authentication. Both the service itself and the
     * pinned identity provider are valid, because webfinger may hand out either.
     */
    bool isAuthenticationUrlAllowed(const QUrl &url) const;

    /**
     * Accept only the pinned OIDC issuer, compared exactly (a trailing slash aside).
     */
    bool isIssuerAllowed(const QUrl &issuer) const;

    /**
     * Accept only OIDC endpoints served below the pinned issuer, so that a
     * tampered discovery document cannot redirect authentication elsewhere.
     */
    bool isEndpointAllowed(const QUrl &endpoint) const;

    /**
     * @return a user facing explanation of why a URL was refused.
     */
    QString rejectionMessage() const;

private:
    /** Common scheme/host/port/user-info checks shared by all the rules. */
    bool hasTrustedOrigin(const QUrl &url, const QUrl &reference) const;

    QUrl _fixedServerUrl;
    QUrl _pinnedIssuerUrl;
};

} // namespace OCC
