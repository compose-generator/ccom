---
title: Use with Docker
---

### Download
You don't have to pull the image first. You also can skip this step.
=== "Docker Hub"
    ```sh
    docker pull chillibits/ccom
    ```
=== "GitHub Container Registry"
    ```sh
    docker pull ghcr.io/chillibits/ccom
    ```

### Use
=== "Docker Hub"
    ```sh
    docker run --rm -it -v ${pwd}:/ccom/out chillibits/ccom
    ```
=== "GitHub Container Registry"
    ```sh
    docker run --rm -it -v ${pwd}:/ccom/out ghcr.io/chillibits/ccom
    ```

### Customize
#### Custom output path
You can use another output path by replacing `${pwd}` with a custom path.

!!! example
    ```sh
    docker run --rm -it -v ./project:/ccom/out chillibits/ccom
    ```