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

#include <QByteArray>

class QIODevice;

namespace OCC {

/**
 * @brief Cryptographic checks behind the update channel.
 * @ingroup gui
 *
 * An update manifest names the installer the client will download and execute, so
 * accepting a forged one amounts to remote code execution. Two independent checks
 * guard that path: the manifest itself is signed, and it carries the digest of the
 * installer, which is recomputed once the file has been downloaded. Neither is
 * sufficient alone. A signed manifest whose installer is not hashed still lets
 * whoever serves the download substitute the binary; a hashed installer whose
 * manifest is not signed lets anyone announce their own hash.
 *
 * This class holds no key material: callers pass the public key compiled into the
 * branding, which keeps it testable against published vectors.
 */
class UpdateSignature
{
public:
    /**
     * Verify a detached Ed25519 signature.
     *
     * Every failure mode returns false rather than throwing or asserting: this runs
     * on data fetched from the network, where malformed input is expected rather
     * than exceptional.
     *
     * @param payload the exact bytes that were signed
     * @param signature the detached signature, 64 bytes
     * @param publicKey the raw public key, 32 bytes
     * @return true only if the signature is valid for this payload and this key
     */
    static bool verify(const QByteArray &payload, const QByteArray &signature, const QByteArray &publicKey);

    /**
     * Compute the SHA-256 of an open device, reading it in chunks.
     *
     * The installer weighs tens of megabytes, so it is never loaded whole in memory.
     *
     * @param device an already opened, readable device
     * @return the digest as lowercase hexadecimal, empty if the device could not be read
     */
    static QByteArray sha256(QIODevice &device);
};

} // namespace OCC
