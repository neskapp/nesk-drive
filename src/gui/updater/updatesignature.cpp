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

#include "updatesignature.h"

#include <QCryptographicHash>
#include <QIODevice>

#include <openssl/evp.h>

namespace {
// fixed by the algorithm, see RFC 8032
constexpr int ed25519SignatureSize = 64;
constexpr int ed25519PublicKeySize = 32;
}

namespace OCC {

bool UpdateSignature::verify(const QByteArray &payload, const QByteArray &signature, const QByteArray &publicKey)
{
    // An empty payload is never a legitimate manifest, and accepting one would mean a
    // truncated download could validate.
    if (payload.isEmpty() || signature.size() != ed25519SignatureSize || publicKey.size() != ed25519PublicKeySize) {
        return false;
    }

    EVP_PKEY *key = EVP_PKEY_new_raw_public_key(
        EVP_PKEY_ED25519, nullptr, reinterpret_cast<const unsigned char *>(publicKey.constData()), ed25519PublicKeySize);
    if (key == nullptr) {
        return false;
    }

    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr) {
        EVP_PKEY_free(key);
        return false;
    }

    bool verified = false;
    if (EVP_DigestVerifyInit(context, nullptr, nullptr, nullptr, key) == 1) {
        // Ed25519 is a one shot algorithm: it takes the whole message at once, the
        // streaming Update/Final pair is not available for it.
        verified = EVP_DigestVerify(context, reinterpret_cast<const unsigned char *>(signature.constData()),
                       static_cast<size_t>(signature.size()), reinterpret_cast<const unsigned char *>(payload.constData()),
                       static_cast<size_t>(payload.size()))
            == 1;
    }

    EVP_MD_CTX_free(context);
    EVP_PKEY_free(key);
    return verified;
}

QByteArray UpdateSignature::sha256(QIODevice &device)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    if (!hash.addData(&device)) {
        return QByteArray();
    }
    return hash.result().toHex();
}

} // namespace OCC
