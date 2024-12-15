# squawkbus

A real time message bus supporting:

* publish,
* subscribe,
* notify,
* authentication,
* authorization,
* transport layer security.

## Status

This is work in progress.

## Messages

The clients communicate with the server through messages.

#### Data Packets

Many of the messages involve sending *data-packets*.

The data packets consist of

* An entitlement (an integer)
* A content type (a string)
* Data (an array of bytes)

### Subscribe

Clients may subscribe to a topic.
When data is published to that topic, the parts for which
the client is entitled to receive will be forwarded.

### Notify

A client may request to be notified if another client has
subscribed to a given topic pattern. When a client is notified
it is sent an identifier to the client, which may be used for
direct communication.

### Publish

Clients may publish data to topics. These may be sent to any
client that has subscribed, or to a specific client.

## Authentication

Authentication is provided through Apache [`htpasswd`](https://httpd.apache.org/docs/2.4/programs/htpasswd.html).
This utility can be downloaded without the web server. On ubuntu the packages is `apache2-utils`.

After creating the passwords, the server can be passed the password file as follows.

```bash
squawkbus --password-file htpasswd.dat
```

Sending the signal `HUP` to the process will force the password file to be re-read.

## Authorization

An authenticated server can be configured to support authorizations.
Each data Packet sent to the server has an *entitlement*.
The server will only forward entitlements that a user is configured to receive.

Entitlements are applied to user's and topic's through a regular expression.
The default pattern is for the user `.*` and topic `.*` for roles "`Subscribe`,
`Publish` and `Notify`" with the entitlement `0`.
The means that all users can receive all topics, where the entitlement is `0`.

Authorizations can be specified on the command line:

```bash
squawkbus \
    --password-file htpasswd.dat # authorization required authentication
    --authorization 'tom:LSE.*:1,2:Subscribe' \
    --authorization 'dick:LSE.*:1:Subscribe' \
    --authorization 'harry:LSE.*:1,2:Publish'
```

In the above, `tom` and `dick` are authorized to subscribe to topics starting
with `LSE`, but `tom` can received data with entitlements `1` and `2`, while
`dick` can only receive data with the entitlement `1`. The user `harry` can
publish data with entitlements `1` and `2`.

This may be more easily in yaml.

```yaml authorizations.yaml
# All users can publish, subscribe and notify to topics starting with PUB for entitlement 0.
.*:
  "PUB\\..*":
    entitlements:
    - &PUBLIC 0
    roles:
    - Notifier
    - Publisher
    - Subscriber
# Harry is the publisher for LSE data.
harry:
  "LSE\\..*":
    entitlements:
    - &LSE_LEVEL1 1
    - &LSE_LEVEL2 2
    roles:
    - Notifier
    - Publisher
# Freddy is the publisher for LSE data.
freddy:
  "NYSE\\..*":
    entitlements:
    - &NYSE_LEVEL1 3
    - &NYSE_LEVEL2 4
    roles:
    - Notifier
    - Publisher
# Tom gets both level 1 and 2 data for LSE and NYSE.
tom:
  "LSE\\..*":
    entitlements:
    - *LSE_LEVEL1
    - *LSE_LEVEL2
    roles:
    - Subscriber
  "NYSE\\..*":
    entitlements:
    - *NYSE_LEVEL1
    - *NYSE_LEVEL2
    roles:
    - Subscriber
# Dick only gets level 1.
dick:
  "LSE\\..*":
    entitlements:
    - *LSE_LEVEL1
    roles:
    - Subscriber
  "NYSE\\..*":
    entitlements:
    - *NYSE_LEVEL1
    roles:
    - Subscriber
```

The authorizations file can be used as follows.

```bash
squawkbus \
    --password-file htpasswd.dat # authorization required authentication
    --authorization-file authorizations.yaml
```

Send the signal `HUP` to the process while cause the file to be re-read.

## TLS

The server will run using transport lay security when provided with
a certificate a key file, as follows:

```bash
squawkbus --tls server.crt server.key
```

## Things to do

* Consider a heartbeat
* Make task specific clients
