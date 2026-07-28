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

#include "common/depreaction.h"
#include "theme.h"

#include <QColor>
#include <QString>
#include <QVector>

namespace OCC {

/**
 * @brief OEM theme for Nesk Drive.
 *
 * Nesk Drive is a single-service client: it only ever talks to
 * https://files.nesk.ch and authenticates against the Nesk OIDC identity
 * provider. The theme is header-only because the OEM mechanism compiles it
 * into libsync through THEME_INCLUDE (see THEME.cmake).
 */
class NeskTheme : public Theme
{
public:
    NeskTheme() = default;

    QString appNameGUI() const override { return QStringLiteral("Nesk Drive"); }

    // The service endpoint is fixed; the account wizard must never offer a
    // server selection. Enforcement of the URL at every entry point goes
    // through ServerUrlPolicy (see libsync/config/serverurlpolicy.h).
    OC_DISABLE_DEPRECATED_WARNING
    QString overrideServerUrl() const override { return QStringLiteral("https://files.nesk.ch"); }
    OC_ENABLE_DEPRECATED_WARNING

    // The identity provider is the Nesk platform itself, not the file service.
    // Discovery is served by files.nesk.ch but must announce exactly this issuer,
    // and every OAuth endpoint has to live below it.
    QString oidcIssuerUrl() const override { return QStringLiteral("https://nesk.ch/oidc"); }

    bool allowSystemConfigOverrides() const override { return false; }

    // One Nesk account per installation.
    bool multiAccount() const override { return false; }

    QString helpUrl() const override { return QStringLiteral("https://nesk.ch/drive"); }

    // Dedicated OIDC registration on the Nesk identity provider. Native app
    // credentials are public by nature: security relies on PKCE, exact
    // loopback redirects, short access-token lifetime and server-side
    // revocation, never on the confidentiality of these values.
    // TODO(nesk): replace the placeholders with the production registration
    // before the first signed build (tracked, must not ship as-is).
    QString oauthClientId() const override { return QStringLiteral("nesk-drive-dev-placeholder"); }
    QString oauthClientSecret() const override { return QStringLiteral("nesk-drive-dev-placeholder-secret"); }

    // A random loopback port, as registered on the IdP side.
    QVector<quint16> oauthPorts() const override { return {0}; }

    QString openIdConnectScopes() const override
    {
        return QStringLiteral("openid offline_access email profile");
    }

    // Nesk brand: bordeaux header with white title (see BRANDING.md).
    QColor wizardHeaderBackgroundColor() const override { return QColor(0xB2, 0x3A, 0x4E); }
    QColor wizardHeaderTitleColor() const override { return QColor(Qt::white); }
};

} // namespace OCC
