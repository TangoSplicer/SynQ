FROM ubuntu:22.04

RUN apt update && apt install -y python3 git curl zip
COPY . /synq
WORKDIR /synq

RUN chmod +x synq/scripts/publish_to_hub.sh
ENTRYPOINT ["bash"]