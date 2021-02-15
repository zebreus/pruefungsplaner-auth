FROM alpine:3.13.1 AS auth-builder

RUN apk update && apk add build-base qt5-qtbase-dev qt5-qtwebsockets-dev openssl
ENV PATH="/usr/lib/qt5/bin/:${PATH}"

#RUN apk update && apk add git
#RUN git clone --recursive https://github.com/Zebreus/pruefungsplaner-auth.git /pruefungsplaner-auth
COPY . /pruefungsplaner-auth

RUN mkdir -p /install && cd /pruefungsplaner-auth/ && qmake && make -j8 install INSTALL_ROOT=/install/

FROM alpine:3.13.1
MAINTAINER Lennart E.

RUN apk update && apk add qt5-qtbase qt5-qtwebsockets openssl tini
COPY --from=auth-builder /install/ /

ENTRYPOINT ["tini", "/usr/bin/pruefungsplaner-auth"]
EXPOSE 80
