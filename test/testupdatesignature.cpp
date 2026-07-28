/*
 *    This software is in the public domain, furnished "as is", without technical
 *    support, and with no warranty, express or implied, as to its usefulness for
 *    any purpose.
 *
 */

#include <QtTest>

#include <QBuffer>
#include <QCryptographicHash>

#include "../src/gui/updater/updatesignature.h"

using namespace OCC;

namespace {

QByteArray fromHex(const char *hex)
{
    return QByteArray::fromHex(QByteArray(hex));
}

/**
 * RFC 8032 section 7.1, TEST 2. Verifying a published vector proves we implement
 * Ed25519 itself, and not merely something self consistent.
 */
QByteArray rfcPublicKey()
{
    return fromHex("3d4017c3e843895a92b70aa74d1b7ebc9c982ccf2ec4968cc0cd55f12af4660c");
}

QByteArray rfcMessage()
{
    return fromHex("72");
}

QByteArray rfcSignature()
{
    return fromHex("92a009a9f0d4cab8720e820b5f642540a2b27b5416503f8fb3762223ebdb69da"
                   "085ac1e43e15996e458f3613d0f11d8c387b2eaeb4302aeeb00d291612bb0c00");
}

/** The signing key behind these fixtures is a fixed test seed, never a release key. */
QByteArray testPublicKey()
{
    return fromHex("c561fa9f643fe5c60113cce9db282fde2b9e5ca5fc6b6fc0d1679bb339c9f72f");
}

/** The canonical payload a Nesk Drive manifest is signed over. */
QByteArray referencePayload()
{
    return QByteArrayLiteral(
        "version=7010001\n"
        "versionstring=7.1.0.1\n"
        "channel=stable\n"
        "architecture=x86_64\n"
        "downloadurl=https://downloads.nesk.ch/drive/windows/7.1.0.1/NeskDrive-7.1.0.1-x64.exe\n"
        "web=https://nesk.ch/drive/releases/7.1.0.1\n"
        "sha256=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
        "keyid=update-2026-01\n");
}

QByteArray referenceSignature()
{
    return QByteArray::fromBase64(
        QByteArrayLiteral("lcXeZCUw94TXfcYi87iKFQ/FZc+0MTwoD57ZperY822+0DUUp8C89nQBF1dCboVrsM1FiE4eRTrmGbWEz5P+Ag=="));
}
}

/**
 * An update manifest tells the client which installer to download and run, so a forged
 * one amounts to remote code execution. These tests pin the verification down.
 */
class TestUpdateSignature : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testVerifiesThePublishedRfcVector()
    {
        QVERIFY(UpdateSignature::verify(rfcMessage(), rfcSignature(), rfcPublicKey()));
    }

    void testVerifiesTheReferenceManifest()
    {
        QVERIFY(UpdateSignature::verify(referencePayload(), referenceSignature(), testPublicKey()));
    }

    void testRejectsATamperedPayload()
    {
        QByteArray payload = referencePayload();
        // the download url is precisely what an attacker would want to change
        payload.replace("downloads.nesk.ch", "downloads.evil.example");
        QVERIFY(!UpdateSignature::verify(payload, referenceSignature(), testPublicKey()));
    }

    void testRejectsASingleFlippedBit()
    {
        QByteArray payload = referencePayload();
        payload[0] = payload[0] ^ 0x01;
        QVERIFY(!UpdateSignature::verify(payload, referenceSignature(), testPublicKey()));
    }

    void testRejectsATamperedSignature()
    {
        QByteArray signature = referenceSignature();
        signature[10] = signature[10] ^ 0x01;
        QVERIFY(!UpdateSignature::verify(referencePayload(), signature, testPublicKey()));
    }

    void testRejectsAnotherKey()
    {
        QVERIFY(!UpdateSignature::verify(referencePayload(), referenceSignature(), rfcPublicKey()));
    }

    void testRejectsMalformedInput_data()
    {
        QTest::addColumn<QByteArray>("signature");
        QTest::addColumn<QByteArray>("publicKey");
        QTest::newRow("empty signature") << QByteArray() << testPublicKey();
        QTest::newRow("truncated signature") << referenceSignature().left(63) << testPublicKey();
        QTest::newRow("overlong signature") << (referenceSignature() + QByteArray(1, '\0')) << testPublicKey();
        QTest::newRow("empty key") << referenceSignature() << QByteArray();
        QTest::newRow("truncated key") << referenceSignature() << testPublicKey().left(31);
        QTest::newRow("overlong key") << referenceSignature() << (testPublicKey() + QByteArray(1, '\0'));
    }

    void testRejectsMalformedInput()
    {
        QFETCH(QByteArray, signature);
        QFETCH(QByteArray, publicKey);
        QVERIFY(!UpdateSignature::verify(referencePayload(), signature, publicKey));
    }

    void testRejectsAnEmptyPayload()
    {
        // an empty document must never validate against a signature made for a real one
        QVERIFY(!UpdateSignature::verify(QByteArray(), referenceSignature(), testPublicKey()));
    }

    void testHashesTheInstaller()
    {
        QByteArray content = QByteArrayLiteral("abc");
        QBuffer buffer(&content);
        QVERIFY(buffer.open(QIODevice::ReadOnly));
        QCOMPARE(UpdateSignature::sha256(buffer),
            QByteArrayLiteral("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"));
    }

    void testHashesAnEmptyInstaller()
    {
        QByteArray content;
        QBuffer buffer(&content);
        QVERIFY(buffer.open(QIODevice::ReadOnly));
        QCOMPARE(UpdateSignature::sha256(buffer),
            QByteArrayLiteral("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
    }

    void testHashesLargeContentInChunks()
    {
        // the installer weighs tens of megabytes, so hashing must stream rather than load it whole
        QByteArray content(5 * 1024 * 1024, 'x');
        QBuffer buffer(&content);
        QVERIFY(buffer.open(QIODevice::ReadOnly));
        const QByteArray digest = UpdateSignature::sha256(buffer);
        QCOMPARE(digest.size(), 64);
        QCOMPARE(digest, QCryptographicHash::hash(content, QCryptographicHash::Sha256).toHex());
    }
};

QTEST_GUILESS_MAIN(TestUpdateSignature)
#include "testupdatesignature.moc"
