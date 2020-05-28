PI3HDX414 - HDMI mux (i2c)
==========================

This is a reference workbench for `ehwe`
([project link](https://github.com/helsinova/ehwe))

Documentation for the circuit is under `doc/`

As commit 2e48679 shows, the IC can not operate register-based.
Also mentioned in documentation, but does not explicitly exclude register
based operation, hence worth clarifying.

Running builds of 2e48679 shows:

```bash
$ ehwe.sh -x3
12 23 34 45 56 67 70 89
12 23 34 45 56 67 70 89
00 01 02 03 04 05 06 07
00 01 02 03 04 05 06 07
07 23 34 45 56 67 70 89
07 23 34 45 56 67 70 89
00 01 02 03 04 05 06 07
00 01 02 03 04 05 06 07
07 23 34 45 56 67 70 89
07 23 34 45 56 67 70 89
00 01 02 03 04 05 06 07
00 01 02 03 04 05 06 07
```

Note the first lines which begin with 07. This proves IC last index value
was written in a slot instead of being used as index even if no new start
condition was sent. I.e. IC interface is a simple shift-register.



## How to use this git
See this file in master-branch for more information
