# quasardb node.js API

This API is still experimental and poorly documented. Use at your own risks!

[quasardb](https://www.quasardb.net/) 2.0 or later required. You need a C++ 11 compiler to compile this addon.

## Installation

## quasardb C API

To build the nodejs API, you will need the C API. It can either be installed on the machine (e.g. on unix in /usr/lib or /usr/local/lib) or you can unpack the C API archive in deps/qdb.

## Building the extension

You will need to have [node-gyp](https://github.com/TooTallNate/node-gyp) installed.

In the directory run:

```
    npm install
```

You will then find a qdb.node file which is the quasardb addon in `build/Release`.

## Introduction

Using *quasardb* starts with a Cluster:

```javascript
    var qdb = require('./qdb');

    var c = new qdb.Cluster('qdb://127.0.0.1:2836');
```

Now that we have a connection to the cluster, let's store some binary data:

```javascript
    var b = c.blob('bam');

    b.put(new Buffer("boom"), function(err) { /* */  });
    b.get(function(err, data) { /* */  });
```

Want a queue? We have distributed queues.

```javascript
    var q = c.queue('q2');

    q.pushBack(new Buffer("boom"), function(err) { /* */ });
    q.popFront(function(err, data) { /* */ });
    q.pushFront(new Buffer("bang"), function(err) { /* */ });
```

quasardb comes out of the box with server-side atomic integers:

```javascript
    var i = c.integer('some_int');
    i.put(3, function(err){ /* */});
    i.add(7, function(err, data){ /* */});
```

We also provide distributed sets:

```javascript
    var s = c.set('the_set');

    s.insert(new Buffer("boom"), function(err) { /* */ });
```

## Tags

[quasardb](https://www.quasardb.net/) is advanced key-value store with a powerful tagging feature. Tags lookup are fast, scalable and reliable.

Any entry can be tagged, including tags.

```javascript

    var b = c.blob('bam');

    b.addTag('dasTag', function(err) { /* */ });

    var i = c.integer('bom');

    i.addTag('dasTag', function(err) { /* */ });

    var t = c.tag('dasTag');

    t.getEntries(function(err, entries} { /* entries is the list of entries */ });
```

It is also possible to list the tags of an entry or test for the existence of a tag:

```javascript

    b.hasTag('dasTag', function(err) { /* */ });

    b.getTags(function(err, tags) { /* tags is the list of tags */ });
```

## Expiry

Integers and blob can be configured to automatically expire. The expiry can be specified with an absolute value at the entry creation or update, or can
later be set with expiresAt and expiresFromNow.

The expiry is second precise.

```javascript
    var b = c.blob('bam');

    // value will be removed at the specified time
    b.put(new Buffer("boom"), new Date("October 1st, 2016 11:13:00"), function(err) { /* */  });

    // value expires now!
    b.expiresAt(new Date());

    // entry will be removed in 20 seconds relative to the current time
    b.expiresFromNow(20);
```

Everytime you update a value without providing an expiry, the expiry time is set to "infinite" (i.e. never expires).

You can query the expiry time of an entry with getExpiry:

```javascript
    // returns a Date object
    var expiry = b.getExpiry();
```

Tags, sets and queues do not expire (but can of course be manually removed).

## Not supported yet

The quasardb nodejs addon is still a work in progress, the following quasardb features are not supported:

 * Advanced atomic operations such as compare and swap and conditional removal
 * Batches and transactions
 * Queue size and random access to elements in the queue
