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

#include "config/serverurlpolicy.h"

#include "theme.h"

#include <QCoreApplication>

namespace {

QString httpsScheme()
{
    return QStringLiteral("https");
}

/**
 * A trailing slash carries no meaning here, so compare paths without it.
 */
QString comparablePath(const QUrl &url)
{
    QString path = url.path(QUrl::FullyDecoded);
    while (path.endsWith(QLatin1Char('/'))) {
        path.chop(1);
    }
    return path;
}

bool isSafeUrl(const QUrl &url)
{
    if (!url.isValid() || url.isEmpty() || url.host().isEmpty()) {
        return false;
    }
    // relative segments would let a crafted URL escape the pinned path
    const QStringList segments = url.path(QUrl::FullyDecoded).split(QLatin1Char('/'));
    return !segments.contains(QStringLiteral("..")) && !segments.contains(QStringLiteral("."));
}

}

namespace OCC {

ServerUrlPolicy ServerUrlPolicy::fromTheme()
{
    const Theme *theme = Theme::instance();
    if (theme == nullptr) {
        return ServerUrlPolicy(QUrl(), QUrl());
    }
    return ServerUrlPolicy(QUrl(theme->overrideServerUrlV2()), QUrl(theme->oidcIssuerUrl()));
}

ServerUrlPolicy::ServerUrlPolicy(const QUrl &fixedServerUrl, const QUrl &pinnedIssuerUrl)
    : _fixedServerUrl(fixedServerUrl)
    , _pinnedIssuerUrl(pinnedIssuerUrl)
{
}

bool ServerUrlPolicy::isEnforced() const
{
    return !_fixedServerUrl.isEmpty() && _fixedServerUrl.isValid();
}

QUrl ServerUrlPolicy::fixedServerUrl() const
{
    return _fixedServerUrl;
}

QUrl ServerUrlPolicy::pinnedIssuerUrl() const
{
    return _pinnedIssuerUrl;
}

bool ServerUrlPolicy::hasTrustedOrigin(const QUrl &url, const QUrl &reference) const
{
    if (!isSafeUrl(url) || !reference.isValid()) {
        return false;
    }
    if (url.scheme() != httpsScheme() || reference.scheme() != httpsScheme()) {
        return false;
    }
    // an exact host match: a subdomain is a different server
    if (url.host(QUrl::FullyDecoded).compare(reference.host(QUrl::FullyDecoded), Qt::CaseInsensitive) != 0) {
        return false;
    }
    // credentials in a URL are never legitimate here and would hide the real host from a reader
    if (!url.userInfo().isEmpty()) {
        return false;
    }
    return url.port(443) == reference.port(443);
}

bool ServerUrlPolicy::isServerUrlAllowed(const QUrl &url) const
{
    if (!isEnforced()) {
        return true;
    }
    if (!hasTrustedOrigin(url, _fixedServerUrl)) {
        return false;
    }
    if (url.hasQuery() || url.hasFragment()) {
        return false;
    }
    return comparablePath(url) == comparablePath(_fixedServerUrl);
}

bool ServerUrlPolicy::isIssuerAllowed(const QUrl &issuer) const
{
    if (!isEnforced() || _pinnedIssuerUrl.isEmpty()) {
        return true;
    }
    if (!hasTrustedOrigin(issuer, _pinnedIssuerUrl)) {
        return false;
    }
    if (issuer.hasQuery() || issuer.hasFragment()) {
        return false;
    }
    return comparablePath(issuer) == comparablePath(_pinnedIssuerUrl);
}

bool ServerUrlPolicy::isEndpointAllowed(const QUrl &endpoint) const
{
    if (!isEnforced() || _pinnedIssuerUrl.isEmpty()) {
        return true;
    }
    if (!hasTrustedOrigin(endpoint, _pinnedIssuerUrl)) {
        return false;
    }
    if (endpoint.hasFragment()) {
        return false;
    }
    const QString issuerPath = comparablePath(_pinnedIssuerUrl);
    const QString endpointPath = comparablePath(endpoint);
    if (endpointPath == issuerPath) {
        return true;
    }
    // compare on a segment boundary so that /oidcevil does not pass for /oidc
    return endpointPath.startsWith(issuerPath + QLatin1Char('/'));
}

bool ServerUrlPolicy::isAuthenticationUrlAllowed(const QUrl &url) const
{
    if (!isEnforced()) {
        return true;
    }
    return isServerUrlAllowed(url) || isIssuerAllowed(url);
}

QString ServerUrlPolicy::rejectionMessage() const
{
    return QCoreApplication::translate("OCC::ServerUrlPolicy", "This application can only connect to %1.")
        .arg(_fixedServerUrl.toDisplayString());
}

} // namespace OCC
