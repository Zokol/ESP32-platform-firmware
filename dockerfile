FROM debian:stretch

WORKDIR /opt/badgebuilder

# Update everything
RUN apt update && \
	apt install -y libncurses5-dev flex bison gperf python-serial libffi-dev libsdl2-dev libmbedtls-dev perl unzip git make build-essential

COPY . .

RUN unzip -p toolchain/xtensa-esp32-elf-linux64.zip xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar | tar xvf -

RUN export PATH="$PATH:/path/to/my/toolchain/xtensa-esp32-elf/bin"

CMD ["bash", "-c", "cp firmware/configs/disobey2020_defconfig firmware/sdkconfig && /opt/badgebuilder/build.sh"]
