FROM debian:trixie@sha256:4c25275446ca371766b8b093c696782eabce06263fccacb8d5d813b3dc44b308

ARG GODOT_VERSION=4.7.2
ARG EXPECTED_SUMS_SHA256=b8bdff6704f833e7a021df16ec08e1478edd5a285c1f894929b91a7f74a5c7c0
ARG EXPECTED_ENGINE_SHA512=dd59918da086bd49bde2f5450b5e567ff8650cbde9abbd7b8f4ca1197ff8c609baa38834666d032deafb47099078d7822279e2a0e06e5665745468f26533e7e2
ARG EXPECTED_TEMPLATES_SHA512=ca4d71c4d7b81dfc15d1a98baa07534aa95b03fdda78a0075b06672e1648d2e5f40980c9adc28d23e1b92e732ee7bf3461997aa804af74ec2fcd7a93ccb84079
ARG EXPECTED_ENGINE_SHA256=5dd0d86405cf7e8adf79fb6377b38ba682a2846cb378ffe5364f38c01ad29b9d
ARG EXPECTED_TEMPLATES_SHA256=f298490b8d44d934be425a5a65a51bf15f422428b229a06a6e11d9ffea248011
ENV DEBIAN_FRONTEND=noninteractive \
    GODOT_VERSION=${GODOT_VERSION} \
    GODOT_TEMPLATES=/root/.local/share/godot/export_templates/4.7.2.stable

RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates curl unzip xz-utils xauth xvfb \
      libasound2 libfontconfig1 libgl1 libglib2.0-0 libpulse0 \
      libx11-6 libxcursor1 libxext6 libxfixes3 libxi6 libxinerama1 \
      libxrandr2 libxrender1 libxkbcommon0 libwayland-client0 \
      libegl1 libvulkan1 mesa-vulkan-drivers \
    && rm -rf /var/lib/apt/lists/*

# Files and hashes are checked against the official Godot GitHub release's
# SHA512-SUMS.txt during the image build; URLs are the official download CDN.
RUN set -eux; \
    mkdir -p /opt/godot/bin /root/.local/share/godot/export_templates/4.7.2.stable; \
    curl -fsSL "https://downloads.godotengine.org/?version=${GODOT_VERSION}&flavor=stable&slug=linux.arm64.zip" -o /tmp/godot.zip; \
    curl -fsSL "https://downloads.godotengine.org/?version=${GODOT_VERSION}&flavor=stable&slug=export_templates.tpz" -o /tmp/templates.tpz; \
    curl -fsSL "https://github.com/godotengine/godot-builds/releases/download/${GODOT_VERSION}-stable/SHA512-SUMS.txt" -o /tmp/SHA512-SUMS.txt; \
    printf '%s  %s\n' "${EXPECTED_SUMS_SHA256}" /tmp/SHA512-SUMS.txt | sha256sum -c -; \
    grep 'Godot_v4.7.2-stable_linux.arm64.zip$' /tmp/SHA512-SUMS.txt > /tmp/engine.sha512; \
    grep 'Godot_v4.7.2-stable_export_templates.tpz$' /tmp/SHA512-SUMS.txt > /tmp/templates.sha512; \
    printf '%s  %s\n' "${EXPECTED_ENGINE_SHA512}" /tmp/godot.zip | sha512sum -c -; \
    printf '%s  %s\n' "${EXPECTED_TEMPLATES_SHA512}" /tmp/templates.tpz | sha512sum -c -; \
    printf '%s  %s\n' "${EXPECTED_ENGINE_SHA256}" /tmp/godot.zip | sha256sum -c -; \
    printf '%s  %s\n' "${EXPECTED_TEMPLATES_SHA256}" /tmp/templates.tpz | sha256sum -c -; \
    { cat /tmp/engine.sha512 /tmp/templates.sha512; sha256sum /tmp/godot.zip /tmp/templates.tpz; } > /opt/godot/provenance.txt; \
    unzip -q /tmp/godot.zip -d /opt/godot/bin; \
    unzip -q /tmp/templates.tpz -d /root/.local/share/godot/export_templates/4.7.2.stable; \
    mv /root/.local/share/godot/export_templates/4.7.2.stable/templates/* /root/.local/share/godot/export_templates/4.7.2.stable/; \
    rmdir /root/.local/share/godot/export_templates/4.7.2.stable/templates; \
    mv /opt/godot/bin/Godot_v4.7.2-stable_linux.arm64 /opt/godot/bin/godot; \
    chmod +x /opt/godot/bin/godot; \
    rm -f /tmp/godot.zip /tmp/templates.tpz /tmp/*.sha512

ENV PATH="/opt/godot/bin:${PATH}" \
    GODOT4="/opt/godot/bin/godot" \
    GODOT_HEADLESS="/opt/godot/bin/godot --headless --rendering-method gl_compatibility" \
    GODOT_RENDERED="xvfb-run -a /opt/godot/bin/godot --rendering-method gl_compatibility --display-driver x11"
WORKDIR /workspace
