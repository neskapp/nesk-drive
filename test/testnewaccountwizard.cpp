/*
 *    This software is in the public domain, furnished "as is", without technical
 *    support, and with no warranty, express or implied, as to its usefulness for
 *    any purpose.
 *
 */

#include <QtTest>

#include <QLineEdit>
#include <QWizardPage>

#include "../src/gui/newaccountwizard/newaccountmodel.h"
#include "../src/gui/newaccountwizard/newaccountwizard.h"
#include "../src/gui/newaccountwizard/newaccountwizardcontroller.h"
#include "../src/gui/newaccountwizard/urlpagecontroller.h"
#include "../src/gui/folderman.h"
#include "libsync/accessmanager.h"
#include "libsync/config/serverurlpolicy.h"
#include "libsync/theme.h"

using namespace OCC;

namespace {
constexpr const char *serverAddressFieldName = "ServerAddressLineEdit";

ServerUrlPolicy neskPolicy()
{
    return ServerUrlPolicy(QUrl(QStringLiteral("https://files.nesk.ch")), QUrl(QStringLiteral("https://nesk.ch/oidc")));
}
}

/**
 * Nesk Drive is locked to a single service. These tests cover the branded
 * fixed-server flow of the new account wizard: no server can be picked, no
 * foreign host can be injected, and the OIDC issuer is pinned.
 */
class TestNewAccountWizard : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        // the fixed server URL must not depend on the tester's environment
        qunsetenv("OWNCLOUD_OVERRIDE_SERVER_URL");
    }

    void testThemeProvidesTheFixedServerAndIssuer()
    {
        const ServerUrlPolicy policy = ServerUrlPolicy::fromTheme();
        QVERIFY(policy.isEnforced());
        QCOMPARE(policy.fixedServerUrl(), QUrl(QStringLiteral("https://files.nesk.ch")));
        QCOMPARE(policy.pinnedIssuerUrl(), QUrl(QStringLiteral("https://nesk.ch/oidc")));
    }

    void testAcceptedServerUrls_data()
    {
        QTest::addColumn<QString>("url");
        QTest::newRow("exact") << "https://files.nesk.ch";
        QTest::newRow("trailing slash") << "https://files.nesk.ch/";
        QTest::newRow("explicit default port") << "https://files.nesk.ch:443";
        QTest::newRow("upper case host") << "https://FILES.NESK.CH";
    }

    void testAcceptedServerUrls()
    {
        QFETCH(QString, url);
        QVERIFY(neskPolicy().isServerUrlAllowed(QUrl(url)));
    }

    void testRejectedServerUrls_data()
    {
        QTest::addColumn<QString>("url");
        QTest::newRow("plain http") << "http://files.nesk.ch";
        QTest::newRow("subdomain") << "https://evil.files.nesk.ch";
        QTest::newRow("suffix host") << "https://files.nesk.ch.evil.example";
        QTest::newRow("other host") << "https://files.example.com";
        QTest::newRow("bare domain") << "https://nesk.ch";
        QTest::newRow("user info") << "https://user@files.nesk.ch";
        QTest::newRow("user info with password") << "https://user:pass@files.nesk.ch";
        QTest::newRow("non default port") << "https://files.nesk.ch:8443";
        QTest::newRow("query") << "https://files.nesk.ch/?next=https://evil.example";
        QTest::newRow("fragment") << "https://files.nesk.ch/#evil";
        QTest::newRow("path") << "https://files.nesk.ch/dav";
        QTest::newRow("empty") << "";
        QTest::newRow("no host") << "https:///";
        QTest::newRow("file scheme") << "file:///etc/passwd";
    }

    void testRejectedServerUrls()
    {
        QFETCH(QString, url);
        QVERIFY(!neskPolicy().isServerUrlAllowed(QUrl(url)));
    }

    void testIssuerIsPinned_data()
    {
        QTest::addColumn<QString>("issuer");
        QTest::addColumn<bool>("allowed");
        QTest::newRow("exact") << "https://nesk.ch/oidc" << true;
        QTest::newRow("trailing slash") << "https://nesk.ch/oidc/" << true;
        QTest::newRow("http") << "http://nesk.ch/oidc" << false;
        QTest::newRow("other host") << "https://evil.example/oidc" << false;
        QTest::newRow("subdomain") << "https://evil.nesk.ch/oidc" << false;
        QTest::newRow("other path") << "https://nesk.ch/oidc-evil" << false;
        QTest::newRow("parent path") << "https://nesk.ch" << false;
        QTest::newRow("deeper path") << "https://nesk.ch/oidc/sub" << false;
        QTest::newRow("empty") << "" << false;
    }

    void testIssuerIsPinned()
    {
        QFETCH(QString, issuer);
        QFETCH(bool, allowed);
        QCOMPARE(neskPolicy().isIssuerAllowed(QUrl(issuer)), allowed);
    }

    void testOidcEndpointsMustLiveUnderTheIssuer_data()
    {
        QTest::addColumn<QString>("endpoint");
        QTest::addColumn<bool>("allowed");
        QTest::newRow("authorization") << "https://nesk.ch/oidc/auth" << true;
        QTest::newRow("token") << "https://nesk.ch/oidc/token" << true;
        QTest::newRow("jwks") << "https://nesk.ch/oidc/jwks" << true;
        QTest::newRow("issuer itself") << "https://nesk.ch/oidc" << true;
        QTest::newRow("path prefix confusion") << "https://nesk.ch/oidcevil/auth" << false;
        QTest::newRow("outside the path") << "https://nesk.ch/auth" << false;
        QTest::newRow("other host") << "https://evil.example/oidc/auth" << false;
        QTest::newRow("subdomain") << "https://evil.nesk.ch/oidc/auth" << false;
        QTest::newRow("http") << "http://nesk.ch/oidc/auth" << false;
        QTest::newRow("other port") << "https://nesk.ch:8443/oidc/auth" << false;
        QTest::newRow("empty") << "" << false;
    }

    void testOidcEndpointsMustLiveUnderTheIssuer()
    {
        QFETCH(QString, endpoint);
        QFETCH(bool, allowed);
        QCOMPARE(neskPolicy().isEndpointAllowed(QUrl(endpoint)), allowed);
    }

    void testAuthenticationUrlAcceptsServerAndIssuer()
    {
        const ServerUrlPolicy policy = neskPolicy();
        // webfinger may hand out either the service itself or the pinned identity provider
        QVERIFY(policy.isAuthenticationUrlAllowed(QUrl(QStringLiteral("https://files.nesk.ch"))));
        QVERIFY(policy.isAuthenticationUrlAllowed(QUrl(QStringLiteral("https://nesk.ch/oidc"))));
        QVERIFY(!policy.isAuthenticationUrlAllowed(QUrl(QStringLiteral("https://evil.example"))));
    }

    void testPolicyIsInertWithoutAFixedServerUrl()
    {
        // upstream behaviour must be untouched for themes that do not pin a server
        const ServerUrlPolicy policy((QUrl()), QUrl());
        QVERIFY(!policy.isEnforced());
        QVERIFY(policy.isServerUrlAllowed(QUrl(QStringLiteral("https://cloud.example.com"))));
        QVERIFY(policy.isServerUrlAllowed(QUrl(QStringLiteral("http://cloud.example.com"))));
        QVERIFY(policy.isIssuerAllowed(QUrl(QStringLiteral("https://idp.example.com"))));
        QVERIFY(policy.isEndpointAllowed(QUrl(QStringLiteral("https://idp.example.com/authorize"))));
    }

    void testEnvironmentCannotOverrideTheBrandedServerUrl()
    {
        qputenv("OWNCLOUD_OVERRIDE_SERVER_URL", "https://evil.example");
        const QString serverUrl = Theme::instance()->overrideServerUrlV2();
        qunsetenv("OWNCLOUD_OVERRIDE_SERVER_URL");
        QCOMPARE(serverUrl, QStringLiteral("https://files.nesk.ch"));
    }

    void testUrlControllerReportsAFixedServer()
    {
        QWizardPage page;
        OCC::AccessManager accessManager(nullptr);
        UrlPageController controller(&page, &accessManager, nullptr);

        QVERIFY(controller.isServerUrlFixed());

        auto *field = page.findChild<QLineEdit *>(QLatin1String(serverAddressFieldName));
        QVERIFY(field);
        QCOMPARE(field->text(), QStringLiteral("https://files.nesk.ch"));
        QVERIFY(!field->isEnabled());
    }

    void testTheWizardNeverShowsAServerPage()
    {
        // the advanced settings page asks the folder manager for a default sync root
        const std::unique_ptr<FolderMan> folderMan = FolderMan::createInstance();

        NewAccountWizard wizard(nullptr);
        NewAccountModel model(nullptr);
        NewAccountWizardController controller(&model, &wizard, nullptr);

        const QList<int> pageIds = wizard.pageIds();
        QVERIFY(!pageIds.isEmpty());
        for (int id : pageIds) {
            QWizardPage *page = wizard.page(id);
            QVERIFY(page);
            QVERIFY2(page->findChild<QLineEdit *>(QLatin1String(serverAddressFieldName)) == nullptr,
                "the fixed server flow must not expose an editable server address field");
        }
    }
};

QTEST_MAIN(TestNewAccountWizard)
#include "testnewaccountwizard.moc"
