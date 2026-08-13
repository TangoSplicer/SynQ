#!/usr/bin/env sh
set -eu

MERCURY_VERSION="22.01.8"
MERCURY_SHA512="61c1f34f48bf2bfc045ff5b78f7c2803fed99430e1a5ccb341f71f34c71f6fdcb2d3eb25c8c4cee04786d1b2b1ab203cc4eaefea67e17643e65dbf048a0e0ea4"
MERCURY_BASE_URL="https://dl.mercurylang.org/release"
MERCURY_PREFIX="${MERCURY_PREFIX:-${HOME}/.cache/synq-mercury/${MERCURY_VERSION}}"
MERCURY_ARCHIVE="mercury-srcdist-${MERCURY_VERSION}.tar.xz"
MERCURY_WORKDIR="${MERCURY_WORKDIR:-${HOME}/.cache/synq-mercury/work-${MERCURY_VERSION}}"

if [ -x "${MERCURY_PREFIX}/bin/mmc" ]; then
    "${MERCURY_PREFIX}/bin/mmc" --version
    printf '%s\n' "${MERCURY_PREFIX}/bin"
    exit 0
fi

mkdir -p "${MERCURY_WORKDIR}"
cd "${MERCURY_WORKDIR}"

if [ ! -f "${MERCURY_ARCHIVE}" ]; then
    curl --fail --location --silent --show-error \
        --output "${MERCURY_ARCHIVE}" \
        "${MERCURY_BASE_URL}/${MERCURY_ARCHIVE}"
fi

printf '%s  %s\n' "${MERCURY_SHA512}" "${MERCURY_ARCHIVE}" | sha512sum -c -

SOURCE_DIR="${MERCURY_WORKDIR}/mercury-srcdist-${MERCURY_VERSION}"
if [ ! -d "${SOURCE_DIR}" ]; then
    tar -xf "${MERCURY_ARCHIVE}"
fi

cd "${SOURCE_DIR}"
sh configure \
    --prefix="${MERCURY_PREFIX}" \
    --enable-libgrades=asm_fast.gc \
    --disable-java-grade \
    --disable-csharp-grade \
    --disable-deep-profiler \
    --disable-shared-libs
make PARALLEL=-j2
make PARALLEL=-j2 install
"${MERCURY_PREFIX}/bin/mmc" --version
printf '%s\n' "${MERCURY_PREFIX}/bin"
