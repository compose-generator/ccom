---
title: Install on Linux
---

### Install from repository
=== "Debian/Ubuntu"
    To install CCom on Debian or Ubuntu, execute the following commands in your terminal:
    ```sh
	sudo apt-get install ca-certificates
    curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
	sudo add-apt-repository "deb https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main"
	sudo apt-get update
	sudo apt-get install ccom
    ```

=== "Fedora"
    To install CCom on Fedora, execute the following commands in your terminal:
    ```sh
    sudo dnf -y install dnf-plugins-core
	sudo dnf config-manager --add-repo https://server.chillibits.com/files/repo/fedora.repo
	sudo dnf install ccom
    ```

=== "CentOS"
    To install CCom on CentOS, execute the following commands in your terminal:
    ```sh
    sudo yum install -y yum-utils
	sudo yum-config-manager --add-repo https://server.chillibits.com/files/repo/centos.repo
	sudo yum install ccom
    ```

=== "Raspbian"
    To install CCom on Raspbian, execute the following commands in your terminal:
    ```sh
	sudo apt-get install ca-certificates
    curl -fsSL https://server.chillibits.com/files/repo/gpg | sudo apt-key add -
	sudo echo "deb [arch=armhf] https://repo.chillibits.com/$(lsb_release -is | awk '{print tolower($0)}')-$(lsb_release -cs) $(lsb_release -cs) main" > /etc/apt/sources.list.d/chillibits.list
	sudo apt-get update
	sudo apt-get install ccom
    ```

### Install from package file
You can also install CCom from a Linux package.

=== "Debian/Ubuntu/Raspbian"
    [Download amd64](https://github.com/compose-generator/ccom/releases/latest/download/ccom_amd64.deb){ .md-button .md-button--primary }
    [Download arm64](https://github.com/compose-generator/ccom/releases/latest/download/ccom_arm64.deb){ .md-button .md-button--primary }
    [Download armv5](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv5.deb){ .md-button .md-button--primary }
    [Download armv6](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv6.deb){ .md-button .md-button--primary }
    [Download armv7](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv7.deb){ .md-button .md-button--primary }

    To install it, execute the following command:
    ```sh
    dpkg -i <deb-file-name>
    ```

=== "Fedora/CentOS"
    [Download amd64](https://github.com/compose-generator/ccom/releases/latest/download/ccom_amd64.rpm){ .md-button .md-button--primary }
    [Download arm64](https://github.com/compose-generator/ccom/releases/latest/download/ccom_arm64.rpm){ .md-button .md-button--primary }
    [Download armv5](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv5.rpm){ .md-button .md-button--primary }
    [Download armv6](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv6.rpm){ .md-button .md-button--primary }
    [Download armv7](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv7.rpm){ .md-button .md-button--primary }

    To install it, execute the following command:
    ```sh
    rpm -U <rpm-file-name>
    ```

=== "Alpine"
    [Download amd64](https://github.com/compose-generator/ccom/releases/latest/download/ccom_amd64.apk){ .md-button .md-button--primary }
    [Download arm64](https://github.com/compose-generator/ccom/releases/latest/download/ccom_arm64.apk){ .md-button .md-button--primary }
    [Download armv5](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv5.apk){ .md-button .md-button--primary }
    [Download armv6](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv6.apk){ .md-button .md-button--primary }
    [Download armv7](https://github.com/compose-generator/ccom/releases/latest/download/ccom_armv7.apk){ .md-button .md-button--primary }

    To install it, execute the following command:
    ```sh
    apk add --allow-untrusted <apk-file-name>
    ```

### Use
```sh
ccom [options] <input>
```
