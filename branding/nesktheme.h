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
#include "common/utility.h"
#include "common/version.h"
#include "config.h"
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
    // They are injected at configure time (NESK_OAUTH_CLIENT_ID and
    // NESK_OAUTH_CLIENT_SECRET, see branding/OEM.cmake), so rotating the
    // registration never requires a commit. Without injection the build falls
    // back to development placeholders, which the provider rejects.
    QString oauthClientId() const override { return QStringLiteral(NESK_OAUTH_CLIENT_ID); }
    QString oauthClientSecret() const override { return QStringLiteral(NESK_OAUTH_CLIENT_SECRET); }

    // A random loopback port, as registered on the IdP side.
    QVector<quint16> oauthPorts() const override { return {0}; }

    QString openIdConnectScopes() const override
    {
        return QStringLiteral("openid offline_access email profile");
    }

    // Upstream points users at the ownCloud community forum and at a commit in
    // owncloud/client that does not exist in this fork. Both would send a Nesk
    // user to the wrong place, so the whole block is replaced.
    //
    // What must NOT be replaced, and is reproduced verbatim below: the ownCloud
    // GmbH copyright notice, the upstream author list and the GPL notice. The
    // GPL requires the copyright and licence notices to be kept, and removing
    // the only nominative attribution in the product would be wrong regardless
    // (see BRANDING.md). The link to our own sources also serves GPL section 3.
    //
    // The trademark sentence deliberately says "trademarks" and not "registered
    // trademarks": upstream asserts registration for whatever vendor builds the
    // client, which would be a false claim as long as no registration exists.
    QString about() const override
    {
        return tr("<p>Version %1. For more information visit <a href=\"%2\">https://%3</a></p>"
                  "<p>For help, please visit: <a href=\"https://nesk.ch/support\">https://nesk.ch/support</a></p>"
                  "<p>Source code: <a href=\"https://github.com/neskapp/nesk-drive\">https://github.com/neskapp/nesk-drive</a></p>"
                  "<p><small>By Klaas Freitag, Daniel Molkentin, Olivier Goffart, Markus Götz, "
                  " Jan-Christoph Borchardt, Thomas Müller,<br>"
                  "Dominik Schmidt, Michael Stingl, Hannah von Reth, Fabian Müller and others.</small></p>"
                  "<p>Copyright ownCloud GmbH (A Kiteworks Company)</p>"
                  "<p>Distributed by %4 and licensed under the GNU General Public License (GPL) Version 2.0.<br/>"
                  "%5 and the %5 logo are trademarks of %4.</p>"
                  "<p><small>%6</small></p>")
            .arg(Utility::escape(Version::displayString()), Utility::escape(QStringLiteral("https://" APPLICATION_DOMAIN)),
                Utility::escape(QStringLiteral(APPLICATION_DOMAIN)), Utility::escape(QStringLiteral(APPLICATION_VENDOR)),
                Utility::escape(appNameGUI()), aboutVersions(Theme::VersionFormat::RichText));
    }

    // Two effects, both wanted. It hides the duplicate, untranslated copyright
    // label at the bottom of the General settings page (generalsettings.cpp
    // checks this flag), and it stops Theme::gitSHA1 from linking the commit to
    // github.com/owncloud/client, where this fork's commits do not exist. The
    // copyright notice itself stays in the About dialog above, which is what
    // the GPL asks for.
    bool aboutShowCopyright() const override { return false; }

    // Derived from helpUrl() upstream, which would point at
    // https://nesk.ch/drive/conflicts.html: no such page exists, and the sync
    // error widget hides the link when it is empty. Point it at a real page
    // once one is written.
    QString conflictHelpUrl() const override { return QString(); }

    // Nesk brand: bordeaux header with white title (see BRANDING.md).
    QColor wizardHeaderBackgroundColor() const override { return QColor(0xB2, 0x3A, 0x4E); }
    QColor wizardHeaderTitleColor() const override { return QColor(Qt::white); }
};

} // namespace OCC
