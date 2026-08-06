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

    // The About dialog is the one place where a rebranded fork can quietly break
    // the law. These assertions are a guard rail, not decoration: they fail if a
    // future edit drops a notice the GPL requires us to keep.
    void testAboutKeepsUpstreamNotices()
    {
        const QString about = Theme::instance()->about();
        QVERIFY(about.contains(QStringLiteral("Copyright ownCloud GmbH")));
        QVERIFY(about.contains(QStringLiteral("GNU General Public License")));
        QVERIFY(about.contains(QStringLiteral("Klaas Freitag")));
        QVERIFY(about.contains(QStringLiteral("Hannah von Reth")));
        // GPL section 3: the corresponding sources must be reachable.
        QVERIFY(about.contains(QStringLiteral("github.com/neskapp/nesk-drive")));
    }

    void testAboutSendsUsersToNesk()
    {
        Theme *theme = Theme::instance();
        const QString about = theme->about();
        // Support must not point at the ownCloud community forum.
        QVERIFY(!about.contains(QStringLiteral("central.owncloud.com")));
        QVERIFY(about.contains(QStringLiteral("https://nesk.ch/support")));
        // Claiming a registered trademark we do not hold would be a false
        // statement; upstream asserts it for whatever vendor builds the client.
        QVERIFY(!about.contains(QStringLiteral("registered trademarks")));
        QVERIFY(about.contains(QStringLiteral("trademarks of Neskapp")));
        // Hides the duplicate copyright label in the settings, and keeps
        // Theme::gitSHA1 from linking to a commit that does not exist upstream.
        QVERIFY(!theme->aboutShowCopyright());
        QVERIFY(!theme->aboutVersions(Theme::VersionFormat::RichText)
                     .contains(QStringLiteral("github.com/owncloud/client")));
        // Empty on purpose: the derived URL would be a 404.
        QVERIFY(theme->conflictHelpUrl().isEmpty());
    }
};

QTEST_GUILESS_MAIN(TestNeskTheme)
#include "testnesktheme.moc"
