INA233 - Current/Power monitor (i2c)
====================================

This is a reference workbench for `ehwe`
([project link](https://github.com/helsinova/ehwe))

## How to use this git

First, git-clone ``EHWE`` somewhere. Make sure you got all submodules by
reading it's ``README.md``.

The following assumes you have a fresh newly created git repository which
is empty. I.e. in essence the following:

```bash
mkdir newproject
cd newproject
git init
git commit --allow-empty -m"Initial commit"
```

Lets now allocate the path of this directory for the sake of exemplification
later on (you can off course upload to a server and use that as a origin
instead):

```bash
REMOTE_REPO=$(pwd)
```

* Copy & paste **this project** into a newly created .git project. I.e.
  all but the .git directory. *Hint:* Copy in two steps. 
	1. Into a temporary directory. In this directory you remove the .git/
	   sub-directory. Intermediate directory is now not a git-repo anymore.
	2. Copy everything from the intermediate directory into your newly prepared
	   git-directory.
* Add & commit everything in your new workbench. Possibly rename the
  source-files and/or add more first.
* Adapt EHWE to use your new workbench instead as follows:

Lets assume ``$EHWE`` contains the path to *ehwe*

```bash
cd $EHWE
git submodule deinit src/embedded
git submodule add $REMOTE_REPO src/embedded
git submodule init
git submodule update
```

