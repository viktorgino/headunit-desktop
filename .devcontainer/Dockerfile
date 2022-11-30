FROM debian:bullseye

ENV USER headunit
ENV DEBIAN_FRONTEND=noninteractive

# Install Base Packages
RUN apt-get update && apt-get -y upgrade
RUN apt-get install -y  sudo openssh-client \
                        bash-completion curl \
                        git nano automake alsa-utils strace

# Install Qt Packages
RUN apt-get install -y  qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtdeclarative5-dev qtmultimedia5-dev \
                        libqt5multimedia5-plugins libqt5xmlpatterns5-dev libqt5bluetooth5 qtconnectivity5-dev libqt5charts5 libqt5serialport5 \
                        qml-module-qtquick2 qml-module-qtquick-layouts qml-module-qtquick-dialogs qml-module-qtquick-controls2 \
                        qml-module-qtquick-extras qml-module-qtquick-window2 qml-module-qtmultimedia qml-module-qt-labs-settings \
                        qml-module-qt-labs-folderlistmodel qml-module-qt-labs-platform qml-module-qtbluetooth qml-module-qtcharts \
                        qml-module-qtgraphicaleffects qml-module-qtquick-shapes

# Install Additional tools and libraries
RUN apt-get install -y  build-essential protobuf-compiler libusb-1.0-0-dev libssl-dev openssl libglib2.0-dev \
                        libboost-dev libudev-dev libtag1-dev libprotobuf-dev libgstreamer1.0-dev gstreamer1.0-plugins-base-apps \
                        gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-libav gstreamer1.0-alsa libgstreamer-plugins-base1.0-dev \
                        gstreamer1.0-pulseaudio libfaad-dev libfftw3-dev librtlsdr-dev libairspy-dev libusb-1.0-0-dev mesa-common-dev \
                        libglu1-mesa-dev zlib1g-dev  portaudio19-dev libsndfile1-dev libsamplerate0-dev ofono ofono-dev \
                        pulseaudio bluez bluez-obexd pulseaudio-module-bluetooth qtconnectivity5-dev qtquickcontrols2-5-dev \
                        qml-module-qtbluetooth libkf5bluezqt-dev qml-module-org-kde-bluezqt qtbase5-private-dev libcanberra-dev \
                        libqt5charts5 libqt5charts5-dev qml-module-qtcharts libqt5serialport5-dev \
                        libgconf2-dev libpulse-dev libmp3lame-dev libsoapysdr-dev libmpg123-dev libi2c-dev

# Create a non-root user USER
RUN id ${USER} 2>/dev/null || useradd --create-home ${USER}
RUN echo "${USER} ALL=(ALL) NOPASSWD: ALL" | tee -a /etc/sudoers

#  Symlink /bin/sh  to bash instead of dash
RUN echo "dash dash/sh boolean false" | debconf-set-selections
RUN dpkg-reconfigure dash

RUN echo "${USER}:${USER}" | sudo chpasswd
RUN sudo usermod -aG dialout ${USER}
RUN sudo usermod -aG plugdev ${USER}
RUN sudo usermod -aG audio ${USER}
RUN sudo usermod -aG i2c ${USER}
RUN sudo usermod -aG bluetooth ${USER}
RUN sudo chown -R ${USER}:${USER} /home/${USER}

WORKDIR /home/${USER}
CMD ["/bin/bash"]
