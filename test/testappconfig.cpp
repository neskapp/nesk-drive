// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtTest>

#include "libsync/owncloudtheme.h"
#include "libsync/config/appconfig.h"

class TestAppConfig : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testConfigPath()
    {
        // the product identity comes from the branding this client was built with, so the expected
        // paths are derived from the theme rather than hardcoded to the upstream product
        auto t = OCC::ownCloudTheme();
        QCOMPARE(OCC::AppConfig::configPath(QOperatingSystemVersion::Windows, t),
            QString("HKEY_LOCAL_MACHINE\\Software\\Policies\\%1\\%2").arg(t.vendor(), t.appNameGUI()));
        QCOMPARE(OCC::AppConfig::configPath(QOperatingSystemVersion::MacOS, t),
            QString("/Library/Preferences/%1/%2.ini").arg(t.orgDomainName(), t.appName()));
        QCOMPARE(OCC::AppConfig::configPath(QOperatingSystemVersion::Unknown, t), QString("/etc/%1/%1.ini").arg(t.appName()));
    }
};

QTEST_GUILESS_MAIN(TestAppConfig)
#include "testappconfig.moc"
