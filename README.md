## A utility to print java class bytecode version.

# How to build
So far it is tested on Mac only.
To build it run:
> make build

# How to use
> jbc test/*.class
> *          test/ShipContract.class: [ ca fe ba be 00 00 00 34 ] jdk v8
> *    test/ShipSpringBootMain.class: [ ca fe ba be 00 00 00 37 ] jdk v11

