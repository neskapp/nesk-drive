/*
 *    This software is in the public domain, furnished "as is", without technical
 *    support, and with no warranty, express or implied, as to its usefulness for
 *    any purpose.
 *
 */

#include <QtTest>

#include "libsync/theme.h"

using namespace OCC;

class TestNeskTheme : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        // The fixed server URL must not depend on the tester's environment.
        qunsetenv("OWNCLOUD_OVERRIDE_SERVER_URL");
    }

    void testProductIdentity()
    {
        Theme *theme = Theme::instance();
        QVERIFY(theme);
        QCOMPARE(theme->appNameGUI(), QStringLiteral("Nesk Drive"));
        QCOMPARE(theme->appName(), QStringLiteral("nesk-drive"));
        QCOMPARE(theme->vendor(), QStringLiteral("Neskapp"));
        QCOMPARE(theme->orgDomainName(), QStringLiteral("ch.nesk.drive"));
    }

    void testServerUrlIsFixed()
    {
        Theme *theme = Theme::instance();
        QCOMPARE(theme->overrideServerUrlV2(), QStringLiteral("https://files.nesk.ch"));
        QVERIFY(!theme->allowSystemConfigOverrides());
    }

    void testUpdateUrl()
    {
        QCOMPARE(Theme::instance()->updateCheckUrl(),
            QUrl(QStringLiteral("https://downloads.nesk.ch/drive/windows/stable.xml")));
    }

    void testOidcRegistration()
    {
        Theme *theme = Theme::instance();
        // Nesk Drive uses its own client registration, never the upstream
        // ownCloud credentials. The values are intentionally never printed.
        QVERIFY(!theme->oauthClientId().isEmpty());
        QVERIFY(!theme->oauthClientSecret().isEmpty());
        QVERIFY(theme->oauthClientId()
            != QStringLiteral("xdXOt13JKxym1B1QcEncf2XDkLAexMBFwiT9j6EfhhHFJhs2KM9jbjTmf8JBXE69"));
        QVERIFY(theme->oauthClientSecret()
            != QStringLiteral("UBntmLjC2yYCeHwsyj73Uwo9TAaecAetRwMw0xYcvNL9yRdLSUi0hUAHfvCHFeFh"));
        QCOMPARE(theme->openIdConnectScopes(), QStringLiteral("openid offline_access email profile"));
        QCOMPARE(theme->oauthPorts(), (QVector<quint16>{0}));
    }

    void testSingleAccount()
    {
        QVERIFY(!Theme::instance()->multiAccount());
    }
};

QTEST_GUILESS_MAIN(TestNeskTheme)
#include "testnesktheme.moc"
